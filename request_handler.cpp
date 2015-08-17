#include "request_handler.hpp"

#include <stdexcept>

#include <daneel/network/utility.hpp>

#include "city_list.hpp"
#include "config.hpp"
#include "utils.hpp"

const std::string MyRequestHandler::PARAM_ID = "id";
const std::string MyRequestHandler::PARAM_Q  = "q";

std::mutex MyRequestHandler::_cache_mutex;
std::map<CityID, WeatherResultPtr> MyRequestHandler::_cached_results;

daneel::Response MyRequestHandler::handleRequest(const daneel::RequestParameters& request_parameters)
{
  // Concatenate all parameters to form query to weather API
  const std::vector<std::string>& parameters = request_parameters.getWhatParameters();

  daneel::Response response;
  if( parameters.empty() )
  {
    response.addError(std::string("Not enough WHAT parameters"));
    return response;
  }

  // Find total length of future string to prevent future reallocating when concatenating parameters between them
  const size_t parameters_size = parameters.size();
  size_t total_length {parameters[0].length()};
  for( size_t i = 1; i < parameters_size; ++i )
    total_length += parameters[i].length() + 1; // +1 for the space between words

  std::string request_parameters_str {parameters[0]};
  request_parameters_str.reserve(total_length);

  for( size_t i = 1; i < parameters_size; ++i )
    request_parameters_str += " " + parameters[i];

  try
  {
    // Get weather
    WeatherResultPtr weather_result = getWeather( request_parameters_str );
    response.addResult(weather_result->toJSON());
  }
  catch(std::exception& e)
  {
    response.addError(std::string("Error while trying to get weather for '" + request_parameters_str + "': ") + e.what());
  }

  return response;
}

WeatherResultPtr MyRequestHandler::getWeather(const std::string& input)
{
  // Do we already know the city ID for this input
  const CityID city_id = CityList::instance()->getCityID(input);
  if(city_id == CityList::NO_CITY_ID)
  {
    WeatherResultPtr result = std::make_shared<WeatherResult>(sendQuery(PARAM_Q, input));
    CityList::instance()->update(input, result->cityID());
    return result;
  }

  // Did we already have the answer for this question?
  WeatherResultPtr cached_result = getCachedResult(city_id);
  if(cached_result)
  {
    return cached_result;
  }
  else
  {
    WeatherResultPtr result = std::make_shared<WeatherResult>(sendQuery(PARAM_ID, std::to_string(city_id)));

    // Add result to cache
    cacheResult(result);

    return result;
  }
}

std::string MyRequestHandler::sendQuery(const std::string& param, const std::string& value)
{
  std::string query_response;
  if( !daneel::sendHTTPRequest(daneel::HTTPMethod::Get, Config::uri(param, value), query_response) )
    throw std::runtime_error("bad http request");

  return query_response;
}

void MyRequestHandler::cacheResult(const WeatherResultPtr& result)
{
  std::lock_guard<std::mutex> lock(_cache_mutex);
  _cached_results[result->cityID()] = result;
}

WeatherResultPtr MyRequestHandler::getCachedResult(CityID city_id)
{
  // Protect access in case we are currently writing/modifying cache
  std::lock_guard<std::mutex> lock(_cache_mutex);

  auto found = _cached_results.find( city_id );
  if( found == _cached_results.end() )
    return {};

  const WeatherResultPtr cached_result = found->second;
  if( isUpToDate(cached_result) )
    return cached_result;
  else return {};
}

bool MyRequestHandler::isUpToDate(const WeatherResultPtr& result)
{
  return result->timestamp() + ResultCacheTime > utils::currentTimestamp();
}

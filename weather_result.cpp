#include "weather_result.hpp"

#include <exception>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "utils.hpp"

WeatherResult::WeatherResult(const std::string& result)
{
  parseResult(result);
}

void WeatherResult::parseResult(const std::string& result)
{
  _timestamp = utils::currentTimestamp();

  rapidjson::Document document;
  document.Parse<0>(result.c_str());

  if( !document.IsObject() )
    throw std::runtime_error("result syntax error - invalid json: " + result);

  const auto& city    = document["city"];
  const auto& list    = document["list"][static_cast<rapidjson::SizeType>(0)];
  const auto& weather = list["weather"][static_cast<rapidjson::SizeType>(0)];
  const auto& extra   = list["main"];

  _city_name           = city["name"].GetString();
  _city_id             = city["id"].GetInt();
  _weather             = weather["description"].GetString();
  _current_temperature = extra["temp"].GetDouble();
  _min_temperature     = extra["temp_min"].GetDouble();
  _max_temperature     = extra["temp_max"].GetDouble();
  _humidity            = extra["humidity"].GetInt();
}

std::string WeatherResult::toJSON() const
{
  static const std::string response = R"JSON(
{
  "city": "_city_name",
  "weather": "_weather",
  "humidity": _humidity,
  "temperatures":
   {
      "min": _min_temperature,
      "max": _max_temperature,
      "current": _current_temperature
   }
}
)JSON";

  std::string result = boost::algorithm::replace_first_copy(response, "_city_name", _city_name);
  boost::algorithm::replace_first(result, "_weather", _weather);
  boost::algorithm::replace_first(result, "_humidity", std::to_string(_humidity));
  boost::algorithm::replace_first(result, "_min_temperature", jsonTemperature(_min_temperature));
  boost::algorithm::replace_first(result, "_max_temperature", jsonTemperature(_max_temperature));
  boost::algorithm::replace_first(result, "_current_temperature", jsonTemperature(_current_temperature));

  return result;
}

std::string WeatherResult::jsonTemperature(double temperature)
{
  static const std::string response = R"JSON(
    {
      "kelvin": _kelvin,
      "celsus": _celcus,
      "fahrenheit": _fahrenheit
    }
    )JSON";

  std::string result = boost::algorithm::replace_first_copy(response, "_kelvin", std::to_string(temperature));
  boost::algorithm::replace_first(result, "_celcus", std::to_string(toCelcus(temperature)));
  boost::algorithm::replace_first(result, "_fahrenheit", std::to_string(toFahrenheit(temperature)));

  return result;
}

double WeatherResult::toCelcus(double kelvin)
{
  return kelvin - 273.15;
}

double WeatherResult::toFahrenheit(double kelvin)
{
  return (kelvin - 273.15) * 1.8 + 32;
}

//void WeatherResult::readError(const std::string& field)
//{
//  throw std::runtime_error("result syntax error - no field " + field);
//}

//void WeatherResult::validateAttribute(const rapidjson::Value& document, const std::string& attribute)
//{
//  if( !document.HasMember(attribute.c_str()) )
//    readError(attribute);
//}

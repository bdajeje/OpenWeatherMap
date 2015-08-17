#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <map>
#include <mutex>
#include <string>

#include <daneel/daneel.hpp>

#include "weather_result.hpp"

class MyRequestHandler final : public daneel::RequestHandler
{
  public:

    daneel::Response handleRequest(const daneel::RequestParameters& request_parameters);

  private:

    WeatherResultPtr getWeather(const std::string& input);
    WeatherResultPtr getCachedResult(CityID city_id);
    void cacheResult(const WeatherResultPtr& result);

    static std::string sendQuery(const std::string& param, const std::string& value);
    static bool isUpToDate(const WeatherResultPtr& result);

  private:

    static std::mutex _cache_mutex;
    static std::map<CityID, WeatherResultPtr> _cached_results;
    static const unsigned int ResultCacheTime {600}; // 10 min in seconds
    static const std::string PARAM_ID;
    static const std::string PARAM_Q;
};

#endif // WEATHER_RESULT_H

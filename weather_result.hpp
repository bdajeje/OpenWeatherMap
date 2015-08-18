#ifndef WEATHER_RESULT_HPP
#define WEATHER_RESULT_HPP

#include <chrono>
#include <memory>
#include <string>

#include "city_list.hpp"
#include "rapidjson/document.h"

class WeatherResult final
{
  public:

    WeatherResult(const std::string& result);

    unsigned int timestamp() const { return _timestamp; }
    CityID cityID() const { return _city_id; }
    const std::string& cityName() const { return _city_name; }
    const std::string& weather() const { return _weather; }
    double getMinTemperature() const { return _min_temperature; }
    double getCurrentTemperature() const { return _current_temperature; }
    double getMaxTemperature() const { return _max_temperature; }
    int getHumidity() const { return _humidity; }

    void setResult(const std::string& result) { parseResult(result); }

    std::string toJSON() const;

  private:

    void parseResult(const std::string& result);

    static void validate(const rapidjson::GenericValue<rapidjson::UTF8<>>& document, const char* key);
    static std::string jsonTemperature(double temperature);
    static double toCelcus(double kelvin);
    static double toFahrenheit(double kelvin);

  private:

    unsigned int _timestamp;
    std::string _city_name;
    std::string _weather;
    CityID _city_id;
    double _current_temperature;
    double _min_temperature;
    double _max_temperature;
    int _humidity;
};

typedef std::shared_ptr<WeatherResult> WeatherResultPtr;

#endif // WEATHER_RESULT_HPP

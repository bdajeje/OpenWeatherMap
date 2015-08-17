#ifndef CITY_LIST_HPP
#define CITY_LIST_HPP

#include <limits>
#include <map>
#include <memory>
#include <string>

typedef unsigned int CityID;

class CityList final
{
  public:

    static const unsigned int NO_CITY_ID { std::numeric_limits<CityID>::max() };

    static bool init(const std::string& filepath);
    static const std::unique_ptr<CityList>& instance() { return _instance; }

    CityID getCityID(const std::string& input);
    void update(const std::string& city_name, CityID city_id);

  private:

    CityList(const std::string& filepath);
    bool readCityList(const std::string& filepath);

    static std::string safeInput(const std::string& input);

  private:

    std::map<std::string, CityID> _city_ids;
    static std::unique_ptr<CityList> _instance;
};

#endif // WEATHER_RESULT_H

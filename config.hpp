#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <memory>
#include <string>

#include <Poco/URI.h>

class Config final
{
  public:

    static void init(const std::string& app_id);
    static Poco::URI uri(const std::string& param, const std::string& value);

  private:

    Config(const std::string& app_id);

  private:

    const std::string _uri;
    static std::unique_ptr<Config> _instance;
};

#endif // WEATHER_RESULT_H

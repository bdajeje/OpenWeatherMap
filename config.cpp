#include "config.hpp"

#include <iostream>

std::unique_ptr<Config> Config::_instance;

void Config::init(const std::string& app_id)
{
  if(_instance)
    return;

  _instance.reset( new Config(app_id) );
}

Config::Config(const std::string& app_id)
  : _uri{ "http://api.openweathermap.org/data/2.5/forecast/city?APPID=" + app_id }
{}

Poco::URI Config::uri(const std::string& param, const std::string& value)
{
  Poco::URI uri {_instance->_uri};
  uri.addQueryParameter(param, value);

  return uri;
}

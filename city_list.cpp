#include "city_list.hpp"

#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "utils.hpp"
#include "rapidjson/document.h"

std::unique_ptr<CityList> CityList::_instance;

bool CityList::init(const std::string& filepath)
{
  if(_instance || filepath.empty())
    return false;

  _instance.reset( new CityList(filepath) );
  return true;
}

CityList::CityList(const std::string& filepath)
{
  if(!readCityList(filepath))
    throw std::runtime_error("can't read city list from: " + filepath);
}

CityID CityList::getCityID(const std::string& input)
{
  auto found = _city_ids.find(safeInput(input));
  if( found != _city_ids.end() )
    return found->second;

  return NO_CITY_ID;
}

void CityList::update(const std::string& city_name, CityID city_id)
{
  _city_ids[safeInput(city_name)] = city_id;
}

std::string CityList::safeInput(const std::string& input)
{
  // Always lower input string so searches are case insensitive
  return boost::algorithm::to_lower_copy(input);
}

bool CityList::readCityList(const std::string& filepath)
{
  std::string content;
  if(!utils::file::read(filepath, content))
    return false;

  std::vector<std::string> lines;
  boost::algorithm::split(lines, content, boost::is_any_of("\n"));

  for( const std::string& line : lines )
  {
    rapidjson::Document document;
    document.Parse<0>(line.c_str());
    update(document["name"].GetString(), document["_id"].GetUint());
  }

  std::cout << "Read " << _city_ids.size() << " cities." << std::endl;
  return !_city_ids.empty();
}

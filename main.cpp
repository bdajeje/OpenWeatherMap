#include <iostream>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>
#include <daneel/daneel.hpp>
#include <Poco/URI.h>

#include "city_list.hpp"
#include "config.hpp"
#include "request_handler.hpp"
#include "utils.hpp"
#include "weather_result.hpp"

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    std::cerr << "Missing argument: APP ID" << std::endl;
    return EXIT_FAILURE;
  }

  Config::init(argv[1]);

  // Read city list of IDs
  if(!CityList::init("cities.json"))
  {
    std::cerr << "Can't read city list" << std::endl;
    return EXIT_FAILURE;
  }

  daneel::Daneel<MyRequestHandler>::start(argc, argv);

  return EXIT_SUCCESS;
}

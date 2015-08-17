cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(emailer)

set(CMAKE_BUILD_TYPE Release)

add_executable(${PROJECT_NAME}
  city_list.cpp
  city_list.hpp
  config.cpp
  config.hpp
  main.cpp
  request_handler.cpp
  request_handler.hpp
  utils.cpp
  utils.hpp
  weather_result.cpp
  weather_result.hpp
  rapidjson/document.h
  rapidjson/filestream.h
  rapidjson/prettywriter.h
  rapidjson/rapidjson.h
  rapidjson/reader.h
  rapidjson/stringbuffer.h
  rapidjson/writer.h
  rapidjson/internal/pow10.h
  rapidjson/internal/stack.h
  rapidjson/internal/strfunc.h
)

# For C++11 functionnalities
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Linking libraries
target_link_libraries(${PROJECT_NAME} -pthread
                                      -lboost_filesystem -lboost_regex -lboost_system
                                      -lPocoNet -lPocoUtil -lPocoFoundation
                                      -ldaneel)

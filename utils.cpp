#include "utils.hpp"

#include <chrono>
#include <fstream>

namespace utils {

unsigned int currentTimestamp()
{
  return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

namespace file {

bool read( const std::string& filepath, std::string& result )
{
  std::ifstream file(filepath.c_str());
  if( !file.is_open() )
    return false;

  // Get length of file
  file.seekg(0, file.end);
  const int length = file.tellg();
  file.seekg(0, file.beg);

  // If file content is empty, exit
  if( length < 1 )
    return true;

  // Read the file
  result.resize( length );
  file.read(&result[0], length);

  // Close the file and return good read
  file.close();
  return true;
}

} // file
} // utils

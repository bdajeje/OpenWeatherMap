#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace utils {

unsigned int currentTimestamp();
std::string extractDir(const std::string& path);

namespace file {

bool read( const std::string& filepath, std::string& result );

} // namespace file
} // namespace utils

#endif // UTILS_HPP

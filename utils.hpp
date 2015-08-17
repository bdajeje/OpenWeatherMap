#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace utils {

unsigned int currentTimestamp();

namespace file {

bool read( const std::string& filepath, std::string& result );

} // namespace file
} // namespace utils

#endif // UTILS_HPP

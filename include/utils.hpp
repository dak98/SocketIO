#ifndef SOCKETIO_UTILS_HPP_
#define SOCKETIO_UTILS_HPP_

#include <cerrno>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

/*
 * @brief Convertes value in str to an integer in base 10
 * @returns Converted value
 * @throws - std::invalid_argument => no conversion could be performed
 *         - std::out_of_range => absolute value is too large
 */
template<typename integer>
auto string_to_integer(std::string const& str) -> integer
{
    static_assert(std::is_integral<integer>::value);
    const long long converted_integer = std::stoi(str, 0, 10);
    if (converted_integer < std::numeric_limits<integer>::min() ||
	converted_integer > std::numeric_limits<integer>::max())
	throw std::out_of_range{"Input value out of range"};
    return static_cast<integer>(converted_integer);
}

/*
 * This version is not thread-safe
 *
 * @throws - std::bad_alloc => from std::string constructor
 */
inline auto get_errno_as_string() -> std::string
{
    return std::strerror(errno);
}

#endif

#ifndef SOCKETIO_UTILS_HPP_
#define SOCKETIO_UTILS_HPP_

#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

/*
 * @brief Convertes value in str to a signed integer in base 10
 * @returns Converted value or:
 *          - -1 => conversion could not be performed
 *          - -2 => over- or underflow occured
 */
template<typename signed_integer>
auto string_to_signed_integer(std::string const& str) noexcept -> signed_integer
try
{
    static_assert(std::is_integral<signed_integer>::value &&
		  std::is_signed<signed_integer>::value);
    const long long converted_integer = std::stoi(str, 0, 10);
    if (converted_integer < std::numeric_limits<signed_integer>::min() ||
	converted_integer > std::numeric_limits<signed_integer>::max())
	return static_cast<signed_integer>(-2);	
    return static_cast<signed_integer>(converted_integer);    
}
catch (std::invalid_argument const& e)
{
    return static_cast<signed_integer>(-1);
}
catch (std::out_of_range const& e)
{
    return static_cast<signed_integer>(-2);
}


#endif

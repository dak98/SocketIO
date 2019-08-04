#ifndef SOCKETIO_LOGGER_HPP_
#define SOCKETIO_LOGGER_HPP_

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

namespace socket_io
{

inline void init_logging()
{
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

    logging::add_file_log
    (
        keywords::file_name = "socket_io.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    logging::add_common_attributes();
}

} // socket_io

#endif // SOCKETIO_LOGGER_HPP_

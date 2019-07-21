#ifndef SOCKETIO_LOGGER_HPP_
#define SOCKETIO_LOGGER_HPP_

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

inline void initLogging() {
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

    logging::add_file_log
    (
        keywords::file_name = "socketIO.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    logging::add_common_attributes();
}

#endif

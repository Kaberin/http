#pragma once
#include <iostream>
#include <string>

#include "MagicEnum.hpp"
namespace web {

    enum class LogType { INFO, ERROR, WARNING };
    class Logger {
       public:
        static Logger& GetInstance() {
            static Logger logger;
            return logger;
        }

        void Log(LogType level, const std::string& message,
                 std::ostream& ostream = std::cout) {
            ostream << "[ " << magic_enum::enum_name(level) << " ] " << message
                    << '\n';
        }

       protected:
        Logger() {}
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
    };
}  // namespace web
#ifndef CBPP_LOGGER_H
#define CBPP_LOGGER_H

#include <stdio.h>
#include "cbpp/cbdef.h"

namespace cbpp {
    /*
        Logging utilities
    */
    class Logger {
        public:
            enum {
                LOG_INFO,
                LOG_WARN,
                LOG_ERROR
            };

            Logger() = default;
            Logger(const Logger& other) = delete;
            Logger(const char* lnam);

            void SetName(const char* nname);

            Logger& operator=(const Logger& other) = delete;

            bool Write(int typ, const char* text);
            ~Logger();

        private:
            char m_logname[CB_MAX_LOGNAME+1];
    };
}

#endif

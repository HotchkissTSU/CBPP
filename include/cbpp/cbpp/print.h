#ifndef CBPP_PRINTF_H
#define CBPP_PRINTF_H

#include <stdlib.h>

namespace cbpp {
    size_t PrintBuffer(char* buffer, size_t max_write, const char* cb_format, ...);
}

#endif
#ifndef CBPP_SDK_MAIN_H
#define CBPP_SDK_MAIN_H

#include <stdio.h>

#define sdk_errorf(s, ...) { char sBuffer[128]; snprintf(sBuffer, 128, s, __VA_ARGS__); sdk_error(sBuffer); }
#define sdk_cdf_validate(expr) { cdf_retcode _i = expr; if(_i != CDF_OK) { sdk_errorf(#expr" -> %s", cdf_get_error(_i)); } }

#ifndef sdk_error
void sdk_error(const char* sText) {
    fprintf(stderr, "Error: %s\n", sText);
    exit(EXIT_FAILURE);
}
#endif

#endif

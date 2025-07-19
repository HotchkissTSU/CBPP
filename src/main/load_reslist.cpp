#include "cb_main/aux.h"
#include "cbpp/error.h"
#include "cbvs/render.h"

/*
    Load supported resolutions list from the gamefile
*/

bool RegisterScreenResolutions( yyjson_val* jValue ) noexcept {
    if( !yyjson_is_arr(jValue) ) {
        cbpp::PushError(cbpp::ERROR_GF, "'resolutions' must be a valid JSON array");
        return false;
    }

    static char s_sErrorBuffer[256];

    const size_t iResAmount = yyjson_arr_size(jValue);
    for( size_t i = 0; i < iResAmount; i++ ) {
        yyjson_val* jCurrent = yyjson_arr_get(jValue, i);

        if( !yyjson_is_arr(jCurrent) || (yyjson_arr_size(jCurrent) < 2) ) {
            snprintf(s_sErrorBuffer, 256, "Bad resolution at index (%zu) : must be an array of size >= 2", i);
            cbpp::PushError(cbpp::ERROR_GF, s_sErrorBuffer);
            return false;
        }

        cbvs::ScreenResolution NewRes;
        NewRes.iWidth = yyjson_get_int( yyjson_arr_get(jCurrent, 0) );
        NewRes.iHeight = yyjson_get_int( yyjson_arr_get(jCurrent, 1) );

        cbvs::g_aScrRes.PushBack(NewRes);
    }

    cbvs::g_aScrRes.Shrink();
    return true;
}

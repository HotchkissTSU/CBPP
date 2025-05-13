#ifndef CBPP_SDK_DATATYPES_H
#define CBPP_SDK_DATATYPES_H

/*
    Definitions for standart I/O datatypes, shared by the engine and the SDK
*/

#include "cdf/cdf.h"
#include <stdint.h>

typedef struct {
    cdf_uint iNameID;
    float fX,fY,fW,fH;
} sdk_Sprite;

//See docs/glyth.jpg
typedef struct {
    uint32_t iKeycode;  //An UTF-32 codepoint
    float fX, fY;       //Drawing origin
    float fWidth;       //Kinda self-explanatory
    float fBearY;       //Y bearing
    float fMaxBBoxY;    //bbox_ymax
} sdk_FontGlyth;

#endif

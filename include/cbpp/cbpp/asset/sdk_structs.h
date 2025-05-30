#ifndef CBPP_SDK_DATATYPES_H
#define CBPP_SDK_DATATYPES_H

/*
    Definitions for standart I/O datatypes, shared by the engine and the SDK.
    The CBPP SDK is designed to be as independent from the engine as possible
*/

#include "cdf/cdf.h"
#include <stdint.h>

typedef struct {
    cdf_uint iNameID;
    uint32_t iX, iY, iW, iH;
} sdk_Sprite;

typedef struct {
    uint32_t m_iResolution;
    uint32_t m_iChannels;
} sdk_ImageInfo;

//See docs/glyth.jpg
typedef struct {
    uint32_t iKeycode;  //An UTF-32 codepoint
    float fX, fY;       //Drawing origin
    float fWidth;       //Kinda self-explanatory
    float fBearY;       //Y bearing
    float fMaxBBoxY;    //bbox_ymax
} sdk_FontGlyth;

#endif

#ifndef CBPP_SDK_DATATYPES_H
#define CBPP_SDK_DATATYPES_H

/*
    Definitions for standart I/O datatypes, shared by the engine and the SDK.
*/

#include "cdf/cdf.h"
#include <stdint.h>

/*
    'cta_mapping' array member.
    Spritsheets store mapping in the regular pixel coordinates.
*/
typedef struct {
    cdf_uint iNameID;
    uint32_t iX, iY, iW, iH;
} sdk_Sprite;

/*
    cta_imginfo
*/
typedef struct {
    uint32_t m_iWidth;
    uint32_t m_iHeight;
    uint32_t m_iChannels; // Spritesheets can store their source images in formats different from RGBA
} sdk_ImageInfo;

/*
    'cff_mapping' array member.
    See docs/glyth.jpg
*/
typedef struct {
    uint32_t iKeycode;  //An UTF-32 codepoint
    float fX, fY;       //Drawing origin
    float fWidth;       //Kinda self-explanatory
    float fBearY;       //Y bearing
    float fMaxBBoxY;    //bbox_ymax
} sdk_FontGlyth;

#endif

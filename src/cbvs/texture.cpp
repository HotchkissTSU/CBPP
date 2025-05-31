#include "cbvs/texture.h"

#include "cbpp/fileio.h"
#include "cbpp/error.h"
#include "cbvs/error_check.h"
#include "cbpp/texture_default.h"
#include "cbpp/geomath.h"

namespace cbvs {
    struct PixelRGB { char r,g,b; };
    struct PixelLA  { char l,a; };

    cbpp::Color ConvertPixelRGB(const uint8_t* aSourceRGB) {
        const PixelRGB Data = *(const PixelRGB*)(aSourceRGB);
        return cbpp::Color(Data.r, Data.g, Data.b, 255);
    }

    cbpp::Color ConvertPixelLA(const uint8_t* aSourceLA) {
        const PixelLA Data = *(const PixelLA*)(aSourceLA);
        return cbpp::Color(Data.l, Data.l, Data.l, Data.a);
    }

    cbpp::Color ConvertPixelL(const uint8_t* aSourceL) {
        const char Data = *(const char*)(aSourceL);
        return cbpp::Color(Data, Data, Data, 255);
    }

    bool ConvertImage(int iSrcChannels, const uint8_t* aSource, cbpp::Color* pTarget, texres_t iW, texres_t iH) {
        cbpp::Color (*pConvFunc)(const uint8_t*);

        switch ( iSrcChannels ) {
            case 1:
                pConvFunc = &ConvertPixelL; break;

            case 2:
                pConvFunc = &ConvertPixelLA; break;

            case 3:
                pConvFunc = &ConvertPixelRGB; break;

            default:
                cbpp::PushError(cbpp::ERROR_IO, "Invalid source image format for converting it to RGBA");
                return false;
        }

        for( texres_t x = 0; x < iW; x++ ) {
            for(texres_t y = 0; y < iH; y++) {
                const uint8_t* pPixel = aSource + x*(y+1)*iSrcChannels;
                pTarget[x*y + x] = pConvFunc(pPixel);
            }
        }

        return true;
    }

    bool Image::IsSheetCompatible() const noexcept {
        return cbpp::math::IsPOT(m_iH) && cbpp::math::IsPOT(m_iW);
    }

    Image::Image(const uint8_t* aPixels, texres_t iW, texres_t iH, int iChannels) noexcept : m_iH(iH), m_iW(iW) {
        m_aImageData = cbpp::Malloc<cbpp::Color> (iW*iH);
        ConvertImage(iChannels, aPixels, m_aImageData, iW, iH);
    }

    Image::Image(texres_t iW, texres_t iH) noexcept {
        m_aImageData = cbpp::Malloc<cbpp::Color> (iW*iH);
    }
    
    Image::~Image() {
        if(m_aImageData != NULL) {
            cbpp::Free(m_aImageData);
        }
    }
}

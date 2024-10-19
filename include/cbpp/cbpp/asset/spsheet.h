#ifndef CBPP_SPSHEET_H
#define CBPP_SPSHEET_H

#include "cbpp/vec2.h"
#include "json/libjson.h"

#include <cstdint>
#include <map>
#include <string>
#include <stddef.h>

namespace cbpp {
    class Image {
        public:
            Image() = default;
            Image(const uint8_t* bytes, size_t blen);

            bool Load(const uint8_t* bytes, size_t blen);

        private:
            uint32_t w = 0, h = 0;
            uint8_t* data = nullptr;
    };

    struct SpriteData_h {
        float_t x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
    };

    class SpriteSheet {
        public:
            SpriteSheet() = default;
            SpriteSheet( Image& image, JSONNODE* json_data );

            bool Load( Image& img_data, JSONNODE* json_data );

        private:
            std::map<std::string, SpriteData_t> data;
            Image img_data;
    };
}

#endif
#ifndef CBPP_ASSET_SPRITE_H
#define CBPP_ASSET_SPRITE_H

#include <cstdint>

#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"

namespace cbpp {
    class Sprite {
        public:
            Sprite() = default;

        private:
            uint16_t width, height;
    };
}

#endif
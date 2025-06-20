#ifndef CBVS_RENDER_H
#define CBVS_RENDER_H

#include "cbvs/shader.h"
#include "cbvs/texture.h"
#include "cbpp/color.h"
#include "cbpp/asset/spsheet.h"

//The engine will try to compose as much spritesheets as possible to a single huge texture this size
#define CBVS_SPRITEBATCH_SIZE 2048

//We assume that the minimal single spritesheet`s size is this
#define CBVS_SPRITEMAPPER_MINSIZE 64

namespace cbvs {
    class IRenderable {
        public:
            IRenderable(const char* sPipeName);

            void CallPipe() const noexcept;

            virtual void Render() const noexcept = 0;

        protected:
            const Pipe* m_pPipe;
    };

    extern cbpp::float_t g_fScreenRatio;
    extern cbpp::Vec2 g_vScreenSize;
}

namespace cbvs {
    bool InitRender() noexcept;
    void CleanupRender() noexcept;

    /*
        A tool for mapping multiple spritesheets to a single big texture for optimising on draw calls.
        This goon weights up to 16 megabytes, so do not attempt to allocate him on the stack.
    */
    class SpriteComposer {
        public:
            // Reset the mapper for reuse
            void Reset();

            // Output all currently mapped spritesheets to an OpenGL texture
            GLuint GenerateTexture() const;

            // Find a free mapping location for the given sheet
            bool LocateFreeRegion(int iW, int iH, int& iX, int& iY) const;

            // Check whether this rectangle collides with any occupied tiles (true if no collisions occured)
            bool ScanRegion(int iX, int iY, int iW, int iH) const;

            // Change this region`s status
            void MarkRegionAs(int iX, int iY, int iW, int iH, bool bMark);

            void BlitImage(const cbvs::Image& Source, texres_t iX, texres_t iY);

            void PrintMappingInfo() const;

        private:
            cbpp::Color m_aImage[CBVS_SPRITEBATCH_SIZE][CBVS_SPRITEBATCH_SIZE];

            /*
                We divide our image with tiles of size CBVS_SPRITEMAPPER_MINSIZE. This is used to
                optimize the sheets packaging algorythm. It`s possible to use even less memory, but i don`t want to
                mess around with bitmasks.
            */
            char m_aMappingInfo[CBVS_SPRITEBATCH_SIZE / CBVS_SPRITEMAPPER_MINSIZE][CBVS_SPRITEBATCH_SIZE / CBVS_SPRITEMAPPER_MINSIZE];
    };
    
    struct SpriteVertex {
        cbpp::Vec2 vPos;
        cbpp::Vec2 vUV;
    };

    void RenderSprite(cbpp::spriteid_t, cbpp::Vec2 vPos, cbpp::Vec2 vScale, cbpp::float_t fAngle, cbpp::Color iColor, cbpp::float_t fDepth = 1.0f);
}

#endif

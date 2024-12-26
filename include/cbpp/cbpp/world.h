#ifndef CBPP_WORLD_H
#define CBPP_WORLD_H

#include "cbpp/entity/BaseEntity.h"

//The initial size of the world`s entity buffer
#define CBPP_ENTBUFFER_INIT_SIZE 1024

//Add this amount of places to the entity buffer when it`s filled up
#define CBPP_ENTBUFFER_ADD_AMOUNT 128

//These values are recommended to be a power of 2 to guarantee their divisibility
#define CBPP_MAP_CHUNK_DEFAULT 16

#define CBPP_MAP_SIZE_DEFAULT 1024

/*
    The map consists of a set of static world geometry - polygons,
    and of dynamic entity objects.

    All polygons are splitted by chunks - a square tiles of equal size.
    One polygon can be present in more than one chunk.

    Chunks are used to optimise collision detection and some entity logic.
*/

namespace cbpp {
    typedef uint32_t polyid_t;

    struct MapPolygon {
        int placeholder;
    };

    struct MapChunk {
        size_t iNumPolys = 0;               //The list of all polygons that are inside this chunk with at least 1 point
        polyid_t* aChunkPolygons = NULL;
    };

    //A class to rule them all
    class World {
        public:
            World();
            
            World(const World& refOther) = delete;
            void operator=(const World& refOther) = delete;

            //Convert a world 2D coordinates into chunk index
            size_t WorldToChunk(Vec2 vWorldPos) const noexcept;

            //Convert a chunk index into 2D world coordinates
            Vec2 ChunkToWorld(size_t iChunkIndex) const noexcept;

            entid_t FindFreeEntityID();
            entid_t SpawnEntity(BaseEntity* eEntity);
            void AddEntityPlaces(entid_t iAmount);

            bool IsInsideWorld(Vec2 vWorldPos) const noexcept;

            //Find a chunk closest to the given point, excluding (if possible) the chunk the point is in
            size_t LocateClosestChunk(Vec2 vWorldPos) const;

            ~World();

        private:
            void AllocateEntityBuffer();
            void AllocateChunks(size_t iAmount);

            BaseEntity** m_aEntityBuffer = NULL;
            entid_t m_iEntityBufferLen = 0;

            uint64_t m_iWorldSize = CBPP_MAP_SIZE_DEFAULT;
            uint32_t m_iChunkSize = CBPP_MAP_CHUNK_DEFAULT, m_iChunksPerBorder = CBPP_MAP_SIZE_DEFAULT / CBPP_MAP_CHUNK_DEFAULT;

            MapChunk* m_aChunks = NULL;
    };
}

#endif

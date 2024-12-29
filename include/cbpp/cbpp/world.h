#ifndef CBPP_WORLD_H
#define CBPP_WORLD_H

#include "cbpp/entity/BaseEntity.h"

//The initial size of the world`s entity buffer
#define CBPP_ENTBUFFER_INIT_SIZE 1024

//Add this amount of places to the entity buffer when it`s filled up
#define CBPP_ENTBUFFER_ADD_AMOUNT 128

//A single chunk will be this power of 2 in size
#define CBPP_MAP_CHUNK_POW 4
#define CBPP_MAP_CHUNK_DEFAULT 1 << CBPP_MAP_CHUNK_POW

#define CBPP_MAP_SIZE_DEFAULT 1024

/*
    The map consists of a set of static world geometry - polygons,
    and of dynamic entity objects.

    All polygons are splitted by chunks - a square tiles of equal size.
    One polygon can be present in more than one chunk.

    Chunks are used to optimise collision detection and some entity logic.

    The world is a box with the coordinates originorigin in it`s center

    *-------*
    |  -Y   |
    |-X O +X|
    |  +Y   |
    *-------*
*/

namespace cbpp {
    typedef uint32_t polyid_t;

    struct MapPolygon {
        int placeholder;
    };

    struct MapChunk {
        size_t iNumPolys = 0;               //The list of all polygons that are inside this chunk with at least 1 point
        polyid_t* aChunkPolygons = NULL;

        MapChunk(size_t iPolyCount);
        ~MapChunk();
    };

    //A class to rule them all
    class World {
        public:
            World();
            World(uint64_t iWorldSize);
            
            World(const World& refOther) = delete;
            void operator=(const World& refOther) = delete;

            //Convert a world 2D coordinates into chunk index
            size_t WorldToChunk(Vec2 vWorldPos) const noexcept;

            //Convert a chunk index into 2D world coordinates
            Vec2 ChunkToWorld(size_t iChunkIndex) const noexcept;

            entid_t FindFreeEntityID();
            void AddEntityPlaces(entid_t iAmount);

            entid_t SpawnEntity(BaseEntity* eEntity);
            void RemoveEntity(entid_t iIndex);
            void RemoveAllEntities();

            //Perform a search in the given box. The amount of results is returned
            entid_t FindEntitiesInBox(Vec2 vPos1, Vec2 vPos2);

            //Perform a search in the given circle. The amount of results is returned
            entid_t FindEntitiesInCircle(Vec2 vCenter, float_t fRadius);

            bool IsInsideWorld(Vec2 vWorldPos) const noexcept;

            //[FIXME] Find a chunk closest to the given point, excluding (if possible) the chunk the point is in
            size_t LocateClosestChunk(Vec2 vWorldPos) const;

            uint32_t ChunksPerBorder() const noexcept {
                return m_iChunksPerBorder;
            }

            ~World();

        private:
            void AllocateEntityBuffer();
            void AllocateChunks();

            BaseEntity** m_aEntityBuffer = NULL;
            size_t* m_aEntityChunks = NULL;
            entid_t m_iEntityBufferLen = 0;

            uint64_t m_iWorldSize = CBPP_MAP_SIZE_DEFAULT;
            uint64_t m_iHalfWorld = CBPP_MAP_SIZE_DEFAULT / 2;
            uint32_t m_iChunkSize = CBPP_MAP_CHUNK_DEFAULT, m_iChunksPerBorder = CBPP_MAP_SIZE_DEFAULT >> CBPP_MAP_CHUNK_POW;

            MapChunk* m_aChunks = NULL;
    };
}

#endif

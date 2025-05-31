#include "cbpp/world.h"

#include <cstring>
#include <math.h>

#include "cbpp/cb_alloc.h"
#include "cbpp/geomath.h"

namespace cbpp {
    World::World() : m_aFindBuffer(CBPP_EFINDBUFFER_INIT_SIZE) {
        this->AllocateEntityBuffer();
    }

    World::World(size_t iWorldSize) {
        m_iWorldSize = iWorldSize;
        m_iHalfWorld = iWorldSize >> 1;
        m_iChunksPerBorder = iWorldSize / m_iChunkSize;

        AllocateEntityBuffer();
        AllocateChunks();
    }

    void World::AllocateEntityBuffer() {
        if(m_aEntityBuffer != NULL) {
            return;
        }

        m_aEntityBuffer = Malloc<BaseEntity*>(CBPP_ENTBUFFER_INIT_SIZE);
        m_iEntityBufferLen = CBPP_ENTBUFFER_INIT_SIZE;
    }

    void World::AllocateChunks() {
        if(m_aChunks != NULL) {
            return;
        }

        m_aChunks = Malloc<MapChunk>(m_iChunksPerBorder*m_iChunksPerBorder);
    }

    entid_t World::FindFreeEntityID() {
        BaseEntity* pCurrent;
        entid_t iCounter = 0;
        while(pCurrent != NULL && iCounter < m_iEntityBufferLen) {
            iCounter++;
            pCurrent = m_aEntityBuffer[iCounter];
        }

        return iCounter;
    }

    void World::AddEntityPlaces(entid_t iAmount) {
        entid_t iOldLen = m_iEntityBufferLen;
        m_iEntityBufferLen += iAmount;
        BaseEntity** pTemp = Realloc<BaseEntity*>(m_aEntityBuffer, m_iEntityBufferLen);
        if(pTemp != NULL) {
            m_aEntityBuffer = pTemp;
        }
    }

    entid_t World::SpawnEntity(BaseEntity* eEntity) {
        if( eEntity == NULL ) { return -1; }
        if( eEntity->GetSpawned() ) { return -1; }

        entid_t iFreeID = this->FindFreeEntityID();

        //we are running out of space
        if(iFreeID > m_iEntityBufferLen - 16) {
            this->AddEntityPlaces(CBPP_ENTBUFFER_ADD_AMOUNT);
        }

        eEntity->SetSpawned(true);

        m_iEntities = m_iEntityBufferLen;
        while( m_iEntities > 0) {
            if(m_aEntityBuffer[m_iEntities] != NULL) {
                break;
            } //here we are searching the last valid entity id

            m_iEntities--;
        }

        m_aEntityBuffer[iFreeID] = eEntity;
        return iFreeID;
    }
    
    BaseEntity* World::EntityByID(entid_t iIndex) {
        if(iIndex >= m_iEntityBufferLen) {
            return NULL;
        }

        return m_aEntityBuffer[iIndex];
    }

    void World::RemoveEntity(entid_t iIndex) {
        BaseEntity* eTemp = m_aEntityBuffer[iIndex];
        if(eTemp != NULL) {
            delete eTemp;
            m_aEntityBuffer[iIndex] = NULL;
        }

        if(iIndex == m_iEntities) {
            m_iEntities--;
        }
    }
    
    Vec2 World::ChunkToWorld(size_t iChunkIndex) const noexcept {
        size_t iX, iY;

        iX = iChunkIndex % m_iChunksPerBorder;
        iY = (iChunkIndex - iX) / m_iChunksPerBorder;

        return Vec2( (float_t)(iX << CBPP_MAP_CHUNK_POW) - m_iHalfWorld, //The chunk coordinates origin is in the top-left corner unlike the world ones
                     (float_t)(iY << CBPP_MAP_CHUNK_POW) - m_iHalfWorld );
    }

    size_t World::WorldToChunk(Vec2 vWorldPos) const noexcept {
        size_t iX, iY, iIndex;

        vWorldPos = vWorldPos + m_iHalfWorld;

        iX = (size_t)(vWorldPos.x) >> CBPP_MAP_CHUNK_POW;
        iY = (size_t)(vWorldPos.y) >> CBPP_MAP_CHUNK_POW;

        iIndex = iX + iY*m_iChunksPerBorder;
        return iIndex;
    }

    bool World::IsInsideWorld(Vec2 vWorldPos) const noexcept {
        static Vec2 s_vHalfWorld(m_iHalfWorld);
        return math::InRange(vWorldPos, s_vHalfWorld*-1, s_vHalfWorld);
    }

    //Currently broken
    size_t World::LocateClosestChunk(Vec2 vWorldPos) const {
        float_t fX, fY;
        size_t iChunkX, iChunkY;

        //The coordinates of the top-left corner of the chunk, in the world space
        fX = floor(vWorldPos.x / m_iChunkSize) * m_iChunkSize;
        fY = floor(vWorldPos.y / m_iChunkSize) * m_iChunkSize;

        iChunkX = (size_t)floor(vWorldPos.x) >> CBPP_MAP_CHUNK_POW;
        iChunkY = (size_t)floor(vWorldPos.y) >> CBPP_MAP_CHUNK_POW;

        float_t aDists[4]; //Left, Top, Right, Bottom

        aDists[0] = vWorldPos.x - fX;
        aDists[1] = vWorldPos.y - fY;
        aDists[2] = m_iChunkSize - aDists[0];
        aDists[3] = m_iChunkSize - aDists[1];

        float_t fMin = m_iChunkSize*2;
        int iIndex = -1;

        for(int i = 0; i < 4; i++) {
            float_t fCurrent = aDists[i];
            if(fCurrent < fMin) {
                fMin = fCurrent;
                iIndex = i;
            }
        }

        if(iIndex == -1) { //how is this even possible?
            return iChunkX + iChunkY*m_iChunkSize;
        }

        switch(iIndex) {
            case 0:
                if(iChunkX > 1) {
                    iChunkX--;
                    break;
                }
            
            case 1:
                if(iChunkY > 1) {
                    iChunkY--;
                    break;
                }

            case 2:
                if(iChunkX < m_iChunksPerBorder-1) {
                    iChunkX++;
                    break;
                }

            case 3:
                if(iChunkY < m_iChunksPerBorder-1) {
                    iChunkY++;
                    break;
                }
        }

        return iChunkX + iChunkY*m_iChunkSize;
    }

    void World::FindEntitiesInBox(Vec2 vPos1, Vec2 vPos2) {
        m_aFindBuffer.Clear();
        for(entid_t i = 0; i < m_iEntities; i++) {
            BaseEntity* eCandidate = m_aEntityBuffer[i];

            if(eCandidate != NULL) {
                if( math::InRange(eCandidate->GetPosition(), vPos1, vPos2) ) {
                    m_aFindBuffer.PushBack(i);
                }
            }
        }
    }

    void World::FindEntitiesInCircle(Vec2 vOrigin, float_t fRadius) {
        const float_t fRadSquare = fRadius*fRadius;
        m_aFindBuffer.Clear();
        for(entid_t i = 0; i < m_iEntities; i++) {
            BaseEntity* eCandidate = m_aEntityBuffer[i];

            if(eCandidate != NULL) {
                if( math::SquareDistance(vOrigin, eCandidate->GetPosition()) <= fRadSquare ) {
                    m_aFindBuffer.PushBack(i);
                }
            }
        }
    }

    void World::FindEntitiesInSector(Vec2 vOrigin, Vec2 vDirection, float_t fRadius, float_t fAngle) {
        const float_t fRadSquare = fRadius*fRadius;
        m_aFindBuffer.Clear();
        for(entid_t i = 0; i < m_iEntities; i++) {
            BaseEntity* eCandidate = m_aEntityBuffer[i];

            if(eCandidate != NULL) {
                if( math::SquareDistance(vOrigin, eCandidate->GetPosition()) <= fRadSquare ) {
                    Vec2 vPos = eCandidate->GetPosition();
                    float_t fDot = vDirection.Dot(vPos - vOrigin);

                    if(fDot <= fAngle) {
                        m_aFindBuffer.PushBack(i);
                    }
                }
            }
        }
    }

    List<entid_t>& World::FindResults() noexcept {
        return m_aFindBuffer;
    }

    World::~World() {
        //Free all entities
        if(m_aEntityBuffer != NULL) {
            for(entid_t i = 0; i < m_iEntityBufferLen; i++) {
                BaseEntity* eCurrent = m_aEntityBuffer[i];
                if(eCurrent != NULL) {
                    delete eCurrent; //this pointer is originally produced by new
                }
            }

            Free(m_aEntityBuffer);
        }

        //Free map chunks
        Free(m_aChunks);
    }

    MapChunk::MapChunk(size_t iPolyAmount) {
        
    }

    MapChunk::~MapChunk() {
        
    }
}

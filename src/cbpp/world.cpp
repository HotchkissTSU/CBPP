#include "cbpp/world.h"

#include <cstring>
#include <math.h>

#include "cbpp/cb_alloc.h"
#include "cbpp/geomath.h"

namespace cbpp {
    World::World() {
        this->AllocateEntityBuffer();
    }

    void World::AllocateEntityBuffer() {
        if(m_aEntityBuffer != NULL) {
            return;
        }

        m_aEntityBuffer = Allocate<BaseEntity*>(CBPP_ENTBUFFER_INIT_SIZE);
        m_iEntityBufferLen = CBPP_ENTBUFFER_INIT_SIZE;
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
        BaseEntity** pTemp = Reallocate<BaseEntity*>(m_aEntityBuffer, m_iEntityBufferLen-iAmount, m_iEntityBufferLen);
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

        m_aEntityBuffer[iFreeID] = eEntity;
        return iFreeID;
    }

    Vec2 World::ChunkToWorld(size_t iChunkIndex) const noexcept {
        return Vec2(iChunkIndex % m_iChunksPerBorder, iChunkIndex / m_iChunksPerBorder);
    }

    size_t World::WorldToChunk(Vec2 vWorldPos) const noexcept {
        size_t iX, iY, iIndex;

        iX = (size_t)floor(vWorldPos.x) / m_iChunkSize;
        iY = (size_t)floor(vWorldPos.y) / m_iChunkSize;

        iIndex = iX + iY*m_iChunksPerBorder;

        return iIndex;
    }

    bool World::IsInsideWorld(Vec2 vWorldPos) const noexcept {
        static Vec2 s_vHalfWorld(m_iWorldSize >> 1);
        return math::InRange(vWorldPos, s_vHalfWorld*-1, s_vHalfWorld);
    }

    size_t World::LocateClosestChunk(Vec2 vWorldPos) const {
        float_t fX, fY;
        size_t iChunkX, iChunkY;

        //The coordinates of the top-left corner of the chunk, in the world space
        fX = floor(vWorldPos.x / m_iChunkSize) * m_iChunkSize;
        fY = floor(vWorldPos.y / m_iChunkSize) * m_iChunkSize;

        iChunkX = (size_t)floor(vWorldPos.x) / m_iChunkSize;
        iChunkY = (size_t)floor(vWorldPos.y) / m_iChunkSize;

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

    World::~World() {
        //Free all the entities
        if(m_aEntityBuffer != NULL) {
            for(entid_t i = 0; i < m_iEntityBufferLen; i++) {
                BaseEntity* eCurrent = m_aEntityBuffer[i];
                if(eCurrent != NULL) {
                    delete eCurrent; //this pointer is originally produced by new
                }
            }

            Free<BaseEntity*>(m_aEntityBuffer, m_iEntityBufferLen);
        }
    }
}

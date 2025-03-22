#ifndef CBPP_ASSET_IRESOURCE_H
#define CBPP_ASSET_IRESOURCE_H

#include <utility>
#include <new>

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "cbpp/geomath.h"
#include "cbpp/bit.h"
#include "cbpp/error.h"

#define CBPP_POOL_APPEND_AMOUNT sizeof(mask_t)

#define pooled_class(className) class className : public cbpp::IResource<className>
#define pooled_struct(className) struct className : public cbpp::IResource<className>

namespace cbpp {
    template <typename type_t, typename mask_t = uint32_t> struct PoolChunk {
        type_t m_aChunk[ sizeof(mask_t)*8 ];
        mask_t m_iUseMask = 0;

        type_t& operator[](size_t iIndex) noexcept {
            #ifdef CBPP_DEBUG
            if(iIndex > sizeof(mask_t)*8) {
                CbThrowErrorf("Local chunk index %zu is out of bounds (%zu)", iIndex, sizeof(mask_t)*8);
            }
            #endif

            return m_aChunk[iIndex];
        }

        //Find a free place in this chunk and mark it as used (upon success)
        //Pass NULL as argument to check if this chunk has any free space at all
        bool Allocate(size_t* pIndex) noexcept {
            uint8_t iIndex;
            for(uint8_t i = 0; i < sizeof(mask_t)*8; i++) {
                if( GetBit<mask_t>(m_iUseMask, i) == 0 ) {
                    if(pIndex != NULL) { 
                        *pIndex = i;
                    }
                    SetBit<mask_t>(m_iUseMask, i, 1);
    
                    return true;
                }
            }
            return false;
        }

        //Mark this place as free
        void Free(size_t iIndex) noexcept {
            SetBit<mask_t>(m_iUseMask, iIndex, 0);
        }
    };

    /*
        Pool allocator.
        Stores same objects continuously, so they probably will have a chance to
        fit inside the cache line and boost processing speed to hypersonic values.

        type_t - what type to store
        mask_t - which type is used as a bitmask to mark used/free space
    */
    template <typename type_t, typename mask_t = uint32_t> class Pool {
        typedef PoolChunk<type_t, mask_t> chunk_t;

        public:
            Pool() noexcept = default;
            Pool(size_t iInitialSize) noexcept : m_iChunks(iInitialSize) {
                m_aChunks = (chunk_t*) malloc(sizeof(chunk_t) * iInitialSize);
                memset(m_aChunks, 0, sizeof(chunk_t)*m_iChunks);
            }

            //Check if this index holds a valid, properly allocated object
            bool CheckIndex(size_t iIndex) noexcept {
                size_t iLocalIndex, iChunkIndex;
                math::LinearToPlanar(sizeof(mask_t)*8, iIndex, iLocalIndex, iChunkIndex);

                if(iLocalIndex > sizeof(mask_t)*8 || iChunkIndex > m_iChunks) {
                    return false;
                }

                return GetBit<mask_t>(m_aChunks[iChunkIndex].m_iUseMask, iLocalIndex);
            }

            //Locate and mark as used a new index for the object storage. 
            bool FindPlace(size_t* pIndex) noexcept {
                size_t iLocalIndex;
                for(size_t i = 0; i < m_iChunks; i++) {
                    if(m_aChunks[i].Allocate(&iLocalIndex)) {
                        if(pIndex != NULL) {
                            *pIndex = math::PlanarToLinear(sizeof(mask_t)*8, iLocalIndex, i);
                        }
                        return true;
                    }
                }

                return false;
            }

            //Push N new chunks to the pool
            bool PushChunks(size_t iChunks) noexcept {
                chunk_t* pTest = (chunk_t*) realloc(m_aChunks, sizeof(chunk_t) * (m_iChunks + iChunks));
                if(pTest == NULL) {
                    return false;
                }

                memset(pTest + m_iChunks, 0, sizeof(chunk_t)*iChunks);

                m_aChunks = pTest;
                m_iChunks += iChunks;

                return true;
            }

            //Allocate and initialize a new object
            //If there is no free space left, new chunks are added
            template <typename... args_t> type_t* Allocate(args_t&&... vaArgs) noexcept {
                size_t iIndex;
                if(!FindPlace(&iIndex)) {
                    if(!PushChunks(CBPP_POOL_APPEND_AMOUNT)) {
                        #ifdef CBPP_DEBUG
                        CbThrowErrorf("Failed to append %zu chunks to the pool", CBPP_POOL_APPEND_AMOUNT);
                        #endif

                        return NULL;
                    }
                    FindPlace(&iIndex);
                }

                return new(&At(iIndex)) type_t(std::forward<args_t>(vaArgs)...);
            }

            //Free a previously allocated object
            void Free(type_t* pObject) noexcept {
                if(pObject == NULL) { return; }

                size_t iOffset = (size_t)pObject - (size_t)m_aChunks;

                if(iOffset > m_iChunks * sizeof(chunk_t)) {
                    #ifdef CBPP_DEBUG
                    CbThrowErrorf("Wrong pointer '%x': not from this pool", pObject);
                    #endif
                    return;
                }

                size_t iChunkIndex = iOffset / sizeof(chunk_t);
                size_t iChunk = (size_t)m_aChunks + sizeof(chunk_t)*iChunkIndex;
                size_t iLocalIndex = ((size_t)pObject - iChunk) / sizeof(type_t);

                m_aChunks[iChunkIndex].Free(iLocalIndex);
                m_aChunks[iChunkIndex][iLocalIndex].~type_t();
            }

            //Free an object by it`s internal index
            void FreeByIndex(size_t iIndex) noexcept {
                if(!CheckIndex(iIndex)) { return; }

                size_t iChunkIndex, iLocalIndex;
                math::LinearToPlanar(sizeof(mask_t)*8, iIndex, iLocalIndex, iChunkIndex);

                m_aChunks[iChunkIndex].Free(iLocalIndex);
                m_aChunks[iChunkIndex][iLocalIndex].~type_t();
            }

            //Access a specific object. Use carefully as this index can hold an uninitialized memory block
            type_t& At(size_t iIndex) noexcept {
                size_t iLocalIndex, iChunkIndex;
                math::LinearToPlanar(sizeof(mask_t)*8, iIndex, iLocalIndex, iChunkIndex);

                #ifdef CBPP_DEBUG
                if(iChunkIndex > m_iChunks) {
                    CbThrowErrorf("Index %zu is out of bounds (%zu)", iIndex, m_iChunks*sizeof(mask_t)*8);
                }
                #endif

                return m_aChunks[iChunkIndex][iLocalIndex];
            }

            ~Pool() noexcept {
                for(size_t i = 0; i < m_iChunks; i++) {
                    for(size_t j = 0; j < sizeof(mask_t)*8; j++) {
                        if(GetBit(m_aChunks[i].m_iUseMask, j)) {
                            m_aChunks[i].Free(j);
                            m_aChunks[i][j].~type_t();
                        }
                    }
                }
                free(m_aChunks);
            }

        private:
            chunk_t* m_aChunks = NULL;
            size_t m_iChunks = 0;
    };

    template <typename type_t, uint16_t t_iInitialSize = 4, typename mask_t = uint32_t> class IResource {
        public:
            static Pool<type_t, mask_t>* GetGlobalPool(size_t iInitialSize) noexcept {
                static Pool<type_t, mask_t> s_Pool(iInitialSize);
                return &s_Pool;
            }

            template <typename... args_t> static type_t* Allocate(args_t... vaArgs) noexcept {
                return GetGlobalPool(t_iInitialSize)->Allocate(vaArgs...);
            }

            static void Free(type_t* pObject) noexcept {
                GetGlobalPool(t_iInitialSize)->Free(pObject);
            }
    };
}

#endif

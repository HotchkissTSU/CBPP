#ifndef CBPP_ASSET_IRESOURCE_H
#define CBPP_ASSET_IRESOURCE_H

#include <utility>

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "cbpp/geomath.h"
#include "cbpp/bit.h"
#include "cbpp/error.h"

#define CBPP_POOL_APPEND_AMOUNT sizeof(mask_t)

namespace cbpp {
    template <typename type_t, typename mask_t = uint32_t> struct PoolChunk {
        mask_t m_iUseMask = 0;
        type_t m_aChunk[ sizeof(mask_t)*8 ];

        type_t& operator[](size_t iIndex) noexcept {
            #ifdef CBPP_DEBUG
            if(iIndex > sizeof(mask_t)*8) {
                CbThrowErrorf("Local pool chunk index %zu is out of bounds (%zu)", iIndex, sizeof(mask_t)*8);
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
                        SetBit<mask_t>(m_iUseMask, i, 1);
                    }
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

    //Pool allocator.
    //Used to store large amounts of the same objects
    template <typename type_t, typename mask_t = uint32_t> class Pool {
        typedef PoolChunk<type_t, mask_t> chunk_t;

        public:
            Pool() = default;
            Pool(size_t iInitialSize) noexcept : m_iChunks(iInitialSize) {
                m_aChunks = (chunk_t*) malloc(sizeof(chunk_t) * iInitialSize);
            }

            bool CheckIndex(size_t iIndex) noexcept {
                size_t iLocalIndex, iChunkIndex;
                math::LinearToPlanar(sizeof(mask_t)*8, iIndex, iLocalIndex, iChunkIndex);

                if(iLocalIndex > sizeof(mask_t)*8 || iChunkIndex > m_iChunks) {
                    return false;
                }

                return GetBit<mask_t>(m_aChunks[iChunkIndex].m_iUseMask, iLocalIndex);
            }

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

            bool PushChunks(size_t iChunks) noexcept {
                chunk_t* pTest = (chunk_t*) realloc(m_aChunks, sizeof(chunk_t) * (m_iChunks + iChunks));
                if(pTest == NULL) {
                    return false;
                }

                m_aChunks = pTest;
                m_iChunks += iChunks;

                return true;
            }

            template <typename... args_t> type_t* Allocate(size_t* pIndex, args_t&&... vaArgs) noexcept {
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

                if(pIndex != NULL) {
                    *pIndex = iIndex;
                }

                return new(&At(iIndex)) type_t(std::forward<args_t>(vaArgs)...);
            }

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

                //Apic (from the word "ape") duplication of the code because of endless template recursion happening when i
                //try to call the first function with first argument as NULL

                return new(&At(iIndex)) type_t(std::forward<args_t>(vaArgs)...);
            }

            void Free(size_t iIndex) noexcept {
                if(!CheckIndex(iIndex)) { return; }

                delete &At(iIndex);

                size_t iLocalIndex, iChunkIndex;
                math::LinearToPlanar(sizeof(mask_t)*8, iIndex, iLocalIndex, iChunkIndex);

                m_aChunks[iChunkIndex].Free(iLocalIndex);
            }

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

        private:
            chunk_t* m_aChunks = NULL;
            size_t m_iChunks = 0;
    };

    template <typename type_t, typename mask_t> Pool<type_t, mask_t>* AllocateGlobalPool(size_t iInitialSize) noexcept {
        static Pool<type_t, mask_t> s_Pool(iInitialSize);
        return &s_Pool;
    }
}

#endif

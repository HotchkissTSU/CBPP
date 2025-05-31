#ifndef CBPP_ARRAY_H
#define CBPP_ARRAY_H

#include "cbpp/cb_alloc.h"

namespace cbpp {
    template <typename T> class Array {
        public:
            Array() = default;
            Array(const T* pMemory, size_t iSize) : m_iSize(iSize) {
                m_pMemory = Malloc<T>(iSize);
                memcpy(m_pMemory, pMemory, iSize * sizeof(T));
            }

            Array(Array<T>&& Other) {
                m_iSize = Other.m_iSize;
                m_pMemory = Other.m_pMemory;
            }

            Array(size_t iSize) : m_iSize(iSize) {
                m_pMemory = Calloc<T>(iSize);
            }

            T& At(size_t iIndex) {
                #ifdef CBPP_DEBUG
                if(iIndex >= m_iSize) {
                    CbThrowErrorf("Array index %lu is out of range (%lu)", iIndex, m_iSize);
                }
                #endif

                return m_pMemory[iIndex];
            }

            T const& At(size_t iIndex) const {
                #ifdef CBPP_DEBUG
                if(iIndex >= m_iSize) {
                    CbThrowErrorf("Array index %lu is out of range (%lu)", iIndex, m_iSize);
                }
                #endif

                return const_cast<T const&>(m_pMemory[iIndex]);
            }

            size_t Length() const {
                return m_iSize;
            }

            void Resize(size_t iNewSize) {
                if(iNewSize < m_iSize) {
                    for(size_t i = iNewSize; i < m_iSize; i++) {
                        m_pMemory.~T();
                    }
                }

                m_pMemory = Realloc<T>(m_pMemory, iNewSize);
            }

            void Clear() {
                for(size_t i = 0; i < m_iSize; i++) {
                    m_pMemory.~T();
                }

                Free(m_pMemory);
            }

            ~Array() {
                if(m_pMemory != NULL) {
                    this->Clear();
                    Free(m_pMemory);
                }
            }

        protected:
            T* m_pMemory = NULL;
            size_t m_iSize = 0;
    };
}

#endif

#ifndef CBPP_TTYPE_List_H
#define CBPP_TTYPE_List_H

/*
    0   - allocate two times more memory each time (like std::vector does)
    N>0 - add N pieces upon each re-allocation
*/
#define CBPP_LIST_MODE 0 

#include "cbpp/ttype/array.h"

namespace cbpp {
    //Array-like container with push optimisations
    template <typename T> class List {
        public:
            List() = default;

            List(const List<T>& other) {
                m_array.Resize(other.Length());
                m_len_imag = m_array.Length();
                for(size_t i = 0; i < m_len_imag; i++) {
                    m_array.At(i) = other.At(i);
                }
            }

            List(const Array<T>& other) : m_array(other) {
                m_len_imag = m_array.Length();
            }

            List(const T* ptr, size_t ptr_ln) : m_array(ptr, ptr_ln) {
                m_len_imag = m_array.Length();
            }
            
            List(size_t ln) : m_array(ln) {
                m_len_imag = m_array.Length();
            }
            
            bool PushBack(const T& value) {
                m_len_imag++;
                bool result = true;
                if(m_len_imag > m_array.Length()) { //we need to up-allocate
                    #if CBPP_LIST_MODE == 0
                        size_t new_len;
                        if(m_array.Length() != 0) {
                            new_len = m_array.Length() * 2;
                        }else{
                            new_len = 1;
                        }
                    #else
                        size_t new_len = m_array.Length() + (size_t)CBPP_LIST_MODE;
                    #endif

                    result = m_array.Resize(new_len);
                }

                m_array.At(m_len_imag-1) = value;

                return result;
            }

            bool PopBack() {
                if(m_len_imag >= 1) {
                    m_len_imag--;
                }else{
                    return false;
                }

                bool result = true;

                if(m_len_imag*2 < m_array.Length() && m_len_imag != 0) { //the physical memory buffer is way bigger than the actual data, so shrink it
                    result = m_array.Resize(m_len_imag);

                    if(!result && std::is_destructible<T>::value) {
                        m_array[m_len_imag].~T(); //it`s so over, the realloc compromised, so we must manually destroy the popped object (hold your ground! leave no quarter!)
                    }

                }else{
                    if( std::is_destructible<T>::value ) {
                        m_array[m_len_imag].~T(); //if no shrink happened, we need to manually destroy the popped object
                    }
                }

                return result;
            }

            T& At(size_t index) {
                if(index >= m_len_imag) {
                    CbThrowWarningf("Index %lu is inside the allocated chunk, but is outside of the list bounds (%lu)", index, m_len_imag);
                }

                return m_array.At(index);
            }

            const T& At(size_t index) const {
                return const_cast<const T&>( At(index) );
            }

            T& operator[](size_t index) {
                return At(index);
            }
            
            bool IsValid() const noexcept {
                return (m_array.IsValid() && (m_len_imag <= m_array.Length()) && m_len_imag != 0);
            }

            //Length of the actually used part of the allocated chunk
            size_t Length() const noexcept {
                return m_len_imag;
            }

            //The full length of the physically alocated memory
            size_t LengthPhys() const noexcept {
                return m_array.Length();
            }

        protected:
            size_t m_len_imag = 0;
            Array<T> m_array;
    };
}

#endif
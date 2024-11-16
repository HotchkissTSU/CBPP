#ifndef CBPP_TTYPE_ARRAY_H
#define CBPP_TTYPE_ARRAY_H

#include <stddef.h>
#include "cbpp/cb_alloc.h"
#include "cbpp/error.h"

namespace cbpp {
    template <typename T> class Array;
    template <typename T> Array<T> operator+(const Array<T>&, const Array<T>&);

    //Wrapper for static C-like arrays
    template <typename T> class Array {
        public:
            Array() = default;

            Array(size_t length) {
                m_array = Allocate<T>(length);
                if(m_array == NULL) {
                    CbThrowErrorf("Failed to allocate Array of size %lu", length);
                }

                m_length = length;
            }

            Array(size_t length, const T& fill_value) {
                m_array = Allocate<T>(length);
                if(m_array == NULL) {
                    CbThrowErrorf("Failed to allocate Array of size %lu", length);
                }

                m_length = length;

                Fill(fill_value);
            }

            Array(const Array<T>& other) {
                Resize(other.Length());

                if(!std::is_trivially_copy_assignable<T>::value) {
                    for(size_t i = 0; i < m_length; i++) {
                        At(i) = other.At(i);
                    }
                }else {
                    memcpy(m_array, other.m_array, sizeof(T) * m_length);
                }
            }

            explicit Array(const T* other, size_t ln) {
                if(other == NULL || ln == 0) {
                    CbThrowError("Invalid pointer is passed to the Array constructor");
                }
            }

            Array<T>& operator = (const Array<T>& other) {
                if(!other.IsValid()) {
                    CbThrowError("Invalid Array is passed to the copy operator");
                }

                if(IsValid()) {
                    Free<T>(m_array, m_length);
                }

                m_array = Allocate<T>(other.Length());
                if(m_array == NULL) {
                    CbThrowErrorf("Failed to allocate Array of size %lu", other.Length());
                }

                memcpy(m_array, other.CArr(), sizeof(T)*other.Length());
                m_length = other.Length();

                return *this;
            }

            //Attempt to resize the array, true if everything is OK, false if something terrible happened
            bool Resize(size_t new_size) {
                T* tmp_ptr = Reallocate(m_array, m_length, new_size);
                if(tmp_ptr == NULL) {
                    CbThrowWarningf("Array resizing from %lu to %lu failed", m_length, new_size);
                    return false; //something terrible it is
                }

                m_array = tmp_ptr;
                m_length = new_size;

                return true;
            }

            //Append value to the end of the array. If you are planning to do this frequently, please use cbpp::List instead
            bool PushBack(const T& value) {
                bool result = Resize(m_length+1);
                if(result){ m_length++; }

                return result;
            }

            void Fill(const T& fill_value) {
                for(size_t i = 0; i < m_length; i++) {
                    m_array[i] = fill_value;
                }
            }

            size_t Length() const noexcept {
                return m_length;
            }

            bool IsValid() const noexcept {
                return (m_array != NULL) && (m_length != 0);
            }

            T& At(size_t index) {
                if(index >= m_length) {
                    CbThrowErrorf("Index %lu is out of array bounds (%lu)", index, m_length);
                }

                return m_array[index];
            }

            const T& At(size_t index) const {
                return const_cast<const T&>(At(index));
            }

            T& operator [] (size_t index) {
                return At(index);
            }

            //Get the const pointer to the underlying C array
            const T* CArr() const noexcept {
                return const_cast<const T*>(m_array);
            }

            //Get array`s length in bytes
            size_t Size() {
                return m_length * sizeof(T);
            }

            ~Array() {
                if(IsValid()) {
                    Free<T>(m_array, m_length);
                    m_length = 0;
                }
            }
            
            template <typename T2> friend Array<T2> operator+(const Array<T2>&, const Array<T2>&); //fren ^-^

        private:
            T* m_array = NULL;
            size_t m_length = 0;
    };

    template <typename T> Array<T> operator+(const Array<T>& A, const Array<T>& B) {
        if(!A.IsValid() || !B.IsValid()) {
            CbThrowError("Invalid array is passed to the addition operator");
        }

        Array<T> out(A.m_length + B.m_length);

        if(!std::is_trivially_copy_assignable<T>::value) { //T has custom operator=, and we better call him
            for(size_t i = 0; i < A.m_length; i++) {
                out[i] = A.At(i);
            }

            for(size_t i = 0; i < B.m_length; i++) {
                out[i+A.m_length] = B.At(i);
            }

        }else{ //T is probably a primitive type, so just use memcpy()
            memcpy(out.m_array, A.m_array, A.m_length * sizeof(T));
            memcpy(out.m_array + A.m_length, B.m_array, B.m_length * sizeof(T));
        }

        return out;
    }
}

#endif
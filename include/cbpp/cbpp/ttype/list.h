#ifndef CBPP_TTYPE_List_H
#define CBPP_TTYPE_List_H

/*
    0   - allocate two times more memory each time (like std::vector does)
    N>0 - add N pieces upon each re-allocation
*/
#define CBPP_LIST_MEMMODE 0 

#include <cstdint>
#include <stddef.h>
#include <stdlib.h>

#include "cbpp/cbdef.h"
#include "cbpp/error.h"

namespace cbpp {
    template <typename T> class List {
        public:
            List() = default;
            List( const List<T>& other ) {
                len_phy = other.LengthPhysical();
                len_img = other.Length();

                for(id_t i = 0; i < len_img; i++) {
                    At(i) = other.At(i);
                }
            }

            T& At( id_t index ) {
                if(data == NULL) {
                    CbThrowError("The list is not allocated");
                }

                if(index < 0 || index > len_phy - 1) {
                    CbThrowErrorf("Index is out of bounds (%lu/%lu)", index, len_phy);
                }else if(index > len_img - 1) {
                    CbThrowWarningf("Index is inside the allocated chunk, but is outside of the list bounds (%lu/%lu)", index, len_img);
                }
                
                return data[index];
            }

            T& operator[]( id_t index ) {
                return this->At( index );
            }

            void PushBack( const T& value ) {
                len_img++;
                if(len_img > len_phy) {
                    this->Reallocate();
                }
                data[len_img-1] = value;
            }

            void Reserve( id_t size ) {
                len_img = size;
                this->Reallocate();
            }

            size_t Length() const {
                return len_img;
            }

            size_t LengthPhysical() const {
                return len_phy;
            }

            ~List() {
                free(data);
            }

        private:
            T* data = NULL;
            size_t len_phy = 0, len_img = 0;

            void Reallocate() {
                if(len_phy < len_img) {
                    len_phy = len_img;
                }

                if(len_phy <= 0) { 
                    len_phy = 1;
                }

                if(len_img*2 < len_phy) {
                    len_phy = len_img;
                }

                #if CBPP_LIST_MEMMODE == 0
                    len_phy = len_phy*2;
                #else
                    #if CBPP_LIST_MEMMODE > 0
                        len_phy = len_phy + (size_t)CBPP_List_MEMMODE;
                    #else
                        #error CBPP_LIST_MEMMODE must be greater or equal than zero
                    #endif
                #endif

                data = (T*)realloc(data, len_phy * sizeof(T));

                if(data == NULL) {
                    CbThrowErrorf("Failed to re-allocate memory chunk of size %lu", len_phy);
                }
            }
    };
}

#endif
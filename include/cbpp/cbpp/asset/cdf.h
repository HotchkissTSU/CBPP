#ifndef CBPP_ASSET_CDF_H
#define CBPP_ASSET_CDF_H

#include <cstdint>
#include <stddef.h>
#include <typeinfo>

#include "cbpp/fileio.h"
#include "cbpp/cbdef.h"
#include "cbpp/ttype/list.h"
#include "cbpp/cb_alloc.h"

#define CDF_VERSION_MAJOR 1
#define CDF_VERSION_MINOR 0

#define CDF_MAX_VNAME_LEN 128
#define CDF_ID_INVALID (uint32_t)(-1)
#define CDF_ID_MAX (uint32_t)(-1) - 1

namespace cbpp {
    class DataFile {
        public:
            enum class VType : uint8_t {
                INVALID,
                BYTE, CHAR,
                U16, I16,
                U32, I32,
                U64, I64,
                FLOAT
            };

            union VData {
                uint8_t u8; char i8;
                uint16_t u16; int16_t i16;
                uint32_t u32; int32_t i32;
                uint64_t u64; int64_t i64;
                float flt;
            };

            struct BlockValueInfo {
                uint32_t nid;
                uint32_t arrb;
                uint64_t offset;
                VType type;
            };

            struct BlockTypeInfo {
                uint32_t nid;
                uint32_t vnum;
                BlockValueInfo* valinf = NULL;
            };

            struct Value {
                void Print() {
                    if(Name != NULL) { printf("Value '%s':\n", Name); }
                    else{ return; }

                    printf("\tLength: %u\n", Length);
                    printf("\tTypeid: %u\n", Type);
                }

                Value() = default;

                Value(VType typ) {
                    Length = 1;
                    Type = typ;
                    _allocate();
                }

                Value(VType typ, uint32_t ln) {
                    Length = ln;
                    Type = typ;
                    _allocate();
                }

                VData* At(uint32_t index) {
                    if( index < Length ) {
                        return &Data[index];
                    }else{
                        char buff[128];
                        snprintf(buff, 128, "Array index %u is out of range (%u)", index, Length);
                        PushError(ERROR_MEM, buff);

                        return NULL;
                    }
                }
                
                VData* operator[](uint32_t index) {
                    return At(index);
                }

                void SetName(const char* nname) {
                    free(Name);
                    Name = strdup(nname);
                }

                Value& operator=(Value& other) {
                    Name = strdup(other.Name);
                    
                    free(Data);
                    Length = other.Length;
                    Data = (VData*)malloc(sizeof(VData) * Length);
                    memcpy(Data, other.Data, Length*sizeof(VData));

                    Type = other.Type;
                    NID = other.NID;

                    return *this;
                }

                VType Type = VType::INVALID;
                uint32_t Length = 0;
                VData* Data = NULL;
                char* Name = NULL;

                uint32_t NID = 0;

                void _allocate() {
                    Data = (VData*)realloc(Data, Length);
                }

                ~Value() {
                    free(Name);
                    free(Data);
                }
            };

            struct Block {
                uint32_t NID = 0;
                uint32_t TID = 0;

                uint32_t Length = 0;
                Value* Data = NULL;
                char* Name = NULL;

                void SetName(const char* nname) {
                    free(Name);
                    Name = strdup(nname);
                }

                Value* At(const char* vname) {
                    for(uint32_t i = 0; i < Length; i++) {
                        if(strcmp(vname, Data[i].Name) == 0) {
                            return &Data[i];
                        }
                    }

                    return NULL;
                }

                Block& operator=(Block& other) {
                    Name = strdup(other.Name);

                    Free<Value>(Data, Length);
                    Length = other.Length;
                    Data = Allocate<Value>(Length);
                    NID = other.NID;
                    TID = other.TID;
                    memcpy(Data, other.Data, Length * sizeof(Value));

                    return *this;
                }

                Value* operator[](const char* vname) {
                    return At(vname);
                }

                void PushValue(Value& val) {
                    Length++;
                    Data = Reallocate<Value>(Data, -1, Length); //(Value*)realloc(Data, Length);
                    Data[Length-1] = val;
                }

                ~Block() {
                    free(Name);
                    Free<Value>(Data, Length);
                }
            };

            DataFile() = default;
            DataFile( const char* path );

            //Load data from the file
            bool Load( const char* path );

            //Dump all currently stored data to the file
            bool Save( const char* path );

            //Get the result of the last CDF::Load() call
            bool LoadResult();

            Block* At(const char* name);
            Block* operator[](const char* name);

            void PushBlock(Block& blk);
            void PopBlock(const char* bname);

            size_t SizeOf(VType typ);

        private:
            bool load_result = false;

            struct {
                union {
                    char str[4];
                    uint32_t num = 0;
                } sig;

                union {
                    uint16_t whole = 0;
                    uint8_t major, minor;
                } version;

                uint32_t nnum = 0;
                uint32_t bnum = 0;
            } header;

            Block* blocks = NULL;

            void _allocate(uint32_t bnum);

            bool read_header(File& handle, bool check_version);
            //Read N tightly packed NULL-terminated strings
            bool read_strtab(File& handle, char** target, uint32_t ln );
            bool read_typereg(File& handle, BlockTypeInfo& target);
    };
}

#endif
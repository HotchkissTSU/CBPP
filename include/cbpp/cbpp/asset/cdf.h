#ifndef CBPP_ASSET_CDF_H
#define CBPP_ASSET_CDF_H

#include <cstdint>

#include "cbpp/cbstring.h"
#include "cbpp/ttype/array.h"
#include "cbpp/cb_hash.h"
#include "cbpp/cbdef.h"
#include "cbpp/fileio.h"
#include "cbpp/vec2.h"

#define CDF_VERSION_MAJOR 1
#define CDF_VERSION_MINOR 0

#define CDF_MAX_NAME_LEN 128
#define CDF_CHECK_VERSION 1

namespace cbpp {
    /*
        CBPP main binary file format
    */
    class CDF {
        public:
            enum VType : uint8_t {
                INVALID,
                BYTE, CHAR,
                U16, I16,
                U32, I32,
                U64, I64,
                FLOAT, DOUBLE,
                VECTOR
            };

            union VData {
                uint8_t u8; char i8;
                uint16_t u16; int16_t i16;
                uint32_t u32; int32_t i32;
                uint64_t u64; int64_t i64;
                float flt; double dbl;

                struct {
                    float_t x,y;
                } vec;
            };

            struct Value {
                Value() = default;
                Value(const char* nm) {
                    SetName(nm);
                }

                void SetName(const char* vn) {
                    free(Name);
                    Name = strdup(vn);
                }

                VType Type = INVALID;
                char* Name = NULL;
                Array<VData> Data;

                ~Value() {
                    free(Name);
                }
            };

            struct Block {
                Block() = default;
                Block(const char* nm) {
                    SetName(nm);
                }

                void SetName(const char* name) {
                    free(Name);
                    Name = strdup(name);
                }

                char* Name = NULL;
                Array<Value> Data;

                ~Block() {
                    free(Name);
                }
            };

            struct BlockValueInfo {
                uint32_t nid = 0;
                uint32_t arrb = 0;
                uint64_t offset = 0;
                VType type = VType::INVALID;
            }

            struct BlockTypeInfo {
                uint32_t nid = 0, vnum = 0;
                Array<BlockValueInfo> _values_info;
            };

            struct Header {
                union {
                    char str[4];
                    uint32_t num = 0;
                } sign;

                union {
                    uint16_t whole = 0;
                    uint8_t major, minor;
                } version;

                uint32_t nnum, bnum;
            } m_CDFHead;

            CDF() = default;
            CDF(const char* path);

            bool Load(const char* path);
            bool Save(const char* path);

            size_t SizeOf(VType typ);

        private:
            bool read_header(File& hand, bool check_ver);
            bool read_typedef(File& hand, BlockTypeInfo& bti);
            bool read_strtab(File& hand, char** target, uint32_t ln);

            bool m_bRWresult = false; //the result of the last read/write call
            Array<Block> m_aBlocks;
    };
}

#endif

#ifndef CBPP_ASSET_CDF_H
#define CBPP_ASSET_CDF_H

#include <cstdint>

#include "cbpp/cbstring.h"
#include "cbpp/cb_hash.h"
#include "cbpp/cbdef.h"

#define CDF_VERSION_MAJOR 1
#define CDF_VERSION_MINOR 0

#define CDF_MAX_NAME_LEN 128

namespace cbpp {
    class CDF {
        public:
            enum VType : uint8_t {
                INVALID,
                BYTE, INT8,
                UINT16, INT16,
                UINT32, INT32,
                UINT64, INT64,
                FLOAT
            };

            union VData {
                uint8_t u8; char i8;
                uint16_t u16; int16_t i16;
                uint32_t u32; int32_t i32;
                uint64_t u64; int64_t i64;
                float flt;
            };

            class Value {
                VType type = INVALID;
                String Name;
                Array<VData> Data;
            };

            class BaseDataType {
                public:

                private:

            };

            class Block {
                String Name;
                Array<Value> Data;
            };

            CDF() = default;
            CDF(const char* path);

            bool Load(const char* path);
            bool Save(const char* path);

        private:
            bool rles = false; //the result of the last read/write call
    };
}

#endif
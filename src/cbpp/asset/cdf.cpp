#include "cbpp/asset/cdf.h"

namespace cbpp {
    bool DataFile::read_header(File& hand, bool check_version) {
        if(hand.Read(&header.sig.str, 4) != 4) { //read file signature
            return false;
        }
        header.sig.str[3] = '\0';

        if( strcmp(header.sig.str, "CDF") != 0 ) { //check file signature
            char buffer[128];
            snprintf(buffer, 128, "File signature mismatch: expected 'CDF', got '%s'", header.sig.str);
            PushError(ERROR_IO, buffer);
        }

        if( hand.Read(&header.version.whole, 2) != 2 ) { //read file format version
            return false;
        }

        if(check_version && header.version.major != CDF_VERSION_MAJOR) {
            char buffer[128];
            snprintf(buffer, 128, "CDF format version mismatch. Expected %u.*, got %u.*", CDF_VERSION_MAJOR, header.version.major);
            PushError(ERROR_IO, buffer);
        }

        if( hand.Read(&header.nnum, 4) != 4 ) { //read these lads
            return false;
        }

        if( hand.Read(&header.bnum, 4) != 4 ) {
            return false;
        }

        return true;
    }

    bool DataFile::read_strtab(File& hand, char** target, uint32_t ln) {
        char buffer[CDF_MAX_VNAME_LEN+1];
        uint32_t k = 0;
        char current = 'a';

        for(uint32_t i = 0; i < ln; i++) {
            k = 0;
            current = 'a';

            while( current != '\0' && k < CDF_MAX_VNAME_LEN ) {
                if(hand.Read(&current, 1) != 1) {
                    return false;
                }

                buffer[k] = current;
                k++;
            }

            buffer[k] = '\0';
            
            target[i] = (char*)malloc(k+1);
            strcpy(target[i], buffer);
        }

        return true;
    }

    bool DataFile::read_typereg(File& handle, BlockTypeInfo& target) {
        if( handle.Read(&target.nid, 4) != 4 ) {
            return false;
        }

        if( handle.Read(&target.vnum, 4) != 4 ) {
            return false;
        }

        target.valinf = Allocate<BlockValueInfo>(target.vnum);
        if(target.valinf == NULL) {
            return false;
        }

        BlockValueInfo buff;
        for(uint32_t i = 0; i < target.vnum; i++ ) {
            if( handle.Read(&buff.nid, 4) != 4 ) {
                free(target.valinf);
                return false;
            }

            if( handle.Read(&buff.arrb, 4) != 4 ) {
                free(target.valinf);
                return false;
            }

            if( handle.Read(&buff.offset, 8) != 8 ) {
                free(target.valinf);
                return false;
            }

            if( handle.Read(&buff.type, 1) != 1 ) {
                free(target.valinf);
                return false;
            }

            target.valinf[i] = buff;
        }

        return true;
    }

    

    size_t DataFile::SizeOf(VType typ) {
        if(typ == VType::CHAR || typ == VType::BYTE) {
            return 1;
        }

        if(typ == VType::I32 || typ == VType::U32 || typ == VType::FLOAT) {
            return 4;
        }

        if(typ == VType::I16 || typ == VType::U16) {
            return 2;
        }

        if(typ == VType::U64 || typ == VType::I64) {
            return 8;
        }

        return -1;
    }

    void DataFile::PushBlock(Block& blk) {
        header.bnum++;
        blocks = Reallocate<Block>(blocks, -1, header.bnum);
        blocks[header.bnum-1] = blk;

        header.nnum += blk.Length;
    }

    void DataFile::PopBlock(const char* bname) {

    }
}
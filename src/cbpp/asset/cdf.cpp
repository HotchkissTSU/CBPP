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

    bool DataFile::Load(const char* path) {
        File handle(path, "rb");

        if(!handle.IsOpen()) {
            load_result = false;
            return load_result;
        }

        if( !read_header(handle, true) ) {
            load_result = false;
            return load_result;
        }

        char** vname_buffer = Allocate<char*>(header.nnum);
        char** bname_buffer = Allocate<char*>(header.bnum);

        if(vname_buffer == NULL || bname_buffer == NULL) {
            load_result = false;
            return load_result;
        }

        //read name tables
        if(!read_strtab(handle, vname_buffer, header.nnum)){ //VNT
            load_result = false;
            free(vname_buffer);
            free(bname_buffer);
            return load_result;
        }

        if(!read_strtab(handle, bname_buffer, header.bnum)){ //BNT
            load_result = false;
            free(vname_buffer);
            free(bname_buffer);
            return load_result;
        }

        //now, read the TST

        BlockTypeInfo* tst_buffer = Allocate<BlockTypeInfo>(header.bnum);

        uint32_t nid, vnum;
        BlockTypeInfo buff;
        for(uint32_t i = 0; i < header.bnum; i++) {
            if(!read_typereg(handle, buff)) {
                free(tst_buffer);
                free(vname_buffer);
                free(bname_buffer);
                return false;
            }
            tst_buffer[i] = buff;
        }

        //blocks reading time!

        blocks = cbpp::Allocate<Block>(header.bnum);
        if(blocks == NULL) {
            free(tst_buffer);
            free(vname_buffer);
            free(bname_buffer);
            return false;
        }
        
        uint32_t tid;
        uint64_t sum = 0;

        uint8_t* data_buffer = NULL;

        for(uint32_t i = 0; i < header.bnum; i++) {
            sum = 0;

            if(handle.Read(&tid, 4) != 4) {
                free(tst_buffer);
                free(vname_buffer);
                free(bname_buffer);
                load_result = false;
                return false;
            }

            BlockTypeInfo& _inf = tst_buffer[tid];

            for(size_t j = 0; j < _inf.vnum; j++) {
                sum = sum + SizeOf(_inf.valinf[j].type);
            }

            data_buffer = (uint8_t*)malloc(sum);
            if( handle.Read(data_buffer, sum) != sum ) {
                free(tst_buffer);
                free(vname_buffer);
                free(bname_buffer);
                load_result = false;
                return false;
            }

            Block tmp;
            tmp.Length = _inf.vnum;

            tmp.Data = Allocate<Value>(_inf.vnum);
            if(tmp.Data == NULL) {
                free(tst_buffer);
                free(vname_buffer);
                free(bname_buffer);
                load_result = false;
                return false;
            }

            for(size_t j = 0; j < _inf.vnum; j++) {
                Value vtmp;
                vtmp.Data = (VData*)malloc(sizeof(VData) * _inf.valinf[j].arrb );
                vtmp.Length = _inf.valinf[j].arrb;

                if(vtmp.Data == NULL) {
                    free(tst_buffer);
                    free(vname_buffer);
                    free(bname_buffer);
                    free(tmp.Data);
                    load_result = false;
                    return false;
                }

                memcpy(&vtmp.Data, data_buffer + _inf.valinf[j].offset, SizeOf(_inf.valinf[j].type) * _inf.valinf[j].arrb );

                vtmp.SetName(vname_buffer[_inf.nid]);
                tmp.Data[j] = vtmp;
            }

            tmp.SetName(bname_buffer[_inf.nid]);

            blocks[i] = tmp;
        }

        free(data_buffer);
        free(tst_buffer);
        free(vname_buffer);
        free(bname_buffer);

        load_result = true;
        return load_result;
    }

    bool DataFile::Save(const char* path) {
        File out(path, "wb");

        header.version.major = CDF_VERSION_MAJOR;
        header.version.minor = CDF_VERSION_MINOR;

        //first, write the header
        char head[] = "CDF";
        out.Write(head, 4);
        out.Write(&header.version.whole, sizeof(header.version.whole));
        out.Write(&header.nnum, 4);
        out.Write(&header.bnum, 4);

        char** vnames_buffer = Allocate<char*>(header.nnum);
        if(vnames_buffer == NULL) {
            return false;
        }

        char** bnames_buffer = Allocate<char*>(header.bnum);
        if(bnames_buffer == NULL) {
            return false;
        }

        uint32_t vname_c = 0, bname_c = 0;
        //we need to rebuild the name tables
        for(uint32_t i = 0; i < header.bnum; i++) {
            Block& blk = blocks[i];

            blk.NID = bname_c;
            bnames_buffer[bname_c] = blk.Name;
            bname_c++;

            for(uint32_t j = 0; j < blk.Length; j++) {
                Value& val = blk.Data[j];
                
                val.NID = vname_c;
                vnames_buffer[vname_c] = val.Name;
                vname_c++;
            }
        }

        //write `em
        for(uint32_t i = 0; i < header.nnum; i++) {
            out.Write(vnames_buffer[i], strlen(vnames_buffer[i])+1); //VNT
        }

        for(uint32_t i = 0; i < header.bnum; i++) {
            out.Write(bnames_buffer[i], strlen(bnames_buffer[i])+1); //BNT
        }

        uint64_t offset;

        for(uint32_t i = 0; i < header.bnum; i++) { //TST
            offset = 0;

            Block& blk = blocks[i];

            blk.TID = i;

            out.Write(&blk.NID, 4);
            out.Write(&blk.Length, 4);

            for(uint32_t j = 0; j < blk.Length; j++) {
                Value& val = blk.Data[j];
                out.Write(&val.NID, 4);
                out.Write(&val.Length, 4);
                out.Write(&offset, 8);
                out.Write(&val.Type, 1);

                offset = offset + SizeOf(val.Type) * val.Length;
            }
        }

        uint8_t* data_buffer = NULL;
        uint64_t full_ln = 0;
        
        for(uint32_t i = 0; i < header.bnum; i++) {
            offset = 0;
            full_ln = 0;
            Block& blk = blocks[i];

            out.Write(&blk.TID, 4);

            for(uint32_t j = 0; j < blk.Length; j++) {
                Value& val = blk.Data[j];
                full_ln = full_ln + SizeOf(val.Type) * val.Length;
            }

            data_buffer = (uint8_t*)realloc(data_buffer, full_ln);

            for(uint32_t j = 0; j < blk.Length; j++) {
                Value& val = blk.Data[j];
                
                for(uint32_t m = 0; m < val.Length; m++) {
                    memcpy(data_buffer + offset, &val.Data[m], SizeOf(val.Type) * val.Length);
                    offset = offset + SizeOf(val.Type);
                }
            }

            out.Write(data_buffer, full_ln);
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
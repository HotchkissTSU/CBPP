#include "cbpp/asset/cdf.h"
#include "cbpp/error.h"

#define CDF_RET(res) m_bRWresult = (bool)(res); if(!m_bRWresult) {return m_bRWresult;}

namespace cbpp {
    bool CDF::read_header(File& hand, bool check_version) {
        if(hand.Read(&m_CDFHead.sign.str, 4) != 4) { //read file signature
            return false;
        }
        m_CDFHead.sign.str[3] = '\0';

        if( strcmp(m_CDFHead.sign.str, "CDF") != 0 ) { //check file signature
            char buffer[128];
            snprintf(buffer, 128, "File signature mismatch: expected 'CDF', got '%s'", m_CDFHead.sign.str);
            PushError(ERROR_IO, buffer);
            return false;
        }

        if( hand.Read(&m_CDFHead.version.whole, 2) != 2 ) { //read file format version
            return false;
        }

        if(check_version && m_CDFHead.version.major != CDF_VERSION_MAJOR) {
            char buffer[128];
            snprintf(buffer, 128, "CDF format version mismatch. Expected %u.*, got %u.*", CDF_VERSION_MAJOR, m_CDFHead.version.major);
            PushError(ERROR_IO, buffer);
            return false;
        }

        if( hand.Read(&m_CDFHead.nnum, 4) != 4 ) { //read these lads
            return false;
        }

        if( hand.Read(&m_CDFHead.bnum, 4) != 4 ) {
            return false;
        }

        return true;
    }

    bool CDF::read_strtab(File& hand, char** target, uint32_t ln) {
        char buffer[CDF_MAX_NAME_LEN+1];
        uint32_t k = 0;
        char current = 'a';

        for(uint32_t i = 0; i < ln; i++) {
            k = 0;
            current = 'a';

            while( current != '\0' && k < CDF_MAX_NAME_LEN ) {
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

    size_t CDF::SizeOf(VType typ) {
        if(typ == VType::CHAR || typ == VType::BYTE) {
            return 1;
        }

        if(typ == VType::I32 || typ == VType::U32 || typ == VType::FLOAT) {
            return 4;
        }

        if(typ == VType::I16 || typ == VType::U16) {
            return 2;
        }

        if(typ == VType::U64 || typ == VType::I64 || typ == VType::DOUBLE || typ == VType::VECTOR) {
            return 8;
        }

        return -1;
    }

    bool CDF::read_typedef(File& hand, BlockTypeInfo& bti) {
        CDF_RET(hand.Read(&bti.nid, 4) == 4)
        CDF_RET(hand.Read(&bti.vnum, 4) == 4)
    }

    bool CDF::Load(const char* path) {
        m_bRWresult = true; //assume that everything is going to be OK

        File inp(path, "rb");
        CDF_RET(inp.IsOpen())

        CDF_RET(read_m_CDFHeader(inp, CDF_CHECK_VERSION))

        char** buffer_vnt = (char**)malloc(m_CDFHead.nnum*sizeof(char*));
        CDF_RET(buffer_vnt != NULL)

        char** buffer_bnt = (char**)malloc(m_CDFHead.bnum*sizeof(char*));
        CDF_RET(buffer_bnt != NULL)

        if(!read_strtab(inp, buffer_vnt, m_CDFHead.nnum)){ //reading the VNT
            free(buffer_vnt);
            m_bRWresult = false;
            return m_bRWresult;
        }

        if(!read_strtab(inp, buffer_bnt, m_CDFHead.bnum)) { //and the BNT
            free(buffer_vnt);
            free(buffer_bnt);

            m_bRWresult = false;
            return m_bRWresult;
        }



        return m_bRWresult;
    }
}
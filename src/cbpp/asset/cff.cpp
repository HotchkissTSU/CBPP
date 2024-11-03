#include "cbpp/asset/cff.h"
#include <algorithm>
#include "cbpp/fileio.h"

namespace cbpp {
    FontFile::FontFile(const char* path) {
        this->Load(path);
    }

    bool FontFile::Load(const char* path) {
        File inp(path, "rb");

        CFF_Header_parse head_p;
        inp.Read(&head_p, 1, sizeof(CFF_Header_parse));

        head.md = head_p.md;
        head.sheet = head_p.sheet;
        strcpy(head.sig, head_p.sig);
        head.type = head_p.type;
        head.sorted = head_p.sorted;
        head.ver = head_p.ver;

        if(head.sig != (char*)( *(uint32_t*)"CFF" )) {
            PushError(ERROR_IO, "File header signature mismatch");
            return false;
        }
        
        maps_num = head.sheet;
        md_num = head.md;

        if(maps != NULL) {
            delete[] maps;
        }
        maps = new CFF_Bitmap[maps_num];

        if(chars_data != NULL) {
            delete[] chars_data;
        }
        chars_data = new CFF_CharMD[md_num];

        //first, get the markdown data

        CFF_CharMD_parse chartmp_p;
        CFF_CharMD chartmp;
        for(uint32_t i = 0; i < md_num; i++) {
            inp.Read(&chartmp_p, 1, sizeof(CFF_CharMD_parse));

            chartmp.advance = chartmp_p.advance;
            chartmp.bbox_ymax = chartmp_p.bbox_ymax;
            chartmp.bearingy = chartmp_p.bearingy;
            chartmp.code = chartmp_p.code;
            chartmp.width = chartmp_p.width;

            chars_data[i] = chartmp;
        }

        //read bitmap data then
        CFF_Bitmap_parse tmp_p;
        CFF_Bitmap tmp;
        for(uint8_t i = 0; i < maps_num; i++) {
            inp.Read(&tmp_p, 1, sizeof(CFF_Bitmap_parse) - sizeof(uint8_t*)); //read everything except the last pointer

            snprintf(tmp.name, 33, "%s", tmp_p.name);
            tmp.channels = tmp_p.channels;
            tmp.res = tmp_p.res;

            tmp.bitmap = new uint8_t[tmp.res * tmp.res * tmp.channels];
            inp.Read(tmp.bitmap, tmp.res * tmp.res * tmp.channels, 1);

            maps[i] = tmp;
        }

        if(!head.sorted) { //symbols data in the file is not sorted, so sort it
            std::sort(chars_data, chars_data + md_num, char_comparator);
        }

        return true;
    }

    bool FontFile::char_comparator( CFF_CharMD& first, CFF_CharMD& second ) {
        return first.code > second.code;
    }
}

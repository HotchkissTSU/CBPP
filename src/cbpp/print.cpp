#include "cbpp/print.h"
#include "cbpp/cbstring.h"
#include "cbpp/cbdef.h"

#include <stdarg.h>

namespace cbpp {
    /*
        CBPP printf analog with more types supported   ***FIXME***
    */
    /*size_t PrintBuffer(char* buffer, size_t max_write, const char* format, ...) {
        CharStream<char> stream(format);
        char fmt_buffer[CBPP_PRINT_FMTBUFF_SIZE], type_buffer[CBPP_PRINT_FMTBUFF_SIZE];
        size_t write_count = 0; //how many bytes were written to the buffer

        bool b_is_rawchar = false; //if the current character is protected by a backslash before him

        bool b_is_scobed = false; //if we are inside the [...] construction
        size_t scobe_count = 0; //how many characters are located after the last '[' by now
        size_t scobe_blocks_count = 0; //how many valid [...] constructions were found by now

        bool b_is_inside_mark = false;

        va_list vargs;
        va_start(vargs, format);

        while(true) {
            char cur = stream.Current();

            b_is_inside_mark = (cur == '[' || cur == ']') && !b_is_rawchar || b_is_scobed;

            //printf("%d", b_is_inside_mark);
            switch (cur) {
                case '\\':
                    b_is_rawchar = true; break;

                case '[':
                    if(b_is_rawchar) {break;}
                    if(!b_is_scobed) {
                        b_is_scobed = true;
                    }else{
                        //error, attempt to open a new scobe sequence inside another one
                        return -1;
                    }
                    break;

                case ']':
                    if(b_is_rawchar) {break;}
                    if(b_is_scobed) {
                        b_is_scobed = false;
                        scobe_count = 0;
                        scobe_blocks_count++;
                        
                        if(scobe_blocks_count == 1 && stream.Offset(1) != '[') { //only one block, assume that this is type info
                            memcpy(type_buffer, fmt_buffer, CBPP_PRINT_FMTBUFF_SIZE);
                            printf("BLK: %s\n", type_buffer);
                        }

                        if(scobe_blocks_count == 2) { //got the both blocks we needed
                            printf("FORM: %s, TYPE: %s\n", fmt_buffer, type_buffer);
                        }

                    }else{
                        //error, attempt to close a non-existing sequence
                        return -1;
                    }
                    break;

                default:
                    if(b_is_scobed) {
                        if(scobe_blocks_count == 0 && scobe_count < CBPP_PRINT_FMTBUFF_SIZE-1) { //writing into a format buffer
                            fmt_buffer[scobe_count] = cur;
                            fmt_buffer[scobe_count+1] = '\0';
                        }

                        if(scobe_blocks_count == 1 && scobe_count < CBPP_PRINT_FMTBUFF_SIZE-1) { //writing into a type buffer
                            type_buffer[scobe_count] = cur;
                            type_buffer[scobe_count+1] = '\0';
                        }

                        scobe_count++;
                    }

                    b_is_rawchar = false;
            }

            if(!stream.Advance(1)) { break; } //check for the stream end
        }

        va_end(vargs);
        return write_count;
    }*/
}

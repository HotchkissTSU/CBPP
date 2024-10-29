#include <stdio.h>
#include <cstdint>
#include <stddef.h>
#include <cstring>
#include <stdlib.h>

#include "argparse.h"
#include "SOIL/SOIL.h"

#define START_BUFFER_SIZE 2048
#define STEP 128

char* input = NULL;
char* output = NULL;

char* use_namespace = NULL;
char* array_name = NULL;

size_t per_line = 16;

void arg_callback(const char* cmd, const char* arg) {
    if( strcmp(cmd, "-img") == 0 ) {
        if(arg != NULL) { input = strdup(arg); }
    }

    if( strcmp(cmd, "-h") == 0 ) {
        if(arg != NULL) { output = strdup(arg); }
    }

    if( strcmp(cmd, "-namespace") == 0 ) {
        if(arg != NULL) { use_namespace = strdup(arg); }
    }

    if( strcmp(cmd, "-arrname") == 0 ) {
        if(arg != NULL) { array_name = strdup(arg); }
    }

    if( strcmp(cmd, "-per_line") == 0 ) {
        if(arg != NULL) { per_line = atoi(arg); }
    }

    if( strcmp(cmd, "-help") == 0 ) {
        printf("\nCBPP SDK Image-To-Header utility\n-img <path>       -> input image file path\n\
-h <path>         -> output header file path\n\
-namespace <name> -> if specified, creates an array inside this namespace\n\
-arrname <name>   -> sets the name of the resulting array\n\
-per_line <int>   -> sets how many columns are present in the resulting code\n\
-help             -> show this message\n\n");
        exit(0);
    }
}

int main(int argc, char** argv) {
    sdk::ResolveArgs(argc, argv, arg_callback);

    if(array_name == NULL) {
        printf("ERROR: No array name providen\n");
        exit(-1);
    }

    if(input == NULL) {
        printf("ERROR: no input image providen\n");
        exit(-1);
    }

    if(output == NULL) {
        printf("WARNING: no output filename providen, using the default one\n");

        char fname_buffer[64];
        snprintf(fname_buffer, 64, "img2h_%s_%s.h", use_namespace, array_name);

        output = strdup(fname_buffer);
    }

    int w, h, channels;
    uint32_t* img_data = (uint32_t*)SOIL_load_image(input, &w, &h, &channels, SOIL_LOAD_RGBA);

    char header_guard_buffer[256], header_name_buffer[64];

    if(use_namespace){
        snprintf(header_name_buffer, 64, "CBSDK_IMGTOH_%s_%s_H", use_namespace, array_name);
    }else{
        snprintf(header_name_buffer, 64, "CBSDK_IMGTOH_nospace_%s_H", array_name);
    }

    snprintf(header_guard_buffer, 256, "#ifndef %s\n#define %s\n#include <cstdint>\n", header_name_buffer, header_name_buffer);

    char* code_buffer = (char*)malloc(START_BUFFER_SIZE);
    memset(code_buffer, 0, START_BUFFER_SIZE);

    size_t counter = 0;
    size_t length = START_BUFFER_SIZE;

    char num_buffer[16];

    for(size_t i = 0; i < w*h; i++) {
        if(counter + STEP > length) {
            length = length + STEP*2;
            code_buffer = (char*)realloc(code_buffer, length);
        }

        char* format;
        if(i == w*h - 1) {
            format = "%u";
        }else if((i+1) % per_line == 0) {
            if(use_namespace){
                format = "%u,\n\t\t";
            }else{
                format = "%u,\n\t";
            }
        }else {
            format = "%u, ";
        }

        counter += snprintf(num_buffer, 16, format, img_data[i]);
        code_buffer = strcat(code_buffer, num_buffer);
    }

    char commentary_buffer[] = { "\n\n/*\nGenerated by CBPP SDK Image-To-Header utility\n*/\n\n" };
    
    char* finalb = (char*)malloc(length + 256);

    if(use_namespace){
        snprintf(finalb, length + 256, "%s%snamespace %s {\n\tconst uint32_t %s[] = {\n\t\t%s\n\t};\n}\n#endif\n", commentary_buffer,
                                                                                                                        header_guard_buffer, 
                                                                                                                        use_namespace,
                                                                                                                        array_name,
                                                                                                                        code_buffer);
    }else{
        snprintf(finalb, length + 256, "%s%sconst uint32_t %s[] = {\n\t%s\n};\n#endif\n", commentary_buffer,
                                                                                               header_guard_buffer, 
                                                                                               array_name,
                                                                                               code_buffer);
    }

    FILE* out = fopen(output, "wt");
    fputs(finalb, out);
    fclose(out);

    free(code_buffer);
    free(finalb);

    return 0;
}
#include <stdio.h>

#include <cstdint>
#include <stddef.h>
#include <stdlib.h>
#include <cmath>

#include "argparse.h"
#include "SOIL/SOIL.h"

bool has_input = false;
char* input = NULL;
char* output = NULL;
char* log_path = NULL;
bool out_sdf = true;
FILE* log_io = NULL;

uint32_t search_sq_side = 64;
float spread = 50.0f;

#define LOG(format, ...) if(log_io != NULL) {fprintf(log_io, format,  __VA_ARGS__);}

using namespace std;

void arg_callback(const char* cmd, const char* arg) {
    if(strcmp(cmd, "-img") == 0) {
        if(arg != NULL) {
            input = strdup(arg);
        }
    }

    if(strcmp(cmd, "-sdf") == 0) {
        if(arg != NULL) {
            output = strdup(arg);
        }
    }

    if(strcmp(cmd, "-out_bmp") == 0) {
        out_sdf = false;
    }

    if(strcmp(cmd, "-log") == 0) {
        if(arg != NULL) {
            log_path = strdup(arg);
        }
    }

    if(strcmp(cmd, "-range") == 0) {
        if(arg != NULL) {
            search_sq_side = atoi(arg);
        }
    }

    if(strcmp(cmd, "-spread") == 0) {
        if(arg != NULL) {
            search_sq_side = (float)atof(arg);
        }
    }
}

union Color {
    uint32_t num = 0;
    float numf;
    struct {
        uint8_t r,g,b,a;
    }clr;
};

struct Image {
    uint32_t w,h;
    Color* data;

    void setPixel( uint32_t x, uint32_t y, Color clr ) {
        data[y * w + x] = clr;
    }

    Color getPixel( uint32_t x, uint32_t y ) {
        return data[y * w + x];
    }
};

struct CFFHeader {
    uint32_t w, h;
};

bool NormColor( Color& in ) {
    return ((float)(in.clr.r + in.clr.g + in.clr.b) * 0.333f) > 128.0f;
}

float map(float value, float inMin, float inMax, float outMin, float outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

void ComputeSDF(Image& in, Image& out) {
    out.w = in.w;
    out.h = in.h;

    out.data = (Color*)malloc( in.w * in.h * sizeof(Color) );

    for(uint32_t x = 0; x < in.w; x++) {
        for(uint32_t y = 0; y < in.h; y++) {

            Color current_in = in.getPixel(x,y);
            bool current_in_norm = NormColor(current_in);

            uint32_t ch_x1 = x - search_sq_side/2, ch_y1 = y - search_sq_side/2;
            uint32_t ch_x2 = x + search_sq_side/2, ch_y2 = y + search_sq_side/2;

            float dist_min = search_sq_side*100;

            for(uint32_t chx = ch_x1; chx < ch_x2; chx++) {
                for(uint32_t chy = ch_y1; chy < ch_y2; chy++) {
                    if(chx < 0 || chx > in.w || chy < 0 || chy > in.h) {
                        continue;
                    }

                    Color chcurr = in.getPixel(chx, chy);
                    bool chcurr_norm = NormColor(chcurr);

                    if(chcurr_norm == current_in_norm){
                        continue;
                    }

                    uint32_t dx = chx - x;
                    uint32_t dy = chy - y;
                    float dist_cur = dx*dx + dy*dy;

                    if(dist_cur < dist_min*dist_min) {
                        dist_min = sqrt(dist_cur);
                    }
                }
            }

            float dist_norm = map( dist_min, -spread/2, spread/2, 0.0f, 1.0f );
            Color current_out; current_out.numf = dist_norm;
            out.setPixel(x,y,current_out);

        }
    }
}

int main( int argc, char** argv ) {
    sdk::ResolveArgs(argc, argv, arg_callback);

    if(log_path != NULL) {
        log_io = fopen(log_path, "wt");
        if(!log_io){
            printf("ERROR: Failed to open log file '%s'\n", log_path);
            exit(-1);
        }
    }

    if(input == NULL) {
        printf("ERROR: no input image providen\n");
        exit(-1);
    }

    if(output == NULL) {
        printf("ERROR: no output filename providen\n");
        exit(-1);
    }

    Image img, img_out;
    int channels;
    uint8_t* img_raw = SOIL_load_image( input, (int*)&img.w, (int*)&img.h, &channels, SOIL_LOAD_RGBA );

    if(!img_raw) {
        printf("ERROR: SOIL: %s\n", SOIL_last_result());
        exit(-1);
    }

    if(img.w != img.h) {
        printf("ERROR: input image must be square, got (%ux%u)\n", img.w, img.h);
        exit(-1);
    }

    img.data = (Color*)img_raw;

    ComputeSDF(img, img_out);

    if(out_sdf) { //output as CBPP SDF file
        FILE* out = fopen(output, "wb");
        if(!out) {
            printf("ERROR: Can`t open output file '%s'\n", output);
            exit(-1);
        }

        const char* sdf_head = "SDF";
        uint32_t sdf_head_int = *( (uint32_t*)sdf_head );

        CFFHeader header = {img_out.w, img_out.h};

        fwrite(&sdf_head_int, sizeof(uint32_t), 1, out);
        fwrite(&header, sizeof(header), 1, out );
        fwrite(img_out.data, sizeof(Color), img_out.w*img_out.h, out);
    }else{ //output as BMP
        int res = SOIL_save_image(output, SOIL_SAVE_TYPE_BMP, (int)img_out.w, (int)img_out.h, SOIL_LOAD_RGBA, (unsigned char*)(img_out.data) );
        if(res == 0) {
            printf("ERROR: SOIL: %s\n", SOIL_last_result());
            exit(-1);
        }
    }
    
    free(input);
    free(output);
    free(log_path);

    return 0;
}
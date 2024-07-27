#include "cbpp/asset/image.h"

#include "cbpp/error.h"

#include <stdio.h>
#include <stdexcept>
#include <cstdint>

namespace cbpp {
	Image::Image(const char* filename) {
		FILE* filep = fopen(filename, "rb");
		
		if(filep == NULL){
			SetError("PNG Error", "Failed to open file");
			throw std::runtime_error("");
		}
		
		uint8_t header[8];
		fread(header, 1, 8, filep);
		valid = (png_sig_cmp(header, 0, 8) == 0);
		
		rewind(filep);
		
		if(!valid){
			SetError("PNG Error", "File is not a PNG");
			fclose(filep);
			throw std::runtime_error("");
		}
		
		img_base = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		
		if(img_base == NULL){
			SetError("PNG Error", "Unable to create 'png_struct' struct");
			
			fclose(filep);
			png_destroy_read_struct(&img_base, NULL, NULL);
			img_base = NULL;
			
			throw std::runtime_error("");
		}
		
		img_info = png_create_info_struct(img_base);
		
		if(img_info == NULL){
			SetError("PNG Error", "Unable to create 'png_info' struct");
			fclose(filep);
			
			png_destroy_info_struct(img_base, &img_info);
			png_destroy_read_struct(&img_base, NULL, NULL);
			
			throw std::runtime_error("");
		}
		
		png_init_io(img_base, filep);
		//png_read_info(img_base, img_info);
		
		png_read_png(img_base, img_info, PNG_TRANSFORM_IDENTITY, NULL);
		
		img_rows = png_get_rows(img_base, img_info);
	}
	
	Image::~Image() {
		if(valid) {
			//free ma memory
		}
	}
	
	uint32_t Image::Width(){ return png_get_image_width(img_base, img_info); }
	uint32_t Image::Height(){ return png_get_image_height(img_base, img_info); }
}
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
			throw std::runtime_error("");
		}
		
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		
		if(png_ptr == NULL){
			SetError("PNG Error", "Unable to create 'png_struct' struct");
			throw std::runtime_error("");
		}
		
		png_infop png_info = png_create_info_struct(png_ptr);
		
		if(png_info == NULL){
			SetError("PNG Error", "Unable to create 'png_info' struct");
			throw std::runtime_error("");
		}
		
		png_init_io(png_ptr, filep);
		png_read_png(png_ptr, png_info, PNG_TRANSFORM_IDENTITY, NULL);
		
		img_rows = png_get_rows(png_ptr, png_info);
		
		img_base = png_ptr;
		img_info = png_info;
	}
	
	uint32_t Image::Width(){ return png_get_image_width(img_base, img_info); }
	uint32_t Image::Height(){ return png_get_image_height(img_base, img_info); }
}
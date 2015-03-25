/*
 * Vcap C++ Bindings
 * 
 * Copyright (C) 2014 James McLean
 * 
 * This library is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
#include <Vcap/Vcap.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#include <iostream>
#include <string>
#include <vector>

typedef struct {
	std::uint8_t *data;
	size_t size;
} PngBuffer;

int rgb24ToPng(std::uint8_t *input, PngBuffer *buffer, int width, int height);

/*
 * Grabs a frame from a camera and outputs it as a PNG file. You must have libPNG
 * installed to compile this example.
 */
int main(int argc, char* argv[]) {
	std::vector<Vcap::CameraPtr> cameras;
	
	try {
		cameras = Vcap::Camera::cameras();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	if (0 == cameras.size()) {
		std::cout << "No cameras found!" << std::endl;
		return -1;
	}

	//grab an image from the first camera
	Vcap::CameraPtr camera = cameras[0];
	
	try {
		camera->open();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	//get format
	std::uint32_t formatCode, width, height;
	
	try {
		std::tie(formatCode, width, height) = camera->format();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}	
	
	//start capture
	try {
		camera->start();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	//some cameras require time to initialize
	sleep(3);

	std::uint8_t* rgbBuffer;
	
	//grab a frame and decode it
	try {
		camera->grab(&rgbBuffer, true);
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	//convert raw RGB to PNG
	PngBuffer pngBuffer;
	
	if (rgb24ToPng(rgbBuffer, &pngBuffer, width, height) == -1) {
		std::cout << "Error converting data to PNG" << std::endl;
		return -1;
	}
	
	//save png to file
	FILE* file = fopen("image.png", "wb");
		
	if (fwrite(pngBuffer.data, pngBuffer.size, 1, file) != 1)
		std::cout << "Error writing to file!" << std::endl;
	else
		std::cout << "Wrote output file 'image.png' (" << std::to_string(pngBuffer.size) << " bytes)" << std::endl;
			
	fclose(file);
	
	//free buffer to prevent memory leak (in a real application)
	delete rgbBuffer;
	
	return 0;
}

void pngWriteToBuffer(png_structp png_ptr, png_bytep data, png_size_t length) {
	PngBuffer* buffer = (PngBuffer*)png_get_io_ptr(png_ptr);
	size_t new_size = length + buffer->size;
	
	if (buffer->data) {
		buffer->data = (std::uint8_t*)realloc(buffer->data, new_size);
	} else {
		buffer->data = (std::uint8_t*)malloc(new_size);
	}
	
	if (!buffer->data) {
		png_error(png_ptr, "Write error");
	}
	
	memcpy(buffer->data + buffer->size, data, length);
	buffer->size += length;
}

void pngDummyFlush(png_structp png_ptr) {
}

int rgb24ToPng(std::uint8_t *input, PngBuffer *buffer, int width, int height) {
	/*
	 * Initialize buffer info
	 */ 
	buffer->size = 0;
	buffer->data = NULL;
	
	/*
	 * More initalization
	 */
	int code = 0;
	png_structp png_ptr;
	png_infop info_ptr;

	/*
	 * Initialize write structure
	 */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL) {
		std::cout << "Could not allocate write struct" << std::endl;
		return -1;
	}

	png_bytep* rows = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));

	/*
	 * Initialize info structure
	 */
	info_ptr = png_create_info_struct(png_ptr);
	
	if (info_ptr == NULL) {
		std::cout << "Could not allocate info struct" << std::endl;
		code = -1;
		goto finally;
	}

	/*
	 * Setup exception handling
	 */
	if (setjmp(png_jmpbuf(png_ptr))) {
		std::cout << "Error during png creation" << std::endl;
		code = -1;
		goto finally;
	}	

	/*
	 * Write header (8 bit colour depth)
	 */
	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	for (int i = 0; i < height; i++) {
		rows[i] = (png_bytep)&input[3 * i * width];
	}

	png_set_rows(png_ptr, info_ptr, rows);

	png_set_write_fn(png_ptr, buffer, pngWriteToBuffer, pngDummyFlush);

	png_write_info(png_ptr, info_ptr);
	
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	
	finally: {
		if (info_ptr != NULL) {
			png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		}
		
		if (png_ptr != NULL) {
			png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		}
		
		if (rows != NULL) {
			for (int i = 0; i < height; i++) {
				png_free(png_ptr, rows[i]);
			}
			
			png_free(png_ptr, rows);
		}
	}
	
	return code;
}

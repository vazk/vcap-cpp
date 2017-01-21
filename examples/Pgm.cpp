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

//https://www.w3.org/People/Bos/DJ1000toppm/dj1000toppm.c
 
#include <Vcap/Vcap.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>


typedef struct {
	uint8_t *data;
	size_t size;
} PGMBuffer;

int rgb24ToPGM(uint8_t *input, PGMBuffer *buffer, int width, int height);

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
	Vcap::CameraPtr camera = cameras[1];
	
	try {
		camera->open();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	//get format
	Vcap::Format format;
	
	try {
		format = camera->format();
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
	usleep(3000000);

	uint8_t* rgbBuffer;

    std::ofstream timestamp_file("timestamps.txt");
    std::string output_pattern = "frame_%08lu.pgm";
    const int BUF_SIZE = 1024;
    char filename[BUF_SIZE];
    filename[BUF_SIZE-1] = 0;
    int imgcount = 0;

	
    PGMBuffer pgmBuffer;

    while(true) {
        //grab a frame and decode it
        try {
            camera->grab(&rgbBuffer, true);
        } catch (Vcap::RuntimeError& e) {
            std::cout << e.what() << std::endl;
            return -1;
        }
        
        snprintf(filename, BUF_SIZE-1,  output_pattern.c_str(), imgcount);
        ++imgcount;
        unsigned long ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds> 
                (std::chrono::system_clock::now().time_since_epoch()).count();
        timestamp_file << std::setprecision(5) << ms_since_epoch << " \t " << filename << std::endl;

        
        if (rgb24ToPGM(rgbBuffer, &pgmBuffer, format.size().width(), format.size().height()) == -1) {
            std::cout << "Error converting data to PNG" << std::endl;
            return -1;
        }


        
        //save png to file
        FILE* file = fopen(filename, "wb");
            
        if (fwrite(pgmBuffer.data, pgmBuffer.size, 1, file) != 1)
            std::cout << "Error writing to file!" << std::endl;
        else
            std::cout << "Wrote output file 'image.pgm' (" << std::to_string(pgmBuffer.size) << " bytes)" << std::endl;
                
        fclose(file);
    }
        
    free(pgmBuffer.data);
	delete rgbBuffer;
	
	return 0;
}

int rgb24ToPGM(uint8_t *input, PGMBuffer *buffer, int width, int height) {

    std::cout<<"SIZE: "<<width<<", "<<height<<std::endl;
	int size = width * height + 256;
	buffer->data = (uint8_t*)malloc(size);
	
    int offset = sprintf((char*)buffer->data, "P5\n%d %d\n255\n", width, height);
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; j++) {
            uint8_t* in = input + i*width*3 + j * 3;
            //float val = 0.3*in[0] + 0.59*in[1] + 0.11*in[2];
            float val = 0.2126*in[0] + 0.7152*in[1] + 0.0722*in[2];
            //float val = (1.0*in[0] + 1.0*in[1] + 1.0*in[2])/3.0;
            buffer->data[offset++] = (uint8_t)val;
        }
    }
    buffer->size = offset;
	return 0;
}

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

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <unistd.h>

/*
 * Grabs raw data from a camera and saves it to the file 'image.raw'
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
	
	try {
		camera->start();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	//some cameras require time to initialize
	sleep(3);

	std::uint8_t *buffer;
	
	//grab a frame and decode it
	std::size_t bufferSize;
	
	try {
		bufferSize = camera->grab(&buffer, true);
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	//save raw image to file
	FILE* file = fopen("image.raw", "wb");
		
	if (fwrite(buffer, bufferSize, 1, file) != 1)
		std::cout << "Error writing to file!" << std::endl;
	else
		std::cout << "Wrote output file 'image.raw' (" << std::to_string(bufferSize) << " bytes)" << std::endl;
			
	fclose(file);
	
	//free buffer to prevent memory leak (in a real application)
	delete buffer;
	
	return 0;
}

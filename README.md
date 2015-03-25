# VCap C++ Bindings

## Introduction

This library implements C++ bindings for [Vcap](https://github.com/jrimclean/vcap) (a V4L2 capture library). Vcap is a 
library written in C that aims to make using the Video4Linux2 API simple. It was designed to make the common case of 
capturing frames using memory-mapped device buffers quick and easy.

VCap was also written with computer vision applications in mind and thus allows setting camera controls that enhance or
interfere with computer vision and image processing techniques. VCap also includes a (growing) number of pixel format
decoding functions, enabling seamless support for many devices. 

In contrast with similar projects, VCap is licensed under the LGPL v2.1 which permits it's use in commercial products.
	
## Building and Installation

You must have CMake and a C++ compiler installed. 

*$ cmake . && make*

To install:

*$ make install*

To generate documentation (if Doxygen is installed):

*$ make doc*

## Example

This example grabs a raw frame, from the first camera on the bus, and stores it in a file:

```cpp
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
```

## License
Copyright (c) 2015 James McLean  
Licensed under LGPL v2.1.

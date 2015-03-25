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

#include <unistd.h>

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <SDL.h>

typedef struct {
	int width;
	int height;
	SDL_Surface *screen;
	SDL_Surface *image;
} SdlContext;

int sdlInit(SdlContext* ctx, int width, int height);
int sdlDisplay(SdlContext* ctx, std::uint8_t* image);
void sdlCleanup(SdlContext* ctx);

/*
 * Grabs frames from a camera and displays them in a window in real-time. 
 * You must have SDL version 1.2 installed for this example to compile.
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

	//capture from the first camera
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
	
	try {
		camera->start();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	//some cameras require time to initialize
	sleep(3);
	
	std::uint8_t* rgbBuffer;
	
	//setup SDL
	SdlContext sdl_ctx;
	
	sdlInit(&sdl_ctx, width, height);
	
	SDL_Event event;
	
	while (SDL_PollEvent(&event) >= 0) {
		if (event.type == SDL_QUIT) {
			break;
		}
		
		//grab a frame and decode it
		try {
			camera->grab(&rgbBuffer, true);
		} catch (Vcap::RuntimeError& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
		
		sdlDisplay(&sdl_ctx, rgbBuffer);
		
		//free buffer to prevent memory leak (in a real application)
		delete [] rgbBuffer;
	}
	
	sdlCleanup(&sdl_ctx);
	
	return 0;
}

/*
 * Initializes the display contexgt
 */
int sdlInit(SdlContext* ctx, int width, int height) {
	atexit(SDL_Quit);
	
	ctx->width = width;
	ctx->height = height;
	
	ctx->screen = SDL_SetVideoMode(width, height, 24, SDL_DOUBLEBUF);
	
	if (ctx->screen == NULL) {
		std::cout << "Unable to set video mode: " << SDL_GetError() << std::endl;
		return -1;
	}
	
	std::uint32_t rmask, gmask, bmask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
	#endif

	ctx->image = SDL_CreateRGBSurface(0, width, height, 24, rmask, gmask, bmask, 0);
	
	return 0;
}

void sdlCleanup(SdlContext* ctx) {
	SDL_FreeSurface(ctx->screen);
	SDL_FreeSurface(ctx->image);
}

/*
 * Displays an image using SDL
 */
int sdlDisplay(SdlContext* ctx, std::uint8_t* image) {
	memcpy(ctx->image->pixels, image, 3 * ctx->width * ctx->height);	
	
	/*
	 * Apply the image to the display
	 */
	if (SDL_BlitSurface(ctx->image, NULL, ctx->screen, NULL) != 0) {
		std::cout << "SDL_BlitSurface() Failed: " << SDL_GetError() << std::endl;
		return -1;
	}
		
	/*
	 * Update the display
	 */
	SDL_Flip(ctx->screen);
	
	return 0;
}

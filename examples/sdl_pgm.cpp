#include <Vcap/Vcap.hpp>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <iomanip>
#include <chrono>

#include <SDL.h>

typedef struct {
	int width;
	int height;
	SDL_Surface *screen;
	SDL_Surface *image;
} SdlContext;

typedef struct {
	char*  data;
	size_t size;
} PGMBuffer;

class FrameLogger {
public:
    FrameLogger() 
     : is_started(false)
    {}
public:
    bool          is_started;
    std::string   directory;
    std::ofstream timestamp_file;
    int           imgcounter;
};

unsigned long ms_since_epoch();
void anchor_position();
void start_stop();
int  sdlInit(SdlContext* ctx, int width, int height);
int  sdlDisplay(SdlContext* ctx, uint8_t* image);
void sdlCleanup(SdlContext* ctx);
void rgb24ToPGM(uint8_t *input, PGMBuffer *buffer, int width, int height);


static FrameLogger frame_logger;

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
	
	try {
		camera->start();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	//some cameras require time to initialize
	sleep(1);
	
	//setup SDL
	SdlContext sdl_ctx;
	
	sdlInit(&sdl_ctx, format.size().width(), format.size().height());
	
	SDL_Event event;

	uint8_t* rgbBuffer;
    PGMBuffer pgmBuffer;
    bool event_enabled = true;
	
	while (SDL_PollEvent(&event) >= 0) {
		if (event.type == SDL_QUIT) {
			break;
		}

        if (event.type == SDL_KEYDOWN && event_enabled) {
            event_enabled = false;
            SDLKey keyPressed = event.key.keysym.sym;
            switch (keyPressed) {
                case SDLK_ESCAPE:
                    break;
                case SDLK_a:
                    anchor_position();
                    break;
                case SDLK_s:
                    start_stop();
                    break;
                default:
                    break;
            }
        }
        if (event.type == SDL_KEYUP) {
            event_enabled = true;
        }
		
		//grab a frame and decode it
		try {
			camera->grab(&rgbBuffer, true);
		} catch (Vcap::RuntimeError& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
		
		sdlDisplay(&sdl_ctx, rgbBuffer);

        if(frame_logger.is_started) {
            std::stringstream ss;
            ss << "frame_" << std::setfill('0') << std::setw(8) << frame_logger.imgcounter++ << ".pgm";
            std::string imgfname = ss.str();
            frame_logger.timestamp_file << ms_since_epoch() << " F " << "\t" << imgfname << std::endl;
            rgb24ToPGM(rgbBuffer, &pgmBuffer, format.size().width(), format.size().height());
            std::ofstream imgf(frame_logger.directory + "/" + imgfname);
            imgf.write(pgmBuffer.data, pgmBuffer.size);
        }
	}
	
    free(pgmBuffer.data);
    //free buffer to prevent memory leak (in a real application)
    delete [] rgbBuffer;
	sdlCleanup(&sdl_ctx);
	
	return 0;
}

unsigned long ms_since_epoch() 
{
    return std::chrono::duration_cast<std::chrono::milliseconds> 
                                (std::chrono::system_clock::now().time_since_epoch()).count();
}

void anchor_position()
{
    if(frame_logger.is_started) {
        frame_logger.timestamp_file << ms_since_epoch() << " A " << std::endl;
        std::cout<<"Anchoring..."<<std::endl;
    }
}

void start_stop()
{
    if (frame_logger.is_started) {
        frame_logger.is_started = false;
        std::cout<<"Stopping..."<<std::endl;
    } else {
        frame_logger.imgcounter = 0;
        std::stringstream ss;
        struct stat st = {0};
        if (stat("capturelogs", &st) == -1) {
            mkdir("capturelogs", 0700);
        }
        ss << "capturelogs" << "/" << (int)(ms_since_epoch()/1000);
        frame_logger.directory = ss.str();
        mkdir(frame_logger.directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        std::cout<<"opening: "<<frame_logger.directory << "/timestamps.txt"<<std::endl;
        frame_logger.timestamp_file.close();
        frame_logger.timestamp_file.open(frame_logger.directory + "/timestamps.txt");
        frame_logger.is_started = true;
        std::cout<<"Starting..."<<std::endl;
    }
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
	
	uint32_t rmask, gmask, bmask;

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
int sdlDisplay(SdlContext* ctx, uint8_t* image) {
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

void rgb24ToPGM(uint8_t *input, PGMBuffer *buffer, int width, int height) {

	int size = width * height + 256;
	buffer->data = (char*)malloc(size);
	
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
}

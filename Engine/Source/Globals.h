#pragma once
#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define CLEAR_LOG() clearLog();


void log(const char file[], int line, const char* format, ...);
void clearLog();
const char* string_format(const char* format, ...);


enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Super Awesomest Engine"
#define CAMERA_SPEED 5
#define CAMERA_TURNING_SPEED pi 
#define CAMERA_ORBITING_SPEED pi / 3

// Deletes a buffer
#define RELEASE( x ) \
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }
#ifndef INPUTFRAME_H
#define INPUTFRAME_H
#pragma once

#include "SDL_opengl.h"

// Data type used by InputDevice to return input data
struct InputFrame
{
	int time;
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

#endif

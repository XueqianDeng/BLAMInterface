#ifndef SPEEDBAR_H
#define SPEEDBAR_H
#pragma once

#include "SDL.h"
#include "SDL_opengl.h"
#include "Region2D.h"
#include "Rect.h"

class SpeedBar
{
private:
	GLfloat speed; // current speed
	GLfloat maxSpeed; // maximum speed to represent
	GLfloat minSpeed; // minimum speed
	GLfloat frameColor[3]; // color of external frame
	GLfloat barColor[3]; // color of bar
	GLfloat bgColor[3];

	GLfloat xpos;
	GLfloat ypos;
	GLfloat width;
	GLfloat height;

	GLint draw;

	Rect* frame;
	Rect* bar;
	Rect* background;

public:
	// constructor
	SpeedBar(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat min, GLfloat max,Rect* f, Rect*b,Rect* bg);

	// destructor
	~SpeedBar(){};

	void SetBounds(GLfloat max, GLfloat min);
	void UpdateSpeed(GLfloat currSpeed);

	void BarOn();
	void BarOff();

	void Draw();

};
#endif
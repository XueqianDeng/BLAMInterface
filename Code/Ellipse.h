#ifndef ELLIPSE_H
#define ELLIPSE_H
#pragma once

#include "SDL.h"
#include "SDL_opengl.h"
#include "Object2D.h"
#include "Circle.h"

// creates a circle object to render as the target
class Ellipse
{
private:
	GLint nVerts;
	GLfloat vertices[2][100]; // vertices in meters from center
	GLfloat color[3]; //
	GLfloat diameter;
	GLfloat height;
	GLfloat width;
	GLfloat xpos;
	GLfloat ypos;
	GLint border;
	GLint draw; // controls whether the object will be drawn or not

	//Object2D obj;
public:
	// constructor
	Ellipse(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat clr[]);
	
	// destructor
	~Ellipse(){}

	// sets the diameter
	void SetDiameter(GLfloat diam);
	void SetRadius(GLfloat rad);

	// draw the circle
	void Draw();

	void MakeVerts();

	void SetColor(GLfloat clr[]);

	void SetPos(GLfloat x, GLfloat y);
	void SetSize(GLfloat w, GLfloat h);

	GLfloat GetX();
	GLfloat GetY();
	GLfloat GetDiam();
	GLfloat GetRadius();

	void BorderOn();
	void BorderOff();
	void On();
	void Off();
	
	GLfloat Distance(Circle* c);
};

#endif
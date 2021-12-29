#ifndef RECT_H
#define RECT_H
#pragma once

#include "SDL.h"
#include "SDL_opengl.h"

class Rect
{
private:
	GLfloat vertices[2][4];
	GLfloat color[3];
	GLfloat xpos;
	GLfloat ypos;
	GLfloat width;
	GLfloat height;
	GLint draw;

public:
	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat clr[]);
	~Rect(){};

	void Draw();
	void On();
	void Off();

	void SetPos(GLfloat x, GLfloat y);
	void SetWidth(GLfloat w);
	void SetHeight(GLfloat h);
	void SetColor(GLfloat clr[]);

	void MakeVerts();
};
#endif
#include <cmath>
#include <limits>
#include <sstream>
#include <iostream>
#include <fstream>
#include <istream>

#include "Rect.h"

Rect::Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat clr[])
{
	xpos = x;
	ypos = y;
	width = w;
	height = h;
	color[0] = clr[0];
	color[1] = clr[1];
	color[2] = clr[2];
	draw = 1;

	MakeVerts();
}

void Rect::MakeVerts()
{
	vertices[0][0] = 0;
	vertices[1][0] = 0;
	vertices[0][1] = width;
	vertices[1][1] = 0;
	vertices[0][2] = width;
	vertices[1][2] = height;
	vertices[0][3] = 0;
	vertices[1][3] = height;
}

void Rect::Draw()
{
	if(draw)
	{
		glColor3f(color[0],color[1],color[2]);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(xpos, ypos, 0);
		for (int i = 0; i<4; i++)
		{
			glVertex3f(vertices[0][i] + xpos,vertices[1][i] + ypos, 0.0f);
		}
		glEnd();
		glColor3f(1.0f,1.0f,1.0f);
	}
}

void Rect::On()
{
	draw = 1;
}

void Rect::Off()
{
	draw = 0;
}

void Rect::SetPos(GLfloat x, GLfloat y)
{
	xpos = x;
	ypos = y;
	MakeVerts();
}

void Rect::SetWidth(GLfloat w)
{
	width = w;
	MakeVerts();
}

void Rect::SetHeight(GLfloat h)
{
	height = h;
	MakeVerts();
}

void Rect::SetColor(GLfloat clr[])
{
	color[0] = clr[0];
	color[1] = clr[1];
	color[2] = clr[2];
}
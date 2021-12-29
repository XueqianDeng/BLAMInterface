#include <cmath>
#include <limits>
#include <sstream>
#include <iostream>
#include <fstream>
#include <istream>

#include "Circle.h"
//#include "Object2D.h"

Circle::Circle(GLfloat x, GLfloat y, GLfloat diam, GLfloat clr[])
{
	diameter = diam;
	SetColor(clr);
	GLfloat defaultBorderColor[3] = {0.0f,0.0f,0.0f};
	SetBorderColor(defaultBorderColor);
	xpos = x;
	ypos = y;
	nVerts = 100;
	borderWidth = .001f;
	MakeVerts();
	border = 1;
	draw = 0;
	fill = 1;

	//vertices = (GLfloat**) malloc(2*sizeof(GLfloat*));  
	//vertices[1] = (GLfloat*) malloc(nVerts*sizeof(GLfloat)); 
	//vertices[2] = (GLfloat*) malloc(nVerts*sizeof(GLfloat)); 
}

void Circle::SetDiameter(GLfloat diam)
{
	diameter = diam;
	MakeVerts();
}

void Circle::SetRadius(GLfloat rad)
{
	diameter = 2*rad;
	MakeVerts();
}

void Circle::MakeVerts()
{
	nVerts = 100; // use 100 vertices
	for (int i=0; i<nVerts; i++)
	{
		GLfloat theta = 2*3.14159*i/(nVerts-1);
		vertices[0][i] = diameter*sin(theta);
		vertices[1][i] = diameter*cos(theta);
		verticesInner[0][i] = (diameter-borderWidth)*sin(theta);
		verticesInner[1][i] = (diameter-borderWidth)*cos(theta);
	}
}

void Circle::SetColor(GLfloat clr[])
{
	color[0] = clr[0];
	color[1] = clr[1];
	color[2] = clr[2];
}

void Circle::SetBorderColor(GLfloat clr[])
{
	borderColor[0] = clr[0];
	borderColor[1] = clr[1];
	borderColor[2] = clr[2];
}

void Circle::SetPos(GLfloat x, GLfloat y)
{
	xpos = x;
	ypos = y;
}

void Circle::Draw()
{
	if(draw)
	{
		if(border)
		{
			// Draw black circle first
			glColor3f(borderColor[0],borderColor[1],borderColor[2]);
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(xpos, ypos, 0);	
			for (int i = 0; i<nVerts; i++)
			{
				glVertex3f(vertices[0][i] + xpos,vertices[1][i] + ypos, 0.0f);
			}
			glEnd();

			glColor3f(color[0],color[1],color[2]);
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(xpos, ypos, 0);	
			for (int i = 0; i<nVerts; i++)
			{
				glVertex3f(verticesInner[0][i] + xpos,verticesInner[1][i] + ypos, 0.0f);
			}

			glEnd();
			glColor3f(1.0f,1.0f,1.0f);
			//glColor4f(1.0f,1.0f,1.0f,0.5f);
		}
		else
		{
			// Draw the polygon
			glColor3f(color[0],color[1],color[2]);
			//glColor4f(color[0],color[1],color[2],1.0f);
	
			glBegin(GL_TRIANGLE_FAN);

			glVertex3f(xpos, ypos, 0);
			for (int i = 0; i<nVerts; i++)
			{
				glVertex3f(vertices[0][i] + xpos,vertices[1][i] + ypos, 0.0f);
			}

			glEnd();
			glColor3f(1.0f,1.0f,1.0f);
		}
	}

}

GLfloat Circle::GetX()
{
	return xpos;
}

GLfloat Circle::GetY()
{
	return ypos;
}

GLfloat Circle::GetDiam()
{
	return diameter;
}

void Circle::BorderOn()
{
	border = 1;
}

void Circle::BorderOff()
{
	border = 0;
}

void Circle::FillOn()
{
	fill = 1;
}

void Circle::FillOff()
{
	fill = 0;
}

void Circle::On()
{
	draw = 1;
}

void Circle::Off()
{
	draw = 0;
}

GLfloat Circle::Distance(Circle* c)
{
	return sqrtf(powf(c->GetX() - xpos, 2.0f) + powf(c->GetY() - ypos, 2.0f));
}

GLfloat Circle::GetRadius()
{
	return diameter/2;
}

void Circle::SetBorderWidth(GLfloat w)
{
	borderWidth = w;
}
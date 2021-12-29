#include <cmath>
#include <limits>
#include <sstream>
#include <iostream>
#include <fstream>
#include <istream>

#include "EllipseE.h"
#include "Circle.h"
#include "Object2D.h"

EllipseE::EllipseE(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat clr[])
{
	diameter = 2;
	width = 1;
	height = h;
	SetColor(clr);
	xpos = x;
	ypos = y;
	nVerts = 100;
	MakeVerts();
	border = 1;
	draw = 0;
	//vertices = (GLfloat**) malloc(2*sizeof(GLfloat*));  
	//vertices[1] = (GLfloat*) malloc(nVerts*sizeof(GLfloat)); 
	//vertices[2] = (GLfloat*) malloc(nVerts*sizeof(GLfloat)); 
}

void EllipseE::SetDiameter(GLfloat diam)
{
	diameter = diam;
	MakeVerts();
}

void EllipseE::SetRadius(GLfloat rad)
{
	diameter = 2*rad;
	MakeVerts();
}

void EllipseE::MakeVerts()
{
	int nVerts = 100; // use 100 vertices
	for (int i=0; i<nVerts; i++)
	{
		double theta = 2*3.14159*i/(nVerts-1);
		vertices[0][i] = width*diameter*sin(theta);
		vertices[1][i] = height*diameter*cos(theta);
	}
}

void EllipseE::SetColor(GLfloat clr[])
{
	color[0] = clr[0];
	color[1] = clr[1];
	color[2] = clr[2];
}

void EllipseE::SetPos(GLfloat x, GLfloat y)
{
	xpos = x;
	ypos = y;
}

void EllipseE::Draw()
{
	if(draw)
	{
		if(border)
		{
			// Draw black circle firsn
			glColor3f(0,0,0);
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
				glVertex3f(vertices[0][i]*.9 + xpos,vertices[1][i]*.9 + ypos, 0.0f);
			}

			glEnd();
			glColor3f(1.0f,1.0f,1.0f);
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

GLfloat EllipseE::GetX()
{
	return xpos;
}

GLfloat EllipseE::GetY()
{
	return ypos;
}

GLfloat EllipseE::GetDiam()
{
	return diameter;
}

void EllipseE::BorderOn()
{
	border = 1;
}

void EllipseE::BorderOff()
{
	border = 0;
}

void EllipseE::On()
{
	draw = 1;
}

void EllipseE::Off()
{
	draw = 0;
}

GLfloat EllipseE::Distance(Circle* c)
{
	return sqrtf(powf(c->GetX() - xpos, 2.0f) + powf(c->GetY() - ypos, 2.0f));
}

GLfloat EllipseE::GetRadius()
{
	return diameter/2;
}

void EllipseE::SetSize(GLfloat w, GLfloat h)
{
	width = w;
	height = h;
	MakeVerts();
}
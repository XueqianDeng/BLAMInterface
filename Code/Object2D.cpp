#include <cmath>
#include "Object2D.h"

Object2D::Object2D(Image* i)
{
	image = i;
	angle = 0.0f;
	sizex = .1;
	sizey = .1;
	drawOn = 0;
}

GLfloat Object2D::GetWidth() const
{
	return image->GetWidth();
}

GLfloat Object2D::GetHeight() const
{
	return image->GetHeight();
}

GLfloat Object2D::GetX() const
{
	return xPos;
}

GLfloat Object2D::GetY() const
{
	return yPos;
}

void Object2D::SetPos(GLfloat x, GLfloat y)
{
	xPos = x;
	yPos = y;
}

void Object2D::SetAngle(GLfloat theta)
{
	angle = theta;
}

void Object2D::Draw()
{
	if(drawOn)
		image->Draw(xPos, yPos, sizex, sizey, angle);
}

void Object2D::Draw(GLfloat width, GLfloat height)
{
	if(drawOn)
		image->Draw(xPos, yPos, width, height, angle);
}

float Object2D::Distance(Object2D* ob1, Object2D* ob2)
{
	return sqrtf(powf(ob2->xPos - ob1->xPos, 2.0f) + powf(ob2->yPos - ob1->yPos, 2.0f));
}

void Object2D::SetSize(GLfloat x, GLfloat y)
{
	sizex = x;
	sizey = y;
}

void Object2D::On()
{
	drawOn = 1;
}

void Object2D::Off()
{
	drawOn = 0;
}


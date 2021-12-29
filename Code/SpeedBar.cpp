#include <cmath>
#include <limits>
#include <sstream>
#include <iostream>
#include <fstream>
#include <istream>

#include "SpeedBar.h"
#include "Rect.h"

SpeedBar::SpeedBar(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat min, GLfloat max, Rect* f, Rect* b, Rect* bg)
{
	speed = 0;
	maxSpeed = max;
	minSpeed = min;
	xpos = x;
	ypos = y;
	width = w; // width on screen
	height = h; // height on screen

	frame = f;
	bar = b;
	background = bg;

	frame->SetPos(xpos,ypos+height*minSpeed/maxSpeed);
	frame->SetWidth(width);
	frame->SetHeight(height*(maxSpeed-minSpeed)/maxSpeed);

	bar->SetPos(xpos+width/4,ypos);
	bar->SetWidth(width/2);
	bar->SetHeight(speed*height/maxSpeed);

	background->SetPos(xpos,ypos);
	background->SetWidth(width);
	background->SetHeight(height+.05f);

	frameColor[0] = .5f;
	frameColor[1] = .5f;
	frameColor[2] = .5f;

	barColor[0] = 0.0f;
	barColor[1] = 0.0f;
	barColor[2] = 0.0f;

	bgColor[0] = .95f;
	bgColor[1] = .95f;
	bgColor[2] = .95f;

	frame->SetColor(frameColor);
	bar->SetColor(barColor);
	bg->SetColor(bgColor);
	

}

void SpeedBar::SetBounds(GLfloat min, GLfloat max)
{
	maxSpeed = max;
	minSpeed = min;
}

void SpeedBar::UpdateSpeed(GLfloat currSpeed)
{
	speed = currSpeed;
	bar->SetHeight(speed*height/maxSpeed);
}

void SpeedBar::BarOn()
{
	draw = 1;
}

void SpeedBar::BarOff()
{
	draw = 0;
}

void SpeedBar::Draw()
{
	background->Draw();
	frame->Draw();
	bar->Draw();
}
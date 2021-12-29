#ifndef HANDCURSOR_H
#define HANDCURSOR_H
#pragma once

#include "Circle.h"
#include "SDL.h"
#include "SDL_opengl.h"
#define NHIST 5

// object to keep track of hand position/velocity and display cursor
class HandCursor
{
private:
	GLfloat xpos;
	GLfloat ypos;
	GLfloat xhist[NHIST]; // x velocity history
	GLfloat yhist[NHIST]; // y velocity history
	GLfloat xhistScr[NHIST]; // x screen position history 
	GLfloat yhistScr[NHIST]; // y screen position history
	GLfloat xvel;
	GLfloat yvel;

	GLfloat xScr;
	GLfloat yScr;
	GLfloat x0;
	GLfloat y0;

	GLfloat xoffset;
	GLfloat yoffset;

	GLfloat xgain; 
	GLfloat ygain;

	GLfloat xFB;
	GLfloat yFB;

	GLint draw; // controls whether cursor will be drawn or not
	GLfloat rotMat[4];
	GLfloat color[3];
	Circle* circ;

	GLint clamp; // controls whether clamp is on
	GLfloat thetaClamp; // clamp angle

	GLint mirror; // controls whether mirror reversal is on

public:
	HandCursor(Circle* c);
	~HandCursor(){};

	void Draw();
	GLfloat GetX();
	GLfloat GetY();
	GLfloat GetXVel();
	GLfloat GetYVel();
	GLfloat GetVel();

	GLfloat GetXfb();
	GLfloat GetYfb();

	void UpdatePos(GLfloat x, GLfloat y);

	void SetRotation(GLfloat theta); // set rotation angle in degrees CW
	void SetOrigin(GLfloat x, GLfloat y); // set origin around which rotation occurs
	GLfloat Distance(GLfloat x, GLfloat y); // get distance
	GLfloat Distance(Circle* c);
	void SetGain(GLfloat xg, GLfloat yg); // set gain
	
	void SetClamp(GLfloat th);
	void SetClampC(Circle* c1, Circle* c2); // sets a clamp through the center of two points
	void Null();
	void ClampOff();
	void On();
	void Off();

	void SetMirror(GLint mirrorOn);

	void Perturb(GLfloat dx, GLfloat dy);
	bool HitTarget(Circle* targ);
	bool HitTarget(GLfloat x, GLfloat y, GLfloat rad);
	void CalcCrossingPoint(GLfloat x_start, GLfloat y_start, GLfloat targ_dist);
};

#endif
#ifndef DYNOBJ_H
#define DYNOBJ_H
#pragma once

#include "Circle.h"
#include "SDL.h"
#include "SDL_opengl.h"
#define NHIST 5

// dynamical object under user control
class DynObj
{
private:
	GLfloat xpos; // object position
	GLfloat ypos;
	GLfloat xvel; // object velocity
	GLfloat yvel;

	GLfloat xposprev; // object position
	GLfloat yposprev;

	GLfloat A[4]; // object position-dynamics
	GLfloat B[4]; // object velocity-dynamics

	GLfloat C[4]; // object control dynamics

	GLint draw; // whether or not to draw the object
	Circle* circ;
	GLfloat color[3];

public:
	DynObj(Circle* c);
	~DynObj(){};

	void Draw();
	GLfloat GetX();
	GLfloat GetY();
	GLfloat GetXVel();
	GLfloat GetYVel();
	GLfloat GetVel();
	GLfloat Distance(Circle* c);

	void UpdateA(GLfloat A11, GLfloat A12, GLfloat A21, GLfloat A22);
	void UpdateB(GLfloat B11, GLfloat B12, GLfloat B21, GLfloat B22);
	void UpdateC(GLfloat C11, GLfloat C12, GLfloat C21, GLfloat C22);

	void UpdateDynamics(GLfloat u1, GLfloat u2);
	void SetPos(GLfloat x, GLfloat y);

	void On();
	void Off();
	void SetColor(GLfloat clr[]);

	bool HitTarget(Circle* targ);
	bool HitTarget(GLfloat x, GLfloat y, GLfloat rad);

	void Perturb(GLfloat dx, GLfloat dy);
};

#endif
#include <cmath>
#include "DynObj.h"
#include "Circle.h"
#include "config.h"
#define PI 3.14159

DynObj::DynObj(Circle* c)
{
	color[0] = 1.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	circ = c;
	circ->BorderOff();
	circ->On();
	circ->SetColor(color);
	draw = 1;

	xpos = START_X;
	ypos = START_Y;

	xvel = 0;
	yvel = 0;

	// Change these numbers to change the dynamics of the object
	A[0] = 0; // probably leave these all zero
	A[1] = 0;
	A[2] = 0;
	A[3] = 0;

	B[0] = -3;
	B[1] = 0; 
	B[2] = 0;
	B[3] = -3;

	C[0] = 5;
	C[1] = -2;
	C[2] = 2;
	C[3] = 5;
	//////////////////////////////////////////////////////////
};

void DynObj::Draw()
{
	if(draw)
	{
		circ->Draw();
	}
}

GLfloat DynObj::GetX()
{
	return xpos;
}

GLfloat DynObj::GetY()
{
	return ypos;
}

GLfloat DynObj::GetXVel()
{
	return xvel;
}

GLfloat DynObj::GetYVel()
{
	return yvel;
}

GLfloat DynObj::GetVel()
{
	return sqrtf(xvel*xvel + yvel*yvel);
}

void DynObj::UpdateA(GLfloat A11, GLfloat A12, GLfloat A21, GLfloat A22)
{
	A[0] = A11; A[1] = A12; A[2] = A21; A[3] = A22;
}

void DynObj::UpdateB(GLfloat B11, GLfloat B12, GLfloat B21, GLfloat B22)
{
	B[0] = B11; B[1] = B12; B[2] = B21; B[3] = B22;
}

void DynObj::UpdateC(GLfloat C11, GLfloat C12, GLfloat C21, GLfloat C22)
{
	C[0] = C11; C[1] = C12; C[2] = C21; C[3] = C22;
}

void DynObj::UpdateDynamics(GLfloat u1, GLfloat u2)
{
	GLfloat newxpos;
	GLfloat newypos;
	GLfloat newxvel;
	GLfloat newyvel;

	newxpos = xpos + DELTA*xvel;
	newypos = ypos + DELTA*yvel;

	//newvel[0] = vel[0] + DELTA*( A[0]*pos[0] + A[1]*pos[1] + B[0]*vel[0] + B[1]*vel[1] + C[0]*u1 + C[1]*u2);
	//newvel[1] = vel[1] + DELTA*( A[2]*pos[0] + A[3]*pos[1] + B[2]*vel[0] + B[3]*vel[1] + C[2]*u1 + C[3]*u2);
	newxvel = xvel + DELTA*( A[0]*xpos + A[1]*ypos + B[0]*xvel + B[1]*yvel + C[0]*u1 + C[1]*u2);
	newyvel = yvel + DELTA*( A[2]*xpos + A[3]*ypos + B[2]*xvel + B[3]*yvel + C[2]*u1 + C[3]*u2);

	xposprev = xpos;
	yposprev = ypos;
	xpos = newxpos;
	ypos = newypos;
	xvel = newxvel;
	yvel = newyvel;

	circ->SetPos(xpos,ypos);
}

void DynObj::SetPos(GLfloat x, GLfloat y)
{
	xpos = x;
	ypos = y;
}

void DynObj::On()
{
	draw = 1;
}

void DynObj::Off()
{
	draw = 0;
}

void DynObj::SetColor(GLfloat clr[])
{
	color[0] = clr[0];
	color[1] = clr[1];
	color[2] = clr[2];

	circ->SetColor(color);
}

bool DynObj::HitTarget(Circle* targ)
{
	return HitTarget(targ->GetX(),targ->GetY(), 2*targ->GetRadius());
}

// check if the cursor path ever crossed through the target
bool DynObj::HitTarget(GLfloat x, GLfloat y, GLfloat rad)
{
	if(powf(xpos-x,2.0f)+powf(ypos-y,2.0f) < rad*rad || powf(xposprev-x,2.0f) + powf(yposprev-y,2.0f) < rad*rad) 
		return 1; // if current or previous sample is in circle, then return HIT
	else
	{
		// otherwise, need to check if cursor path crossed the circle (in case samples skipped over target)

		// solve simultaneous equations of circle and line. Check if either solution is between consecutive samples
		GLfloat A = powf(xpos - xposprev,2.0f) + powf(ypos - yposprev,2.0f);
		GLfloat B = 2*((xpos - xposprev)*(xposprev - x) + (ypos - yposprev)*(yposprev - y));
		GLfloat C = powf(xposprev - x,2.0f) + powf(yposprev - y,2.0f) - rad*rad;

		if(B*B - 4*A*C > 0) // if real solution exists, infinite line intersects target
		{
			GLfloat t1 = (-B + sqrtf(B*B - 4*A*C))/(2*A);
			if(t1 < 1 && t1 > 0) // line intersects target in between the two points
			{
				return 1;
			}
			else
				return 0;
		}
		else
			return 0;
	}
}

GLfloat DynObj::Distance(Circle* c)
{
	return sqrtf(powf(c->GetX() - xpos, 2.0f) + powf(c->GetY() - ypos, 2.0f));
}

void DynObj::Perturb(GLfloat dx, GLfloat dy)
{
	xpos = xpos+dx;
	ypos = ypos+dy;
}
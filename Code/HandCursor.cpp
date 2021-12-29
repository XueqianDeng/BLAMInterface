#include <cmath>
#include "HandCursor.h"
#include "Circle.h"
#define PI 3.14159
HandCursor::HandCursor(Circle* c)
{
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 1.0f;
	circ = c;
	circ->BorderOff();
	circ->On();
	//circ->SetColor(color);
	draw = 1;
	yvel = 0;
	xvel = 0;
	xpos = 0;
	ypos = 0;
	x0 = .5;
	y0 = .5;
	xgain = 1;
	ygain = 1;
	mirror = 0;

	xoffset = 0;
	yoffset = 0;

	rotMat[0] = 1;
	rotMat[1] = 0;
	rotMat[2] = 0;
	rotMat[3] = 1;
	
	clamp = 0;

	for (int i=0; i<5; i++)
	{
		xhist[i] = 0;
		yhist[i] = 0;
		xhistScr[i] = 0;
		yhistScr[i] = 0;
	}

	xFB = 0;
	yFB = 0;
}

void HandCursor::Draw()
{
	if(draw)
	{
		circ->Draw();
	}
}

void HandCursor::UpdatePos(GLfloat x, GLfloat y)
{
	
	for (int i=NHIST-1; i>0; i--)
	{
		xhist[i] = xhist[i-1];
		yhist[i] = yhist[i-1];
	}
	xhist[0] = x;
	yhist[0] = y;

	//---- Estimate velocity as slope of last 5 observations ----
	// get mean
	GLfloat xmean = 0;
	GLfloat ymean = 0;
	for (int i=0; i<NHIST; i++)
	{
		xmean += xhist[i];
		ymean += yhist[i];
	}
	xmean = xmean/NHIST;
	ymean = ymean/NHIST;
	
	// compute xvel as slope of data over last few 5 samples
	xvel = (-2*xhist[4] -1*xhist[3] + 1*xhist[1] + 2*xhist[0])/10;
	yvel = (-2*yhist[4] -1*yhist[3] + 1*yhist[1] + 2*yhist[0])/10;

	xpos = x;
	ypos = y;

	if(clamp)
	{
		GLfloat r = sqrtf((xpos-x0)*(xpos-x0) + (ypos-y0)*(ypos-y0));
		if(1)
		{
			xScr = r*sin(thetaClamp) + x0;
			yScr = r*cos(thetaClamp) + y0;
		}
		else
		{
			xScr = -r*sin(thetaClamp) + x0;
			yScr = -r*cos(thetaClamp) + y0;
		}
	}
	else
	{
		// apply rotation
		xScr = rotMat[0]*(xpos-x0) + rotMat[1]*(ypos-y0) + x0;
		yScr = rotMat[2]*(xpos-x0) + rotMat[3]*(ypos-y0) + y0;

		// apply gain
		xScr = xgain*(xScr-x0) + x0;
		yScr = ygain*(yScr-y0) + y0;

		// apply offset
		xScr = xScr + xoffset;
		yScr = yScr + yoffset;

		// apply mirroring about x=0
		xScr = mirror*(-xScr+2*x0)+(1-mirror)*xScr;
	}

	for (int i=NHIST-1; i>0; i--)
	{
		xhistScr[i] = xhistScr[i-1];
		yhistScr[i] = yhistScr[i-1];
	}
	xhistScr[0] = xScr;
	yhistScr[0] = yScr;

	circ->SetPos(xScr, yScr);
}

void HandCursor::SetRotation(GLfloat theta)
{
	rotMat[0] = cos(theta*PI/180);
	rotMat[1] = sin(theta*PI/180);
	rotMat[2] = -sin(theta*PI/180);
	rotMat[3] = cos(theta*PI/180);
}

void HandCursor::SetOrigin(GLfloat x, GLfloat y)
{
	x0 = x;
	y0 = y;
}

GLfloat HandCursor::GetXVel()
{
	return xvel;
}

GLfloat HandCursor::GetYVel()
{
	return yvel;
}

GLfloat HandCursor::Distance(GLfloat x, GLfloat y)
{
	return sqrtf(powf(xpos - x, 2.0f) + powf(ypos - y, 2.0f));
}

GLfloat HandCursor::Distance(Circle* c)
{
	return sqrtf(powf(xScr - c->GetX(), 2.0f) + powf(yScr - c->GetY(),2.0f));
}

void HandCursor::Null()
{
	rotMat[0] = 1;
	rotMat[1] = 0;
	rotMat[2] = 0;
	rotMat[3] = 1;
}

void HandCursor::SetClampC(Circle* c1, Circle* c2)
{
	GLfloat th = atan2(c2->GetX()-c1->GetX(),c2->GetY() - c1->GetY());
	SetClamp(th*PI/180);
}

void HandCursor::SetClamp(GLfloat th)
{
	/* projection onto fixed line
	th = th*180/3.14159;
	rotMat[0] = cos(th)*cos(th);
	rotMat[1] = -cos(th)*sin(th);
	rotMat[2] = -cos(th)*sin(th);
	rotMat[3] = sin(th)*sin(th);*/

	clamp = 1;
	thetaClamp = th*180/PI; // clamp angle
	/*
	rotMat[0]=cos(th);
	rotMat[1]=cos(th);
	rotMat[2]=sin(th);
	rotMat[3]=sin(th);
	*/
}

void HandCursor::ClampOff()
{
	clamp = 0;
}

void HandCursor::SetMirror(GLint mirrorOn)
{
	mirror = mirrorOn;
}

void HandCursor::On()
{
	draw = 1;
}

void HandCursor::Off()
{
	draw = 0;
}

GLfloat HandCursor::GetX()
{
	return xScr;
}

GLfloat HandCursor::GetY()
{
	return yScr;
}

GLfloat HandCursor::GetXfb()
{
	return xFB;
}

GLfloat HandCursor::GetYfb()
{
	return yFB;
}

GLfloat HandCursor::GetVel()
{
	return sqrtf(xvel*xvel + yvel*yvel);
}

bool HandCursor::HitTarget(Circle* targ)
{
	return HitTarget(targ->GetX(), targ->GetY(), 2*targ->GetRadius());
}

// check if the cursor path ever crossed through the target
bool HandCursor::HitTarget(GLfloat x, GLfloat y, GLfloat rad)
{
	if(powf(xhistScr[0]-x,2.0f)+powf(yhistScr[0]-y,2.0f) < rad*rad || powf(xhistScr[1]-x,2.0f) + powf(yhistScr[1]-y,2.0f) < rad*rad) 
		return 1; // if current or previous sample is in circle, then return HIT
	else
	{
		// otherwise, need to check if cursor path crossed the circle (in case samples skipped over target)

		// solve simultaneous equations of circle and line. Check if either solution is between consecutive samples
		GLfloat A = powf(xhistScr[0] - xhistScr[1],2.0f) + powf(yhistScr[0] - yhistScr[1],2.0f);
		GLfloat B = 2*((xhistScr[0] - xhistScr[1])*(xhistScr[1] - x) + (yhistScr[0] - yhistScr[1])*(yhistScr[1] - y));
		GLfloat C = powf(xhistScr[1] - x,2.0f) + powf(yhistScr[1] - y,2.0f) - rad*rad;

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

void HandCursor::SetGain(GLfloat xg, GLfloat yg)
{
	xgain = xg;
	ygain = yg;
}

void HandCursor::Perturb(GLfloat dx, GLfloat dy)
{
	xoffset = dx;
	yoffset = dy;
}

void HandCursor::CalcCrossingPoint(GLfloat x_start, GLfloat y_start, GLfloat target_dist)
{

	GLfloat theta = atan2(yhistScr[0]-y_start,xhistScr[0]-x_start);
	xFB = x_start+target_dist*cos(theta);
	yFB = y_start+target_dist*sin(theta);

	GLfloat y_solution = 0;

	//GLfloat lambda = (xhistScr[0]-xhistScr[1])/(yhistScr[0]-yhistScr[1]); //slope
	//GLfloat A = lambda*lambda + 1;
	//GLfloat B = -2*lambda*lambda*yhistScr[1]+2*lambda*(xhistScr[1]-x_start)-2*y_start;
	//GLfloat C = powf(lambda*yhistScr[1]-xhistScr[1]+x_start,2.0f)+y_start*y_start-target_dist*target_dist;

	//if(B*B - 4*A*C > 0) // if real solution exists, infinite line intersects target
	//	{
	//		GLfloat t1 = (-B + sqrtf(B*B - 4*A*C))/(2*A);
	//		GLfloat t2 = (-B - sqrtf(B*B - 4*A*C))/(2*A);

	//		//get the right solution, which should be between the two last points
	//		if ((t1-yhistScr[0])*(t1-yhistScr[1]) <= 0)
	//		{
	//			y_solution = t1;
	//		}
	//		else
	//		{
	//			y_solution = t2;
	//		}

	//		GLfloat x_solution = xhistScr[1]+lambda*(y_solution - yhistScr[1]);

	//		xFB = x_solution;
	//		yFB = y_solution;
	//	}
}

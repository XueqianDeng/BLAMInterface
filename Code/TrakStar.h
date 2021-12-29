#ifndef TRAKSTAR_H
#define TRAKSTAR_H
#pragma once

#include <windows.h>
#include "TrackBird.h"
#include "InputDevice.h"
#include "../trakSTAR/ATC3DG.h"
#include "InputFrame.h"

class TrakStar
{
private:
	int index; // tracker number

public:
	TrakStar(int i);
	~TrakStar() {};

	InputFrame GetFrame();

};


#endif
#include <cmath>
#include <Windows.h>
#include "TrakStar.h"


int TrakStar::TrakStar(int i)
{
	index = i;
}

InputFrame TrakStar::GetFrame()
{
	InputFrame input;

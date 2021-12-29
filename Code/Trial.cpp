#include "Trial.h"
#include "ExtFile.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#define NCOLUMNS 8

Trial::Trial(char* filename)
{
	tFile = new ExtFile(filename, NCOLUMNS);
}

Trial::~Trial(void)
{
}

int Trial::Number(void)
{
	return tFile->getCurrent()[0];
}

double Trial::TargX(void)
{
	return tFile->getCurrent()[1];
}

double Trial::TargY(void)
{
	return tFile->getCurrent()[2];
}

double Trial::Rotation(void)
{
	return tFile->getCurrent()[3];
}

//double Trial::Perturbation(void)
//{
//	return tFile->getCurrent()[4];
//}

int Trial::Mirror(void)
{
	return tFile->getCurrent()[4];
}

bool Trial::ShowAim(void)
{
	return tFile->getCurrent()[5];
}

bool Trial::VisualFeedback(void)
{
	return tFile->getCurrent()[6];
}

int Trial::Type(void)
{
	return tFile->getCurrent()[7];
}

//bool Trial::Clamp(void)
//{
//	return tFile->getCurrent()[4];
//}

double* Trial::curTrial(void)
{
	return tFile->getCurrent();
}

void Trial::Next(void)
{
	tFile->getNext();
}
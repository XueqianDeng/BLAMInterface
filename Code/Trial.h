#ifndef TRIAL_H
#define TRIAL_H
#pragma once
#include "ExtFile.h"

class Trial
{
private:
	ExtFile* tFile;

public:
	Trial(char* filename);
	~Trial(void);

	int Number(void);
	bool Clamp(void);
	double Rotation(void);
	double TargX(void);
	double TargY(void);
	int Mirror(void);
	bool ShowAim(void);
	bool VisualFeedback(void);
	int Type(void);
	void Next(void); // move to next trial

	double* curTrial(void);

};

#endif
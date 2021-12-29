#ifndef DATAWRITER_H
#define DATAWRITER_H
#pragma once

#include <fstream>
#include "InputFrame.h"
//#include "Flock_of_Birds\Bird.h"
#include "TrackBird.h"
#include "trakSTAR/ATC3DG.h"
#include "HandCursor.h"

// Records data
class DataWriter
{
private:
	std::ofstream file;
public:
	/* filename defines a custom name to save the file as. If filename is NULL,
	 * a name will be generated automatically.
	 */
	DataWriter(TrackSYSCONFIG *sysconfig, const char* filename = NULL);
	DataWriter();
	~DataWriter();
	// Record a new line of data
	// deviceNo is a number identifying the device sending the data
	// frame is the frame of input data
	void Record(int deviceNo, TrackDATAFRAME frame, int state);
	void Record(HandCursor* c1, HandCursor* c2, HandCursor* c3, int state);
	void Record(HandCursor* c1, HandCursor* c2, HandCursor* c3, int state, bool perturbed, Uint32 time);
	void Close();
};

#endif

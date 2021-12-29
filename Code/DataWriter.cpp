//Modified to add target inputs, 9/10/2012 AW

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "DataWriter.h"


using namespace std;

DataWriter::DataWriter(TrackSYSCONFIG *sysconfig, const char* filename)
{

	//record current date/time
	time_t current_time = time(0);
	tm* ltm = localtime(&current_time);
	stringstream ss1,ss2, ss3;
	
	ss1 << setw(4) << setfill('0') << ltm->tm_year + 1900;
	ss1 << setw(2) << setfill('0') << ltm->tm_mon + 1;
	ss1 << setw(2) << setfill('0') << ltm->tm_mday;
	//ss << "_";
	
	ss2 << setw(2) << setfill('0') << ltm->tm_hour;
	ss2 << setw(2) << setfill('0') << ltm->tm_min;
	ss2 << setw(2) << setfill('0') << ltm->tm_sec;


	// If no filename was supplied, use the date and time
	if (filename == NULL)
	{
		//stringstream ss3;
		ss3 << ss1.str() << "_" << ss2.str() << ".txt";
		file.open(ss3.str(), ios::out);
	}
	else
	{
		//append timestamp to prevent overwrites
		//stringstream ss3;
		ss3 << filename << ss1.str() << ss2.str() << ".dat";
		file.open(ss3.str(), ios::out);
	}

	// Write headers to file
	if (file.is_open())
	{

		//write current date
		file << "Date " << ss1.str() << endl;
		file << "Time " << ss2.str() << endl;
		
		//write bird config parameters of interest
		//file << "Number_of_Birds " << sysconfig->byNumDevices << endl;
		file << "Sampling_Rate " << sysconfig->measureRate << endl;

		file << "--" << endl;
		
		file << "LeftX "
			<< "LeftY "
			<< "RightX "
			<< "RightY "
			<< "CursX "
			<< "CursY "
			<< "State "
			<< "Perturbed "
			//<< "HandZ "
			//<< "HandYaw "
			//<< "HandPitch "
			//<< "HandRoll "
			<< endl;

		file << "-----" << endl;  //flag designator for finding start of the data stream.  everything above is header

	}
}

DataWriter::DataWriter()
{
	//file.open("Test", ios::out);
}

DataWriter::~DataWriter()
{
	file.close();
}

void DataWriter::Record(int deviceNo, TrackDATAFRAME frame, int state)
{
	// Write data
	if (file.is_open())
	{
		file << deviceNo << " "
			//<< frame.time << " "
			<< frame.x[deviceNo] << " "
			<< frame.y[deviceNo] << " "
			<< frame.z[deviceNo] << " "
			<< state << " "
			//<< frame.theta << " "
			//<< frame.pitch << " "
			//<< frame.roll << " "
			<< endl;
	}
}

void DataWriter::Close()
{
	file.close();
}

void DataWriter::Record(HandCursor* c1, HandCursor* c2, HandCursor* c3, int state)
{
	if (file.is_open())
	{
		file << c1->GetX() << ' '
			<< c1->GetY() << ' '
			<< c2->GetX() << ' '
			<< c2->GetY() << ' '
			<< c3->GetX() << ' '
			<< c3->GetY() << ' '
			<< state << ' '
			//<< frame.theta << " "
			//<< frame.pitch << " "
			//<< frame.roll << " "
			<< endl;
	}
}

void DataWriter::Record(HandCursor* c1, HandCursor* c2, HandCursor* c3, int state, bool perturbed, Uint32 time)
{
	if (file.is_open())
	{
		file << c1->GetX() << ' '
			<< c1->GetY() << ' '
			<< c2->GetX() << ' '
			<< c2->GetY() << ' '
			<< c3->GetX() << ' '
			<< c3->GetY() << ' '
			<< state << ' '
			<< perturbed << ' '
			<< time << ' '
			//<< frame.theta << " "
			//<< frame.pitch << " "
			//<< frame.roll << " "
			<< endl;
	}
}
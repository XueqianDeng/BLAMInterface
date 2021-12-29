#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "DataWriter.h"
using namespace std;

DataWriter::DataWriter(const char* filename)
{
	// If no filename was supplied, use the date and time
	if (filename == NULL)
	{
		time_t current_time = time(0);
		tm* ltm = localtime(&current_time);
		stringstream ss;
		ss << "Data/";
		ss << setw(4) << setfill('0') << ltm->tm_year + 1900;
		ss << setw(2) << setfill('0') << ltm->tm_mon + 1;
		ss << setw(2) << setfill('0') << ltm->tm_mday;
		ss << "_";
		ss << setw(2) << setfill('0') << ltm->tm_hour;
		ss << setw(2) << setfill('0') << ltm->tm_min;
		ss << setw(2) << setfill('0') << ltm->tm_sec;
		ss << ".txt";
		file.open(ss.str(), ios::out);
	}
	else
	{
		file.open(filename, ios::out);
	}

	// Write headers to file
	if (file.is_open())
	{
		file << "Device No. "
			<< "Time "
			<< "X "
			<< "Y "
			<< "Angle\n";
	}
}

DataWriter::~DataWriter()
{
	file.close();
}

void DataWriter::Record(int deviceNo, InputFrame frame)
{
	// Write data
	if (file.is_open())
	{
		file << deviceNo << " "
			<< frame.time << " "
			<< frame.x << " "
			<< frame.y << " "
			<< frame.theta << endl;
	}
}

void DataWriter::Close()
{
	file.close();
}
#ifndef EXTFILE_H
#define EXTFILE_H
#include <fstream>
#include <iostream>
#include <iomanip>

class ExtFile{
private:
	std::ifstream file;
	int numCols;
	double* currRowContents;
	//char* currRowChar;
public:
	//ExtFile();
	ExtFile(char* filename, int cols);
	~ExtFile();
	//ExtFile(string fname, int cols); // construct from a file
	double* getNext();     // get next element in the sequence
	//void setFile(string fname, int cols);
	double* getCurrent(){return currRowContents;};
	//string getFname(){return filename;};
	//int getLength(){return numRows;};
};

#endif


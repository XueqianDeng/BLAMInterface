#include "ExtFile.h"
#include <string>

using namespace std;
/**
 * Constructor. Reads in file "filename" and stores the contents in the variable currRowContents.
 *
 * @param filename - string containing the filename
 * @param cols 	   - number of columns in the file. (NB - no need to specify number of columns)
 */

/*ExtFile::ExtFile()
{
}*/

ExtFile::ExtFile(char* filename, int cols)
{
	numCols = cols;
	currRowContents = (double*) malloc(numCols * sizeof(double)); // create array
	file.open(filename, ifstream::in);
	//string tempStr;
	//getline(file, tempStr);
	if(file.good())
	{
		//int j=0;
		for (int j=0; j<numCols; j++)
		//while (file.good())
		{
			//char* temp;
			file >> currRowContents[j];
			//temp << 
			//currRowContents[j] = atof(tempStr.c_str());
			//cout << currRowContents[j];
			//j++;
		}

		//file.clear();
		// eat end of line
		//int tmp;
		//file >> tmp;
	}
	else
		{ // set to all zeros if file doesn't exist
			for (int j=0; j<numCols; j++)
			{
				currRowContents[j]=0;
			}
	}

}


/**
 * Get the next row of the file. Returns (a pointer to) an array containing the current row of the file.
 *
 * @return Array containing the current row of the file. Will be zeros if end of file has been passed
 */
double* ExtFile::getNext()
{
	
	if(file.good())
	{

			for (int j=0; j<numCols; j++)
			{
				if(!file.eof())
				{
					file >> currRowContents[j];
				}
				else
				{
					currRowContents[j] = 0;
					currRowContents[0] = 0; // flag first trial as a 0 if any row ends up incomplete
				}
			}
	}
	else

	{   // fill with zeros
		for (int j=0; j<numCols; j++)
		{
			currRowContents[j] = 0;
		}
	}
	return currRowContents;
}

ExtFile::~ExtFile()
{
	file.close();
}
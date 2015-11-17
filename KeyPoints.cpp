// KeyPoints.cpp
// Bradley Smith

#include <string>
using std::string;
#include <sstream>
using std::istringstream;
#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
#include "KeyPoints.h"

KeyPoints::KeyPoints()
{
	size = 0;
}

// returns the keypoint source pixel x and y
bool KeyPoints::isKeyPoint(int x, int y, int j)
{
	for(int i = 0; i < size - 1; i++){
		if((get<0>(inter[i][j]) == x) && (get<1>(inter[i][j]) == y))
			return true;

	}

	return false;
}

tuple<int,int> KeyPoints::getKeySource(int x, int y, int j)
{
	tuple <int, int> result;

	for(int i = 0; i < size - 1; i++)
		if( (get<0>(inter[i][j]) == x) && (get<1>(inter[i][j]) == y))
			return old[i];

	return result;
}

tuple<int,int> KeyPoints::getKeyDest(int x, int y, int j)
{
	tuple <int, int> result;

	for(int i = 0; i < size - 1; i++)
		if( (get<0>(inter[i][j]) == x) && (get<1>(inter[i][j]) == y))
			return dest[i];

	return result;
}


// Definitely working
void KeyPoints::makeDiff(int j)
{
	diff.clear();
	// sanitize(j); // make sure there aren't duplicates for the given j
	tuple <int, int> orig;
	tuple <int, int> temp;

	for(int i = 0; i < size - 1; i++)
	{
		orig = old[i];

		get<0>(temp) = (get<0>(inter[i][j]) - get<0>(orig) );
		get<1>(temp) = (get<1>(inter[i][j]) - get<1>(orig) );
		diff.push_back(temp);
	}
}

void KeyPoints::makeDiffDest(int j)
{
	diff.clear();
	// sanitize(j); // make sure there aren't duplicates for the given j
	tuple <int, int> orig;
	tuple <int, int> temp;

	for(int i = 0; i < size - 1; i++)
	{
		orig = dest[i];

		get<0>(temp) = (get<0>(inter[i][j]) - get<0>(orig) );
		get<1>(temp) = (get<1>(inter[i][j]) - get<1>(orig) );
		diff.push_back(temp);
	}
}

void KeyPoints::sanitize(int j)
{
	int temp_x;
	int temp_y;
	int ii;
	for(int i = 0; i < size - 1; i++)
	{
		temp_x = get<0>(inter[i][j]);
		temp_y = get<1>(inter[i][j]);

		for(ii = i + 1; ii < size - 1; ii++)
			if(temp_x == get<0>(inter[ii][j]) && temp_y == get<1>(inter[ii][j]))
			{
				exit(-1);
			}
	}
}

tuple<int,int> KeyPoints::getWeight(int x, int y, int j)
{
	tuple <int, int> tup;
	// vector<double> weights;
	double weights[size - 1];
	double x_weights = 0.0;
	double y_weights = 0.0;
	double bottom    = 0.0;
	double temp      = 0.0;
	int key_x;
	int key_y;

	// Sum k of W(x,y),k
	
	for(int i = 0; i < size - 1; i++)
	{
		key_x  = x - get<0>(inter[i][j]);
		key_y  = y - get<1>(inter[i][j]);
		// W(x,y),k
		weights[i] = 1.0 / ( (key_x * key_x) + (key_y * key_y) );
	}

	for(int i = 0; i < size - 1; i++)
	{
		temp = weights[i];
		x_weights += temp * get<0>(diff[i]);
		y_weights += temp * get<1>(diff[i]);
		bottom    += temp;
	}

	x_weights = (x_weights / bottom);
	y_weights = (y_weights / bottom);

	if(x_weights >= 0)	x_weights += .5;
	else				x_weights -= .5;

	if(y_weights >= 0)	y_weights += .5;
	else				y_weights -= .5;

	get<0>(tup) = x - (int)x_weights;
	get<1>(tup) = y - (int)y_weights;
	return tup;
}

bool KeyPoints::read(ifstream &file)
{
	string liney;
	istringstream lineStream;
	istringstream firstLineCheck;
	tuple  <int, int> tempTup;
	vector <tuple<int, int> > tupVect;
	int tempInt;
	int count = 1;
	int row   = 0;

	getline( file, liney );
	firstLineCheck.clear();
	firstLineCheck.str( liney );

	string curr;
	vector<string> tokens;

	while(firstLineCheck >> curr)
		tokens.push_back(curr);

	num_elem = tokens.size();
	if(num_elem < 6) return false; // minumum 6 tokens for a valid key point

	do
	{
		// read in a whole line into the 'liney' string
		if(row != 0)
			getline( file, liney );
		lineStream.clear();
		lineStream.str( liney );

		while(lineStream >> tempInt)
		{
			if(tempInt != -1)
			{
				// read source pixel && push
				if(count == 1)
				{
					get<0>(tempTup) = tempInt;
					lineStream >> tempInt;

					get<1>(tempTup) = tempInt;
					old.push_back(tempTup);

					count += 2;
				}

				// read all intermediate pixels && push
				else if(count == 3)
				{
					while(count < (num_elem - 1) ){
						if(count != 3)
							lineStream >> tempInt;
						get<0>(tempTup) = tempInt;
						lineStream >> tempInt;

						get<1>(tempTup) = tempInt;
						count += 2;
						tupVect.push_back(tempTup);
					}
					inter.push_back(tupVect);
					tupVect.clear();
				}

				// read the destination pixel && push
				else if(count == (num_elem - 1))
				{
					get<0>(tempTup) = tempInt;

					lineStream >> tempInt;
					get<1>(tempTup) = tempInt;

					dest.push_back(tempTup);
					// go to next row and read count
					count = 1; // reset count
					// size++;    // inc size
					row++;	   // inc row
					
					// if the line has more than the first line, peace out
					tempInt = -1;
					lineStream >> tempInt;
					if(tempInt != -1)
						return false;
				}
				else
					return false; // This shouldn't really happen
				
			} else {
				// return false;
			}

			tempInt = -1;
		}
	}while(!file.eof());

	file.close();

	if(count != 1) return false;

	size = row + 1;
	return true;
}

// Bradley Smith
// Image.cpp
// PA4 - CS253
#include "Image.h"

// returns false if read fails
// read checks to see if a files header includes P5 || P2, and calls the appropriate header function
bool Image::read( ifstream& file )
{
	unsigned char magictest = 1;
	file >> magictest;

	if(magictest == 1) {
		// returns to the beginning of the file
		file.seekg(0);

		if(!read_ascii( file )){return false;}
		else                   {return true ;}
	}
	else {
			if( magictest != 'P')
				return false;

			magictest = 1;
			file >> magictest;

			if( magictest == 1 ) {
				file.seekg( 0 );

				if(!read_ascii( file )){return false;}
				else                   {return true ;}
			}
			else {
				
				if( magictest == '5') { 
					file.seekg( 0 );

					if (!read_binary( file )) {return false;}
					else                      {return true ;}
				} 
				else if ( magictest == '2' )
				{
					file.seekg( 0 );
					if( !read_ascii( file )) {return false;}
					else                     {return true ;}
				} else {
					return false;
				}
			}
	}

	return true;
}

bool Image::read_ascii (ifstream & file)
{
	char magic;
	char magic2;
	int max     = 0;
	int big     = 0;
	int small   = 255;
	int numRead = 0;
	unsigned int total = 0;
	int entries; // height * width
	int tempInt = 0;
	bool isComment = false;
	string nullString;
	string tempString;
	string line;
	istringstream lineStream;
	vector<int> pix;

	if( file.fail( ) )
	{
		file.clear( );
		return false;
	}

	if( file.is_open() )
	{
		// read first 2 chars
		file.get( magic  );
		file.get( magic2 ); // read in P, 2

		// if file does not start with P2, 
		if( magic != 'P' || magic2 != '2')
		{
			cerr << "Error: Not a PGM file.\n";
			return false;
		}
		else {
			width  = read_header (file);
			height = read_header (file);

			reserve(width * height);
			pix.reserve(width * height);

			max    = read_header (file);
			if (width  == -1)
				return false;
			if (height == -1)
				return false;
			if (max    == -1)
				return false;

			entries = getWidth() * getHeight();
			getline( file,line );

			tempInt    = -1;
			while( !file.eof() )
			{
				getline( file, line );
				lineStream.clear();
				lineStream.str( line );

				lineStream >> magic;
				lineStream.unget();

				if( magic == '#')
				{
					getline( file, line );
					isComment = true;
				}

				if( !isComment )
				{
					while(lineStream >> tempInt)
					{
						if( tempInt > max || tempInt < 0)
						{
							return false;
						}

						pix.insert (pix.end (), tempInt);
						total  += tempInt;
						numRead++;

						if( tempInt > big )
							big = tempInt;

						if( tempInt < small )
							small = tempInt;
					}
				}

				isComment = false;
				tempInt   = -1;
			}

			if( numRead != entries )
			{
				cerr << "Number of pixels does not match width x height.\n" << endl;
				return false;
			}

			setMax( big );
			setMin( small );
			setAvg( (double) ((double) total / (double) entries) );

		}

		setPix( pix );
		file.close();
	}

	return true;
}

bool Image::read_binary (ifstream & file)
{
	unsigned char temp;
	char trash;
	unsigned int  head[4];

	// trash P and 5
	file >> trash >> trash;

	// read width
	for( int i = 0; i < 4; i++ )
	{
		temp    = file.get( );
		head[i] = temp;
	}
	unsigned int uwidth = head_int( head );
	width = uwidth;
	//cout << "w :" << width << endl;

	// read height
	for( int i = 0; i < 4; i++ )
	{
		head[i] = 0;
		temp    = file.get( );
		head[i] = temp;
	}
	// actually height not width anymore
	uwidth  = head_int( head );
	height = uwidth;
	//cout << "Height: " << height << endl;


	// read max
	temp = file.get();
	// if the binary file uses a 4 byte max
	if( temp == 0)
	{
		temp = file.get();
		temp = file.get();
		if(temp > 1) {return false;}
		temp = file.get();
	}
	unsigned int tempint = temp;
	max  = tempint;
	//cout << "Max: " << max << endl;

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// ---------- Done with header ---------------------
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	reserve(width * height);

	int total  = 0;
	int big    = 0;
	int small  = 999;
	int entries= width * height;
	int numRead= 0;
	vector<int>pix;
	pix.reserve(width * height);
	while(numRead < entries)
	{
		temp = file.get( );
		tempint = temp;

		if( (int) tempint < 0 || (int) tempint > max)
		{
			cerr << "Invalid pixel." << endl;
			return false;
		}

		//cout << "pix: " << tempint << endl;
		pix.insert (pix.end (), tempint);

		total  += tempint;
		numRead++;
		if( (int) tempint > big )
			big = temp;
		if( (int) tempint < small )
			small = temp;
	}
	max = big;
	min = small;

	if( numRead != entries)
	{
		cerr << "Invalid number of pixels." << endl;
		return false;
	}

	avg = total / entries;
	pixels = pix;


	return true;
}

int Image::read_header (ifstream& file)
{
	while( !file.eof( ) )
	{
		char tag;
		int  tempInt;
		file >> tempInt;

		if( file.fail( ) )
		{
			file.clear( );
			file >> tag;

			if( tag == '#')
			{
				string ditch;
				getline( file, ditch );
			} else {
				cerr << "Error: Invalid format.\n" << endl;
				return -1;
			}
		} else {
			if( tempInt >= 0){
				return tempInt;
			} else {
				return -1;
			}
		}
	}

	return false;	
}

void Image::write_ascii ( ofstream & outfile )
{
	unsigned int count = 0;
	outfile << "P2" << endl << width << " " << height << endl << max << endl;
	
	for( int i = 0; i < height; i++ )
	{
		for( int j = 0; j < width; j++)
		{
			outfile << pixels[count] << " ";
			count++;
		}
		outfile << endl;
	}

	outfile.close();
}

void Image::write_binary(ofstream & outfile)
{
	//output P5
	outfile << 'P';
	outfile << '5';


	unsigned int head[4];
	// head = width
	int_head( width, head );
	// output the width
	for( int i = 0; i < 4; i++ )
		outfile << (unsigned char) head[i];
	int_head( height, head );
	for( int i = 0; i < 4; i++ )
		outfile << (unsigned char) head[i];
	int_head( max, head );
	for( int i = 0; i < 4; i++ )
		outfile << (unsigned char) head[i];

	////////////////////////////
	// Done with header
	////////////////////////////

	for( int i = 0; i < height*width; i++ )
			outfile << (unsigned char) pixels[i];

}

// Scales all the pixels in the image to cover the full scale of .pgm pixels
bool Image::scale ( )
{
	float newPix;

	if( max == min )
		return false;

	for( unsigned int i = 0; i < pixels.size( ); i++ )
	{
		// scale pixel
		newPix = (float)(255 * (pixels[i] - min)) / (float)(max - min);

		newPix += .5;
		// set pixel
		pixels[i] = ( int ) newPix;
	}

	setMax( 255 );
	setMin( 0   );

	return true;
}

unsigned int Image::head_int (unsigned int head[])
{
	int output = 0;

	for( int i = 0; i < 4; i++ )
	{
		int temp = (head[i] << (8 * (3 - i ) ) );
		output |= temp;
	}

	return output;
}

void Image::int_head(int num, unsigned int head[])
{
	for( int i = 0; i < 4; i++ )
	{
		head[i] = 0;
		head[i] = num >> (8 * (3 - i) );
	}
}

int Image::getPixel(tuple<int,int> t)
{
	int c = get<0>(t);
	int r = get<1>(t);
	return getPixel(r, c);
}
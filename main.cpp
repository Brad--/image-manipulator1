// Brad Smith
// PA4 - CS253
#include <string.h>
#include "ImageSmith.h"
#include "Image.h"
#include "KeyPoints.h"

int file_count = 1;

void outfile_name(char* out)
{
	string streamName;

	std::stringstream sStream;
	sStream << file_count << ".pgm";
	file_count++;
	streamName = sStream.str();
	// store the string into a char array for use in an ofstream
	strcpy (out, streamName.c_str());
}

int main(int argc, char* argv[])
{
	if(argc < 4) return -1;

	string ext = argv[1];
		   ext = ext.substr(ext.length() - 4, ext.length());

    // open input file / keypoint file
	char       out_name[255];
	ifstream   stream    (argv[1]); // source
	ifstream   stream2   (argv[2]); // destination
	ifstream   keystream (argv[3]); // keypoints

	// process input file
	Image image1;
	if(  stream.fail ())       return -1;
	if( !image1.read (stream)) return -1;

	Image image2;
	if( stream2.fail ())        return -1;
	if( !image2.read (stream2)) return -1;

	KeyPoints keys = KeyPoints();
	if( !keys.read(keystream)) return -1;

	ImageSmith iSmith;

	// calculate number of intermediate images and interpolate them
	int elem = keys.getElem();
	iSmith.setN((elem / 2) - 2);
	
	for(int i = 1; i <= ((elem/2) - 2); i++)
	{
		// temp = iSmith.getImage(i);
		outfile_name(out_name);
		ofstream o (out_name);
		// temp.write_ascii(o);
		if( !iSmith.morph(image1, image2, keys, false, o, i - 1) ) return -1;
	}

	// if( !iSmith.interpolate(image1, image2, (elem / 2) - 2, false)) return -1;

	// currently, the image will always go to the middle interpolation
	// fix that shit yo

	

	// close streams
	stream.   close();
	keystream.close();
	return 0;
}

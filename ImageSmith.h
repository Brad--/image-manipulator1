// ImageSmith.h
// Brad Smith
#ifndef IMAGESMITH_H_INCLUDE
#define IMAGESMITH_H_INCLUDE

#include "Image.h"
#include "KeyPoints.h"

class ImageSmith {
public:
	// inline int   getNumImages ()      {return images.size();}
	// inline Image getImage     (int i) {return images[i];}
	inline void  setN         (int n) {N = n;}
	bool interpolate(Image & image1, Image & image2, int numImages, bool is_b, ofstream &o);
	bool morph      (Image & image1, Image & image2, KeyPoints k, bool is_b, ofstream & o, int j);
  
private:
	// vector <Image> images;
	int N;
};

#endif
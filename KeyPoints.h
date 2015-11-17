// KeyPoints.h
// Bradley Smith
#ifndef KEYPOINTS_H_INCLUDE
#define KEYPOINTS_H_INCLUDE

#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include <tuple>
using std::tuple;
using std::get;

class KeyPoints
{
	public:
	inline int getX    (int i) const {return get<0>(old [i]);}
	inline int getY    (int i) const {return get<1>(old [i]);}
	inline int getDiffX(int i) const {return get<0>(diff[i]);}
	inline int getDiffY(int i) const {return get<1>(diff[i]);}
	inline int getNewX (int i) const {return new_x[i] ;}
	inline int getNewY (int i) const {return new_y[i] ;}
	inline int getSize ()      const {return size     ;}
	inline int getElem ()      const {return num_elem ;}

	     KeyPoints   ();
	void makeDiff    (int j);
	void makeDiffDest(int j);
	void sanitize    (int j);
	bool read        (ifstream &file);
	bool isKeyPoint  (int x, int y, int j );
	tuple<int,int> getWeight   (int x, int y, int j);
	tuple<int,int> getKeySource(int x, int y, int j);
	tuple<int,int> getKeyDest  (int x, int y, int j);
	

	private:
	vector <tuple<int,int> > old ;
	vector <tuple<int,int> > dest;
	vector <tuple<int,int> > diff;
	vector <vector <tuple <int, int> > > inter;
	vector <int> new_x;
	vector <int> new_y;
	int    size;
	int    num_elem;
};
#endif
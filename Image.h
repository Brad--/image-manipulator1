// Bradley Smith
// Image.h
// PA4  -  CS253
#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <tuple>
using std::tuple;
using std::get;
#include <sstream>
using std::istringstream;

class Image {
public:
  Image(int u_min = 0, int u_max = 0, int u_height = 0, int u_width = 0, double u_avg = 0.0)
    :   min(u_min),    max(u_max), height(u_height),  width(u_width),       avg(u_avg) {}

  Image(int u_max, int u_height, int u_width)
  {
    max    = u_max;
    height = u_height;
    width  = u_width;
    min    = 0;
    max    = 0;
    avg    = 0;
  }

  /* simple accessors */
  inline int getMin         ()const {return min;}
  inline int getMax         ()const {return max;}
  inline int getHeight      ()const {return height;}
  inline int getWidth       ()const {return width;}
  inline double getAvg      ()const {return avg;}
  inline vector<int> getPix ()const {return pixels;}
  inline int getPixel       (int r, int c) const {return pixels[(r * width) + c];}

  /* Mutators */
  inline void setMin   (int u_min)         {min    = u_min;}
  inline void setMax   (int u_max)         {max    = u_max;}
  inline void setHeight(int u_height)      {height = u_height;}
  inline void setWidth (int u_width)       {width  = u_width;}
  inline void setAvg   (double u_avg)      {avg    = u_avg;}
  inline void setPix   (vector<int> u_pix) {pixels = u_pix;}

  inline void reserve  (int i)             {pixels.reserve(i);}

  unsigned int head_int(unsigned int head[]);
  int  getPixel        (tuple<int, int> t);
  bool read            (ifstream& stream);
  bool read_ascii      (ifstream& stream);
  bool read_binary     (ifstream& stream);
  int  read_header     (ifstream& stream);
  void write_ascii     (ofstream& outfile);
  void write_binary    (ofstream& outfile);
  void int_head        (int num, unsigned int head[]);
  bool scale           ( );
  

private:

  int          min, max, height, width;
  double       avg;
  vector <int> pixels;

  
};


#endif

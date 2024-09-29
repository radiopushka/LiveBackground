#ifndef X11IMAGE
  #define X11IMAGE
  #include <X11/Xlib.h>
  typedef unsigned char Raster;
//Evan Nikitin 2024 sep 28th
//the larger these values the worse the image but the less the cpu usage
#define MAXERR_R 1
#define MAXERR_G 1
#define MAXERR_B 1

  //our raster declares image type in B G R per each pixel
  //one pixel has 3 chars B G and R
  //the Y is 

  int rescaled_read(char* image, Raster* memory,int width,int height);//read a png image from file
  int getWindowHeight();
  int getWindowWidth();
  unsigned long long getScreenArea();
  Raster* getRaster();

  void putImage(Raster* image);
  void putCorrection(Raster* pixels,unsigned char** pixel_location,unsigned int count);
  //create a correction array for an image based on current data
  unsigned int calculateBsize(unsigned char* input);
  void populateBsize(unsigned char* input,unsigned char** location_array,unsigned char* pixel_array);

   

  void initX11(char* display);
  void XputImage();//refreshes display
  void flushX();//flush the data
  void closeX11(); 
                  //
#endif // !X11IMAGE

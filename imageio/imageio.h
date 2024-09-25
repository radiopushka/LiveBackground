#ifndef X11IMAGE
  #define X11IMAGE
  #include <X11/Xlib.h>
  typedef unsigned char Raster;
  //our raster declares image type in B G R per each pixel
  //one pixel has 3 chars B G and R
  //the Y is 

  void setRGB(int R,int G ,int B);//sets the background RGB
  void drawImage(Raster* im);//sets the background Image
  int rescaled_read(char* image, Raster* memory,int width,int height);//read a png image from file
  unsigned long long getRasterLocation(int x,int y);//returns the location on the raster of that pixel
  int getWindowHeight();
  int getWindowWidth();
  unsigned long long getScreenArea();
  void stopXEvent();
  Raster* getRaster();
  void generateImage(Raster* data);
  XImage* createXImage(Raster** memory);

  void XeventListener(void (*func)(XEvent));//calls function when the mouse moves
   

  void initX11(char* display);
  void setImage();//refreshes display
  void setImaged(XImage* xim);
  void flushX();//flush the data
  void closeX11(); 
                  //
#endif // !X11IMAGE

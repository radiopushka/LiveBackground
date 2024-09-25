#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include "imageio.h"

int Bw=1000; 
int Bh=1000;

XImage* pix;
unsigned char* image;
Display* dpy;
Window root;
GC gc;

int STOP=0;
unsigned long long SCREEN_AREA_RGB;

//unsigned int iid = 0;

XImage *CreatePixmapImage(Display *display, Visual *visual,  unsigned char* data, int width, int height) {
	//static XShmSegmentInfo shminfo;
	XImage *imagex = XCreateImage(display, visual,DefaultDepth(dpy, 0), ZPixmap,0,(char*) data, width, height,32,0); 
	//iid++;
	//shminfo.shmid = shmget(IPC_PRIVATE, imagex->bytes_per_line * imagex->height,IPC_CREAT|0777);
	//data=shmat(shminfo.shmid,0,0);
	//shminfo.shmaddr = imagex->data = *data;
	//shminfo.readOnly = False;

	//XShmAttach(display, &shminfo);
	return imagex;
}


void generateImage(Raster* imagedata){
  unsigned char* cp;
  Raster* end=image+(Bw*Bh)*4;
  for(cp=image;cp<end;){
      *(cp++)=*(imagedata++); //B
      *(cp++)=*(imagedata++); //G 
      *(cp++)=*(imagedata++);  //R
      cp++;

  }
}
void setRGB(int R, int G, int B){
  Raster* cp;
  Raster* end=image+(Bw*Bh)*4;
  for(cp=image;cp<end;){
    *(cp++)=B;
    *(cp++)=G;
    *(cp++)=R;
    cp++;
  }
  
}
void drawImage(Raster* imagein){
  Raster* targ;
  Raster* im=imagein;
  Raster* end=image+(Bw*Bh)*4;
  for(targ=image;targ<end;){
    *(targ++)=*(im++);
  }
}

unsigned long long getRasterLocation(int x,int y){
  return (y*Bw)*4+x*4;
}

void setImage(){
  XPutImage(dpy, root,  gc, pix, 0, 0, 0, 0, Bw, Bh);
}

void setImaged(XImage* ximg){

  
 	XPutImage(dpy, root, gc, ximg, 0, 0, 0, 0, Bw, Bh);

//	printf("RES: %d\n",res);
}

void flushX(){
  XFlush(dpy);
}
XImage* createXImage(unsigned char** memory){
	return CreatePixmapImage(dpy,DefaultVisual(dpy, XDefaultScreen(dpy)),*memory,Bw,Bh);
}
void initX11(char* display){
  dpy=XOpenDisplay(display);
  unsigned int scr=DefaultScreen(dpy);
  root=RootWindow(dpy,scr);
  XWindowAttributes watr;
  XGetWindowAttributes(dpy,root,&watr);
  Bw=watr.width;
  Bh=watr.height;
  XFlush(dpy);
  gc = XCreateGC(dpy, root, 0, NULL);
  
  SCREEN_AREA_RGB=(Bw*Bh)*3;
//  pix=CreatePixmapImage(dpy,DefaultVisual(dpy, 0),(void**)&image,Bw,Bh);


}
void closeX11(){
  XCloseDisplay(dpy);
}
int getWindowWidth(){
  return Bw;
}
int getWindowHeight(){
  return Bh;
}
void XeventListener(void (*func)(XEvent)){
  XEvent ev;
  XSelectInput(dpy,root,PointerMotionMask);
  STOP=0;
  int i;
  int ec;
  while(STOP!=1){
    ec=XEventsQueued(dpy,PointerMotionMask);
    for(i=0;i<ec;i++){
      XNextEvent(dpy,&ev);
    }
    XNextEvent(dpy,&ev);
    func(ev);
  }
}

Raster* getRaster(){
  return image;
}
unsigned long long getScreenArea(){
  return SCREEN_AREA_RGB;
}
void stopXEvent(){
  STOP=1;
}

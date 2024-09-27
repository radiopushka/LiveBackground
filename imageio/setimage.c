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

XImage* holder;
unsigned char* image_canvas;
unsigned char* canvas_end;
Display* dpy;
Window root;
GC gc;
int screen;

XShmSegmentInfo shminfo;

unsigned long long SCREEN_AREA_RGB;

//unsigned int iid = 0;

void flushX(){
  XFlush(dpy);
}

void XputImage(){

 XShmPutImage(dpy, root, gc,
				             holder, 0, 0, 0, 0, Bw, Bh,
					     True); 
}

unsigned char* getPixelAddr(unsigned int location){
  return image_canvas+location*4;
}

void putImage(unsigned char* image){
  unsigned char* iter = image_canvas;
  while(iter<canvas_end){
    *iter=*image;
    iter++;
    image++;
    *iter=*image;
    iter++;
    image++;
    *iter=*image;
    iter++;
    image++;
    *iter = 255;//aplha always max
    iter++;
  }
}

void putCorrection(unsigned char* pixels,unsigned char** pixel_loc, unsigned int count){
  unsigned int track;
  unsigned int data;
  for(track=0; track<count; track++){
    data=4278190080;
    data|=*(pixels);
    pixels++;
    data|=(*pixels)<<8;
    pixels++;
    data|=(*pixels)<<16;
    pixels++;
    //16 - red
    //8 - green
    //0 - blue
    *((unsigned int*)(*pixel_loc))=data;
    pixel_loc++;
  }
}
int pixelcomp(unsigned int pixel1,unsigned  int pixel2){
  int R1=pixel1>>16;
  int G1=(pixel1>>8)&255;
  int B1=pixel1&255;

  int R2=pixel2>>16;
  int G2=(pixel2>>8)&255;
  int B2=pixel2&255;

  if(R1>R2+MAXERR_R || R1<R2-MAXERR_R){
    return -1;
  }
  if(B1>B2+MAXERR_B || B1<B2-MAXERR_B){
    return -1;
  }
  if(G1>G2+MAXERR_G || G1<G2-MAXERR_G){
    return -1;
  }
  return 1;
}
unsigned int calculateBsize(unsigned char* input){

  //count number of differing pixels
  unsigned int count=0;
  unsigned char* iter = image_canvas;
  unsigned char* iter2 =input;
  unsigned int pixel1,pixel2;
  while(iter<canvas_end){
    pixel1 = (*iter2)<<16;
    pixel2 = (*iter)<<16;
    iter2++;
    iter++;
    pixel1|= (*iter2)<<8;
    pixel2|= (*iter)<<8;
    iter2++;
    iter++;
    pixel1|= *iter2;
    pixel2|= *iter;
    iter2++;
    iter++;
    iter++;

    if(pixelcomp(pixel1,pixel2)==-1)
        count++;
    
  }
  return count;
 }
void populateBsize(unsigned char* input,unsigned char** location_array,unsigned char* pixel_array){

  //count number of differing pixels
  unsigned char* iter = image_canvas;
  unsigned char* iter2 =input;
  unsigned int pixel1,pixel2;
  unsigned char* locptr;
  while(iter<canvas_end){
    locptr=iter;
    pixel1 = (*iter2)<<16;
    pixel2 = (*iter)<<16;
    *iter=*iter2;
    iter2++;
    iter++;
    pixel1|= (*iter2)<<8;
    pixel2|= (*iter)<<8;
    *iter=*iter2;
    iter2++;
    iter++;
    pixel1|= *iter2;
    pixel2|= *iter;
    *iter=*iter2;
    iter2++;
    iter++;
    iter++;

    if(pixelcomp(pixel1,pixel2)==-1){
        *location_array=locptr;
        location_array++;
        *pixel_array=pixel1>>16;
        pixel_array++;
        *pixel_array=pixel1>>8;
        pixel_array++;
        *pixel_array=pixel1;
        pixel_array++;
    }
    
  }
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

  screen = DefaultScreen(dpy);

  XShmQueryExtension(dpy);

  holder = XShmCreateImage(dpy, DefaultVisual(dpy, screen),DefaultDepth(dpy, screen), ZPixmap,
				NULL, &shminfo, Bw, Bh);

  shminfo.shmid = shmget(IPC_PRIVATE, holder->bytes_per_line * holder->height,
			       IPC_CREAT|0777);
  
  holder->data = shmat(shminfo.shmid, 0, 0);
	shminfo.shmaddr = holder->data;
	shminfo.readOnly = True;

  XShmAttach(dpy, &shminfo);

  image_canvas=(unsigned char*)holder->data;

  canvas_end = image_canvas + (Bw*Bh*4);

  SCREEN_AREA_RGB=(Bw*Bh)*3;
//  pix=CreatePixmapImage(dpy,DefaultVisual(dpy, 0),(void**)&image,Bw,Bh);


}
void closeX11(){
  XShmDetach(dpy, &shminfo);
  XDestroyImage(holder);
  XFreeGC(dpy,gc);
  XCloseDisplay(dpy);
}
int getWindowWidth(){
  return Bw;
}
int getWindowHeight(){
  return Bh;
}
Raster* getRaster(){
  return image_canvas;
}
unsigned long long getScreenArea(){
  return SCREEN_AREA_RGB;
}


#include "imageio/imageio.h"
#include <stdlib.h>
#include<stdio.h>

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

XImage** images;
Raster** data;
unsigned int image_count;

int maxwidth;
int maxheight;

int skipping = 1;

unsigned int imagecount;

void sort_images(int* order,unsigned int start){

	if(start >= imagecount)
		return;
	
	unsigned int i;
	unsigned int m_index = start;
	int min = order[start];
	for(i=start; i<imagecount; i++){
		if(order[i]<min){
			min = order[i];
			m_index = i;
			
		}

	}

	XImage* mini = images[m_index];
	XImage* candidate = images[start];

	int can_i = order[start];
	order[start]=order[m_index];
	
	order[m_index]=can_i;

	images[start] = mini;
	images[m_index] = candidate; 
	sort_images(order, start+1);
}


int dirimages(char* path){
	printf("loading frames... \n");

	DIR* rdim = opendir(path);

	struct dirent* drd;
	char* namae;
	unsigned int files=0;
	unsigned int tracker=0;
	while((drd=readdir(rdim)) != NULL){
		namae=drd->d_name;
		if(strrchr(namae,'p')!=NULL && tracker%skipping==0)
			files++;

		tracker++;
	}
	closedir(rdim);

	images = malloc(sizeof(XImage*)*files);
	data= malloc(sizeof(Raster*)*files);
	
	int names[files];
	imagecount=files;
	tracker =0;
	char* endptr;
	int namesize;
	char* i;
	unsigned int fc = 0;
	rdim = opendir(path);
	while((drd=readdir(rdim)) != NULL){
                namae=drd->d_name;
                if((strrchr(namae,'p'))!=NULL && tracker%skipping==0){
			endptr = strrchr(namae,'.');
			namesize = (endptr - namae) + 1;
			char name[namesize];
			for(i=namae; i < endptr; i++){
				name[i-namae] = *i; 
			}
			name[i-namae] = 0;
			//printf("loaded %s [PNG] \n",name);
			names[fc] = atoi(name);
			
			char pathr[strlen(path) + 1 + strlen(namae) + 1];
			sprintf(pathr,"%s/%s",path,namae);
			Raster* canvas = malloc(sizeof(Raster)*(maxwidth*maxheight*4));
			images[fc] = createXImage(&canvas);
			data[fc]=canvas;
			//canvas[0]=0;
			if(rescaled_read(pathr,canvas,maxwidth,maxheight)==-1){
                          return -1;
			}
			fc++;
		}
		tracker++;
                        
        }
        closedir(rdim);

	
	sort_images(names,0);
	printf("\n%d frames loaded succesfully\n---\n",imagecount);
	return 1;
}


int main(int argn, char* argv[]){

	if(argn<4){
		printf("a program to animate desktops on tiling window managers like dwm\n");
		printf("%s <display id (:0)> <path to directory with image files> <fps rate> [frame skipping] [bidirectional]\n",argv[0]);
		return 0;
	}
  int bidirectional=0;
	if(argn >4){
		skipping = atoi(argv[4]);
		printf("skipping every %d frames\n",skipping);
    if(argn>5){
      bidirectional=1;
    }
	}

	initX11(argv[1]);//init x11
	//fingerprint display
	maxwidth=getWindowWidth();
        maxheight=getWindowHeight();

	unsigned int fps = atoi(argv[3]);

	double period = 1.0/fps;
	double time = period;
	unsigned long sleeptime=time*1000000;

        printf("window width: %d\nwindow height: %d\n",maxwidth,maxheight);

	if(dirimages(argv[2])==-1){
		printf("aborting...\n");
		return 0;
	}
	//some kill condition
  int incrmentation =1;
	unsigned int cycle_index = 0;
	while(1){

		
		setImaged(images[cycle_index]);
		cycle_index = cycle_index+incrmentation;
    if(bidirectional==1 && cycle_index <= 0){

      incrmentation=1;
    }
    
		if(cycle_index >= imagecount){
      if(bidirectional==1){
        incrmentation=-1;
        cycle_index=cycle_index-2;
      }else
			  cycle_index = 0;
		}
		//printf("setting %d %d\n",cycle_index);
    usleep(sleeptime);
	}

        closeX11();
	
	unsigned int i;
	for(i=0; i<imagecount;i++){
		free(images[i]);
		free(data[i]);
	}
	free(images);
	free(data);
	
        return 0;
}

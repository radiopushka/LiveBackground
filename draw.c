#include "imageio/imageio.h"
#include <stdlib.h>
#include<stdio.h>

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

Raster** images;
unsigned int image_count;

unsigned int optimizationscore=0;
//Evan Nikitin 2024 sep 28th

//optimization structures
unsigned char*** address_holder;
Raster** pixelmaps;
unsigned int* count_map;

int maxwidth;
int maxheight;

int skipping = 1;

unsigned int imagecount;

void setup_optimizations(){
  optimizationscore=0;
  address_holder=malloc(sizeof(unsigned char**)*imagecount);
  pixelmaps=malloc(sizeof(Raster*)*imagecount);
  count_map=malloc(sizeof(unsigned int)*imagecount);
  putImage(images[0]);
  
  int i;
  unsigned int countv;
  Raster* pixeldata;
  unsigned char** accarr;
  for(i=1;i<imagecount;i++){
    countv=calculateBsize(images[i]);
    pixeldata=malloc(sizeof(Raster)*countv*3);
    accarr=malloc(sizeof(unsigned char*)*countv);
    populateBsize(images[i],accarr,pixeldata);

    pixelmaps[i]=pixeldata;
    count_map[i]=countv;
    address_holder[i]=accarr;
    optimizationscore=optimizationscore+countv;
//    if(countvc>8294400)
 //    printf(" - %d\n",countvc);
  }
  countv=calculateBsize(images[0]);
  pixeldata=malloc(sizeof(Raster)*countv*3);
  accarr=malloc(sizeof(unsigned char*)*countv);
  populateBsize(images[0],accarr,pixeldata);
  address_holder[0]=accarr;
  pixelmaps[0]=pixeldata;
  count_map[0]=countv;

}
void depclean(){
  int i;
  for(i=0;i<imagecount;i++){
    free(images[i]);
  }
  free(images);
}
void free_op_mem(){
   int i;
  for(i=0;i<imagecount;i++){

    free(address_holder[i]);
    free(pixelmaps[i]);
  }
  free(address_holder);
  free(pixelmaps);
  free(count_map);

}

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

	Raster* mini = images[m_index];
	Raster* candidate = images[start];

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

	images = malloc(sizeof(Raster*)*files);
	//data= malloc(sizeof(Raster*)*files);
	
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
			Raster* canvas = malloc(sizeof(Raster)*(maxwidth*maxheight*3));
			images[fc]=canvas;
			//canvas[0]=0;
			if(rescaled_read(pathr,canvas,maxwidth,maxheight)==-1){
                          free(canvas);
                          return -1;
			}
			fc++;
		}
		tracker++;
                        
        }
        closedir(rdim);

	
	sort_images(names,0);
	printf("\n%d frames loaded succesfully\n---\noptimizing\n",imagecount);
	return 1;
}


int main(int argn, char* argv[]){

	if(argn<4){
		printf("a program to animate desktops on tiling window managers like dwm\n");
		printf("%s <display id (:0)> <path to directory with image files> <fps rate> [frame skipping]\n",argv[0]);
		return 0;
	}

	if(argn >4){
		skipping = atoi(argv[4]);
		printf("skipping every %d frames\n",skipping);
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
  setup_optimizations();
  double optimization_p=optimizationscore/(maxwidth*maxheight*imagecount*1.0);
  printf("optimization: %g out of 100 \n",100-optimization_p*100);
	putImage(images[0]);
  XputImage();
  depclean();


	unsigned int cycle_index = 1;
	while(1){

    //printf("%d\n",cycle_index);
		
		//putImage(images[cycle_index]);
   putCorrection(pixelmaps[cycle_index],address_holder[cycle_index],count_map[cycle_index]);
    XputImage();
    flushX();
		cycle_index = cycle_index+1;
    if(cycle_index >= imagecount){
    			cycle_index = 0;
		}
		//printf("setting %d %d\n",cycle_index);
    usleep(sleeptime);
	}

    free_op_mem();
    closeX11();
	
	
        return 0;
}

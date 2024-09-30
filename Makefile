all:
	cc draw.c imageio/setimage.c imageio/imageio.c -march=native -Ofast -lX11 -lXext -lpng -Wall -o livebg

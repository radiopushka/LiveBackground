all:
	cc draw.c imageio/setimage.c imageio/imageio.c -march=native -O4 -lX11 -lXext -lpng -Wall -o livebg

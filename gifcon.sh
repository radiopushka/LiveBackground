#!/bin/bash
#Evan Nikitin 2024 sep 28th
DISPLAY_WIDTH=1080
DISPLAY_HEIGHT=1920

if [ -z $1 ]; then
  echo "please specify the image file"
  exit
fi

rm -r "$1.output"
mkdir "$1.output"
cp "$1" "$1.output"
cd "$1.output"

ffmpeg -i "$1" -vf scale=$DISPLAY_HEIGHT:$DISPLAY_WIDTH output.gif
rm "$1"
ffmpeg -i output.gif -pix_fmt rgba %04d.png
rm output.gif

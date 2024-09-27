#!/bin/bash
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

#ffmpeg -i "$1" -vf scale=$DISPLAY_HEIGHT:$DISPLAY_WIDTH output.gif
ffmpeg -i "$1" -pix_fmt rgba %04d.png
rm "$1"

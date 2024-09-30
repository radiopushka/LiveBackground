#!/bin/bash
#Evan Nikitin 2024 sep 28th
DISPLAY_WIDTH=1080
DISPLAY_HEIGHT=1920

if [ -z $1 ]; then
  echo "please specify the image file"
  echo "you can added o at the end to turn on optimization settings escalation"
  exit
fi

rm -r "$1.output"
mkdir "$1.output"
cp "$1" "$1.output"
cd "$1.output"
if [ -z $2 ]; then
  #ffmpeg -i "$1" -vf scale=$DISPLAY_HEIGHT:$DISPLAY_WIDTH output.gif
  ffmpeg -i "$1" -vf scale=$DISPLAY_HEIGHT:$DISPLAY_WIDTH -pix_fmt rgba %04d.png
  rm "$1"
  echo "you can added o at the end to turn on optimization settings escalation"
else
  ffmpeg -i "$1" -vf scale=$DISPLAY_HEIGHT:$DISPLAY_WIDTH image.gif
  rm "$1"
  ffmpeg -i image.gif -pix_fmt rgba %04d.png
  rm image.gif
fi

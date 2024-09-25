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

ffmpeg -i "$1" -vcodec h264 -vf scale=$DISPLAY_HEIGHT:$DISPLAY_WIDTH output.mp4
rm "$1"
ffmpeg -i output.mp4 -vcodec h264 -filter:v fps=10 output2.mp4
rm output.mp4
ffmpeg -i output2.mp4 -pix_fmt rgba %04d.png
rm output2.mp4

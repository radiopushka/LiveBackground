# LiveBackground
Live Background animator for DWM Linux

installation

for mp4 conversion, requires ffmpeg with h264 library installed
on Gentoo:
add openh264 to the use flags and re emerge ffmpeg

then you can run make in this directory

then you can call the video conversion shell script on a video file and it will generate directory with .output at the end

pass the path to that directory as the second argument of livebg

run livebg for options

usage on a worset case scenario: on I7 8th generation, both X11 and this program will consume 1.9-2.5% of one CPU core while running at 15fps, lower frame rates will result in less cpu usage
"The Drive" rendered at 24 fps on the latest i5 core takes about 1.3-2% of CPU. This is a pretty animation heavy render.

mp4 conversion with the shell script can give you a sharp and clear background.

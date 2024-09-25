# LiveBackground
Live Background animator for DWM Linux

installation

requires ffmpeg with h264 library installed
on Gentoo:
add openh264 to the use flags and re emerge ffmpeg

then you can run make in this directory

then you can call the video conversion shell script on a video file and it will generate directory with .output at the end

pass the path to that directory as the second argument of livebg

run livebg for options

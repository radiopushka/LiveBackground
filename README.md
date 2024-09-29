
# Live Backgrounds DWM

### Live Background Animator for DWM Linux

`Live Backgrounds DWM` is a lightweight program designed to animate live backgrounds for the DWM window manager on Linux systems. The program uses video or GIF conversions to run as animated backgrounds with low CPU usage and optimized performance.

## Features
- **FPS Control**: Fine-tune the frame rate to reduce CPU consumption.
- **Frame Amount Control**: Manage the number of frames to display.
- **Low CPU Usage**: Optimized to run smoothly even on older CPUs.
- **Efficient Conversion**: Convert videos or GIFs into live background formats with clear, sharp results.

## Installation Instructions

### Requirements:
- **FFmpeg** with the `openh264` library installed.
  
  On Gentoo Linux, add `openh264` to the use flags and re-emerge ffmpeg:

  ```bash
  sudo emerge --newuse ffmpeg
  ```

### Installation Steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/radiopushka/LiveBackground
    cd LiveBackground
    ```

2. Build the program:

    ```bash
    make
    ```

3. Run the video conversion script to convert an MP4 or GIF file:

    ```bash
    ./videconv.sh <video_file>
    ```

   The script will generate a directory with the `.output` suffix, containing the converted frames.

4. You can now copy the `livebg` binary to any location of your choice:

    ```bash
    cp livebg /usr/local/bin/
    ```

5. Optionally, copy the conversion scripts to easily convert videos in the future:

    ```bash
    cp gifcon.sh videconv.sh /usr/local/bin/
    ```

## Usage

After converting an MP4 or GIF to a live background directory, you can run the program using the following syntax:

```bash
livebg :0 ./directory.output 20 0
```

- `:0` is the X11 display ID.
- `./directory.output` is the output folder generated by the conversion script.
- `20` is the frame rate (FPS).
- `0` is the frame skip modulus.

### Performance:
On an Intel i7 8th generation, both X11 and the program will consume approximately 1.9-2.5% of one CPU core at 20 FPS. For higher frame rates (e.g., 24 FPS on an i5), CPU usage could increase to 4-5%, with animation-heavy renders like "The Drive."

### Note:
The MP4 conversion via the shell script provides a sharp and clear background.

## Contributing
If you'd like to contribute to this project, feel free to contact me at `87.7fmradiopushka@gmail.com`.

## License
This project is licensed under the **Public Domain**.

---

Readme formated by ChatGPT

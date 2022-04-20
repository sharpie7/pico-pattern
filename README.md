# pico-pattern

Pico-pattern is a video pattern generator based on the Raspberry Pi Pico for testing 8-bit era CRT monitors such as the Microvitec Cub. Pico-pattern uses the video generator from [pico-mposite](https://github.com/breakintoprogram/pico-mposite).

The main target of pico-pattern is to generate video with TTL-compatible R,G,B and Composite Sync at 50Hz as used by the BBC Micro.

Pico-pattern offers a range of typical video test patterns including:

- Colour Bars
- Pure White
- Line Grid
- Dot Grid
- Centre Cross Hairs

## Software Build and Deployment

If you want to use the pre-built output then you can just download the file `build/pico-pattern.uf2` and skip to the last paragraph of this section. 

It is recommended to build on a Linux environment. I used Ubuntu under WSL2.

To build yourself follow these instructions:

Install the [Pi Pico C SDK](https://github.com/raspberrypi/pico-sdk) and all the necessary build tools.

Clone this repository to a sibling folder of the SDK. From a commend line cd into the pico-pattern directory and use the following commands (with appropriate change for your system):

```
cd build
cmake -DPICO_SDK_PATH=/path/to/pico-sdk ..
make
```

This should generate a file `build/pico-pattern.uf2`. 


Connect a Pi Pico to the computer while holding down the BOOTSEL button. Copy the `build/pico-pattern.uf2` file to the Pi Pico virtual drive.

## Hardware

Video 

Power

## Use

Program the Pi Pico as described above. Construct the hardware and connect to a suitable monitor. Power On!

The external button will advance through the different patterns. The LED on the Pi Pico will flash the number of the current pattern.

- 1 = Colour Bars
- 2 = White Screen
- 3 = Grid
- 4 = Dots
- 5 = Centre Cross Hairs
- 6 = Border and circle

## Notes

The Pi Pico is a 3.3V part, but this should be ample headroom over the 5V TTL threshold for a "high" signal. Therefore I have not included level shifters in this design.

Professional video pattern generators often include the pattern over the full area of the scan (including areas that are normally off-screen). For most patterns this version only includes the pattern within the border of the normal picture. For my specific application this is preferable, but it wouldn't be a big job to modify the behaviour.

The video output from this device is not interlaced.

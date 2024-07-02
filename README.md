# sdl2_cube
Infinite Dodge Game programmed in C++ with SDL2

# How it Works
When you start the game, click start to start when youre in, just dodge the red "enemy cubes" with WASD and get the highest score possible!

# How to run
Windows:
Download ZIP folder from latest release, extract and the .exe file, but make sure the DLLs are in the same folder as the executable!

Linux:
1. Install SDL2 and SDL2_ttf (Debian-based: `sudo apt install libsdl2-dev libsdl2_ttf-dev`
Arch-based: `sudo pacman -S sdl2 sdl2_ttf`)
2. Download sdl2_cube from latest release
3. open Terminal
4. type: `cd Downloads`
6. type: `chmod +x sdl2_cube`
7. now you can type `./sdl2_cube` or just double click the file

# How to Compile (linux)
1. Download the src1.5 folder
2. Go into the source folder
3. Make sure you have SDL2, g++ and SDL2_ttf installed!! (If not you can install it on Debian-based systems with: `sudo apt install libsdl2-dev libsdl2_ttf-dev g++`
4. Arch-based: `sudo pacman -S sdl2 sdl2_ttf gcc`)
5. open terminal in src
6. type: `g++ main.cpp font.h -o sdl2_cube -lSDL2 -lSDL2_ttf`


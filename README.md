# sdl2_cube
Infinite Dodge Game programmed in C++ with SDL2

# How it Works
As soon as you are starting the executable you are thrown into the game. Just dodge the red "enemy cubes" with the arrow-keys and get the highest score possible!

# How to run
Windows:
Download ZIP folder from latest release, extract and the .exe file, but make sure the DLLs are in the same folder as the executable!

Linux:
1. Install SDL2 and SDL2_ttf (Debian-based: sudo apt install libsdl2-dev libsdl2_ttf-dev)
2. Download sdl2_cube from latest release
3. open Terminal
4. type: cd Downloads
5. type: chmod +x sdl2_cube
6. now you can type ./sdl2_cube or just double click the file

# How to Compile (linux)
1. Download the src folder
2. Go into the source folder
3. Make sure you have SDL2 and SDL2_ttf installed!!
4. open terminal in src
5. type: g++ main.cpp font.h -o sdl2_cube -lSDL2 -lSDL2_ttf


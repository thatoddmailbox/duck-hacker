# Building on Linux
You will need the following libraries built:
* SDL2
* SDL2_mixer
* PhysicsFS
* libpng
* Lua
* wxWidgets (core, base, stc)

## Actually building it
```bash
# path is for wx-config for wxWidgets
export PATH=$PATH:/home/alex/Documents/duck-stuff/root/bin
SDL2DIR=/home/alex/Documents/duck-stuff/root PHYSFSDIR=/home/alex/Documents/duck-stuff/root LIBPNGDIR=/home/alex/Documents/duck-stuff/root cmake ..
```
You can then run `make` to build the `duck-hacker` executable.

# Building on Linux
You will need a working C++ compiler and `cmake` installed.

You will also need the following libraries built:
* SDL2
* SDL2_mixer
* PhysicsFS
* libpng
* Lua
* wxWidgets (core, base, stc)

For the libraries, it is recommended that you build them from source and install them into a local prefix.

## Actually building it
We assume you installed into a local prefix. The sample below uses the prefix og `/home/alex/Documents/duck-stuff/root`

```bash
# path is for wx-config for wxWidgets
export PATH=$PATH:/home/alex/Documents/duck-stuff/root/bin
SDL2DIR=/home/alex/Documents/duck-stuff/root PHYSFSDIR=/home/alex/Documents/duck-stuff/root LIBPNGDIR=/home/alex/Documents/duck-stuff/root cmake ..
```
You can then run `make` to build the `duck-hacker` executable.

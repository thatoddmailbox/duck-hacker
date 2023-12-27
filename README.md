# duck-hacker
This is a game where you hack various things by programming DuckBots, small robots that run Lua code.

## Building
Make sure to run `git submodule update --init --recursive` to clone the submodules, as some dependencies are included that way.

### On Windows
See the [Windows build instructions](./docs/build-windows.md).

### On Linux
```bash
# path is for wx-config for wxWidgets
export PATH=$PATH:/home/alex/Documents/duck-stuff/root/bin
SDL2DIR=/home/alex/Documents/duck-stuff/root PHYSFSDIR=/home/alex/Documents/duck-stuff/root LIBPNGDIR=/home/alex/Documents/duck-stuff/root cmake ..
```
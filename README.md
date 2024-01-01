# duck-hacker
This is a game where you hack various things by programming DuckBots, small robots that run Lua code.

The game is written in C++ and uses OpenGL for rendering. The code is based on [tigame](https://github.com/thatoddmailbox/tigame)'s rendering engine, but with many internal changes.

## Building
Make sure to run `git submodule update --init --recursive` to clone the submodules, as some dependencies are included that way.

### On Windows
See the [Windows build instructions](./docs/build-windows.md).

### On Linux
See the [Linux build instructions](./docs/build-linux.md).

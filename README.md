# duck-hacker
This is a game where you hack various things by programming DuckBots, small robots that run Lua code.

## Building
Make sure to run `git submodule update --init --recursive` to clone the submodules, as some dependencies are included that way.

### On Windows
You will need Visual Studio 2022 and CMake. Make sure that CMake is located in your PATH (meaning, you can run `cmake` from the command line).

#### Dependencies
Make a folder called `deps/`.

##### SDL2
Download [SDL2-devel-2.28.5-VC.zip](https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5). Extract it into the deps folder.

##### PhysFS
Download [v3.2.0 source code](https://github.com/icculus/physfs/releases/tag/release-3.2.0). Extract into deps. Then run:
```
cd physfs-release-3.2.0
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

##### libpng
> [!NOTE]
> This requires some finagling because we are using Visual Studio 2022 (v143) but the included project with libpng is for Visual Studio 2019 (v142). There is also some fun because the included project targets 32-bit, but we need 64-bit.

First, download [zlib source code](https://www.zlib.net/). (tested with v1.3) Extract into deps. **Rename the folder from `zlib-1.3` to `zlib`.**

Now, download [libpng source code](http://www.libpng.org/pub/png/libpng.html). (tested with v1.6.40) Extract into deps.

Now, open `deps\lpng1640\projects\vstudio\libpng\libpng.vcxproj`. Replace all instances of `v142` with `v143`. Repeat this for all the projects. (pnglibconf, pngstest, pngtest, pngunknown, pngvalid, zlib)

Now, open `zlib.props`. In `DisableSpecificWarnings`, add `;4061;4464;4574;5045` to the end.

Now, open the solution in Visual Studio. In the Solution Explorer, right-click on the solution and click Properties. Then go to Configuration Properties. Then click "Configuration Manager...". Then, in the "Active solution platform" dropdown, click New... and create an x64 platform that copies settings from Win32. Save the solution and close Visual Studio.

Then run:
```
cd lpng1640
cd projects\vstudio
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" /p:Configuration=Release /p:Platform="x64"
```

##### Lua
> Original instructions from https://blog.spreendigital.de/2020/05/21/how-to-compile-lua-5-4-0-for-windows/

Download [Lua 5.4.6](https://www.lua.org/versions.html#5.4). Extract into deps.

Open _x64 Native Tools Command Prompt for VS 2022_. Go to the `lua-5.4.6` folder. Then run:
```
cl /MD /O2 /c /DLUA_BUILD_AS_DLL *.c
ren lua.obj lua.o
ren luac.obj luac.o
link /DLL /IMPLIB:lua5.4.6.lib /OUT:lua5.4.6.dll *.obj
link /OUT:lua.exe lua.o lua5.4.6.lib
lib /OUT:lua5.4.6-static.lib *.obj
link /OUT:luac.exe luac.o lua5.4.6-static.lib
```

##### wxWidgets
Download [wxWidgets v3.2.4 Windows Binaries (Visual Studio 2022)](https://www.wxwidgets.org/downloads/#v3.2.4_msw). Make sure to get the "Header Files", "64-Bit Development Files", and "64-Bit Release DLLs".

Create a folder called `wxWidgets` in deps, and extract both archives into there.

#### Building it
Creat a folder called `build`, then run this command: `cmake -DSDL2_LIBRARY="../deps/SDL2-2.28.5/lib/x64/SDL2" -DSDL2MAIN_LIBRARY="../deps/SDL2-2.28.5/lib/x64/SDL2main.lib" -DSDL2_INCLUDE_DIR="../deps/SDL2-2.28.5/include/" -DPHYSFS_LIBRARY="../deps/physfs-release-3.2.0/build/Release/physfs.lib" -DPHYSFS_INCLUDE_DIR="../deps/physfs-release-3.2.0/src" -DLIBPNG_LIBRARY="../deps/lpng1640/projects/vstudio/x64/Release/libpng16.lib" -DLIBPNG_INCLUDE_DIR="../deps/lpng1640/" -DLUA_LIBRARY="../deps/lua-5.4.6/src/lua5.4.6.lib" -DLUA_INCLUDE_DIR="../deps/lua-5.4.6/src/" -DwxWidgets_ROOT_DIR="../deps/wxWidgets/" -DwxWidgets_CONFIGURATION=mswu -DwxWidgets_EXCLUDE_COMMON_LIBRARIES=ON ..`

#### Copying DLLs
These should all be copied into the folder that the final executable (`duck-hacker.exe`) is built.

* `deps\lua-5.4.6\src\lua5.4.6.dll`
* `deps\lpng1640\projects\vstudio\x64\Release\libpng16.dll`
* `deps\physfs-release-3.2.0\build\Release\physfs.dll`
* `deps\SDL2-2.28.5\lib\x64\SDL2.dll`

##### Debug builds
* `deps\wxWidgets\lib\vc14x_x64_dll\wxbase32ud_vc14x_x64.dll`
* `deps\wxWidgets\lib\vc14x_x64_dll\wxmsw32ud_core_vc14x_x64.dll`
* `deps\wxWidgets\lib\vc14x_x64_dll\wxmsw32ud_stc_vc14x_x64.dll`

##### Release builds
* `deps\wxWidgets\lib\vc14x_x64_dll\wxbase32u_vc14x_x64.dll`
* `deps\wxWidgets\lib\vc14x_x64_dll\wxmsw32u_core_vc14x_x64.dll`
* `deps\wxWidgets\lib\vc14x_x64_dll\wxmsw32u_stc_vc14x_x64.dll`

### On Linux
```bash
# path is for wx-config for wxWidgets
export PATH=$PATH:/home/alex/Documents/duck-stuff/root/bin
SDL2DIR=/home/alex/Documents/duck-stuff/root PHYSFSDIR=/home/alex/Documents/duck-stuff/root LIBPNGDIR=/home/alex/Documents/duck-stuff/root cmake ..
```
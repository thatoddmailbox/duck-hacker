SET(LUA_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
)

FIND_PATH(LUA_INCLUDE_DIR png.h
	HINTS
	$ENV{LUADIR}
	PATH_SUFFIXES include/LUA16 include
	PATHS ${LUA_SEARCH_PATHS}
)

FIND_LIBRARY(LUA_LIBRARY
	NAMES lua
	HINTS
	$ENV{LUADIR}
	PATH_SUFFIXES lib64 lib lib/x64
	PATHS ${LUA_SEARCH_PATHS}
)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Lua REQUIRED_VARS LUA_LIBRARY LUA_INCLUDE_DIR)
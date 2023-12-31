SET(LIBPNG_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
)

FIND_PATH(LIBPNG_INCLUDE_DIR png.h
	HINTS
	$ENV{LIBPNGDIR}
	PATH_SUFFIXES include/libpng16 include
	PATHS ${LIBPNG_SEARCH_PATHS}
)

FIND_LIBRARY(LIBPNG_LIBRARY
	NAMES png16
	HINTS
	$ENV{LIBPNGDIR}
	PATH_SUFFIXES lib64 lib lib/x64
	PATHS ${LIBPNG_SEARCH_PATHS}
)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibPNG REQUIRED_VARS LIBPNG_LIBRARY LIBPNG_INCLUDE_DIR)
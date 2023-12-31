cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_CXX_COMPILER MATCHES "MSVC")
	set(CMAKE_CXX_FLAGS "/EHsc /Zi")
	set(CMAKE_SHARED_LINKER_FLAGS "/DEBUG /OPT:REF /OPT:ICF")
	set(CMAKE_CREATE_WIN32_EXE "/SUBSYSTEM:WINDOWS")
endif()

project(duck-hacker)

include_directories("external/")
include_directories("external/glad/include")
include_directories("external/imgui")
include_directories("external/pugixml/src")
include_directories("./")
include_directories("src/")
file(GLOB SOURCES
	"external/*.c"
	"external/*.cpp"
	"external/glad/src/glad.c"
	"external/imgui/*.cpp"
	"external/imgui/backends/imgui_impl_sdl2.cpp"
	"external/imgui/backends/imgui_impl_opengl3.cpp"
	"external/pugixml/src/pugixml.cpp"
	"src/*.c"
	"src/*.cpp"
	"src/content/*.c"
	"src/content/*.cpp"
	"src/game/*.c"
	"src/game/*.cpp"
	"src/game/editor/*.c"
	"src/game/editor/*.cpp"
	"src/input/*.c"
	"src/input/*.cpp"
	"src/music/*.c"
	"src/music/*.cpp"
	"src/progress/*.c"
	"src/progress/*.cpp"
	"src/render/*.c"
	"src/render/*.cpp"
	"src/world/*.c"
	"src/world/*.cpp"
)

if(WIN32)
	list(APPEND SOURCES "duck-hacker.rc")
endif()

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	# glad requires linking against the dynamic linker (dl)
	target_link_libraries(${PROJECT_NAME} "dl")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} "${SDL2_LIBRARY}" "${SDL2MAIN_LIBRARY}")
include_directories("${SDL2_INCLUDE_DIR}")

find_package(SDL2_mixer REQUIRED)
target_link_libraries(${PROJECT_NAME} "${SDL2_MIXER_LIBRARY}")
include_directories("${SDL2_MIXER_INCLUDE_DIR}")

find_package(PhysFS REQUIRED)
target_link_libraries(${PROJECT_NAME} "${PHYSFS_LIBRARY}")
include_directories("${PHYSFS_INCLUDE_DIR}")

find_package(LibPNG REQUIRED)
target_link_libraries(${PROJECT_NAME} "${LIBPNG_LIBRARY}")
include_directories("${LIBPNG_INCLUDE_DIR}")

find_package(Lua REQUIRED)
target_link_libraries(${PROJECT_NAME} "${LUA_LIBRARY}")
include_directories("${LUA_INCLUDE_DIR}")

find_package(wxWidgets REQUIRED core base stc)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
include_directories("${wxWidgets_INCLUDE_DIRS}")
target_compile_definitions(${PROJECT_NAME} PUBLIC ${wxWidgets_DEFINITIONS})
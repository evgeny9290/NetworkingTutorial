cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 98)

project(SocketUtils VERSION 0.1)

set(TARGET_NAME SocketUtils)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

file (GLOB SOURCES CONFIGURE_DEPENDS Code/Src/*.cpp Code/src/*.cpp Code/Source/*.cpp)
file (GLOB HEADERS CONFIGURE_DEPENDS Code/Header/*.h* Code/Header/*.hpp)


add_library( ${TARGET_NAME} STATIC ${SOURCES} ${HEADERS} )

target_include_directories ( ${TARGET_NAME} PUBLIC Code/Header)

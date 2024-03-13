cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 98)

project(SocketServer VERSION 0.1)

set ( NetworkUtilPath ${CMAKE_CURRENT_SOURCE_DIR}/../Utils )
set ( NetworkBinaryDir ${NetworkUtilPath}/out/build/x64-debug )

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()


file (GLOB SOURCES CONFIGURE_DEPENDS Code/Src/*.cpp Code/src/*.cpp Code/Source/*.cpp)
file (GLOB HEADERS CONFIGURE_DEPENDS Code/Header/*.h* Code/Header/*.hpp)

# Includes
include_directories ( Code/Header )

set(TARGET_NAME SocketServer)

add_subdirectory(${NetworkUtilPath} ${NetworkBinaryDir})


add_executable( ${TARGET_NAME} ${SOURCES} ${HEADERS})
target_include_directories ( ${TARGET_NAME} INTERFACE Code/Header)
target_link_libraries(${TARGET_NAME} SocketUtils)




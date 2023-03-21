cmake_minimum_required(VERSION 3.16)

project(SharedMemoryRingBuffer) #Project name

set(CMAKE_CXX_STANDARD 14) #setting C++ 14 standard

include(GNUInstallDirs)

find_package(PkgConfig) #finding pkg-config is a helper tool

#using pkg-config to getting GLib
pkg_check_modules(GLIB REQUIRED glib-2.0>=2.38)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0>=2.38)
pkg_check_modules(GMODULE REQUIRED gmodule-2.0>=2.38)

#linking GLib library directory
link_directories(
        ${GLIB_LIBRARY_DIRS}
)

#including GLib header files directory
include_directories(
        ${GLIB_INCLUDE_DIRS}
)

set(LIBRARY_NAME shmrb)

add_library(${LIBRARY_NAME} SHARED)

#linking GLib library with target executable
target_link_libraries(${LIBRARY_NAME} PUBLIC ${GLIB_LIBRARIES})

#set include directory
target_include_directories(${LIBRARY_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)

install (FILES shmrb/libshmrb-config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${LIBRARY_NAME})

install(EXPORT libshmrb
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${LIBRARY_NAME})
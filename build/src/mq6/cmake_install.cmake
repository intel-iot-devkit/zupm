# Install script for directory: /project/malikabh/upm-c-staging/src/mq6

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/project/malikabh/upm-c-staging/build/src/mq6/upm-mq6.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libupm-mq6.so.0.7.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libupm-mq6.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libupm-mq6.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/project/malikabh/upm-c-staging/build/src/mq6/libupm-mq6.so.0.7.0"
    "/project/malikabh/upm-c-staging/build/src/mq6/libupm-mq6.so.0"
    "/project/malikabh/upm-c-staging/build/src/mq6/libupm-mq6.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libupm-mq6.so.0.7.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libupm-mq6.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libupm-mq6.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_REMOVE
           FILE "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "mq6")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/upm" TYPE FILE FILES "/project/malikabh/upm-c-staging/src/mq6/mq6.hpp")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "mq6")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/upm" TYPE FILE FILES
    "/project/malikabh/upm-c-staging/src/mq6/mq6.hpp"
    "/project/malikabh/upm-c-staging/src/mq6/mq6.h"
    )
endif()


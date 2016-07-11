# Install script for directory: /project/malikabh/upm-c-staging/src

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/project/malikabh/upm-c-staging/include")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/project/malikabh/upm-c-staging/include/upm.h")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/project/malikabh/upm-c-staging/build/src/a110x/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/ad8232/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/dfrph/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/emg/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/es08a/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/es9257/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/flex/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/gp2y0a/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/grovemoisture/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/gsr/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/joystick12/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/led/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/light/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/loudness/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/m24lr64e/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mpr121/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq2/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq3/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq303a/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq4/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq5/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq6/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq7/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq8/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mq9/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/mvs0608/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/o2/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/slide/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/tsl2561/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/ttp223/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/upm/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/urm37/cmake_install.cmake")
  include("/project/malikabh/upm-c-staging/build/src/vdiv/cmake_install.cmake")

endif()


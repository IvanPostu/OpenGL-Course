#!/bin/sh

# Color variables
black=`tput setaf 0`
red=`tput setaf 1`
green=`tput setaf 2`
yellow=`tput setaf 3`
blue=`tput setaf 4`
magenta=`tput setaf 5`
cyan=`tput setaf 6`
white=`tput setaf 7`
reset=`tput sgr0`

flag1=$1

if [ "$flag1" = '--dependency-resolve' ]; then

  # git subtree add --prefix=external/glfw glfw master --squash
  # git subtree pull --prefix=external/glfw glfw master --squash

  exit 0
fi

if [ "$flag1" = '--init' ]; then

  echo  "${cyan}Initialize build dir. for project using CMakeLists.${reset}"

  # sudo apt-get install libglu1-mesa-dev  
  # sudo apt-get install freeglut3-dev
  # sudo apt-get install mesa-common-dev
  # sudo apt-get install libxrandr-dev
  # sudo apt-get install libxinerama-dev
  # sudo apt-get install libxcursor-dev
  # sudo apt-get install libxi-dev

  rm -rf ./build
  mkdir -p ./build
  cd ./build

  mkdir -p Debug Release

  exit 0
fi

if [ "$flag1" = '--release' ]; then

  echo  "${cyan}Build project in RELEASE mode.${reset}"
  cd ./build
  # cmake --build . --config Release
  cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release .
  make

  exit 0
fi

if [ "$flag1" = '--debug' ]; then

  echo  "${cyan}Build project in DEBUG mode.${reset}"
  cd ./build/Debug
  cmake ../.. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
  cmake --build .

  exit 0
fi


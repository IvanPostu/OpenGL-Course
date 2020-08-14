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

flag=$1

if [ "$flag" = '--init' ]; then

  echo  "${cyan}Initialize build dir. for project using CMakeLists.${reset}"
  rm -rf ./build
  mkdir -p ./build
  cd ./build
  cmake ..
  exit 0

fi

if [ "$flag" = '--release' ]; then

  echo  "${cyan}Build project in RELEASE mode.${reset}"
  cd ./build
  # cmake --build . --config Release
  cmake -DCMAKE_BUILD_TYPE=Release .
  make
  exit 0

fi

if [ "$flag" = '--debug' ]; then

  echo  "${cyan}Build project in DEBUG mode.${reset}"
  cd ./build
  cmake -DCMAKE_BUILD_TYPE=Debug .
  make
  exit 0
  
fi


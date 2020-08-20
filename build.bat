@echo off

set BuildFolder=./build
set flag1=%1


IF "%flag1%"=="--init" (

  if exist "%BuildFolder%" (
    ECHO Build folder found. Recreate . . . 
    RD /S /Q "%BuildFolder%"
    MKDIR "%BuildFolder%"
  ) else (
    ECHO Build folder not found. Creating . . .
    MKDIR "%BuildFolder%"
  )

  cd build
  cmake .. -G "MinGW Makefiles"

)

IF "%flag1%"=="--debug" (

  cd build
  cmake --build . --config Debug --target all .

)

IF "%flag1%"=="--release" (

  cd build
  cmake --build . --config Release --target all .

)
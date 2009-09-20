#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta input library.
#
#

set( sources
     Input.cpp
     InputDevice.cpp
     MappedInput.cpp
     InputDeviceMap.cpp
     InputButtonMap.cpp
     RegionMap.cpp
     Keyboard.cpp
     Mouse.cpp
     Gamepad.cpp
     Wiimote.cpp
   )

set( EpsilonDeltaLibs
     graphics2d mmbase file time math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_input  STATIC  ${sources} )

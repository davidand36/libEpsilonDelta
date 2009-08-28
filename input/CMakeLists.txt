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
     Keyboard.cpp
     Mouse.cpp
     Gamepad.cpp
     Wiimote.cpp
   )

set( EpsilonDeltaLibs
     graphic2d mmbase time math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_input  STATIC  ${sources} )

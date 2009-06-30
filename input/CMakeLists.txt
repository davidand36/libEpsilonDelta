#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta input library.
#
#

set( sources
     Keyboard.cpp
     KeyboardState.cpp
     Mouse.cpp
     MouseState.cpp
     Joystick.cpp
     JoystickState.cpp
   )

if ( useSDL )
    set( sources
         ${sources}
         SDLInput.cpp         
       )
    set( sources  ${sources} ${sdlSources} )
endif ( useSDL )

if ( useCWiiD )
    set( sources
         ${sources}
         CWiiDWiimote.cpp
         WiimoteState.cpp
       )
endif ( useCWiiD )
if ( useWiiYourself )
    set( sources
         ${sources}
         WiiYourselfWiimote.cpp
         WiimoteState.cpp
       )
endif ( useWiiYourself )

set( EpsilonDeltaLibs
     mmbase time math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_input  STATIC  ${sources} )

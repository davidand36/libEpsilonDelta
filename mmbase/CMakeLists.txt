#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta mmbase library.
#
#

set( sources
   )

if ( useSDL )
   set( sources
        ${sources}
        SDL.cpp
        SDLException.cpp
        SDLClock.cpp
      )
endif ( useSDL )

set( EpsilonDeltaLibs
     time util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_mmbase
  STATIC  ${sources} )

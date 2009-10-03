#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestGraphics2DLib.
#
#

set( sources
     TestGraphics2DLib.cpp
   )

set( EpsilonDeltaLibs
     graphics2d mmbase file time math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )

if ( useSDL )
    set( libs  ${libs} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} )
endif ( useSDL )
if ( useFreeType )
    set( libs  ${libs} ${FREETYPE_LIBRARY} )
endif ( useFreeType )

add_executable( TestGraphics2DLib  ${sources} )
target_link_libraries( TestGraphics2DLib  ${libs} )

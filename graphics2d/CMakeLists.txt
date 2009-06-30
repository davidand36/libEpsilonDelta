#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta graphics2d library.
#
#

set( sources
     Color.cpp
     Pixel.cpp
     Point2.cpp
     Line2.cpp
     Rectangle.cpp
     Ellipse.cpp
     Circle.cpp
   )

if ( useSDL )
    set( sources
         ${sources}
         SDLGraphics2D.cpp
         SDLSurface.cpp
         SDLImage.cpp
       )
endif ( useSDL )

set( EpsilonDeltaLibs
     mmbase math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_graphics2d  STATIC  ${sources} )

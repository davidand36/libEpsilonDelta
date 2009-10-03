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
     Dot2.cpp
     LineSegment2.cpp
     Rectangle.cpp
     Ellipse.cpp
     Circle.cpp
     Sprite.cpp
     SimpleSpriteSystem.cpp
     Glyph.cpp
     FontManager.cpp
     FTException.cpp
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
     mmbase file time math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_graphics2d  STATIC  ${sources} )

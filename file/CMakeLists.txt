#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta file library.
#
#

set( sources
     DataBuffer.cpp
     File.cpp
     FileName.cpp
     DirUtil.cpp
     ConfigFile.cpp
   )

set( EpsilonDeltaLibs
     time math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_file  STATIC  ${sources} )

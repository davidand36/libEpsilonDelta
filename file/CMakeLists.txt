#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta file library.
#
#

set( sources
     DataBuffer.cpp
     Saver.cpp
     Loader.cpp
     Reader.cpp
     Writer.cpp
     FileException.cpp
     File.cpp
     FileReader.cpp
     FileWriter.cpp
     NestedReader.cpp
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

#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestMathLib and TestRandom.
#
#

set( EpsilonDeltaLibs
     math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )


set( sources
     TestMathLib.cpp
     PrecisionTests.cpp
   )

add_executable( TestMathLib  ${sources} )
target_link_libraries( TestMathLib  ${libs} )


set( sources
     TestRandom.cpp
   )

add_executable( TestRandom  ${sources} )
target_link_libraries( TestRandom  ${libs} )

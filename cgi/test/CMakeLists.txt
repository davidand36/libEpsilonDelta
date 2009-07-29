#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestCGILib.
#
#

set( sources
     TestCGILib.cpp
   )

set( EpsilonDeltaLibs
     cgi util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )

if ( useFastCGI )
    set( libs  ${libs} ${FastCGI_LIBRARIES} )
endif ( useFastCGI )

add_executable( TestCGILib  ${sources} )
target_link_libraries( TestCGILib  ${libs} )

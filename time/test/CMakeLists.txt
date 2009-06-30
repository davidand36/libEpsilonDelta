#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestTimeLib.
#
#

set( sources
     TestTimeLib.cpp
   )

set( EpsilonDeltaLibs
     time math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )


add_executable( TestTimeLib  ${sources} )
target_link_libraries( TestTimeLib  ${libs} )

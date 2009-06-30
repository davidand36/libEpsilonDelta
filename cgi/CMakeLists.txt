#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta cgi library.
#
#

set( sources
     CGIInput.cpp
     CGIRawInput.cpp
     CGIOutput.cpp
   )

set( EpsilonDeltaLibs
     util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_cgi  STATIC  ${sources} )

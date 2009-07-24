# - Find FastCGI
# Find the FastCGI (version 3) includes and library
# This module defines
#  FastCGI_FOUND, If false, do not try to use FastCGI.
#  FastCGI_INCLUDE_DIR, where to find fcgi_stdio.h, etc.
#  FastCGI_LIBRARIES, the libraries needed to use FastCGI.

find_path( FastCGI_INCLUDE_DIR fcgi_stdio.h)

set( FastCGI_NAMES  ${FastCGI_NAMES} fcgi )
find_library( FastCGI_LIBRARY  NAMES ${FastCGI_NAMES} )

#This is for CMake 2.6:
## handle the QUIETLY and REQUIRED arguments and set FastCGI_FOUND to TRUE if 
## all listed variables are TRUE
#include( FindPackageHandleStandardArgs )
#find_package_handle_standard_args( FastCGI  DEFAULT_MSG
#                                   FastCGI_LIBRARY  FastCGI_INCLUDE_DIR )
#if ( FastCGI_FOUND )
#    set( FastCGI_LIBRARIES  ${FastCGI_LIBRARY} )
#endif ( FastCGI_FOUND )
#This is for CMake 2.4:
if ( FastCGI_INCLUDE_DIR  AND  FastCGI_LIBRARY )
    set( FastCGI_LIBRARIES  ${FastCGI_LIBRARY} )
    set( FastCGI_FOUND  "YES" )
    if ( NOT FastCGI_FIND_QUIETLY )
        message( STATUS  "Found FastCGI: ${FastCGI_LIBRARIES}" )
    endif ( NOT FastCGI_FIND_QUIETLY )
else ( FastCGI_INCLUDE_DIR  AND  FastCGI_LIBRARY )
    set( FastCGI_FOUND  "NO" )
    if ( FastCGI_FIND_REQUIRED )
        message( FATAL_ERROR  "Could not find FastCGI library" )
    endif ( FastCGI_FIND_REQUIRED )
endif ( FastCGI_INCLUDE_DIR  AND  FastCGI_LIBRARY )

mark_as_advanced( FastCGI_LIBRARY  FastCGI_INCLUDE_DIR )

# Deprecated declarations.
set( NATIVE_FastCGI_INCLUDE_PATH  ${FastCGI_INCLUDE_DIR} )
if ( FastCGI_LIBRARY )
    get_filename_component( NATIVE_FastCGI_LIB_PATH  ${FastCGI_LIBRARY} PATH )
endif ( FastCGI_LIBRARY )

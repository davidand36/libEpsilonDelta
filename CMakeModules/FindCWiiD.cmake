# - FindCWiiD
# Find the native CWiiD includes and library
# This module defines
#  CWiiD_FOUND, If false, do not try to use CWiiD.
#  CWiiD_INCLUDE_DIR, where to find sqlite3.h, etc.
#  CWiiD_LIBRARIES, the libraries needed to use CWiiD.

find_path( CWiiD_INCLUDE_DIR cwiid.h)

set( CWiiD_NAMES  ${CWiiD_NAMES} cwiid )
find_library( CWiiD_LIBRARIES  NAMES ${CWiiD_NAMES} )

find_package_handle_standard_args( CWiiD  DEFAULT_MSG
                                   CWiiD_LIBRARIES  CWiiD_INCLUDE_DIR )
set( CWiiD_FOUND ${CWIID_FOUND} )

mark_as_advanced( CWiiD_LIBRARIES  CWiiD_INCLUDE_DIR )

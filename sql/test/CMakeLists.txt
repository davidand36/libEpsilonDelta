#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestSQLLib.
#
#

set( sources
     TestSQLLib.cpp
   )

set( EpsilonDeltaLibs
     sql file time math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )

if ( useSQLite )
    set( libs  ${libs} ${SQLite_LIBRARIES} )
endif ( useSQLite )
if ( useMySQL )
    set( libs  ${libs} ${MYSQL_LIBRARIES} )
endif ( useMySQL )
if ( usePostgreSQL )
    set( libs  ${libs} ${PostgreSQL_LIBRARIES} )
endif ( usePostgreSQL )

add_executable( TestSQLLib  ${sources} )
target_link_libraries( TestSQLLib  ${libs} )

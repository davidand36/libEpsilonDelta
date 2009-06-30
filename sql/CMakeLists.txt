#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta sql library.
#
#

set( sources
     SQLResult.cpp
   )

if ( useSQLite )
    set( sources
         ${sources}
         SQLiteDatabase.cpp
         SQLiteResult.cpp
         SQLiteException.cpp
       )
endif ( useSQLite )
if ( useMySQL )
    set( sources
         ${sources}
         MySQLLibrary.cpp
         MySQLDatabase.cpp
         MySQLResult.cpp
         MySQLException.cpp
       )
endif ( useMySQL )
if ( usePostgreSQL )
    set( sources
         ${sources}
         PostgreSQLDatabase.cpp
         PostgreSQLResult.cpp
         PostgreSQLException.cpp
       )
endif ( usePostgreSQL )

set( EpsilonDeltaLibs
     file time math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_sql  STATIC  ${sources} )

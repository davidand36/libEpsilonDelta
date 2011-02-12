# - Find PostgreSQL
# Find the PostgreSQL includes and library
# This module defines
#  PostgreSQL_FOUND, If false, do not try to use PostgreSQL.
#  PostgreSQL_INCLUDE_DIR, where to find postgresql/libpq-fe.h, etc.
#  PostgreSQL_LIBRARIES, the libraries needed to use PostgreSQL3.

find_path( PostgreSQL_INCLUDE_DIR postgresql/libpq-fe.h )

set( PostgreSQL_NAMES  ${PostgreSQL_NAMES} pq )
find_library( PostgreSQL_LIBRARIES  NAMES ${PostgreSQL_NAMES} )

find_package_handle_standard_args( PostgreSQL  DEFAULT_MSG
                                  PostgreSQL_LIBRARIES  PostgreSQL_INCLUDE_DIR )
set( PostgreSQL_FOUND  ${POSTGRESQL_FOUND} )

mark_as_advanced( PostgreSQL_LIBRARIES  PostgreSQL_INCLUDE_DIR )

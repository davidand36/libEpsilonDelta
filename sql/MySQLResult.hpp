#ifndef MYSQLRESULT_HPP
#define MYSQLRESULT_HPP
/*
  MySQLResult.hpp
  Copyright (C) 2007 David M. Anderson

  MySQLResult class, representing the result of an SQL query to
  a MySQL database.
*/


#ifdef USE_MYSQL


#include "SQLResult.hpp"
#include <string>
#include <vector>
#include <mysql.h>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MySQLResult
    :    public SQLResult
{
public:
    MySQLResult( MYSQL_RES * mysqlRes );
    virtual ~MySQLResult( );
    virtual int NumFields( ) const;
    virtual int NumRecords( ) const;
    virtual std::string FieldName( int field ) const;
    virtual SQL::EFieldType FieldType( int field ) const;
    virtual std::string Value( int record, int field ) const;
    virtual bool IsNull( int record, int field ) const;

private:
    void AddRecord( MYSQL_ROW & mysqlRow, unsigned long * lengths );
    SQL::EFieldType MySQLToFieldType( enum_field_types mysqlType );

    struct Field
    {
        std::string m_value;
        bool        m_isNull;
    };

    std::vector< std::string > m_fieldNames;
    std::vector< SQL::EFieldType > m_fieldTypes;
    std::vector< std::vector< Field > > m_records;

    friend class MySQLDatabase;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_MYSQL
#endif //MYSQLRESULT_HPP

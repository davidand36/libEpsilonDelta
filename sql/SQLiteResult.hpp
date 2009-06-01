#ifndef SQLITERESULT_HPP
#define SQLITERESULT_HPP
/*
  SQLiteResult.hpp
  Copyright (C) 2007 David M. Anderson

  SQLiteResult class, representing the result of an SQL query to
  an SQLite database.
*/


#ifdef USE_SQLITE


#include "SQLResult.hpp"
#include <string>
#include <vector>

struct sqlite3_stmt;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SQLiteResult
    :    public SQLResult
{
public:
    SQLiteResult( ::sqlite3_stmt * stmt );
    virtual ~SQLiteResult( );
    virtual int NumFields( ) const;
    virtual int NumRecords( ) const;
    virtual std::string FieldName( int field ) const;
    virtual SQL::EFieldType FieldType( int field ) const;
    virtual std::string Value( int record, int field ) const;
    virtual bool IsNull( int record, int field ) const;

private:
    void AddRecord( ::sqlite3_stmt * stmt );
    SQL::EFieldType SQLiteToFieldType( int sqliteType );

    struct Field
    {
        std::string m_value;
        bool        m_isNull;
    };

    std::vector< std::string > m_fieldNames;
    std::vector< SQL::EFieldType > m_fieldTypes;
    std::vector< std::vector< Field > > m_records;

    friend class SQLiteDatabase;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SQLITE
#endif //SQLITERESULT_HPP

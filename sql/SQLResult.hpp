#ifndef SQLRESULT_HPP
#define SQLRESULT_HPP
/*
  SQLResult.hpp
  Copyright (C) 2007 David M. Anderson

  SQLResult class, representing the result of an SQL query.
*/


#include <string>
#include <vector>
#include <map>
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace SQL
{

enum EFieldType
{
    Integer,
    LongInt,
    Real,
    Text,
    Blob,
    Date,
    Time,
    DateTime,
    UnknownType
};

}


//*****************************************************************************


class SQLResult
{
public:
    SQLResult( ) { }
    virtual ~SQLResult( ) { }
    virtual int NumFields( ) const = 0;
    virtual int NumRecords( ) const = 0;
    virtual std::string FieldName( int field ) const = 0;
    virtual SQL::EFieldType FieldType( int field ) const = 0;
    virtual std::string Value( int record, int field ) const = 0;
    virtual bool IsNull( int record, int field ) const = 0;

    class Record
    {
    public:
        Record( std::tr1::shared_ptr< SQLResult const > pResult, int record );
        void Set( int record );
        std::string operator[]( int field ) const;
        std::string operator[]( std::string fieldName ) const;

    private:
        std::tr1::shared_ptr< SQLResult const >     m_pResult;
        int                                         m_record;
    };
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SQLRESULT_HPP

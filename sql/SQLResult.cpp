/*
  SQLResult.cpp
  Copyright (C) 2007 David M. Anderson

  SQLResult class, representing the result of an SQL query.
*/


#include "SQLResult.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


SQLResult::Record::Record( shared_ptr< SQLResult const > pResult, int record )
    :   m_pResult( pResult ),
        m_record( record )
{
}

//-----------------------------------------------------------------------------

void 
SQLResult::Record::Set( int record )
{
    m_record = record;
}

//=============================================================================

std::string 
SQLResult::Record::operator[]( int field ) const
{
    return m_pResult->Value( m_record, field );
}

//-----------------------------------------------------------------------------

std::string 
SQLResult::Record::operator[]( std::string fieldName ) const
{
    for ( int i = 0; i < m_pResult->NumFields(); ++i )
        if ( m_pResult->FieldName( i ) == fieldName )
            return m_pResult->Value( m_record, i );
    return "";
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

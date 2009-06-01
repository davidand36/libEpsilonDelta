#ifndef CSV_HPP
#define CSV_HPP
/*
  CSV.hpp
  Copyright (C) 2007 David M. Anderson

  Routines for parsing and generating CSV (comma-separated values) format
  strings.
  NOTES:
  1. SplitCSV() parses a "standard" CSV string, first into rows separated by
     line feeds, and then into fields separated by commas. (No line feed is
     expected after the final row, and no comma after the final field in
     each row.) It also trims leading and trailing white space in each field,
     i.e. white space around the separating commas. It treats text inside
     double quotes (") literally, including commas and line feeds in the
     field text, rather than treating them as delimiters. Within quoted text,
     two consecutive quotes are converted to one literal quote.
  2. Although it might seem undesirable, SplitCSV() has an option to provide
     the same behavior as Microsoft Excel, in that it trims white space in
     fields even when that white space is inside quotes.
  3. CreateCSV() is the effective inverse of SplitCSV(): it converts a
     table of strings into a single CSV formatted string. All fields are
     enclosed in quotes.
*/


#include "StringUtil.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename Ch, typename Tr, typename A >
std::vector< std::vector< std::basic_string< Ch, Tr, A > > >
SplitCSV( std::basic_string< Ch, Tr, A > csvText,
          bool trimInsideQuotes = false );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
CreateCSV( std::vector< std::vector< std::basic_string< Ch, Tr, A > > > table );

#ifdef DEBUG
bool TestCSV( );
#endif


//#############################################################################


template < typename Ch, typename Tr, typename A >
std::vector< std::vector< std::basic_string< Ch, Tr, A > > > 
SplitCSV( std::basic_string< Ch, Tr, A > csvText, bool trimInsideQuotes )
{
    typedef std::vector< std::basic_string< Ch, Tr, A > > StringVec;
    typedef std::vector< StringVec > StringVecVec;
    typedef typename StringVec::iterator SVIter;
    StringVecVec table;
    StringVec csvRows = Split( csvText, static_cast< Ch >( '\n' ),
                               static_cast< Ch >( '\"' ), true );
    for ( SVIter pRow = csvRows.begin(); pRow != csvRows.end(); ++pRow )
    {
        StringVec csvFields = Split( *pRow, static_cast< Ch >( ',' ),
                                     static_cast< Ch >( '\"' ), true );
        for ( SVIter pField = csvFields.begin(); pField != csvFields.end();
              ++pField )
        {
            if ( trimInsideQuotes )
                Trim( RemoveQuotes( &(*pField), static_cast< Ch >( '\"' ) ) );
            else
                RemoveQuotes( Trim( &(*pField) ), static_cast< Ch >( '\"' ) );
        }
        table.push_back( csvFields );
    }
    return table;
}

//=============================================================================

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > 
CreateCSV( std::vector< std::vector< std::basic_string< Ch, Tr, A > > > table )
{
    typedef std::vector< std::basic_string< Ch, Tr, A > > StringVec;
    typedef std::vector< StringVec > StringVecVec;
    typedef typename StringVec::iterator SVIter;
    typedef typename StringVecVec::iterator SVVIter;
    std::basic_string< Ch, Tr, A > csvText;
    for ( SVVIter pRow = table.begin(); pRow != table.end(); ++pRow )
    {
        for ( SVIter pField = pRow->begin(); pField != pRow->end(); ++pField )
        {
            AddQuotes( &(*pField), static_cast< Ch >( '\"' ) );
            csvText += *pField;
            if ( pField + 1 != pRow->end() )
                csvText += static_cast< Ch >( ',' );
        }
        if ( pRow + 1 != table.end() )
            csvText += static_cast< Ch >( '\n' );
    }
    return  csvText;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CSV_HPP

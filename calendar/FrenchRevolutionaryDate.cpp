/*
  FrenchRevolutionaryDate.cpp
  Copyright (C) 2007 David M. Anderson

  FrenchRevolutionaryDate class, representing a date in the French
  Revolutionary calendar.
*/


#include "FrenchRevolutionaryDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


FrenchRevolutionaryDate::FrenchRevolutionaryDate( bool today )
    :   DateDMY< FrenchRevolutionaryCalendar >( today )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FrenchRevolutionaryDate::FrenchRevolutionaryDate( long julianDay )
    :   DateDMY< FrenchRevolutionaryCalendar >( julianDay )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FrenchRevolutionaryDate::FrenchRevolutionaryDate( int day, int month,
                                                  long year )
    :   DateDMY< FrenchRevolutionaryCalendar >( day, month, year )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FrenchRevolutionaryDate::FrenchRevolutionaryDate( const DateJD & date )
    :   DateDMY< FrenchRevolutionaryCalendar >( date )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FrenchRevolutionaryDate::FrenchRevolutionaryDate(
    const FrenchRevolutionaryDate & date )
    :   DateDMY< FrenchRevolutionaryCalendar >( date )
{
}

//-----------------------------------------------------------------------------

FrenchRevolutionaryDate::~FrenchRevolutionaryDate( )
{
}

//=============================================================================

FrenchRevolutionaryDate & 
FrenchRevolutionaryDate::operator=( const DateJD & rhs )
{
    if ( &rhs == this )
        return *this;
    DateDMY< FrenchRevolutionaryCalendar >::operator=( rhs );
    return *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FrenchRevolutionaryDate & 
FrenchRevolutionaryDate::operator=(
    const DateDMY< FrenchRevolutionaryCalendar > & rhs )
{
    if ( &rhs == this )
        return *this;
    DateDMY< FrenchRevolutionaryCalendar >::operator=( rhs );
    return *this;
}

//=============================================================================

std::string 
FrenchRevolutionaryDate::ToString( const std::string & format ) const
{
    return  DateDMY< FrenchRevolutionaryCalendar >::ToString( format );
}

//-----------------------------------------------------------------------------
   
void 
FrenchRevolutionaryDate::SetDefaultFormat( const std::string & format )
{
    DateDMY< FrenchRevolutionaryCalendar >::SetDefaultFormat( format );
}

//-----------------------------------------------------------------------------

const std::string & 
FrenchRevolutionaryDate::DefaultFormat( )
{
    return DateDMY< FrenchRevolutionaryCalendar >::DefaultFormat();
}

//=============================================================================

#ifdef DEBUG

bool
TestFrenchRevolutionaryDate( )
{
    bool ok = true;
    cout << "Testing FrenchRevolutionaryDate" << endl;

    long jd;
    int d, m;
    long y;
    FrenchRevolutionaryDate frDate;

    struct
    {
        long julianDay;
        int day;
        int month;
        long year;
    } 
    testDates[]
            = {
                { 1507232,   5, 11, -2378 },
                { 1660038,  14,  3, -1959 },
                { 1746894,   2,  1, -1721 },
                { 1770642,  10,  1, -1656 },
                { 1892732,  19,  4, -1322 },
                { 1931580,   1,  9, -1216 },
                { 1974852,  19,  2, -1097 },
                { 2091165,   5,  8,  -779 },
                { 2121510,   5,  9,  -696 },
                { 2155780,   2,  7,  -602 },
                { 2174030,  20,  6,  -552 },
                { 2191585,  13,  7,  -504 },
                { 2195262,   8,  8,  -494 },
                { 2229275,  23,  9,  -401 },
                { 2245581,  14,  5,  -356 },
                { 2266101,  20,  7,  -300 },
                { 2288543,   2, 13,  -239 },
                { 2290902,  15,  6,  -232 },
                { 2323141,  22,  9,  -144 },
                { 2334849,  12, 10,  -112 },
                { 2348021,   6, 11,   -76 },
                { 2366979,   1, 10,   -24 },
                { 2385649,  14, 11,    27 },
                { 2392826,   6,  7,    47 },
                { 2416224,  28,  7,   111 },
                { 2425849,   7, 12,   137 },
                { 2430267,   7,  1,   150 },
                { 2430834,  29,  7,   151 },
                { 2431005,  15,  1,   152 },
                { 2448699,  27,  6,   200 },
                { 2450139,   6,  6,   204 },
                { 2465738,  20,  2,   247 },
                { 2486077,  30, 10,   302 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        frDate.Set( d, m, y );
        TESTCHECK( frDate.Valid( ), true, &ok );
        TESTCHECK( frDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        frDate.Set( jd );
        TESTCHECK( frDate.Valid( ), true, &ok );
        TESTCHECK( frDate.Day( ), d, &ok );
        TESTCHECK( frDate.Month( ), m, &ok );
        TESTCHECK( frDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "FrenchRevolutionaryDate PASSED." << endl << endl;
    else
        cout << "FrenchRevolutionaryDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

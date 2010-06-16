/*
  ChineseDate.cpp
  Copyright (C) 2007 David M. Anderson

  ChineseDate class, representing a date in the Chinese calendar.
*/


#include "ChineseDate.hpp"
#include "Assert.hpp"
#include <string>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


ChineseDate::ChineseDate( bool today )
    :   JDDate( false )
{
    Set( today );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ChineseDate::ChineseDate( long julianDay )
    :   JDDate( false )
{
    Set( julianDay );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ChineseDate::ChineseDate( int day, int month, bool leap, long year )
    :   JDDate( false )
{
    Set( day, month, leap, year );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ChineseDate::ChineseDate( int day, int month, int yearCyclical, long yearCycle,
                          int leapMonth )
    :   JDDate( false )
{
    Set( day, month, yearCyclical, yearCycle, leapMonth );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ChineseDate::ChineseDate( int day, int month, bool leap,
                          int yearCyclical, long yearCycle )
    :   JDDate( false )
{
    Set( day, month, leap, yearCyclical, yearCycle );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ChineseDate::ChineseDate( const JDDate & date )
    :   JDDate( false )
{
    Set( date.JulianDay() );
}

//=============================================================================

ChineseDate & 
ChineseDate::operator=( const JDDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

void 
ChineseDate::Set( bool today )
{
    if ( today )
    {
        JDDate::Set( true );
        Assert( JDDate::Valid() );
        Calendar::JulianDayToDMYL( JDDate::JulianDay(),
                                   &m_day, &m_month, &m_year, &m_leapMonth );
    }
    else
    {
        JDDate::Set( false );
        Set( 1, 1, 1, Calendar::LMUnknown );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
ChineseDate::Set( long julianDay )
{
    JDDate::Set( julianDay );
    Calendar::JulianDayToDMYL( julianDay,
                               &m_day, &m_month, &m_year, &m_leapMonth );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
ChineseDate::Set( int day, int month, bool leap, long year )
{
    JDDate::Set( JDDate::INVALID );
    m_day = day;
    m_month = month;
    m_year = year;
    if ( leap )
    {
        ++m_month;
        m_leapMonth = m_month;
    }
    else
    {
        m_leapMonth = Calendar::LeapMonth( m_year );
        if ( m_leapMonth <= m_month )
            ++m_month;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
ChineseDate::Set( int day, int month, int yearCyclical, long yearCycle,
                  int leapMonth )
{
    long year = Calendar::SexagesimalToLinear( yearCyclical, yearCycle );
    Set( day, month, year, leapMonth );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
ChineseDate::Set( int day, int month, bool leap,
                  int yearCyclical, long yearCycle )
{
    long year = Calendar::SexagesimalToLinear( yearCyclical, yearCycle );
    Set( day, month, leap, year );
}

//=============================================================================

bool 
ChineseDate::Valid( ) const
{
    return Calendar::Valid( m_day, m_month, m_year, m_leapMonth );
}

//-----------------------------------------------------------------------------

void 
ChineseDate::MakeValid( DateFixup::EMethod fixupMethod )
{
    long julianDay = Calendar::MakeValid( &m_day, &m_month, &m_year,
                                         &m_leapMonth, fixupMethod );
    JDDate::Set( julianDay );
}

//=============================================================================

long 
ChineseDate::JulianDay( ) const
{
    if ( ! JDDate::Valid() )
        m_julianDay = Calendar::DMYToJulianDay( m_day, m_month, m_year );
    return JDDate::JulianDay();
}

//=============================================================================

int 
ChineseDate::MonthNumber( ) const
{
    if ( m_leapMonth == Calendar::LMUnknown )
        m_leapMonth = Calendar::LeapMonth( m_year );
    if ( m_month < m_leapMonth )
        return m_month;
    else
        return m_month - 1;
}

//-----------------------------------------------------------------------------

bool 
ChineseDate::IsMonthLeap( ) const
{
    if ( m_leapMonth == Calendar::LMUnknown )
        m_leapMonth = Calendar::LeapMonth( m_year );
    return (m_month == m_leapMonth);
}

//=============================================================================

int 
ChineseDate::DayCyclical( ) const
{
    return Calendar::DayCyclical( JulianDay() );
}

//-----------------------------------------------------------------------------

int 
ChineseDate::MonthCyclical( ) const
{
    return Calendar::MonthCyclical( m_month, m_year );
}

//-----------------------------------------------------------------------------

int 
ChineseDate::YearCyclical( ) const
{
    int cyclical;
    Calendar::LinearToSexagesimal( m_year, &cyclical, 0 );
    return cyclical;
}

//-----------------------------------------------------------------------------

long 
ChineseDate::YearCycle( ) const
{
    long cycle;
    Calendar::LinearToSexagesimal( m_year, 0, &cycle );
    return cycle;
}

//-----------------------------------------------------------------------------

int 
ChineseDate::DayCelestialStem( ) const
{
    int stem;
    Calendar::DayStemAndBranch( JulianDay(), &stem, 0 );
    return stem;
}

//-----------------------------------------------------------------------------

int 
ChineseDate::DayTerrestrialBranch( ) const
{
    int branch;
    Calendar::DayStemAndBranch( JulianDay(), 0, &branch );
    return branch;
}

//-----------------------------------------------------------------------------

int 
ChineseDate::MonthCelestialStem( ) const
{
    int stem;
    Calendar::MonthStemAndBranch( m_month, m_year, &stem, 0 );
    return stem;
}

//-----------------------------------------------------------------------------

int 
ChineseDate::MonthTerrestrialBranch( ) const
{
    int branch;
    Calendar::MonthStemAndBranch( m_month, m_year, 0, &branch );
    return branch;
}

//-----------------------------------------------------------------------------

int 
ChineseDate::YearCelestialStem( ) const
{
    int stem;
    Calendar::YearStemAndBranch( m_year, &stem, 0 );
    return stem;
}

//-----------------------------------------------------------------------------

int 
ChineseDate::YearTerrestrialBranch( ) const
{
    int branch;
    Calendar::YearStemAndBranch( m_year, 0, &branch );
    return branch;
}

//-----------------------------------------------------------------------------

int 
ChineseDate::MajorSolarTerm( ) const
{
    int term;
    Calendar::SolarTerms( JulianDay(), &term, 0 );
    return term;
}

//-----------------------------------------------------------------------------

int 
ChineseDate::MinorSolarTerm( ) const
{
    int term;
    Calendar::SolarTerms( JulianDay(), 0, &term );
    return term;
}

//=============================================================================

void 
ChineseDate::Increment( int days )
{
    m_day += days;
    MakeValid( DateFixup::Carry );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
ChineseDate::Increment( int days, int months, long years, 
                        DateFixup::EMethod fixupMethod )
{
    m_day += days;
    m_month += months;
    m_year += years;
    MakeValid( fixupMethod );
}

//=============================================================================

std::string 
ChineseDate::m_defaultFormat = ""; //!!!

//-----------------------------------------------------------------------------

std::string 
ChineseDate::ToString( const std::string & format ) const
{
    return format; //!!!
}

//-----------------------------------------------------------------------------

#ifdef DEBUG

bool 
TestChineseDate( )
{
    bool ok = true;
    cout << "Testing ChineseDate" << endl;

    if ( ! ChineseCalendar::Test( ) )
        ok = false;

    long jd;
    int d, m;
    long y;
    bool l;
    cout << "ChineseDate() [default constructor]" << endl;
    ChineseDate chinDate;
    TESTCHECK( chinDate.Valid( ), true, &ok );
    TESTCHECK( chinDate.Day( ), 1, &ok );
    TESTCHECK( chinDate.TrueMonth( ), 1, &ok );
    TESTCHECK( chinDate.MonthNumber( ), 1, &ok );
    TESTCHECK( chinDate.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDate.Year( ), 1L, &ok );
    TESTCHECK( chinDate.JulianDay( ), 758326L, &ok );
    TESTCHECK( chinDate.ToString( ), string( "" ), &ok );
    TESTCHECK( chinDate.ToString( "" ),
               string( "" ), &ok );
    jd = 2448036;
    cout << "Set(" << jd << ") :" << endl;
    chinDate.Set( jd );
    TESTCHECK( chinDate.Valid( ), true, &ok );
    TESTCHECK( chinDate.Day( ), 1, &ok );
    TESTCHECK( chinDate.TrueMonth( ), 5, &ok );
    TESTCHECK( chinDate.MonthNumber( ), 5, &ok );
    TESTCHECK( chinDate.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDate.Year( ), 4627L, &ok );
    TESTCHECK( chinDate.JulianDay( ), 2448036L, &ok );
    TESTCHECK( chinDate.ToString( ), string( "" ), &ok );
    TESTCHECK( chinDate.ToString( "" ),
               string( "" ), &ok );
    int incr = 30;
    cout << "Increment(" << incr << ") :" << endl;
    chinDate.Increment( incr );
    TESTCHECK( chinDate.Valid( ), true, &ok );
    TESTCHECK( chinDate.Day( ), 1, &ok );
    TESTCHECK( chinDate.TrueMonth( ), 6, &ok );
    TESTCHECK( chinDate.MonthNumber( ), 5, &ok );
    TESTCHECK( chinDate.IsMonthLeap( ), true, &ok );
    TESTCHECK( chinDate.Year( ), 4627L, &ok );
    TESTCHECK( chinDate.JulianDay( ), jd + 30, &ok );
    TESTCHECK( chinDate.ToString( ), string( "" ), &ok );

    cout << "ChineseDate( true ) [today constructor]" << endl;
    ChineseDate chinToday( true );
    TESTCHECK( chinToday.Valid( ), true, &ok );
    cout << "chinToday.JulianDay( )=" << chinToday.JulianDay( );
    if ( (chinToday.JulianDay( ) > 2451545)       //test good until 2132 A.D.
         && (chinToday.JulianDay( ) < 2500000) )  // (mid 80th cycle)
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (chinDate == chinToday), false, &ok );
    TESTCHECK( (chinDate < chinToday), true, &ok );
    cout << "chinToday.ToString()=" << chinToday.ToString( ) << endl;

    jd = 2447918;
    cout << "ChineseDate( " << jd << " ) [Julian Day constructor]" << endl;
    ChineseDate chinJD( jd );
    TESTCHECK( chinJD.Valid( ), true, &ok );
    TESTCHECK( chinJD.Day( ), 30, &ok );
    TESTCHECK( chinJD.TrueMonth( ), 12, &ok );
    TESTCHECK( chinJD.MonthNumber( ), 12, &ok );
    TESTCHECK( chinJD.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinJD.Year( ), 4626L, &ok );
    TESTCHECK( (chinJD == chinJD), true, &ok );
    TESTCHECK( (chinJD < chinDate), true, &ok );
    TESTCHECK( (chinJD == chinDate), false, &ok );
    TESTCHECK( chinJD.ToString( ), string( "" ), &ok );

    jd = 2448274;
    cout << "Set( " << jd << " )" << endl;
    chinJD.Set( jd );
    TESTCHECK( chinJD.Valid( ), true, &ok );
    TESTCHECK( chinJD.Day( ), 2, &ok );
    TESTCHECK( chinJD.TrueMonth( ), 13, &ok );
    TESTCHECK( chinJD.MonthNumber( ), 12, &ok );
    TESTCHECK( chinJD.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinJD.Year( ), 4627L, &ok );
    TESTCHECK( (chinJD == chinJD), true, &ok );
    TESTCHECK( (chinJD < chinDate), false, &ok );
    TESTCHECK( (chinJD == chinDate), false, &ok );
    TESTCHECK( chinJD.ToString( ), string( "" ), &ok );

    d = 1, m = 12, y = 4670;
    cout << "ChineseDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    ChineseDate chinDMY( d, m, y );
    TESTCHECK( chinDMY.Valid( ), true, &ok );
    TESTCHECK( (chinJD < chinDMY), true, &ok );
    TESTCHECK( chinDMY.JulianDay( ), 2463954L, &ok );
    TESTCHECK( chinDMY.ToString( "" ),
               string( "" ), &ok );
    incr = 1;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    chinDMY.Increment( 0, incr, 0 );
    TESTCHECK( chinDMY.Valid( ), true, &ok );
    TESTCHECK( chinDMY.Day( ), 1, &ok );
    TESTCHECK( chinDMY.TrueMonth( ), 13, &ok );
    TESTCHECK( chinDMY.MonthNumber( ), 12, &ok );
    TESTCHECK( chinDMY.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDMY.Year( ), 4670L, &ok );
    TESTCHECK( chinDMY.JulianDay( ), 2463983L, &ok );
    incr = -1;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    chinDMY.Increment( 0, incr, 0 );
    TESTCHECK( chinDMY.Valid( ), true, &ok );
    TESTCHECK( chinDMY.Day( ), 1, &ok );
    TESTCHECK( chinDMY.TrueMonth( ), 12, &ok );
    TESTCHECK( chinDMY.MonthNumber( ), 11, &ok );
    TESTCHECK( chinDMY.IsMonthLeap( ), true, &ok );
    TESTCHECK( chinDMY.Year( ), 4670L, &ok );
    TESTCHECK( chinDMY.JulianDay( ), 2463954L, &ok );
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    chinDMY.Increment( 0, incr, 0 );
    TESTCHECK( chinDMY.Valid( ), true, &ok );
    TESTCHECK( chinDMY.Day( ), 1, &ok );
    TESTCHECK( chinDMY.TrueMonth( ), 11, &ok );
    TESTCHECK( chinDMY.MonthNumber( ), 11, &ok );
    TESTCHECK( chinDMY.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDMY.Year( ), 4670L, &ok );
    TESTCHECK( chinDMY.JulianDay( ), 2463924L, &ok );
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    chinDMY.Increment( 0, incr, 0 );
    TESTCHECK( chinDMY.Valid( ), true, &ok );
    TESTCHECK( chinDMY.Day( ), 1, &ok );
    TESTCHECK( chinDMY.TrueMonth( ), 10, &ok );
    TESTCHECK( chinDMY.MonthNumber( ), 10, &ok );
    TESTCHECK( chinDMY.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDMY.Year( ), 4670L, &ok );
    TESTCHECK( chinDMY.JulianDay( ), 2463894L, &ok );
    l = false;
    d = 15; m = 5; y = 4627;
    cout << "ChineseDate( " << d << ", " << m << ", " << l << ", " << y
         << " ) [D,M,L,Y constructor]" << endl;
    ChineseDate chinDMLY( d, m, l, y );
    TESTCHECK( chinDMLY.Valid( ), true, &ok );
    TESTCHECK( chinDMLY.Day( ), 15, &ok );
    TESTCHECK( chinDMLY.TrueMonth( ), 5, &ok );
    TESTCHECK( chinDMLY.MonthNumber( ), 5, &ok );
    TESTCHECK( chinDMLY.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDMLY.Year( ), 4627L, &ok );
    TESTCHECK( chinDMLY.JulianDay( ), 2448050L, &ok );
    l = true;
    cout << "Set( " << d << ", " << m << ", " << l << ", " << y << " )" << endl;
    chinDMLY.Set( d, m, l, y );
    TESTCHECK( chinDMLY.Valid( ), true, &ok );
    TESTCHECK( chinDMLY.Day( ), 15, &ok );
    TESTCHECK( chinDMLY.TrueMonth( ), 6, &ok );
    TESTCHECK( chinDMLY.MonthNumber( ), 5, &ok );
    TESTCHECK( chinDMLY.IsMonthLeap( ), true, &ok );
    TESTCHECK( chinDMLY.Year( ), 4627L, &ok );
    TESTCHECK( chinDMLY.JulianDay( ), 2448080L, &ok );
    m = 6; l = false;
    cout << "Set( " << d << ", " << m << ", " << l << ", " << y << " )" << endl;
    chinDMLY.Set( d, m, l, y );
    TESTCHECK( chinDMLY.Valid( ), true, &ok );
    TESTCHECK( chinDMLY.Day( ), 15, &ok );
    TESTCHECK( chinDMLY.TrueMonth( ), 7, &ok );
    TESTCHECK( chinDMLY.MonthNumber( ), 6, &ok );
    TESTCHECK( chinDMLY.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDMLY.Year( ), 4627L, &ok );
    TESTCHECK( chinDMLY.JulianDay( ), 2448109L, &ok );
    cout << "Set( " << d << ", " << m << ", " << y << " )" << endl;
    chinDMLY.Set( d, m, y );
    TESTCHECK( chinDMLY.Valid( ), true, &ok );
    TESTCHECK( chinDMLY.Day( ), 15, &ok );
    TESTCHECK( chinDMLY.TrueMonth( ), 6, &ok );
    TESTCHECK( chinDMLY.MonthNumber( ), 5, &ok );
    TESTCHECK( chinDMLY.IsMonthLeap( ), true, &ok );
    TESTCHECK( chinDMLY.Year( ), 4627L, &ok );
    TESTCHECK( chinDMLY.JulianDay( ), 2448080L, &ok );
    d = 1; m = 12; y = 4670;
    cout << "Set( " << d << ", " << m << ", " << y << " )" << endl;
    chinDMLY.Set( d, m, y );
    TESTCHECK( chinDMLY.Valid( ), true, &ok );
    TESTCHECK( chinDMLY.Day( ), 1, &ok );
    TESTCHECK( chinDMLY.TrueMonth( ), 12, &ok );
    TESTCHECK( chinDMLY.MonthNumber( ), 11, &ok );
    TESTCHECK( chinDMLY.IsMonthLeap( ), true, &ok );
    TESTCHECK( chinDMLY.Year( ), 4670L, &ok );
    TESTCHECK( chinDMLY.JulianDay( ), 2463954L, &ok );
    l = false;
    cout << "Set( " << d << ", " << m << ", " << l << ", " << y << " )" << endl;
    chinDMLY.Set( d, m, l, y );
    TESTCHECK( chinDMLY.Valid( ), true, &ok );
    TESTCHECK( chinDMLY.Day( ), 1, &ok );
    TESTCHECK( chinDMLY.TrueMonth( ), 13, &ok );
    TESTCHECK( chinDMLY.MonthNumber( ), 12, &ok );
    TESTCHECK( chinDMLY.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDMLY.Year( ), 4670L, &ok );
    TESTCHECK( chinDMLY.JulianDay( ), 2463983L, &ok );
    m = 13;
    cout << "Set( " << d << ", " << m << ", " << y << " )" << endl;
    chinDMLY.Set( d, m, y );
    TESTCHECK( chinDMLY.Valid( ), true, &ok );
    TESTCHECK( chinDMLY.Day( ), 1, &ok );
    TESTCHECK( chinDMLY.TrueMonth( ), 13, &ok );
    TESTCHECK( chinDMLY.MonthNumber( ), 12, &ok );
    TESTCHECK( chinDMLY.IsMonthLeap( ), false, &ok );
    TESTCHECK( chinDMLY.Year( ), 4670L, &ok );
    TESTCHECK( chinDMLY.JulianDay( ), 2463983L, &ok );
    m = 11; l = true;
    cout << "Set( " << d << ", " << m << ", " << l << ", " << y << " )" << endl;
    chinDMLY.Set( d, m, l, y );
    TESTCHECK( chinDMLY.Valid( ), true, &ok );
    TESTCHECK( chinDMLY.Day( ), 1, &ok );
    TESTCHECK( chinDMLY.TrueMonth( ), 12, &ok );
    TESTCHECK( chinDMLY.MonthNumber( ), 11, &ok );
    TESTCHECK( chinDMLY.IsMonthLeap( ), true, &ok );
    TESTCHECK( chinDMLY.Year( ), 4670L, &ok );
    TESTCHECK( chinDMLY.JulianDay( ), 2463954L, &ok );
 
    struct
    {
        long julianDay;
        int day;
        int month;
        bool leap;
        int yearCyclical;
        long yearCycle;
    } 
    testDates[]
            = {
                { 1507232,  12,  6, false, 11, 35 },
                { 1660038,  27, 10, false,  9, 42 },
                { 1746894,   4,  8, false,  7, 46 },
                { 1770642,   9,  8, false, 12, 47 },
                { 1892732,  20, 11, false, 46, 52 },
                { 1931580,   5,  4, false, 33, 54 },
                { 1974852,  15, 10, false, 31, 56 },
                { 2091165,   7,  3, false, 50, 61 },
                { 2121510,  24,  4, false, 13, 63 },
                { 2155780,   9,  2, false, 47, 64 },
                { 2174030,   9,  2, false, 37, 65 },
                { 2191585,  23,  2, false, 25, 66 },
                { 2195262,   9,  3, false, 35, 66 },
                { 2229275,   2,  5, false,  8, 68 },
                { 2245581,   8,  1, false, 53, 68 },
                { 2266101,   4,  3, false, 49, 69 },
                { 2288543,   2,  8, false, 50, 70 },
                { 2290902,  29,  1, false, 57, 70 },
                { 2323141,  20,  4,  true, 25, 72 },
                { 2334849,   5,  6, false, 57, 72 },
                { 2348021,   6,  6, false, 33, 73 },
                { 2366979,   5,  5, false, 25, 74 },
                { 2385649,  12,  6, false, 16, 75 },
                { 2392826,  13,  2, false, 36, 75 },
                { 2416224,  22,  3, false, 40, 76 },
                { 2425849,  21,  7, false,  6, 77 },
                { 2430267,   9,  8, false, 18, 77 },
                { 2430834,  15,  3, false, 20, 77 },
                { 2431005,   9,  9, false, 20, 77 },
                { 2448699,  14,  2, false,  9, 78 },
                { 2450139,   7,  1, false, 13, 78 },
                { 2465738,  14, 10, false, 55, 78 },
                { 2486077,   7,  6, false, 51, 79 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        l = testDates[i].leap;
        int y = testDates[i].yearCyclical;
        long c = testDates[i].yearCycle;
        cout << "Set( " << d << ", " << m <<  ", " << l << ", " << y
             << ", " << c << " ) :" << endl;
        chinDate.Set( d, m, l, y, c );
        TESTCHECK( chinDate.Valid( ), true, &ok );
        TESTCHECK( chinDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        chinDate.Set( jd );
        TESTCHECK( chinDate.Valid( ), true, &ok );
        TESTCHECK( chinDate.Day( ), d, &ok );
        TESTCHECK( chinDate.MonthNumber( ), m, &ok );
        TESTCHECK( chinDate.IsMonthLeap( ), l, &ok );
        TESTCHECK( chinDate.YearCyclical( ), y, &ok );
        TESTCHECK( chinDate.YearCycle( ), c, &ok );
    }

    if ( ok )
        cout << "ChineseDate PASSED." << endl << endl;
    else
        cout << "ChineseDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

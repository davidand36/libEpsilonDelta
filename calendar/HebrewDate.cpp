/*
  HebrewDate.cpp
  Copyright (C) 2007 David M. Anderson

  HebrewDate class, representing dates in the Hebrew calendar.
*/


#include "HebrewDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "GregorianDate.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


// Specialized because the Hebrew year and epoch begin with 
//  the 7th month (Tishri).

template<>
void 
DateDMY< HebrewCalendar >::Set( bool today )
{
    if ( today )
    {
        DateJD::Set( true );
        Assert( DateJD::Valid() );
        HebrewCalendar::JulianDayToDMY( JulianDay(), &m_day, &m_month, &m_year );
    }
    else
    {
        DateJD::Set( false );
        Set( 1, 7, 1 );
    }
}


//=============================================================================


#ifdef DEBUG

bool
TestHebrewDate( )
{
    bool ok = true;
    cout << "Testing HebrewDate" << endl;

    cout << "HebrewDate() [default constructor]" << endl;
    HebrewDate hebDate;
    TESTCHECK( hebDate.Valid( ), true, &ok );
    TESTCHECK( hebDate.Day( ), 1, &ok );
    TESTCHECK( hebDate.Month( ), 7, &ok );
    TESTCHECK( hebDate.Year( ), 1, &ok );
    TESTCHECK( hebDate.JulianDay( ), 347998, &ok );
    TESTCHECK( hebDate.DayOfWeek( ), 1, &ok );
    TESTCHECK( hebDate.ToString( ), string( "Yom Shaynee, 1 Tishri 1" ), &ok );

    int d = 7;
    int m = 9;
    int y = 5706;
    cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
    hebDate.Set( d, m, y );
    TESTCHECK( hebDate.Valid( ), true, &ok );
    TESTCHECK( hebDate.Day( ), d, &ok );
    TESTCHECK( hebDate.Month( ), m, &ok );
    TESTCHECK( hebDate.Year( ), y, &ok );
    TESTCHECK( hebDate.JulianDay( ), 2431772, &ok );
    TESTCHECK( hebDate.DayOfWeek( ), 1, &ok );
    TESTCHECK( hebDate.ToString( ), string( "Yom Shaynee, 7 Kislev 5706" ), &ok );
    int incr = 40;
    cout << "Increment(" << incr << ") :" << endl;
    hebDate.Increment( incr );
    TESTCHECK( hebDate.JulianDay( ), 2431812, &ok );
    TESTCHECK( hebDate.ToString( ), string( "Shabbat, 18 Teveth 5706" ), &ok );
    incr = 13;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    hebDate.Increment( 0, incr, 0 );
    TESTCHECK( hebDate.JulianDay( ), 2432196, &ok );
    TESTCHECK( hebDate.ToString( ), string( "Yom Ha'shee'shee, 18 Teveth 5707" ), &ok );

    cout << "HebrewDate( true ) [today constructor]" << endl;
    HebrewDate hebToday( true );
    TESTCHECK( hebToday.Valid( ), true, &ok );
    cout << "hebToday.JulianDay( )=" << hebToday.JulianDay( );
    if ( (hebToday.JulianDay( ) > 2451545) && (hebToday.JulianDay( ) < 2500000) )
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (hebDate == hebToday), false, &ok );
    TESTCHECK( (hebDate < hebToday), true, &ok );
    cout << "hebToday.ToString()=" << hebToday.ToString( ) << endl;

    int jd = 2431772;
    cout << "HebrewDate( " << jd << " ) [Julian Day constructor]" << endl;
    HebrewDate hebJD( jd );
    TESTCHECK( hebJD.Valid( ), true, &ok );
    TESTCHECK( (hebJD == hebJD), true, &ok );
    TESTCHECK( (hebJD < hebToday), true, &ok );
    TESTCHECK( (hebJD == hebDate), false, &ok );
    TESTCHECK( hebJD.ToString( ), string( "Yom Shaynee, 7 Kislev 5706" ), &ok );

    d = 8;
    m = 9;
    y = 5706;
    cout << "HebrewDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    HebrewDate hebDMY( d, m, y );
    TESTCHECK( hebDMY.Valid( ), true, &ok );
    TESTCHECK( (hebJD < hebDMY), true, &ok );
    TESTCHECK( (hebDMY < hebToday), true, &ok );
    TESTCHECK( hebDMY.JulianDay( ), 2431773, &ok );
    TESTCHECK( hebDMY.ToString( ), string( "Yom Shlee'shee, 8 Kislev 5706" ), &ok );

    jd = 347820;
    cout << "HebrewDate( " << jd << " ) [Julian Day constructor]" << endl;
    HebrewDate heb0( jd );
    TESTCHECK( heb0.Valid( ), true, &ok );
    TESTCHECK( (heb0 == heb0), true, &ok );
    TESTCHECK( (heb0 < hebDate), true, &ok );
    TESTCHECK( (heb0 == hebDate), false, &ok );
    TESTCHECK( heb0.ToString( ), string( "Yom Ha'shee'shee, 29 Adar II 0" ), &ok );

    d = 1; m = 1; y = -1;
    cout << "HebrewDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    HebrewDate heb110( d, m, y );
    TESTCHECK( heb110.Valid( ), true, &ok );
    TESTCHECK( (heb110 < hebJD), true, &ok );
    TESTCHECK( (heb110 < hebToday), true, &ok );
    TESTCHECK( heb110.JulianDay( ), 347437, &ok );
    TESTCHECK( heb110.ToString( ), string( "Yom Reeshone, 1 Nisan -1" ), &ok );

    cout << "HebrewDate( heb0 ) [copy constructor]" << endl;
    HebrewDate hebEq = heb0;
    TESTCHECK( hebEq.Valid( ), true, &ok );
    TESTCHECK( (hebEq == heb0), true, &ok );
    TESTCHECK( hebEq.ToString( ), string( "Yom Ha'shee'shee, 29 Adar II 0" ), &ok );
    cout << "= heb110 [assignment]" << endl;
    hebEq = heb110;
    TESTCHECK( hebEq.Valid( ), true, &ok );
    TESTCHECK( (hebEq == heb0), false, &ok );
    TESTCHECK( (hebEq == heb110), true, &ok );
    TESTCHECK( heb110.ToString( ), string( "Yom Reeshone, 1 Nisan -1" ), &ok );

    cout << "HebrewDate( GregorianDate( 7, 9, -3760 ) ) [DateDMY constructor]" << endl;
    HebrewDate hebGreg( GregorianDate( 7, 9, -3760 ) );
    TESTCHECK( hebGreg.Valid( ), true, &ok );
    TESTCHECK( hebGreg.ToString( ), string( "Yom Shaynee, 1 Tishri 1" ), &ok );
    cout << "= GregorianDate( 12, 11, 1945 )" << endl;
    hebGreg = GregorianDate( 12, 11, 1945 );
    TESTCHECK( hebGreg.Valid( ), true, &ok );
    TESTCHECK( hebGreg.ToString( ), string( "Yom Shaynee, 7 Kislev 5706" ), &ok );
    
    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDates[]
            = {
                { 1507232,  10,  5, 3174 },
                { 1660038,  25,  9, 3593 },
                { 1746894,   3,  7, 3831 },
                { 1770642,   9,  7, 3896 },
                { 1892732,  18, 10, 4230 },
                { 1931580,   4,  3, 4336 },
                { 1974852,  13,  8, 4455 },
                { 2091165,   6,  2, 4773 },
                { 2121510,  23,  2, 4856 },
                { 2155780,   7,  1, 4950 },
                { 2174030,   8, 13, 5000 },
                { 2191585,  21,  1, 5048 },
                { 2195262,   7,  2, 5058 },
                { 2229275,   1,  4, 5151 },
                { 2245581,   7, 11, 5196 },
                { 2266101,   3,  1, 5252 },
                { 2288543,   1,  7, 5314 },
                { 2290902,  27, 12, 5320 },
                { 2323141,  20,  3, 5408 },
                { 2334849,   3,  4, 5440 },
                { 2348021,   5,  5, 5476 },
                { 2366979,   4,  4, 5528 },
                { 2385649,  11,  5, 5579 },
                { 2392826,  12,  1, 5599 },
                { 2416224,  22,  1, 5663 },
                { 2425849,  19,  5, 5689 },
                { 2430267,   8,  7, 5702 },
                { 2430834,  14,  1, 5703 },
                { 2431005,   8,  7, 5704 },
                { 2448699,  12, 13, 5752 },
                { 2450139,   5, 12, 5756 },
                { 2465738,  12,  8, 5799 },
                { 2486077,   5,  5, 5854 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        hebDate.Set( d, m, y );
        TESTCHECK( hebDate.Valid( ), true, &ok );
        TESTCHECK( hebDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        hebDate.Set( jd );
        TESTCHECK( hebDate.Valid( ), true, &ok );
        TESTCHECK( hebDate.Day( ), d, &ok );
        TESTCHECK( hebDate.Month( ), m, &ok );
        TESTCHECK( hebDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "HebrewDate PASSED." << endl << endl;
    else
        cout << "HebrewDate FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  IslamicDate.cpp
  Copyright (C) 2007 David M. Anderson

  IslamicDate class, representing dates in the Islamic calendar.
*/


#include "IslamicDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "GregorianDate.hpp"
#include "Array.hpp"
#include "AngleDMS.hpp"
#include "LunarVisibility.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool
TestIslamicDate( )
{
    bool ok = true;
    cout << "Testing IslamicDate" << endl;

    cout << "pArithmeticSystem = new IslamicCalendar::ArithmeticSystem( "
            "IslamicCalendar::ArithmeticSystem::Nizari )" << endl;
    shared_ptr< IslamicCalendar::ArithmeticSystem > pArithmeticSystem( 
        new IslamicCalendar::ArithmeticSystem(
            IslamicCalendar::ArithmeticSystem::Nizari ) );
    cout << "IslamicCalendar::SetSystem( pArithmeticSystem )" << endl;
    IslamicCalendar::SetSystem( pArithmeticSystem );

    cout << "IslamicDate() [default constructor]" << endl;
    IslamicDate islDate;
    TESTCHECK( islDate.Valid( ), true, &ok );
    TESTCHECK( islDate.Day( ), 1, &ok );
    TESTCHECK( islDate.Month( ), 1, &ok );
    TESTCHECK( islDate.Year( ), 1, &ok );
    TESTCHECK( islDate.JulianDay( ), 1948440, &ok );
    TESTCHECK( islDate.DayOfWeek( ), 5, &ok );
    TESTCHECK( islDate.ToString( ), string( "yawm al-jum`a, 1 Muharram 1" ), &ok );

    int d = 6;
    int m = 12;
    int y = 1364;
    cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
    islDate.Set( d, m, y );
    TESTCHECK( islDate.Valid( ), true, &ok );
    TESTCHECK( islDate.Day( ), d, &ok );
    TESTCHECK( islDate.Month( ), m, &ok );
    TESTCHECK( islDate.Year( ), y, &ok );
    TESTCHECK( islDate.JulianDay( ), 2431772, &ok );
    TESTCHECK( islDate.DayOfWeek( ), 1, &ok );
    TESTCHECK( islDate.ToString( ), string( "yawm al-'ithnayn, 6 Dhu al-Hijja 1364" ), &ok );
    int incr = 40;
    cout << "Increment(" << incr << ") :" << endl;
    islDate.Increment( incr );
    TESTCHECK( islDate.JulianDay( ), 2431812, &ok );
    TESTCHECK( islDate.ToString( ), string( "yawm as-sabt, 17 Muharram 1365" ), &ok );
    incr = 12;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    islDate.Increment( 0, incr, 0 );
    TESTCHECK( islDate.JulianDay( ), 2432166, &ok );
    TESTCHECK( islDate.ToString( ), string( "yawm al-'arba`a', 17 Muharram 1366" ), &ok );

    cout << "IslamicDate( true ) [today constructor]" << endl;
    IslamicDate islToday( true );
    TESTCHECK( islToday.Valid( ), true, &ok );
    cout << "islToday.JulianDay( )=" << islToday.JulianDay( );
    if ( (islToday.JulianDay( ) > 2451545) && (islToday.JulianDay( ) < 2500000) )
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (islDate == islToday), false, &ok );
    TESTCHECK( (islDate < islToday), true, &ok );
    cout << "islToday.ToString()=" << islToday.ToString( ) << endl;

    int jd = 2431772;
    cout << "IslamicDate( " << jd << " ) [Julian Day constructor]" << endl;
    IslamicDate islJD( jd );
    TESTCHECK( islJD.Valid( ), true, &ok );
    TESTCHECK( (islJD == islJD), true, &ok );
    TESTCHECK( (islJD < islToday), true, &ok );
    TESTCHECK( (islJD == islDate), false, &ok );
    TESTCHECK( islJD.ToString( ), string( "yawm al-'ithnayn, 6 Dhu al-Hijja 1364" ), &ok );

    d = 7;
    m = 12;
    y = 1364;
    cout << "IslamicDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    IslamicDate islDMY( d, m, y );
    TESTCHECK( islDMY.Valid( ), true, &ok );
    TESTCHECK( (islJD < islDMY), true, &ok );
    TESTCHECK( (islDMY < islToday), true, &ok );
    TESTCHECK( islDMY.JulianDay( ), 2431773, &ok );
    TESTCHECK( islDMY.ToString( ), string( "yawm ath-thalatha', 7 Dhu al-Hijja 1364" ), &ok );

    jd = 1948439;
    cout << "IslamicDate( " << jd << " ) [Julian Day constructor]" << endl;
    IslamicDate isl0( jd );
    TESTCHECK( isl0.Valid( ), true, &ok );
    TESTCHECK( (isl0 == isl0), true, &ok );
    TESTCHECK( (isl0 < islDate), true, &ok );
    TESTCHECK( (isl0 == islDate), false, &ok );
    TESTCHECK( isl0.ToString( ), string( "yawm al-khamis, 29 Dhu al-Hijja 0" ), &ok );

    d = 1; m = 1; y = 0;
    cout << "IslamicDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    IslamicDate isl110( d, m, y );
    TESTCHECK( isl110.Valid( ), true, &ok );
    TESTCHECK( (isl110 < islJD), true, &ok );
    TESTCHECK( (isl110 < islToday), true, &ok );
    TESTCHECK( isl110.JulianDay( ), 1948086, &ok );
    TESTCHECK( isl110.ToString( ), string( "yawm al-'ithnayn, 1 Muharram 0" ), &ok );

    cout << "IslamicDate( isl0 ) [copy constructor]" << endl;
    IslamicDate islEq = isl0;
    TESTCHECK( islEq.Valid( ), true, &ok );
    TESTCHECK( (islEq == isl0), true, &ok );
    TESTCHECK( islEq.ToString( ), string( "yawm al-khamis, 29 Dhu al-Hijja 0" ), &ok );
    cout << "= isl110 [assignment]" << endl;
    islEq = isl110;
    TESTCHECK( islEq.Valid( ), true, &ok );
    TESTCHECK( (islEq == isl0), false, &ok );
    TESTCHECK( (islEq == isl110), true, &ok );
    TESTCHECK( isl110.ToString( ), string( "yawm al-'ithnayn, 1 Muharram 0" ), &ok );

    cout << "IslamicDate( GregorianDate( 19, 7, 622 ) ) [DateDMY constructor]" << endl;
    IslamicDate islGreg( GregorianDate( 19, 7, 622 ) );
    TESTCHECK( islGreg.Valid( ), true, &ok );
    TESTCHECK( islGreg.ToString( ), string( "yawm al-jum`a, 1 Muharram 1" ), &ok );
    cout << "= GregorianDate( 12, 11, 1945 )" << endl;
    islGreg = GregorianDate( 12, 11, 1945 );
    TESTCHECK( islGreg.Valid( ), true, &ok );
    TESTCHECK( islGreg.ToString( ), string( "yawm al-'ithnayn, 6 Dhu al-Hijja 1364" ), &ok );
    
    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDatesArithmetic[]
            = {
                { 1507232,   9, 12,-1245 },
                { 1660038,  23,  2, -813 },
                { 1746894,   1,  4, -568 },
                { 1770642,   6,  4, -501 },
                { 1892732,  17, 10, -157 },
                { 1931580,   3,  6,  -47 },
                { 1974852,  13,  7,   75 },
                { 2091165,   5, 10,  403 },
                { 2121510,  22,  5,  489 },
                { 2155780,   7,  2,  586 },
                { 2174030,   7,  8,  637 },
                { 2191585,  20,  2,  687 },
                { 2195262,   7,  7,  697 },
                { 2229275,   1,  7,  793 },
                { 2245581,   6,  7,  839 },
                { 2266101,   1,  6,  897 },
                { 2288543,  30,  9,  960 },
                { 2290902,  27,  5,  967 },
                { 2323141,  18,  5, 1058 },
                { 2334849,   2,  6, 1091 },
                { 2348021,   4,  8, 1128 },
                { 2366979,   3,  2, 1182 },
                { 2385649,  10, 10, 1234 },
                { 2392826,  11,  1, 1255 },
                { 2416224,  21,  1, 1321 },
                { 2425849,  19,  3, 1348 },
                { 2430267,   8,  9, 1360 },
                { 2430834,  13,  4, 1362 },
                { 2431005,   7, 10, 1362 },
                { 2448699,  13,  9, 1412 },
                { 2450139,   5, 10, 1416 },
                { 2465738,  12, 10, 1460 },
                { 2486077,   5,  3, 1518 }
            };

    for ( int i = 0; i < ARRAYSIZE( testDatesArithmetic ); ++i )
    {
        jd = testDatesArithmetic[i].julianDay;
        d = testDatesArithmetic[i].day;
        m = testDatesArithmetic[i].month;
        y = testDatesArithmetic[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        islDate.Set( d, m, y );
        TESTCHECK( islDate.Valid( ), true, &ok );
        TESTCHECK( islDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        islDate.Set( jd );
        TESTCHECK( islDate.Valid( ), true, &ok );
        TESTCHECK( islDate.Day( ), d, &ok );
        TESTCHECK( islDate.Month( ), m, &ok );
        TESTCHECK( islDate.Year( ), y, &ok );
    }

    GeodeticLocation cairo( Angle( 31.3, Angle::Degree ),
                            Angle( 30.3, Angle::Degree ), 200 );
    cout << "spLocalMonthFunc = new IslamicCalendar::LocalMonthFunc( "
            "LunarVisibility::CheckNext, cairo )" << endl;
    shared_ptr< IslamicCalendar::LocalMonthFunc > spLocalMonthFunc( 
        new IslamicCalendar::LocalMonthFunc( LunarVisibility::CheckNext,
                                             cairo ) );
    cout << "LunarVisibility::SetCriterion( LunarVisibility::Shaukat )"
         << endl;
    LunarVisibility::SetCriterion( LunarVisibility::Shaukat );
    cout << "spAstronomicalSystem "
            "= new IslamicCalendar::AstronomicalSystem( spLocalMonthFunc )"
         << endl;
    shared_ptr< IslamicCalendar::AstronomicalSystem > spAstronomicalSystem( 
        new IslamicCalendar::AstronomicalSystem( spLocalMonthFunc ) );
    cout << "IslamicCalendar::SetSystem( spAstronomicalSystem )" << endl;
    IslamicCalendar::SetSystem( spAstronomicalSystem );
    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDatesShaukat1996[]
            = {
//!!! My calculations result in a 1-day difference from Reingold & Dershowitz.
//!!!                { 1507232,  11, 12,-1245 },
//!!!                { 1660038,  25,  2, -813 },
                { 1746894,   2,  4, -568 },
                { 1770642,   7,  4, -501 },
                { 1892732,  18, 10, -157 },
                { 1931580,   3,  6,  -47 },
                { 1974852,  13,  7,   75 },
                { 2091165,   5, 10,  403 },
                { 2121510,  22,  5,  489 },
                { 2155780,   7,  2,  586 },
                { 2174030,   7,  8,  637 },
                { 2191585,  21,  2,  687 },
                { 2195262,   7,  7,  697 },
                { 2229275,  30,  6,  793 },
                { 2245581,   6,  7,  839 },
                { 2266101,   2,  6,  897 },
                { 2288543,  30,  9,  960 },
                { 2290902,  27,  5,  967 },
                { 2323141,  18,  5, 1058 },
                { 2334849,   3,  6, 1091 },
                { 2348021,   4,  8, 1128 },
                { 2366979,   4,  2, 1182 },
                { 2385649,  10, 10, 1234 },
                { 2392826,  11,  1, 1255 },
                { 2416224,  20,  1, 1321 },
                { 2425849,  19,  3, 1348 },
                { 2430267,   7,  9, 1360 },
                { 2430834,  14,  4, 1362 },
                { 2431005,   7, 10, 1362 },
                { 2448699,  12,  9, 1412 },
                { 2450139,   5, 10, 1416 },
                { 2465738,  12, 10, 1460 },
                { 2486077,   5,  3, 1518 }
            };

    for ( int i = 0; i < ARRAYSIZE( testDatesShaukat1996 ); ++i )
    {
        jd = testDatesShaukat1996[i].julianDay;
        d = testDatesShaukat1996[i].day;
        m = testDatesShaukat1996[i].month;
        y = testDatesShaukat1996[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        islDate.Set( d, m, y );
        TESTCHECK( islDate.Valid( ), true, &ok );
        TESTCHECK( islDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        islDate.Set( jd );
        TESTCHECK( islDate.Valid( ), true, &ok );
        TESTCHECK( islDate.Day( ), d, &ok );
        TESTCHECK( islDate.Month( ), m, &ok );
        TESTCHECK( islDate.Year( ), y, &ok );
    }

    cout << "spLocalMonthFunc->SetVisibilityFunc( "
            "IslamicCalendar::UmmAlQuraVisibility )" << endl;
    spLocalMonthFunc->SetVisibilityFunc(
        IslamicCalendar::UmmAlQuraVisibility );
    cout << "spLocalMonthfunc->SetLocation( Mecca )" << endl;
    spLocalMonthFunc->SetLocation( Mecca );
    struct
    {
        int id;
        int im;
        int iy;
        int gd;
        int gm;
        int gy;
    }
    testDatesUmmAlQura[]
            = {
                {  1,  1, 1420, 17,  4, 1999 },
                {  1,  1, 1421,  6,  4, 2000 },
                {  1,  1, 1422, 26,  3, 2001 },
                {  1,  1, 1423, 15,  3, 2002 },
                {  1,  1, 1424,  4,  3, 2003 },
                {  1,  1, 1425, 21,  2, 2004 },
                {  1,  1, 1426, 10,  2, 2005 },
                {  1,  1, 1427, 31,  1, 2006 },
                {  1,  1, 1428, 20,  1, 2007 },
                {  1,  1, 1429, 10,  1, 2008 },
                {  1,  1, 1430, 29, 12, 2008 },
                {  1,  1, 1431, 18, 12, 2009 },
                {  1,  1, 1432,  7, 12, 2010 },
                {  1,  1, 1433, 26, 11, 2011 },
                {  1,  1, 1434, 15, 11, 2012 },
                {  1,  1, 1435,  4, 11, 2013 },
                {  1,  1, 1436, 25, 10, 2014 },
                {  1,  1, 1437, 14, 10, 2015 },
                {  1,  1, 1438,  2, 10, 2016 },
                {  1,  1, 1439, 21,  9, 2017 },
                {  1,  1, 1440, 11,  9, 2018 },
                {  1,  1, 1441, 31,  8, 2019 },
                {  1,  1, 1442, 20,  8, 2020 },
                {  1,  1, 1443,  9,  8, 2021 },
                {  1,  1, 1444, 30,  7, 2022 },
                {  1,  1, 1445, 19,  7, 2023 },
                {  1,  1, 1446,  7,  7, 2024 },
                {  1,  1, 1447, 26,  6, 2025 },
                {  1,  1, 1448, 16,  6, 2026 },
                {  1,  1, 1449,  6,  6, 2027 },
                {  1,  1, 1450, 25,  5, 2028 },
                { 12,  3, 1420, 26,  6, 1999 },
                { 12,  3, 1421, 14,  6, 2000 },
                { 12,  3, 1422,  4,  6, 2001 },
                { 12,  3, 1423, 24,  5, 2002 },
                { 12,  3, 1424, 13,  5, 2003 },
                { 12,  3, 1425,  1,  5, 2004 },
                { 12,  3, 1426, 21,  4, 2005 },
                { 12,  3, 1427, 10,  4, 2006 },
                { 12,  3, 1428, 31,  3, 2007 },
                { 12,  3, 1429, 20,  3, 2008 },
                { 12,  3, 1430,  9,  3, 2009 },
                { 12,  3, 1431, 26,  2, 2010 },
                { 12,  3, 1432, 15,  2, 2011 },
                { 12,  3, 1433,  4,  2, 2012 },
                { 12,  3, 1434, 24,  1, 2013 },
                { 12,  3, 1435, 13,  1, 2014 },
                { 12,  3, 1436,  3,  1, 2015 },
                { 12,  3, 1437, 23, 12, 2015 },
                { 12,  3, 1438, 11, 12, 2016 },
                { 12,  3, 1439, 30, 11, 2017 },
                { 12,  3, 1440, 20, 11, 2018 },
                { 12,  3, 1441,  9, 11, 2019 },
                { 12,  3, 1442, 29, 10, 2020 },
                { 12,  3, 1443, 18, 10, 2021 },
                { 12,  3, 1444,  8, 10, 2022 },
                { 12,  3, 1445, 27,  9, 2023 },
                { 12,  3, 1446, 15,  9, 2024 },
                { 12,  3, 1447,  4,  9, 2025 },
                { 12,  3, 1448, 25,  8, 2026 },
                { 12,  3, 1449, 14,  8, 2027 },
                { 12,  3, 1450,  3,  8, 2028 },
                {  1,  9, 1420,  9, 12, 1999 },
                {  1,  9, 1421, 27, 11, 2000 },
                {  1,  9, 1422, 16, 11, 2001 },
                {  1,  9, 1423,  6, 11, 2002 },
                {  1,  9, 1424, 26, 10, 2003 },
                {  1,  9, 1425, 15, 10, 2004 },
                {  1,  9, 1426,  4, 10, 2005 },
                {  1,  9, 1427, 24,  9, 2006 },
                {  1,  9, 1428, 13,  9, 2007 },
                {  1,  9, 1429,  1,  9, 2008 },
                {  1,  9, 1430, 22,  8, 2009 },
                {  1,  9, 1431, 11,  8, 2010 },
                {  1,  9, 1432,  1,  8, 2011 },
                {  1,  9, 1433, 20,  7, 2012 },
                {  1,  9, 1434,  9,  7, 2013 },
                {  1,  9, 1435, 28,  6, 2014 },
                {  1,  9, 1436, 18,  6, 2015 },
                {  1,  9, 1437,  6,  6, 2016 },
                {  1,  9, 1438, 27,  5, 2017 },
                {  1,  9, 1439, 16,  5, 2018 },
                {  1,  9, 1440,  6,  5, 2019 },
                {  1,  9, 1441, 24,  4, 2020 },
                {  1,  9, 1442, 13,  4, 2021 },
                {  1,  9, 1443,  2,  4, 2022 },
                {  1,  9, 1444, 23,  3, 2023 },
                {  1,  9, 1445, 11,  3, 2024 },
                {  1,  9, 1446,  1,  3, 2025 },
                {  1,  9, 1447, 18,  2, 2026 },
                {  1,  9, 1448,  8,  2, 2027 },
                {  1,  9, 1449, 28,  1, 2028 },
                {  1,  9, 1450, 16,  1, 2029 },
                {  1, 10, 1420,  8,  1, 2000 },
                {  1, 10, 1421, 27, 12, 2000 },
                {  1, 10, 1422, 16, 12, 2001 },
                {  1, 10, 1423,  5, 12, 2002 },
                {  1, 10, 1424, 25, 11, 2003 },
                {  1, 10, 1425, 14, 11, 2004 },
                {  1, 10, 1426,  3, 11, 2005 },
                {  1, 10, 1427, 23, 10, 2006 },
                {  1, 10, 1428, 13, 10, 2007 },
                {  1, 10, 1429,  1, 10, 2008 },
                {  1, 10, 1430, 20,  9, 2009 },
                {  1, 10, 1431, 10,  9, 2010 },
                {  1, 10, 1432, 30,  8, 2011 },
                {  1, 10, 1433, 19,  8, 2012 },
                {  1, 10, 1434,  8,  8, 2013 },
                {  1, 10, 1435, 28,  7, 2014 },
                {  1, 10, 1436, 17,  7, 2015 },
                {  1, 10, 1437,  6,  7, 2016 },
                {  1, 10, 1438, 25,  6, 2017 },
                {  1, 10, 1439, 15,  6, 2018 },
                {  1, 10, 1440,  4,  6, 2019 },
                {  1, 10, 1441, 24,  5, 2020 },
                {  1, 10, 1442, 13,  5, 2021 },
                {  1, 10, 1443,  2,  5, 2022 },
                {  1, 10, 1444, 21,  4, 2023 },
                {  1, 10, 1445, 10,  4, 2024 },
                {  1, 10, 1446, 30,  3, 2025 },
                {  1, 10, 1447, 20,  3, 2026 },
                {  1, 10, 1448,  9,  3, 2027 },
                {  1, 10, 1449, 26,  2, 2028 },
                {  1, 10, 1450, 14,  2, 2029 },
                { 10, 12, 1420, 16,  3, 2000 },
                { 10, 12, 1421,  5,  3, 2001 },
                { 10, 12, 1422, 22,  2, 2002 },
                { 10, 12, 1423, 11,  2, 2003 },
                { 10, 12, 1424,  1,  2, 2004 },
                { 10, 12, 1425, 21,  1, 2005 },
                { 10, 12, 1426, 10,  1, 2006 },
                { 10, 12, 1427, 31, 12, 2006 },
                { 10, 12, 1428, 20, 12, 2007 },
                { 10, 12, 1429,  8, 12, 2008 },
                { 10, 12, 1430, 27, 11, 2009 },
                { 10, 12, 1431, 16, 11, 2010 },
                { 10, 12, 1432,  6, 11, 2011 },
                { 10, 12, 1433, 26, 10, 2012 },
                { 10, 12, 1434, 15, 10, 2013 },
                { 10, 12, 1435,  4, 10, 2014 },
                { 10, 12, 1436, 23,  9, 2015 },
                { 10, 12, 1437, 11,  9, 2016 },
                { 10, 12, 1438,  1,  9, 2017 },
                { 10, 12, 1439, 21,  8, 2018 },
                { 10, 12, 1440, 11,  8, 2019 },
                { 10, 12, 1441, 31,  7, 2020 },
                { 10, 12, 1442, 20,  7, 2021 },
                { 10, 12, 1443,  9,  7, 2022 },
                { 10, 12, 1444, 28,  6, 2023 },
                { 10, 12, 1445, 16,  6, 2024 },
                { 10, 12, 1446,  6,  6, 2025 },
                { 10, 12, 1447, 27,  5, 2026 },
                { 10, 12, 1448, 16,  5, 2027 },
                { 10, 12, 1449,  5,  5, 2028 },
                { 10, 12, 1450, 24,  4, 2029 }
            };
    for ( int i = 0; i < ARRAYSIZE( testDatesUmmAlQura ); ++i )
    {
        int id = testDatesUmmAlQura[i].id;
        int im = testDatesUmmAlQura[i].im;
        int iy = testDatesUmmAlQura[i].iy;
        int gd = testDatesUmmAlQura[i].gd;
        int gm = testDatesUmmAlQura[i].gm;
        int gy = testDatesUmmAlQura[i].gy;
        cout << "Set( " << id << ", " << im << ", " << iy << " ) :" << endl;
        islDate.Set( id, im, iy );
        TESTCHECK( islDate.Valid( ), true, &ok );
        cout << "GregorianDate( " << gd << ", " << gm << ", " << gy << " )"
             << endl;
        GregorianDate gregDate( gd, gm, gy );
        jd = gregDate.JulianDay( );
        TESTCHECK( islDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " )" << endl;
        islDate.Set( jd );
        TESTCHECK( islDate.Day(), id, &ok );
        TESTCHECK( islDate.Month(), im, &ok );
        TESTCHECK( islDate.Year(), iy, &ok );
    }

    cout << "pISNA_Hijri = new IslamicCalendar::ISNA_Hijri" << endl;
    shared_ptr< IslamicCalendar::ISNA_Hijri > spISNA_Hijri( 
        new IslamicCalendar::ISNA_Hijri );
    cout << "spAstronomicalSystem->SetMonthFunc( pISNA_Hijri )" << endl;
    spAstronomicalSystem->SetMonthFunc( spISNA_Hijri );
    struct
    {
        int id;
        int im;
        int iy;
        int gd;
        int gm;
        int gy;
    }
    testDatesISNA_Hijri[]
            = {
                {  1,  1, 1420, 17,  4, 1999 },
                {  1,  1, 1421,  6,  4, 2000 },
                {  1,  1, 1422, 26,  3, 2001 },
                {  1,  1, 1423, 15,  3, 2002 },
                {  1,  1, 1424,  4,  3, 2003 },
                {  1,  1, 1425, 21,  2, 2004 },
                {  1,  1, 1426, 10,  2, 2005 },
                {  1,  1, 1427, 31,  1, 2006 },
                {  1,  1, 1428, 20,  1, 2007 },
                {  1,  1, 1429,  9,  1, 2008 },
                {  1,  1, 1430, 29, 12, 2008 },
                {  1,  1, 1431, 18, 12, 2009 },
                {  1,  1, 1432,  7, 12, 2010 },
                {  1,  1, 1433, 26, 11, 2011 },
                {  1,  1, 1434, 15, 11, 2012 },
                {  1,  1, 1435,  5, 11, 2013 },
                {  1,  1, 1436, 25, 10, 2014 },
                {  1,  1, 1437, 14, 10, 2015 },
                {  1,  1, 1438,  2, 10, 2016 },
                {  1,  1, 1439, 21,  9, 2017 },
                {  1,  1, 1440, 11,  9, 2018 },
                {  1,  1, 1441, 31,  8, 2019 },
                {  1,  1, 1442, 20,  8, 2020 },
                {  1,  1, 1443, 10,  8, 2021 },
                {  1,  1, 1444, 30,  7, 2022 },
                {  1,  1, 1445, 19,  7, 2023 },
                {  1,  1, 1446,  7,  7, 2024 },
                {  1,  1, 1447, 26,  6, 2025 },
                {  1,  1, 1448, 16,  6, 2026 },
                {  1,  1, 1449,  6,  6, 2027 },
                {  1,  1, 1450, 25,  5, 2028 },
                { 12,  3, 1420, 26,  6, 1999 },
                { 12,  3, 1421, 15,  6, 2000 },
                { 12,  3, 1422,  4,  6, 2001 },
                { 12,  3, 1423, 24,  5, 2002 },
                { 12,  3, 1424, 14,  5, 2003 },
                { 12,  3, 1425,  2,  5, 2004 },
                { 12,  3, 1426, 21,  4, 2005 },
                { 12,  3, 1427, 10,  4, 2006 },
                { 12,  3, 1428, 31,  3, 2007 },
                { 12,  3, 1429, 20,  3, 2008 },
                { 12,  3, 1430,  9,  3, 2009 },
                { 12,  3, 1431, 26,  2, 2010 },
                { 12,  3, 1432, 15,  2, 2011 },
                { 12,  3, 1433,  4,  2, 2012 },
                { 12,  3, 1434, 24,  1, 2013 },
                { 12,  3, 1435, 13,  1, 2014 },
                { 12,  3, 1436,  3,  1, 2015 },
                { 12,  3, 1437, 23, 12, 2015 },
                { 12,  3, 1438, 12, 12, 2016 },
                { 12,  3, 1439, 30, 11, 2017 },
                { 12,  3, 1440, 20, 11, 2018 },
                { 12,  3, 1441,  9, 11, 2019 },
                { 12,  3, 1442, 29, 10, 2020 },
                { 12,  3, 1443, 18, 10, 2021 },
                { 12,  3, 1444,  8, 10, 2022 },
                { 12,  3, 1445, 27,  9, 2023 },
                { 12,  3, 1446, 15,  9, 2024 },
                { 12,  3, 1447,  4,  9, 2025 },
                { 12,  3, 1448, 25,  8, 2026 },
                { 12,  3, 1449, 14,  8, 2027 },
                { 12,  3, 1450,  3,  8, 2028 },
                {  1,  9, 1420,  9, 12, 1999 },
                {  1,  9, 1421, 27, 11, 2000 },
                {  1,  9, 1422, 16, 11, 2001 },
                {  1,  9, 1423,  6, 11, 2002 },
                {  1,  9, 1424, 27, 10, 2003 },
                {  1,  9, 1425, 15, 10, 2004 },
                {  1,  9, 1426,  4, 10, 2005 },
                {  1,  9, 1427, 23,  9, 2006 },
                {  1,  9, 1428, 13,  9, 2007 },
                {  1,  9, 1429,  1,  9, 2008 },
                {  1,  9, 1430, 21,  8, 2009 },
                {  1,  9, 1431, 11,  8, 2010 },
                {  1,  9, 1432,  1,  8, 2011 },
                {  1,  9, 1433, 20,  7, 2012 },
                {  1,  9, 1434,  9,  7, 2013 },
                {  1,  9, 1435, 28,  6, 2014 },
                {  1,  9, 1436, 18,  6, 2015 },
                {  1,  9, 1437,  6,  6, 2016 },
                {  1,  9, 1438, 27,  5, 2017 },
                {  1,  9, 1439, 16,  5, 2018 },
                {  1,  9, 1440,  6,  5, 2019 },
                {  1,  9, 1441, 24,  4, 2020 },
                {  1,  9, 1442, 13,  4, 2021 },
                {  1,  9, 1443,  2,  4, 2022 },
                {  1,  9, 1444, 23,  3, 2023 },
                {  1,  9, 1445, 11,  3, 2024 },
                {  1,  9, 1446,  1,  3, 2025 },
                {  1,  9, 1447, 19,  2, 2026 },
                {  1,  9, 1448,  8,  2, 2027 },
                {  1,  9, 1449, 28,  1, 2028 },
                {  1,  9, 1450, 16,  1, 2029 },
                {  1, 10, 1420,  8,  1, 2000 },
                {  1, 10, 1421, 27, 12, 2000 },
                {  1, 10, 1422, 16, 12, 2001 },
                {  1, 10, 1423,  5, 12, 2002 },
                {  1, 10, 1424, 25, 11, 2003 },
                {  1, 10, 1425, 14, 11, 2004 },
                {  1, 10, 1426,  3, 11, 2005 },
                {  1, 10, 1427, 23, 10, 2006 },
                {  1, 10, 1428, 12, 10, 2007 },
                {  1, 10, 1429, 30,  9, 2008 },
                {  1, 10, 1430, 20,  9, 2009 },
                {  1, 10, 1431,  9,  9, 2010 },
                {  1, 10, 1432, 30,  8, 2011 },
                {  1, 10, 1433, 19,  8, 2012 },
                {  1, 10, 1434,  8,  8, 2013 },
                {  1, 10, 1435, 28,  7, 2014 },
                {  1, 10, 1436, 17,  7, 2015 },
                {  1, 10, 1437,  5,  7, 2016 },
                {  1, 10, 1438, 25,  6, 2017 },
                {  1, 10, 1439, 15,  6, 2018 },
                {  1, 10, 1440,  4,  6, 2019 },
                {  1, 10, 1441, 24,  5, 2020 },
                {  1, 10, 1442, 13,  5, 2021 },
                {  1, 10, 1443,  2,  5, 2022 },
                {  1, 10, 1444, 21,  4, 2023 },
                {  1, 10, 1445, 10,  4, 2024 },
                {  1, 10, 1446, 30,  3, 2025 },
                {  1, 10, 1447, 20,  3, 2026 },
                {  1, 10, 1448,  9,  3, 2027 },
                {  1, 10, 1449, 26,  2, 2028 },
                {  1, 10, 1450, 14,  2, 2029 },
                { 10, 12, 1420, 16,  3, 2000 },
                { 10, 12, 1421,  5,  3, 2001 },
                { 10, 12, 1422, 22,  2, 2002 },
                { 10, 12, 1423, 11,  2, 2003 },
                { 10, 12, 1424,  1,  2, 2004 },
                { 10, 12, 1425, 21,  1, 2005 },
                { 10, 12, 1426, 10,  1, 2006 },
                { 10, 12, 1427, 31, 12, 2006 },
                { 10, 12, 1428, 20, 12, 2007 },
                { 10, 12, 1429,  8, 12, 2008 },
                { 10, 12, 1430, 27, 11, 2009 },
                { 10, 12, 1431, 16, 11, 2010 },
                { 10, 12, 1432,  6, 11, 2011 },
                { 10, 12, 1433, 26, 10, 2012 },
                { 10, 12, 1434, 15, 10, 2013 },
                { 10, 12, 1435,  4, 10, 2014 },
                { 10, 12, 1436, 23,  9, 2015 },
                { 10, 12, 1437, 11,  9, 2016 },
                { 10, 12, 1438,  1,  9, 2017 },
                { 10, 12, 1439, 21,  8, 2018 },
                { 10, 12, 1440, 11,  8, 2019 },
                { 10, 12, 1441, 31,  7, 2020 },
                { 10, 12, 1442, 20,  7, 2021 },
                { 10, 12, 1443,  9,  7, 2022 },
                { 10, 12, 1444, 28,  6, 2023 },
                { 10, 12, 1445, 17,  6, 2024 },
                { 10, 12, 1446,  6,  6, 2025 },
                { 10, 12, 1447, 27,  5, 2026 },
                { 10, 12, 1448, 16,  5, 2027 },
                { 10, 12, 1449,  5,  5, 2028 },
                { 10, 12, 1450, 24,  4, 2029 }
            };
    for ( int i = 0; i < ARRAYSIZE( testDatesISNA_Hijri ); ++i )
    {
        int id = testDatesISNA_Hijri[i].id;
        int im = testDatesISNA_Hijri[i].im;
        int iy = testDatesISNA_Hijri[i].iy;
        int gd = testDatesISNA_Hijri[i].gd;
        int gm = testDatesISNA_Hijri[i].gm;
        int gy = testDatesISNA_Hijri[i].gy;
        cout << "Set( " << id << ", " << im << ", " << iy << " ) :" << endl;
        islDate.Set( id, im, iy );
        TESTCHECK( islDate.Valid( ), true, &ok );
        cout << "GregorianDate( " << gd << ", " << gm << ", " << gy << " )"
             << endl;
        GregorianDate gregDate( gd, gm, gy );
        jd = gregDate.JulianDay( );
        TESTCHECK( islDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " )" << endl;
        islDate.Set( jd );
        TESTCHECK( islDate.Day(), id, &ok );
        TESTCHECK( islDate.Month(), im, &ok );
        TESTCHECK( islDate.Year(), iy, &ok );
    }
    
    if ( ok )
        cout << "IslamicDate PASSED." << endl << endl;
    else
        cout << "IslamicDate FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

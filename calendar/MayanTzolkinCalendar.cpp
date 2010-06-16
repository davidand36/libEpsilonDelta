/*
  MayanTzolkinCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  MayanTzolkinCalendar class, representing the Mayan tzolkin calendar.
*/


#include "MayanTzolkinCalendar.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


static const long s_mayanTzolkinEpoch = 584124;


//=============================================================================


void 
MayanTzolkinCalendar::JulianDayToTzolkin( long julianDay,
                                          int * pTrecena, int * pVeintena )
{
    long days = julianDay - s_mayanTzolkinEpoch;
    int trecena = (int)ModP( days, 13L ) + 1;
    int veintena = (int)ModP( days, 20L ) + 1;
    if ( pTrecena )
        *pTrecena = trecena;
    if ( pVeintena )
        *pVeintena = veintena;
}

//=============================================================================

const std::string & 
MayanTzolkinCalendar::VeintenaName( int veintena )
{
    Assert( (veintena >= 1) && (veintena <= 20) );
    return g_mayanTzolkinVeintenaNames[ veintena - 1 ];
}

//=============================================================================

#ifdef DEBUG

bool 
MayanTzolkinCalendar::Test( )
{
    bool ok = true;
    cout << "Testing MayanTzolkinCalendar" << endl;

    long jd;
    int t, v;
    struct
    {
        long julianDay;
        int trecena;
        int veintena;
    } 
    testDates[]
            = {
                { 1507232,   5,  9 },
                { 1660038,   9, 15 },
                { 1746894,  12, 11 },
                { 1770642,   9, 19 },
                { 1892732,   3,  9 },
                { 1931580,   7, 17 },
                { 1974852,   2,  9 },
                { 2091165,   4,  2 },
                { 2121510,   7,  7 },
                { 2155780,   9, 17 },
                { 2174030,   7,  7 },
                { 2191585,  12,  2 },
                { 2195262,  10, 19 },
                { 2229275,   2, 12 },
                { 2245581,   6, 18 },
                { 2266101,  12, 18 },
                { 2288543,   3, 20 },
                { 2290902,   9, 19 },
                { 2323141,   8, 18 },
                { 2334849,   3,  6 },
                { 2348021,   6, 18 },
                { 2366979,  10, 16 },
                { 2385649,  12,  6 },
                { 2392826,  13,  3 },
                { 2416224,  11,  1 },
                { 2425849,   3,  6 },
                { 2430267,   1,  4 },
                { 2430834,   9, 11 },
                { 2431005,  11,  2 },
                { 2448699,  12, 16 },
                { 2450139,   9, 16 },
                { 2465738,   8, 15 },
                { 2486077,   2, 14 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        JulianDayToTzolkin( jd, &t, &v );
        cout << "jd = " << jd << endl;
        TESTCHECK( t, testDates[i].trecena, &ok );
        TESTCHECK( v, testDates[i].veintena, &ok );
    }

    if ( ok )
        cout << "MayanTzolkinCalendar PASSED." << endl << endl;
    else
        cout << "MayanTzolkinCalendar FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

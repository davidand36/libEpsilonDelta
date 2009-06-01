/*
  TestCalendarLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta calendar library.
*/


#include <cstdio>
#include <iostream>
#include "Assert.hpp"
#include "TestCheck.hpp"
#include "GregorianDate.hpp"
#include "JulianDate.hpp"
#include "ISO8601Date.hpp"
#include "PersianArithmeticDate.hpp"
#include "PersianAstronomicalDate.hpp"
#include "IslamicDate.hpp"
#include "LunarVisibility.hpp"
#include "HebrewDate.hpp"
#include "CopticDate.hpp"
#include "EthiopianDate.hpp"
#include "BahaiDate.hpp"
#include "BadiDate.hpp"
#include "ChineseDate.hpp"
#include "JPLEphemeris.hpp"
#include "HinduSolarDate.hpp"
#include "HinduLunisolarDate.hpp"
#include "OldHinduSolarDate.hpp"
#include "OldHinduLunisolarDate.hpp"
#include "EgyptianDate.hpp"
#include "ArmenianDate.hpp"
#include "MayanLongCountDate.hpp"
#include "MayanHaabDate.hpp"
#include "MayanTzolkinCalendar.hpp"
#include "FrenchRevolutionaryDate.hpp"
using namespace std;
using namespace EpsilonDelta;


int Main( int /*argc*/, char ** /*argv*/ );
static bool ComputeEpochs( );


//*****************************************************************************


int main( int argc, char ** argv )
{
    try
    {
        int res = Main( argc, argv );
        return res;
    }
    catch ( Exception & except )
    {
        cout << except.Description( ) << endl;
    }
    catch ( exception & except )
    {
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    catch ( ... )
    {
        Exception exc( "Unknown exception!" ); //!!!
        cout << exc.Description( ) << endl;
    }
    return  2;
}

//-----------------------------------------------------------------------------


int Main( int /*argc*/, char ** argv )
{
    bool ok = true;

#ifdef DEBUG
    string libBasePath = argv[0];
    int slashPos = libBasePath.rfind( '/' );
    if ( slashPos == static_cast<int>( string::npos ) )
        libBasePath = "";
    else
        libBasePath.erase( slashPos + 1 );
    libBasePath += "../";
    JPLEphemeris de405( libBasePath + "astrodata/JPL_DE405.be", false );
    JPLEphemeris de406( libBasePath + "astrodata/JPL_DE406.be", false );
    JPLEphemeris::RegisterEphemeris( de405 );
    JPLEphemeris::RegisterEphemeris( de406 );

    if ( ! TestJulianDate( ) )
        ok = false;
    if ( ! TestISO8601Date( ) )
        ok = false;
    if ( ! TestPersianArithmeticDate( ) )
        ok = false;
    if ( ! TestIslamicDate( ) )
        ok = false;
    if ( ! TestHebrewDate( ) )
        ok = false;
    if ( ! TestCopticDate( ) )
        ok = false;
    if ( ! TestEthiopianDate( ) )
        ok = false;
    if ( ! TestBahaiDate( ) )
        ok = false;
    if ( ! TestBadiDate( ) )
        ok = false;
    if ( ! TestChineseDate( ) )
        ok = false;
    if ( ! TestHinduSolarDate( ) )
        ok = false;
    if ( ! TestHinduLunisolarDate( ) )
        ok = false;
    if ( ! TestOldHinduSolarDate( ) )
        ok = false;
    if ( ! TestOldHinduLunisolarDate( ) )
        ok = false;
    if ( ! TestEgyptianDate( ) )
        ok = false;
    if ( ! TestArmenianDate( ) )
        ok = false;
    if ( ! TestMayanLongCountDate( ) )
        ok = false;
    if ( ! TestMayanHaabDate( ) )
        ok = false;
    if ( ! MayanTzolkinCalendar::Test( ) )
        ok = false;
    if ( ! TestFrenchRevolutionaryDate( ) )
        ok = false;
    if ( ! TestPersianAstronomicalDate( ) )
        ok = false;

    cout << endl << endl;
#endif //DEBUG

    if ( ! ComputeEpochs( ) )
        ok = false;

    if ( ok )
    {
        cout << endl << "All tests PASSED!" << endl;
        return  0;
    }
    else
    {
        cout << endl << "Some test(s) FAILED!" << endl;
        return  1;
    }
}

//.............................................................................

static
bool
ComputeEpochs( )
{
    cout << "Testing calendar epochs" << endl;
    bool ok = true;

#ifdef DEBUG

    int d, m, y, jd, d1, m1, y1;

    JulianDate julDate;
    GregorianDate gregDate;

    d = 1;
    m = 1;
    y = 1;
    gregDate.Set( d, m, y );
    jd = gregDate.JulianDay( );
    printf( "Gregorian epoch: %d/%d/%d (Gregorian) => jd=%d\n",  d, m, y, jd );
    gregDate.Set( jd );
    d1 = gregDate.Day();    m1 = gregDate.Month();    y1 = gregDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( GregorianDate().JulianDay(), jd, &ok );
    TESTCHECK( gregDate == GregorianDate(), true, &ok );
    TESTCHECK( gregDate == GregorianDate( 1, 1, 1 ), true, &ok );

    d = 1;    m = 1;    y = 1;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Julian epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( JulianDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == JulianDate(), true, &ok );
    TESTCHECK( julDate == JulianDate( 1, 1, 1 ), true, &ok );

    d = 30;    m = 12;    y = 0;
    gregDate.Set( d, m, y );
    jd = gregDate.JulianDay( );
    printf( "Julian epoch: %d/%d/%d (Gregorian) => jd=%d\n",  d, m, y, jd );
    gregDate.Set( jd );
    d1 = gregDate.Day();    m1 = gregDate.Month();    y1 = gregDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( JulianDate().JulianDay(), jd, &ok );
    TESTCHECK( gregDate == JulianDate(), true, &ok );
    TESTCHECK( gregDate == JulianDate( 1, 1, 1 ), true, &ok );

    d = 19;
    m = 3;
    y = 622;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Persian epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( PersianAstronomicalDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == PersianAstronomicalDate(), true, &ok );
    TESTCHECK( julDate == PersianAstronomicalDate( 1, 1, 1 ), true, &ok );

    d = 19;
    m = 3;
    y = 622;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Persian epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( PersianArithmeticDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == PersianArithmeticDate(), true, &ok );
    TESTCHECK( julDate == PersianArithmeticDate( 1, 1, 1 ), true, &ok );

    d = 16;
    m = 7;
    y = 622;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Islamic epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    cout << "IslamicCalendar::ArithmeticSystem("
            " IslamicCalendar::ArithmeticSystem::Nizari )"
         << endl;
    shared_ptr< IslamicCalendar::ArithmeticSystem > spNizariSystem(
        new IslamicCalendar::ArithmeticSystem(
            IslamicCalendar::ArithmeticSystem::Nizari ) );
    cout << "IslamicCalendar::SetSystem( spNizariSystem )" << endl;
    IslamicCalendar::SetSystem( spNizariSystem );
    TESTCHECK( IslamicDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == IslamicDate(), true, &ok );
    TESTCHECK( julDate == IslamicDate( 1, 1, 1 ), true, &ok );
    cout << "IslamicCalendar::ArithmeticSystem("
            " IslamicCalendar::ArithmeticSystem::Labban )"
         << endl;
    shared_ptr< IslamicCalendar::ArithmeticSystem > spLabbanSystem(
        new IslamicCalendar::ArithmeticSystem(
            IslamicCalendar::ArithmeticSystem::Labban ) );
    cout << "IslamicCalendar::SetSystem( spLabbanSystem )" << endl;
    IslamicCalendar::SetSystem( spLabbanSystem );
    TESTCHECK( IslamicDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == IslamicDate(), true, &ok );
    TESTCHECK( julDate == IslamicDate( 1, 1, 1 ), true, &ok );
#if 0   //Umm al-Qura has epoch one day earlier
    cout << "IslamicCalendar::SetSystem( "
            "new IslamicCalendar::AstronomicalSystem( "
            "new IslamicCalendar::LocalMonthFunc( "
            "IslamicCalendar::UmmAlQuraVisibility, Mecca ) ) )" << endl;
    IslamicCalendar::SetSystem( new IslamicCalendar::AstronomicalSystem(
                                    new IslamicCalendar::LocalMonthFunc(
                                        IslamicCalendar::UmmAlQuraVisibility,
                                        Mecca ) ) );
    TESTCHECK( IslamicDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == IslamicDate(), true, &ok );
    TESTCHECK( julDate == IslamicDate( 1, 1, 1 ), true, &ok );
#endif
    cout << "IslamicCalendar::LocalMonthFunc( "
            "LunarVisibility::CheckNext, Mecca )" << endl;
    shared_ptr< IslamicCalendar::LocalMonthFunc > spMeccaVisibility(
        new IslamicCalendar::LocalMonthFunc( LunarVisibility::CheckNext,
                                             Mecca ) );
    cout << "IslamicCalendar::AstronomicalSystem( spMeccaVisibility )" << endl;
    shared_ptr< IslamicCalendar::AstronomicalSystem > spMeccaVisibilitySystem(
        new IslamicCalendar::AstronomicalSystem( spMeccaVisibility ) );
    cout << "IslamicCalendar::SetSystem( spMeccaVisibilitySystem )" << endl;
    IslamicCalendar::SetSystem( spMeccaVisibilitySystem );
    TESTCHECK( IslamicDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == IslamicDate(), true, &ok );
    TESTCHECK( julDate == IslamicDate( 1, 1, 1 ), true, &ok );

    d = 7;
    m = 10;
    y = -3760;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Hebrew epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( HebrewDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == HebrewDate(), true, &ok );
    TESTCHECK( julDate == HebrewDate( 1, 7, 1 ), true, &ok );

    d = 29;
    m = 8;
    y = 284;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Coptic epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( CopticDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == CopticDate(), true, &ok );
    TESTCHECK( julDate == CopticDate( 1, 1, 1 ), true, &ok );

    d = 29;
    m = 8;
    y = 8;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Ethiopian epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( EthiopianDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == EthiopianDate(), true, &ok );
    TESTCHECK( julDate == EthiopianDate( 1, 1, 1 ), true, &ok );

    d = 21;
    m = 3;
    y = 1844;
    gregDate.Set( d, m, y );
    jd = gregDate.JulianDay( );
    printf( "Baha'i epoch: %d/%d/%d (Gregorian) => jd=%d\n",  d, m, y, jd );
    gregDate.Set( jd );
    d1 = gregDate.Day();    m1 = gregDate.Month();    y1 = gregDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( BahaiDate().JulianDay(), jd, &ok );
    TESTCHECK( gregDate == BahaiDate(), true, &ok );
    TESTCHECK( gregDate == BahaiDate( 1, 1, 1 ), true, &ok );

    d = 6;
    m = 9;
    y = -3113;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Mayan epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( MayanLongCountDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == MayanLongCountDate(), true, &ok );
    TESTCHECK( julDate == MayanLongCountDate( 0, 0, 0, 0, 0 ), true, &ok );

    d = 18;
    m = 2;
    y = -3101;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Old Hindu (Kali Yuga) epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( OldHinduSolarDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == OldHinduSolarDate(), true, &ok );
    TESTCHECK( julDate == OldHinduSolarDate( 1, 1, 0 ), true, &ok );

    d = 23;
    m = 1;
    y = -3101;
    gregDate.Set( d, m, y );
    jd = gregDate.JulianDay( );
    printf( "Old Hindu (Kali Yuga) epoch: %d/%d/%d (Gregorian) => jd=%d\n",  d, m, y, jd );
    gregDate.Set( jd );
    d1 = gregDate.Day();    m1 = gregDate.Month();    y1 = gregDate.Year();
    GregorianCalendar::JulianDayToDMY( jd, &d1, &m1, &y1 );
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( OldHinduSolarDate().JulianDay(), jd, &ok );
    TESTCHECK( gregDate == OldHinduSolarDate(), true, &ok );
    TESTCHECK( gregDate == OldHinduSolarDate( 1, 1, 0 ), true, &ok );

    d = 23;
    m = 1;
    y = -3101;
    gregDate.Set( d, m, y );
    jd = gregDate.JulianDay( );
    printf( "Old Hindu (Kali Yuga) epoch: %d/%d/%d (Gregorian) => jd=%d\n",  d, m, y, jd );
    gregDate.Set( jd );
    d1 = gregDate.Day();    m1 = gregDate.Month();    y1 = gregDate.Year();
    GregorianCalendar::JulianDayToDMY( jd, &d1, &m1, &y1 );
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( OldHinduLunisolarDate().JulianDay(), jd, &ok );
    TESTCHECK( gregDate == OldHinduLunisolarDate(), true, &ok );
    TESTCHECK( gregDate == OldHinduLunisolarDate( 1, 1, false, 0 ), true, &ok );

    d = 22;
    m = 9;
    y = 1792;
    gregDate.Set( d, m, y );
    jd = gregDate.JulianDay( );
    printf( "French Rev. epoch: %d/%d/%d (Gregorian) => jd=%d\n",  d, m, y, jd );
    gregDate.Set( jd );
    d1 = gregDate.Day();    m1 = gregDate.Month();    y1 = gregDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( FrenchRevolutionaryDate().JulianDay(), jd, &ok );
    TESTCHECK( gregDate == FrenchRevolutionaryDate(), true, &ok );
    TESTCHECK( gregDate == FrenchRevolutionaryDate( 1, 1, 1 ), true, &ok );

    d = 15;
    m = 2;
    y = -2636;
    gregDate.Set( d, m, y );
    jd = gregDate.JulianDay( );
    printf( "Chinese epoch: %d/%d/%d (Gregorian) => jd=%d\n",  d, m, y, jd );
    gregDate.Set( jd );
    d1 = gregDate.Day();    m1 = gregDate.Month();    y1 = gregDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( ChineseDate().JulianDay(), jd, &ok );
    TESTCHECK( gregDate == ChineseDate(), true, &ok );
    TESTCHECK( gregDate == ChineseDate( 1, 1, 1 ), true, &ok );

    d = 26;
    m = 2;
    y = -746;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Egyptian epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( EgyptianDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == EgyptianDate(), true, &ok );
    TESTCHECK( julDate == EgyptianDate( 1, 1, 1 ), true, &ok );

    d = 11;
    m = 7;
    y = 552;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Armenian epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
    TESTCHECK( ArmenianDate().JulianDay(), jd, &ok );
    TESTCHECK( julDate == ArmenianDate(), true, &ok );
    TESTCHECK( julDate == ArmenianDate( 1, 1, 1 ), true, &ok );

    d = 16;
    m = 6;
    y = 632;
    julDate.Set( d, m, y );
    jd = julDate.JulianDay( );
    printf( "Yazdegerd epoch: %d/%d/%d (Julian) => jd=%d\n",  d, m, y, jd );
    julDate.Set( jd );
    d1 = julDate.Day();    m1 = julDate.Month();    y1 = julDate.Year();
    TESTCHECK( d1, d, &ok );
    TESTCHECK( m1, m, &ok );
    TESTCHECK( y1, y, &ok );
//    TESTCHECK( YazdegerdDate().JulianDay(), jd, &ok );
//    TESTCHECK( julDate == YazdegerdDate(), true, &ok );
//    TESTCHECK( julDate == YazdegerdDate( 1, 1, 1 ), true, &ok );

#endif //DEBUG
    if ( ok )
        cout << "Calendar epochs PASSED." << endl << endl;
    else
        cout << "Calendar epochs FAILED." << endl << endl;
    return ok;
}


//*****************************************************************************

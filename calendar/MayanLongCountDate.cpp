/*
  MayanLongCountDate.cpp
  Copyright (C) 2007 David M. Anderson

  MayanLongCountDate class, which defines a date in the Mayan long count
  calendar.
*/


#include "MayanLongCountDate.hpp"


#include "MayanLongCountDate.hpp"
#include "MayanLongCountCalendar.hpp"
#include "BahaiDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


MayanLongCountDate::MayanLongCountDate( bool today )
{
    Set( today );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MayanLongCountDate::MayanLongCountDate( long julianDay )
    :   DateJD( false )
{
    DateJD::Set( julianDay );
    Calendar::JulianDayToLongCount( JulianDay(), &m_kin, &m_uinal, &m_tun,
                                    &m_katun, &m_baktun, &m_pictun,
                                    &m_calabtun, &m_kinchiltun, &m_alautun );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MayanLongCountDate::MayanLongCountDate( int kin, int uinal, int tun, int katun,
                                        int baktun, int pictun, int calabtun,
                                        int kinchiltun, int alautun )
    :   DateJD( false ),
        m_kin( kin ),
        m_uinal( uinal ),
        m_tun( tun ),
        m_katun( katun ),
        m_baktun( baktun ),
        m_pictun( pictun ),
        m_calabtun( calabtun ),
        m_kinchiltun( kinchiltun ),
        m_alautun( alautun )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MayanLongCountDate::MayanLongCountDate( const DateJD & date )
    :   DateJD( false )
{
    Set( date.JulianDay() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MayanLongCountDate::MayanLongCountDate( const MayanLongCountDate & date )
    :   DateJD( false )
{
    Set( date.m_kin, date.m_uinal, date.m_tun, date.m_katun, date.m_baktun,
         date.m_pictun, date.m_calabtun, date.m_kinchiltun, date.m_alautun );
    if ( date.DateJD::Valid() )
        DateJD::Set( date.JulianDay() );
}

//-----------------------------------------------------------------------------

MayanLongCountDate::~MayanLongCountDate( )
{
}

//=============================================================================

void 
MayanLongCountDate::Set( bool today )
{
    if ( today )
    {
        DateJD::Set( true );
        Assert( DateJD::Valid() );
        Calendar::JulianDayToLongCount( JulianDay(),
                                        &m_kin, &m_uinal, &m_tun,
                                        &m_katun, &m_baktun,
                                        &m_pictun, &m_calabtun,
                                        &m_kinchiltun, &m_alautun );
    }
    else
    {
        DateJD::Set( false );
        Set( 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
MayanLongCountDate::Set( long julianDay )
{
    DateJD::Set( julianDay );
    Calendar::JulianDayToLongCount( julianDay,
                                    &m_kin, &m_uinal, &m_tun,
                                    &m_katun, &m_baktun,
                                    &m_pictun, &m_calabtun,
                                    &m_kinchiltun, &m_alautun );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
MayanLongCountDate::Set( int kin, int uinal, int tun, int katun, int baktun,
                         int pictun, int calabtun,
                         int kinchiltun, int alautun )
{
    DateJD::Set( DateJD::INVALID );
    m_kin = kin;
    m_uinal = uinal;
    m_tun = tun;
    m_katun = katun;
    m_baktun = baktun;
    m_pictun = pictun;
    m_calabtun = calabtun;
    m_kinchiltun = kinchiltun;
    m_alautun = alautun;
}

//=============================================================================

MayanLongCountDate & 
MayanLongCountDate::operator=( const DateJD & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.JulianDay() );
    return *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MayanLongCountDate & 
MayanLongCountDate::operator=( const MayanLongCountDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.m_kin, rhs.m_uinal, rhs.m_tun, rhs.m_katun, rhs.m_baktun,
         rhs.m_pictun, rhs.m_calabtun, rhs.m_kinchiltun, rhs.m_alautun );
    if ( rhs.DateJD::Valid() )
        DateJD::Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

bool 
MayanLongCountDate::Valid( ) const
{
    if ( (m_kinchiltun < 0)
         || (m_kinchiltun >= Calendar::KinchiltunInAlautun) )
        return false;
    if ( (m_calabtun < 0) || (m_calabtun >= Calendar::CalabtunInKinchiltun) )
        return false;
    if ( (m_pictun < 0) || (m_pictun >= Calendar::PictunInCalabtun) )
        return false;
    if ( (m_baktun < 0) || (m_baktun >= Calendar::BaktunInPictun) )
        return false;
    if ( (m_katun < 0) || (m_katun >= Calendar::KatunInBaktun) )
        return false;
    if ( (m_tun < 0) || (m_tun >= Calendar::TunInKatun) )
        return false;
    if ( (m_uinal < 0) || (m_uinal >= Calendar::UinalInTun) )
        return false;
    if ( (m_kin < 0) || (m_kin >= Calendar::KinInUinal) )
        return false;
    return true;
}

//-----------------------------------------------------------------------------

void 
MayanLongCountDate::MakeValid( DateFixup::EMethod fixupMethod )
{
    if ( fixupMethod == DateFixup::Carry )
    {
        while ( m_kin < 0 )
        {
            m_kin += Calendar::KinInUinal;
            --m_uinal;
        }
        while ( m_kin >= Calendar::KinInUinal )
        {
            m_kin -= Calendar::KinInUinal;
            ++m_uinal;
        }
        while ( m_uinal < 0 )
        {
            m_uinal += Calendar::UinalInTun;
            --m_tun;
        }
        while ( m_uinal >= Calendar::UinalInTun )
        {
            m_uinal -= Calendar::UinalInTun;
            ++m_tun;
        }
        while ( m_tun < 0 )
        {
            m_tun += Calendar::TunInKatun;
            --m_katun;
        }
        while ( m_tun >= Calendar::TunInKatun )
        {
            m_tun -= Calendar::TunInKatun;
            ++m_katun;
        }
        while ( m_katun < 0 )
        {
            m_katun += Calendar::KatunInBaktun;
            --m_baktun;
        }
        while ( m_katun >= Calendar::KatunInBaktun )
        {
            m_katun -= Calendar::KatunInBaktun;
            ++m_baktun;
        }
        while ( m_baktun < 0 )
        {
            m_baktun += Calendar::BaktunInPictun;
            --m_pictun;
        }
        while ( m_baktun >= Calendar::BaktunInPictun )
        {
            m_baktun -= Calendar::BaktunInPictun;
            ++m_pictun;
        }
        while ( m_pictun < 0 )
        {
            m_pictun += Calendar::PictunInCalabtun;
            --m_calabtun;
        }
        while ( m_pictun >= Calendar::PictunInCalabtun )
        {
            m_pictun -= Calendar::PictunInCalabtun;
            ++m_calabtun;
        }
        while ( m_calabtun < 0 )
        {
            m_calabtun += Calendar::CalabtunInKinchiltun;
            --m_kinchiltun;
        }
        while ( m_calabtun >= Calendar::CalabtunInKinchiltun )
        {
            m_calabtun -= Calendar::CalabtunInKinchiltun;
            ++m_kinchiltun;
        }
        while ( m_kinchiltun < 0 )
        {
            m_kinchiltun += Calendar::KinchiltunInAlautun;
            --m_alautun;
        }
        while ( m_kinchiltun >= Calendar::KinchiltunInAlautun )
        {
            m_kinchiltun -= Calendar::KinchiltunInAlautun;
            ++m_alautun;
        }
    }
    else //DateFixup::Clamp
    {
        if ( m_kin < 0 )
            m_kin = 0;
        else if ( m_kin >= Calendar::KinInUinal )
            m_kin = Calendar::KinInUinal - 1;
        if ( m_uinal < 0 )
            m_uinal = 0;
        else if ( m_uinal >= Calendar::UinalInTun )
            m_uinal = Calendar::UinalInTun - 1;
        if ( m_tun < 0 )
            m_tun = 0;
        else if ( m_tun >= Calendar::TunInKatun )
            m_tun = Calendar::TunInKatun - 1;
        if ( m_katun < 0 )
            m_katun = 0;
        else if ( m_katun >= Calendar::KatunInBaktun )
            m_katun = Calendar::KatunInBaktun - 1;
        if ( m_baktun < 0 )
            m_baktun = 0;
        else if ( m_baktun >= Calendar::BaktunInPictun )
            m_baktun = Calendar::BaktunInPictun - 1;
        if ( m_pictun < 0 )
            m_pictun = 0;
        else if ( m_pictun >= Calendar::PictunInCalabtun )
            m_pictun = Calendar::PictunInCalabtun - 1;
        if ( m_calabtun < 0 )
            m_calabtun = 0;
        else if ( m_calabtun >= Calendar::CalabtunInKinchiltun )
            m_calabtun = Calendar::CalabtunInKinchiltun - 1;
        if ( m_kinchiltun < 0 )
            m_kinchiltun = 0;
        else if ( m_kinchiltun >= Calendar::KinchiltunInAlautun )
            m_kinchiltun = Calendar::KinchiltunInAlautun - 1;
    }
    Assert( Valid() );
}

//=============================================================================

long 
MayanLongCountDate::JulianDay( ) const
{
    if ( ! DateJD::Valid() )
        m_julianDay = Calendar::LongCountToJulianDay( m_kin, m_uinal, m_tun,
                                                      m_katun, m_baktun,
                                                      m_pictun, m_calabtun,
                                                      m_kinchiltun, m_alautun );
    return DateJD::JulianDay();
}

//=============================================================================

void 
MayanLongCountDate::Increment( int days )
{
    DateJD::Increment( days );
    m_kin += days;
    MakeValid( DateFixup::Carry );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
MayanLongCountDate::Increment( int kin, int uinal, int tun, int katun,
                               int baktun, int pictun, int calabtun,
                               int kinchiltun, int alautun,
                               DateFixup::EMethod fixupMethod )
{
    DateJD::Set( DateJD::INVALID );
    m_kin += kin;
    m_uinal += uinal;
    m_tun += tun;
    m_katun += katun;
    m_baktun += baktun;
    m_pictun += pictun;
    m_calabtun += calabtun;
    m_kinchiltun += kinchiltun;
    m_alautun += alautun;
    MakeValid( fixupMethod );
}

//=============================================================================

std::string 
MayanLongCountDate::ToString( const std::string & format ) const
{
    return format; //!!!
}

//-----------------------------------------------------------------------------

std::string 
MayanLongCountDate::m_defaultFormat = ""; //!!!

//-----------------------------------------------------------------------------

void 
MayanLongCountDate::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

const std::string & 
MayanLongCountDate::DefaultFormat( )
{
    return m_defaultFormat;
}

//=============================================================================

bool operator==( const MayanLongCountDate & lhs, const MayanLongCountDate & rhs )
{
    return ( (lhs.Kin() == rhs.Kin()) && (lhs.Uinal() == rhs.Uinal())
             && (lhs.Tun() == rhs.Tun()) && (lhs.Katun() == rhs.Katun())
             && (lhs.Baktun() == rhs.Baktun())
             && (lhs.Pictun() == rhs.Pictun())
             && (lhs.Calabtun() == rhs.Calabtun())
             && (lhs.Kinchiltun() == rhs.Kinchiltun())
             && (lhs.Alautun() == rhs.Alautun()) );
}

//-----------------------------------------------------------------------------

bool operator<( const MayanLongCountDate & lhs, const MayanLongCountDate & rhs )
{
    if ( lhs.Alautun() < lhs.Alautun() )
        return true;
    else if ( lhs.Alautun() > lhs.Alautun() )
        return false;
    if ( lhs.Kinchiltun() < lhs.Kinchiltun() )
        return true;
    else if ( lhs.Kinchiltun() > lhs.Kinchiltun() )
        return false;
    if ( lhs.Calabtun() < lhs.Calabtun() )
        return true;
    else if ( lhs.Calabtun() > lhs.Calabtun() )
        return false;
    if ( lhs.Pictun() < lhs.Pictun() )
        return true;
    else if ( lhs.Pictun() > lhs.Pictun() )
        return false;
    if ( lhs.Baktun() < lhs.Baktun() )
        return true;
    else if ( lhs.Baktun() > lhs.Baktun() )
        return false;
    if ( lhs.Katun() < lhs.Katun() )
        return true;
    else if ( lhs.Katun() > lhs.Katun() )
        return false;
    if ( lhs.Tun() < lhs.Tun() )
        return true;
    else if ( lhs.Tun() > lhs.Tun() )
        return false;
    if ( lhs.Uinal() < lhs.Uinal() )
        return true;
    else if ( lhs.Uinal() > lhs.Uinal() )
        return false;
    else
        return ( lhs.Kin() < rhs.Kin() );
}

//=============================================================================

#ifdef DEBUG

bool 
TestMayanLongCountDate( )
{
    bool ok = true;
    cout << "Testing MayanLongCountDate" << endl;

    long jd;
    int kin, uinal, tun, katun, baktun;

    struct
    {
        long julianDay;
        int kin;
        int uinal;
        int tun;
        int katun;
        int baktun;
    } 
    testDates[]
            = {
                { 1507232,   9, 13,  3,  8,  6 },
                { 1660038,  15,  3,  8,  9,  7 },
                { 1746894,  11,  8,  9,  1,  8 },
                { 1770642,  19,  7, 15,  4,  8 },
                { 1892732,   9, 10, 14,  1,  9 },
                { 1931580,  17,  8,  2,  7,  9 },
                { 1974852,   9, 12,  2, 13,  9 },
                { 2091165,   2, 14,  5,  9, 10 },
                { 2121510,   7,  1, 10, 13, 10 },
                { 2155780,  17,  4,  5, 18, 10 },
                { 2174030,   7, 17, 15,  0, 11 },
                { 2191585,   2, 13,  4,  3, 11 },
                { 2195262,  19, 16, 14,  3, 11 },
                { 2229275,  12,  7,  9,  8, 11 },
                { 2245581,  18, 12, 14, 10, 11 },
                { 2266101,  18, 12, 11, 13, 11 },
                { 2288543,   0,  1, 14, 16, 11 },
                { 2290902,  19, 10,  0, 17, 11 },
                { 2323141,  18,  2, 10,  1, 12 },
                { 2334849,   6, 12,  2,  3, 12 },
                { 2348021,  18,  4, 19,  4, 12 },
                { 2366979,  16, 16, 11,  7, 12 },
                { 2385649,   6, 14,  3, 10, 12 },
                { 2392826,   3, 13,  3, 11, 12 },
                { 2416224,   1, 13,  8, 14, 12 },
                { 2425849,   6,  8, 15, 15, 12 },
                { 2430267,   4, 13,  7, 16, 12 },
                { 2430834,  11,  5,  9, 16, 12 },
                { 2431005,   2, 14,  9, 16, 12 },
                { 2448699,  16, 16, 18, 18, 12 },
                { 2450139,  16, 16,  2, 19, 12 },
                { 2465738,  15,  4,  6,  1, 13 },
                { 2486077,  14, 13,  2,  4, 13 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        kin = testDates[i].kin;
        uinal = testDates[i].uinal;
        tun = testDates[i].tun;
        katun = testDates[i].katun;
        baktun = testDates[i].baktun;
        cout << "MayanLongCountDate( " << kin << ", " << uinal << ", " << tun
             << ", " << katun << ", " << baktun << " ) :" << endl;
        MayanLongCountDate mlcDate( kin, uinal, tun, katun, baktun );
        TESTCHECK( mlcDate.Valid( ), true, &ok );
        TESTCHECK( mlcDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        mlcDate.Set( jd );
        TESTCHECK( mlcDate.Valid( ), true, &ok );
        TESTCHECK( mlcDate.Kin( ), kin, &ok );
        TESTCHECK( mlcDate.Uinal( ), uinal, &ok );
        TESTCHECK( mlcDate.Tun( ), tun, &ok );
        TESTCHECK( mlcDate.Katun( ), katun, &ok );
        TESTCHECK( mlcDate.Baktun( ), baktun, &ok );
    }


    if ( ok )
        cout << "MayanLongCountDate PASSED." << endl << endl;
    else
        cout << "MayanLongCountDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

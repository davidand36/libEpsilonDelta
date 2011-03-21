/*
  Precession.cpp
  Copyright (C) 2007 David M. Anderson

  Precession class: represents coordinate correction due to lunisolar
  precession (the smooth, long-period motion of the pole and equator,
  regressing the vernal equinox about 50" / year) and planetary precession
  (the motion of the ecliptic due the planets at about 47" / century).
*/


#include "Precession.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "AngleHMS.hpp"
#include "AngleDMS.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Precession::Precession( double julianDay, double epoch )
{
    //Explanatory Supplement (3.211-2) and Table 3.211.1
    double zetaSec, zSec, thetaSec;
    double t = Century2000( julianDay );
    if ( epoch == J2000 )
    {
        zetaSec = t * (2306.2181  +  t * (0.30188  +  t * 0.017998));
        zSec = t * (2306.2181  +  t * (1.09468  +  t * 0.018203));
        thetaSec = t * (2004.3109  +  t * (-0.42665  +  t * -0.041833));
    }
    else
    {
        double T = Century2000( epoch );
        zetaSec = t * ( (2306.2181  +  T * (1.39656  +  T * -0.000139))
                        +  t * (0.30188  +  T * -0.000344)
                        +  t * 0.017998 );
        zSec = t * ( (2306.2181  +  T * (1.39656  +  T * -0.000139))
                     +  t * (1.09468  +  T * 0.000066)
                     +  t * 0.018203 );
        thetaSec = t * ( (2004.3109  +  T * (-0.85330  +  T * -0.000217))
                         +  t * (-0.42665  +  T * -0.000217)
                         +  t * -0.041833 );
    }
    m_zeta.Set( zetaSec, Angle::ArcSecond );
    m_z.Set(  zSec, Angle::ArcSecond );
    m_theta.Set( thetaSec, Angle::ArcSecond );
    m_cosTheta = m_theta.Cos( );
    m_sinTheta = m_theta.Sin( );
}

//=============================================================================

Matrix3D 
Precession::Matrix( bool fromEpoch ) const
{
    //Explanatory Supplement (3.21-8)
    double cosZeta = m_zeta.Cos( );
    double sinZeta = m_zeta.Sin( );
    double cosZ = m_z.Cos( );
    double sinZ = m_z.Sin( );
    double p00 = cosZeta * cosZ * m_cosTheta  -  sinZeta * sinZ;
    double p01 = - sinZeta * cosZ * m_cosTheta  -  cosZeta * sinZ;
    double p02 = - cosZ * m_sinTheta;
    double p10 = cosZeta * sinZ * m_cosTheta  +  sinZeta * cosZ;
    double p11 = - sinZeta * sinZ * m_cosTheta  +  cosZeta * cosZ;
    double p12 = - sinZ * m_sinTheta;
    double p20 = cosZeta * m_sinTheta;
    double p21 = - sinZeta * m_sinTheta;
    double p22 = m_cosTheta;
    if ( fromEpoch )
        return Matrix3D( p00, p01, p02,  p10, p11, p12,  p20, p21, p22 );
    else //The matrix is orthogonal, so the inverse is the transpose.
        return Matrix3D( p00, p10, p20,  p01, p11, p21,  p02, p12, p22 );
}

//-----------------------------------------------------------------------------

Equatorial
Precession::Reduce( Equatorial equatorial, bool fromEpoch ) const
{
    //Explanatory Supplement (3.212-2, 3.212-3), Meeus, p 134-35
    if ( fromEpoch )
    {
        double cosDec0 = equatorial.Declination().Cos( );
        double sinDec0 = equatorial.Declination().Sin( );
        Angle ra0PlusZeta = equatorial.RightAscension() + m_zeta;
        double cosRa0PlusZeta = ra0PlusZeta.Cos( );
        double sinRa0PlusZeta = ra0PlusZeta.Sin( );
        double a = cosDec0 * sinRa0PlusZeta;
        double b = m_cosTheta * cosDec0 * cosRa0PlusZeta
                -  m_sinTheta * sinDec0;
        double c = m_sinTheta * cosDec0 * cosRa0PlusZeta
                +  m_cosTheta * sinDec0;
        Angle dec1;
        if ( c < -0.9 )
            dec1 = - ArcCos( sqrt( a * a  +  b * b ) );
        else if ( c <= 0.9 )
            dec1 = ArcSin( c );
        else
            dec1 = ArcCos( sqrt( a * a  +  b * b ) );
        Angle ra1 = ArcTan( a, b ) + m_z;
        Equatorial equat1( ra1, dec1, equatorial.Distance() );
        equat1.Normalize( );
        return equat1;
    }
    else
    {
        double cosDec1 = equatorial.Declination().Cos( );
        double sinDec1 = equatorial.Declination().Sin( );
        Angle ra1MinusZ = equatorial.RightAscension() - m_zeta;
        double cosRa1MinusZ = ra1MinusZ.Cos( );
        double sinRa1MinusZ = ra1MinusZ.Sin( );
        double a = cosDec1 * sinRa1MinusZ;
        double b = m_cosTheta * cosDec1 * cosRa1MinusZ
                +  m_sinTheta * sinDec1;
        double c = - m_sinTheta * cosDec1 * cosRa1MinusZ
                +  m_cosTheta * sinDec1;
        Angle dec0;
        if ( c < -0.9 )
            dec0 = - ArcCos( sqrt( a * a  +  b * b ) );
        else if ( c <= 0.9 )
            dec0 = ArcSin( c );
        else
            dec0 = ArcCos( sqrt( a * a  +  b * b ) );
        Angle ra0 = ArcTan( a, b ) - m_zeta;
        Equatorial equat0( ra0, dec0, equatorial.Distance() );
        equat0.Normalize( );
        return equat0;
    }
}

//=============================================================================

#ifdef DEBUG

bool 
Precession::Test( )
{
    bool ok = true;
    cout << "Testing Precession" << endl;

    //Paul J. Heafner, "Fundamental Ephemeris Computations", p. 43
    double jd = 2447160.5;
    cout << "JD: " << jd << endl;
    Precession precession( jd );
    Matrix3D precMat = precession.Matrix( );
    TESTCHECKF( precMat(0,0), 0.99999572, &ok );
    TESTCHECKFE( precMat(0,1), 0.00268422, &ok, 4.e-6 );
    TESTCHECKFE( precMat(0,2), 0.00116649, &ok, 1.e-5 );
    TESTCHECKFE( precMat(1,0), -0.00268422, &ok, 4.e-6 );
    TESTCHECKF( precMat(1,1), 0.99999640, &ok );
    TESTCHECKFE( precMat(1,2), -0.00000157, &ok, 6.e-3 );
    TESTCHECKFE( precMat(2,0), -0.00116649, &ok, 9.e-6 );
    TESTCHECKFE( precMat(2,1), -0.00000157, &ok, 6.e-3 );
    TESTCHECKF( precMat(2,2), 0.99999932, &ok );

    //Meeus, p. 135
    jd = 2462088.69;
    cout << "JD: " << jd << endl;
    precession = Precession( jd );
    Equatorial thetaPer2000( Angle( 41.054063, Angle::Degree ),
                          Angle( 49.227750, Angle::Degree ) );
    Equatorial thetaPer2028( Angle( 41.547214, Angle::Degree ),
                          Angle( 49.348483, Angle::Degree ) );
    TESTCHECKF( precession.Reduce( thetaPer2000 ).RightAscension().Degrees(),
                thetaPer2028.RightAscension().Degrees(), &ok );
    TESTCHECKF( precession.Reduce( thetaPer2000 ).Declination().Degrees(),
                thetaPer2028.Declination().Degrees(), &ok );
    TESTCHECKF( precession.Reduce( thetaPer2028, false )
                .RightAscension().Degrees(),
                thetaPer2000.RightAscension().Degrees(), &ok );
    TESTCHECKF( precession.Reduce( thetaPer2028, false )
                .Declination().Degrees(),
                thetaPer2000.Declination().Degrees(), &ok );
    //Meeus, p. 136
    Equatorial polaris2000J2000( Angle( AngleHMS( 2, 31, 48.704 ) ),
                                 Angle( AngleDMS( 89, 15, 50.72 ) ) );
    Angle raPropMotion = Angle( AngleHMS( 0, 0, 0.19877 ) );
    Angle decPropMotion = Angle( -0.0152, Angle::ArcSecond );
    double b1900 = Epoch( 1900, Epoch::Besselian ).JulianDay();
    double diffYrs = (b1900 - J2000) / 365.25;
    Angle ra1900J2000 = polaris2000J2000.RightAscension()
            +  diffYrs * raPropMotion;
    Angle dec1900J2000 = polaris2000J2000.Declination()
            +  diffYrs * decPropMotion;
    Equatorial polaris1900J2000( ra1900J2000, dec1900J2000 );
    precession = Precession( b1900, J2000 );
    Equatorial polaris1900B1900 = precession.Reduce( polaris1900J2000 );
    TESTCHECKF( polaris1900B1900.RightAscension().Degrees(),
                Angle( AngleHMS( 1, 22, 33.90 ) ).Degrees(), &ok );
    TESTCHECKF( polaris1900B1900.Declination().Degrees(),
                Angle( AngleDMS( 88, 46, 26.18 ) ).Degrees(), &ok );
    double j2050 = Epoch( 2050, Epoch::Julian ).JulianDay();
    diffYrs = (j2050 - J2000) / 365.25;
    Angle ra2050J2000 = polaris2000J2000.RightAscension()
            +  diffYrs * raPropMotion;
    Angle dec2050J2000 = polaris2000J2000.Declination()
            +  diffYrs * decPropMotion;
    Equatorial polaris2050J2000( ra2050J2000, dec2050J2000 );
    precession = Precession( j2050, J2000 );
    Equatorial polaris2050J2050 = precession.Reduce( polaris2050J2000 );
    TESTCHECKF( polaris2050J2050.RightAscension().Degrees(),
                Angle( AngleHMS( 3, 48, 16.43 ) ).Degrees(), &ok );
    TESTCHECKF( polaris2050J2050.Declination().Degrees(),
                Angle( AngleDMS( 89, 27, 15.38 ) ).Degrees(), &ok );
    double j2100 = Epoch( 2100, Epoch::Julian ).JulianDay();
    diffYrs = (j2100 - J2000) / 365.25;
    Angle ra2100J2000 = polaris2000J2000.RightAscension()
            +  diffYrs * raPropMotion;
    Angle dec2100J2000 = polaris2000J2000.Declination()
            +  diffYrs * decPropMotion;
    Equatorial polaris2100J2000( ra2100J2000, dec2100J2000 );
    precession = Precession( j2100, J2000 );
    Equatorial polaris2100J2100 = precession.Reduce( polaris2100J2000 );
    TESTCHECKF( polaris2100J2100.RightAscension().Degrees(),
                Angle( AngleHMS( 5, 53, 29.17 ) ).Degrees(), &ok );
    TESTCHECKF( polaris2100J2100.Declination().Degrees(),
                Angle( AngleDMS( 89, 32, 22.18 ) ).Degrees(), &ok );
    
    if ( ok )
        cout << "Precession PASSED." << endl << endl;
    else
        cout << "Precession FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

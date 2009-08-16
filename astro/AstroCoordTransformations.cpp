/*
  AstroCoordTransformations.cpp
  Copyright (C) 2007 David M. Anderson

  Routines for transforming positions in space from one coordinate system to
  another. Includes transformations between various spherical coordinate
  systems and generation of matrices for transformations in rectangular
  coordinates.
  The formulae for spherical transformations are from Meeus, "Astronomical
  Algorithms".
*/


#include "AstroCoordTransformations.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "AngleDMS.hpp"
#include "AngleHMS.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Equatorial 
EclipticalToEquatorial( const Ecliptical & ecliptical, Angle obliquity )
{
    double sinLong = ecliptical.Longitude().Sin( );
    double cosLong = ecliptical.Longitude().Cos( );
    double sinLat = ecliptical.Latitude().Sin( );
    double cosLat = ecliptical.Latitude().Cos( );
    double tanLat = (cosLat == 0.)  ?  Angle::bazillion  :  sinLat / cosLat;
    double sinObl = obliquity.Sin( );
    double cosObl = obliquity.Cos( );
    Angle ra = ArcTan( sinLong * cosObl  -  tanLat * sinObl,  cosLong );
    ra.NormalizePositive( );
    Angle dec = ArcSin( sinLat * cosObl  +  sinLong * cosLat * sinObl );
    return Equatorial( ra, dec, ecliptical.Distance() );
}

//-----------------------------------------------------------------------------

Ecliptical 
EquatorialToEcliptical( const Equatorial & equatorial, Angle obliquity )
{
    double sinRA = equatorial.RightAscension().Sin( );
    double cosRA = equatorial.RightAscension().Cos( );
    double sinDec = equatorial.Declination().Sin( );
    double cosDec = equatorial.Declination().Cos( );
    double tanDec = (cosDec == 0.)  ?  Angle::bazillion  :  sinDec / cosDec;
    double sinObl = obliquity.Sin( );
    double cosObl = obliquity.Cos( );
    Angle lng = ArcTan( sinRA * cosObl  +  tanDec * sinObl,  cosRA );
    lng.NormalizePositive( );
    Angle lat = ArcSin( sinDec * cosObl  -  sinRA * cosDec * sinObl );
    return Ecliptical( lng, lat, equatorial.Distance() );
}

//-----------------------------------------------------------------------------

Horizontal 
EquatorialToHorizontal( const Equatorial & equatorial,
                        Angle localSiderealTime, Angle geographicLatitude )
{
    Angle hourAngle = localSiderealTime - equatorial.RightAscension();
    double sinHA = hourAngle.Sin( );
    double cosHA = hourAngle.Cos( );
    double sinDec = equatorial.Declination().Sin( );
    double cosDec = equatorial.Declination().Cos( );
    double tanDec = (cosDec == 0.)  ?  Angle::bazillion  :  sinDec / cosDec;
    double sinLat = geographicLatitude.Sin( );
    double cosLat = geographicLatitude.Cos( );
    Angle az = ArcTan( sinHA,  cosHA * sinLat  -  tanDec * cosLat );
    az += Angle( M_PI );
    az.NormalizePositive( );
    Angle alt = ArcSin( sinDec * sinLat  +  cosHA * cosDec * cosLat );
    return Horizontal( az, alt, equatorial.Distance() );
}

//-----------------------------------------------------------------------------

Horizontal 
EquatorialToHorizontal( const Equatorial & equatorial,
                        double localSiderealTime, Angle geographicLatitude )
{
    return EquatorialToHorizontal( equatorial,
                                   Angle( localSiderealTime, Angle::Cycle ),
                                   geographicLatitude );
}

//-----------------------------------------------------------------------------

Equatorial 
HorizontalToEquatorial( const Horizontal & horizontal,
                        Angle localSiderealTime, Angle geographicLatitude )
{
    Angle az = horizontal.Azimuth() - Angle( M_PI );
    double sinAz = az.Sin( );
    double cosAz = az.Cos( );
    double sinAlt = horizontal.Altitude().Sin( );
    double cosAlt = horizontal.Altitude().Cos( );
    double tanAlt = (cosAlt == 0.)  ?  Angle::bazillion  :  sinAlt / cosAlt;
    double sinLat = geographicLatitude.Sin( );
    double cosLat = geographicLatitude.Cos( );
    // (Meeus has an error here, which I've corrected.)
    Angle hourAngle = ArcTan( sinAz,  cosAz * sinLat  +  tanAlt * cosLat );
    Angle ra = localSiderealTime - hourAngle;
    ra.NormalizePositive( );
    Angle dec = ArcSin( sinAlt * sinLat  -  cosAz * cosAlt * cosLat );
    return Equatorial( ra, dec, horizontal.Distance() );
}

//-----------------------------------------------------------------------------

Equatorial 
HorizontalToEquatorial( const Horizontal & horizontal,
                        double localSiderealTime, Angle geographicLatitude )
{
    return HorizontalToEquatorial( horizontal,
                                   Angle( localSiderealTime, Angle::Cycle ),
                                   geographicLatitude );
}

//-----------------------------------------------------------------------------

Galactic 
EquatorialToGalactic( const Equatorial & equatorial )
{
    Angle galNorthRA( 192.25, Angle::Degree );
    Angle galNorthDec( 27.4, Angle::Degree );
    Angle longOffset( 33. + 270., Angle::Degree );
    Angle adjRA = galNorthRA  - equatorial.RightAscension();
    double sinAdjRA = adjRA.Sin( );
    double cosAdjRA = adjRA.Cos( );
    double sinDec = equatorial.Declination().Sin( );
    double cosDec = equatorial.Declination().Cos( );
    double tanDec = (cosDec == 0.)  ?  Angle::bazillion  :  sinDec / cosDec;
    double sinNDec = galNorthDec.Sin( );
    double cosNDec = galNorthDec.Cos( );
    Angle x = ArcTan( sinAdjRA,  cosAdjRA * sinNDec  -  tanDec * cosNDec );
    Angle lng = longOffset - x;
    lng.NormalizePositive( );
    Angle lat = ArcSin( sinDec * sinNDec  +  cosAdjRA * cosDec * cosNDec );
    return Galactic( lng, lat, equatorial.Distance() );
}

//-----------------------------------------------------------------------------

Equatorial 
GalacticToEquatorial( const Galactic & galactic )
{
    Angle galNorthRAAdj( 192.25 - 180., Angle::Degree );
    Angle galNorthDec( 27.4, Angle::Degree );
    Angle longOffset( 33. + 90., Angle::Degree );
    Angle adjLong = galactic.Longitude() - longOffset;
    double sinAdjLong = adjLong.Sin( );
    double cosAdjLong = adjLong.Cos( );
    double sinLat = galactic.Latitude().Sin( );
    double cosLat = galactic.Latitude().Cos( );
    double tanLat = (cosLat == 0.)  ?  Angle::bazillion  :  sinLat / cosLat;
    double sinNDec = galNorthDec.Sin( );
    double cosNDec = galNorthDec.Cos( );
    Angle y = ArcTan( sinAdjLong,  cosAdjLong * sinNDec  -  tanLat * cosNDec );
    Angle ra = y + galNorthRAAdj;
    ra.NormalizePositive( );
    Angle dec = ArcSin( sinLat * sinNDec  +  cosAdjLong * cosLat * cosNDec );
    return Equatorial( ra, dec, galactic.Distance() );
}

//=============================================================================

Matrix3D 
EclipticalToEquatorialMatrix( Angle obliquity )
{
    return Matrix3D( 0, obliquity );
}

//-----------------------------------------------------------------------------

Matrix3D 
EquatorialToEclipticalMatrix( Angle obliquity )
{
    return Matrix3D( 0, - obliquity );
}

//-----------------------------------------------------------------------------

Matrix3D 
EquatorialToHorizontalMatrix( Angle localSiderealTime,
                              Angle geographicLatitude )
{
    // This is equivalent to
    // Matrix3D( 2, pi ) * Matrix3D( 1, pi/2 - lat )
    //  * Matrix3D( 1,0,0, 0,-1,0, 0,0,1 ) * Matrix3D( 2, lst ).
    double sinLST = localSiderealTime.Sin( );
    double cosLST = localSiderealTime.Cos( );
    double sinLat = geographicLatitude.Sin( );
    double cosLat = geographicLatitude.Cos( );
    return Matrix3D( - cosLST * sinLat,  - sinLST * sinLat,  cosLat,
                     - sinLST,           cosLST,             0,
                     cosLST * cosLat,    sinLST * cosLat,    sinLat );
}

//-----------------------------------------------------------------------------

Matrix3D 
EquatorialToHorizontalMatrix( double localSiderealTime,
                              Angle geographicLatitude )
{
    return EquatorialToHorizontalMatrix( Angle( localSiderealTime,
                                                Angle::Cycle ),
                                         geographicLatitude );
}

//-----------------------------------------------------------------------------

Matrix3D 
HorizontalToEquatorialMatrix( Angle localSiderealTime,
                              Angle geographicLatitude )
{
    // This is equivalent to
    // Matrix3D( 2, -lst ) * Matrix3D( 1,0,0, 0,-1,0, 0,0,1 )
    //  * Matrix3D( 1, lat - pi/2 ) * Matrix3D( 2, pi ).
    double sinLST = localSiderealTime.Sin( );
    double cosLST = localSiderealTime.Cos( );
    double sinLat = geographicLatitude.Sin( );
    double cosLat = geographicLatitude.Cos( );
    return Matrix3D( - cosLST * sinLat,  - sinLST,  cosLST * cosLat,
                     - sinLST * sinLat,  cosLST,    sinLST * cosLat,
                     cosLat,             0,         sinLat );
}

//-----------------------------------------------------------------------------

Matrix3D 
HorizontalToEquatorialMatrix( double localSiderealTime,
                              Angle geographicLatitude )
{
    return HorizontalToEquatorialMatrix( Angle( localSiderealTime,
                                                Angle::Cycle ),
                                         geographicLatitude );
}

//-----------------------------------------------------------------------------

Matrix3D 
EquatorialToGalacticMatrix( )
{
    // This is equivalent to
    // Matrix3D( 2, -33deg ) * Matrix3D( 0, 90 - 27.4deg )
    //  * Matrix3D( 2, 192.25 - 270deg ).
    return Matrix3D( -0.06698873941515, -0.87275576585199, -0.48353891463218,
                      0.49272846607532, -0.45034695801996,  0.74458463328303,
                     -0.86760081115143, -0.18837460172292,  0.46019978478385 );
}

//-----------------------------------------------------------------------------

Matrix3D 
GalacticToEquatorialMatrix( )
{
    // This is equivalent to
    // Matrix( 2, 270 - 192.25deg ) * Matrix3D( 0, 27.4 - 90deg)
    //  * Matrix3D( 2, 33deg ).
    return Matrix3D( -0.06698873941515,  0.49272846607532, -0.86760081115143,
                     -0.87275576585199, -0.45034695801996, -0.18837460172292,
                     -0.48353891463218,  0.74458463328303,  0.46019978478385 );
}

//=============================================================================

Angle 
EclipticalLongitude( const Point3D & equatorialRect, Angle obliquity )
{
    double x = equatorialRect.X();
    double y = obliquity.Cos() * equatorialRect.Y()
            +  obliquity.Sin() * equatorialRect.Z();
    if ( (x == 0.) && (y == 0.) )
        return Angle( 0. );
    else
        return ArcTan( y, x );
}

//=============================================================================

#ifdef DEBUG

bool 
TestAstroCoordTransformations( )
{
    bool ok = true;
    cout << "Testing AstroCoordTransformations" << endl;

    int h = 7;
    int m = 45;
    double s = 18.946;
    cout << "raPollux: AngleHMS( " << h << ", " << m << ", " << s << " )" << endl;
    Angle raPollux( AngleHMS( h, m, s ) );
    int d = 28;
    m = 1;
    s = 34.26;
    cout << "decPollux: AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    Angle decPollux( AngleDMS( d, m, s ) );
    Equatorial equatPollux( raPollux, decPollux );
    TESTCHECKF( equatPollux.RightAscension().Degrees(), 116.328942, &ok );
    TESTCHECKF( equatPollux.Declination().Degrees(), 28.026183, &ok );
    TESTCHECKF( equatPollux.Distance(), 1., &ok );
    double obl = 23.4392911;
    cout << "obliquity: " << obl << endl;
    Angle obliquity( obl, Angle::Degree );
    cout << "EquatorialToEcliptical( equatPollux, " << obl << " )" << endl;
    Ecliptical ecliptPollux = EquatorialToEcliptical( equatPollux, obliquity );
    TESTCHECKF( ecliptPollux.Longitude().Degrees(), 113.21562958, &ok );
    TESTCHECKF( ecliptPollux.Latitude().Degrees(), 6.6841698, &ok );
    TESTCHECKF( ecliptPollux.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    Point3D equatPolluxV = equatPollux.Rectangular();
    Matrix3D equat2eclipt = EquatorialToEclipticalMatrix( obliquity );
    Point3D ecliptPolluxV = equat2eclipt * equatPolluxV;
    ecliptPollux.Set( ecliptPolluxV );
    TESTCHECKF( ecliptPollux.Longitude().Degrees(), 113.215630, &ok );
    TESTCHECKF( ecliptPollux.Latitude().Degrees(), 6.684170, &ok );
    TESTCHECKF( ecliptPollux.Distance(), 1., &ok );
    TESTCHECKF( EclipticalLongitude( equatPolluxV, obliquity ).Degrees(),
                113.215630, &ok );
    cout << "EclipticalToEquatorial( ecliptPollux, " << obl << " )" << endl;
    equatPollux = EclipticalToEquatorial( ecliptPollux, obliquity );
    TESTCHECKF( equatPollux.RightAscension().Degrees(), 116.328942, &ok );
    TESTCHECKF( equatPollux.Declination().Degrees(), 28.026183, &ok );
    TESTCHECKF( equatPollux.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    Matrix3D eclipt2equat = EclipticalToEquatorialMatrix( obliquity );
    equatPolluxV = eclipt2equat * ecliptPolluxV;
    equatPollux.Set( equatPolluxV );
    TESTCHECKF( equatPollux.RightAscension().Degrees(), 116.328942, &ok );
    TESTCHECKF( equatPollux.Declination().Degrees(), 28.026183, &ok );
    TESTCHECKF( equatPollux.Distance(), 1., &ok );

    h = 23;
    m = 9;
    s = 16.641;
    cout << "raVenus: AngleHMS( " << h << ", " << m << ", " << s << " )" << endl;
    Angle raVenus( AngleHMS( h, m, s ) );
    d = -6;
    m = 43;
    s = 11.61;
    cout << "decVenus: AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    Angle decVenus( AngleDMS( d, m, s ) );
    Equatorial equatVenus( raVenus, decVenus );
    TESTCHECKF( equatVenus.RightAscension().Degrees(), 347.3193375, &ok );
    TESTCHECKF( equatVenus.Declination().Degrees(), -6.71988889, &ok );
    TESTCHECKF( equatVenus.Distance(), 1., &ok );
    h = 3;
    m = 26;
    s = 41.153;
    cout << "local sidereal time: " << h << "h" << m << "m" << s << "s" << endl;
    AngleHMS lstHMS( h, m, s );
    Angle lst( lstHMS );
    d = 38;
    m = 55;
    s = 17;
    cout << "lat U.S.N.O.: AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    Angle latUSNO( AngleDMS( d, m, s ) );
    cout << "EquatorialToHorizontal( equatVenus, lst, latUSNO )" << endl;
    Horizontal horizVenus = EquatorialToHorizontal( equatVenus, lst, latUSNO );
    TESTCHECKF( horizVenus.Azimuth().Degrees(), 248.03369608, &ok );
    TESTCHECKF( horizVenus.Altitude().Degrees(), 15.12487568, &ok );
    TESTCHECKF( horizVenus.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    Point3D equatVenusV = equatVenus.Rectangular();
    Matrix3D equat2horiz = EquatorialToHorizontalMatrix( lst, latUSNO );
    Point3D horizVenusV = equat2horiz * equatVenusV;
    horizVenus.Set( horizVenusV );
    TESTCHECKF( horizVenus.Azimuth().Degrees(), 248.03369608, &ok );
    TESTCHECKF( horizVenus.Altitude().Degrees(), 15.12487568, &ok ); 
    TESTCHECKF( horizVenus.Distance(), 1., &ok );
    cout << "HorizontalToEquatorial( horizVenus, lst, latUSNO )" << endl;
    equatVenus = HorizontalToEquatorial( horizVenus, lst, latUSNO );
    TESTCHECKF( equatVenus.RightAscension().Degrees(), 347.3193375, &ok );
    TESTCHECKF( equatVenus.Declination().Degrees(), -6.71988889, &ok );
    TESTCHECKF( equatVenus.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    horizVenusV = horizVenus.Rectangular();
    Matrix3D horiz2equat = HorizontalToEquatorialMatrix( lst, latUSNO );
    equatVenusV = horiz2equat * horizVenusV;
    equatVenus.Set( equatVenusV );
    TESTCHECKF( equatVenus.RightAscension().Degrees(), 347.3193375, &ok );
    TESTCHECKF( equatVenus.Declination().Degrees(), -6.71988889, &ok );
    TESTCHECKF( equatVenus.Distance(), 1., &ok );

    h = 17;
    m = 48;
    s = 59.74;
    cout << "raNovaSer: AngleHMS( " << h << ", " << m << ", " << s << " )" << endl;
    Angle raNovaSer( AngleHMS( h, m, s ) );
    d = -14;
    m = 43;
    s = 8.2;
    cout << "decNovaSer: AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    Angle decNovaSer( AngleDMS( d, m, s ) );
    Equatorial equatNovaSer( raNovaSer, decNovaSer );
    TESTCHECKF( equatNovaSer.RightAscension().Degrees(), 267.248917, &ok );
    TESTCHECKF( equatNovaSer.Declination().Degrees(), -14.7189444, &ok );
    cout << "EquatorialToGalactic( equatNovaSer )" << endl;
    Galactic galNovaSer = EquatorialToGalactic( equatNovaSer );
    TESTCHECKF( galNovaSer.Longitude().Degrees(), 12.95925004, &ok );
    TESTCHECKF( galNovaSer.Latitude().Degrees(), 6.0462985, &ok );
    TESTCHECKF( galNovaSer.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    Point3D equatNovaSerV = equatNovaSer.Rectangular();
    Matrix3D equat2gal = EquatorialToGalacticMatrix( );
    Point3D galNovaSerV = equat2gal * equatNovaSerV;
    galNovaSer.Set( galNovaSerV );
    TESTCHECKF( galNovaSer.Longitude().Degrees(), 12.95925004, &ok );
    TESTCHECKF( galNovaSer.Latitude().Degrees(), 6.0462985, &ok );
    TESTCHECKF( galNovaSer.Distance(), 1., &ok );
    cout << "GalacticToEquatorial( galNovaSer )" << endl;
    equatNovaSer = GalacticToEquatorial( galNovaSer );
    TESTCHECKF( equatNovaSer.RightAscension().Degrees(), 267.248917, &ok );
    TESTCHECKF( equatNovaSer.Declination().Degrees(), -14.7189444, &ok );
    TESTCHECKF( equatNovaSer.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    galNovaSerV = galNovaSer.Rectangular();
    Matrix3D gal2equat = GalacticToEquatorialMatrix( );
    equatNovaSerV = gal2equat * galNovaSerV;
    equatNovaSer.Set( equatNovaSerV );
    TESTCHECKF( equatNovaSer.RightAscension().Degrees(), 267.248917, &ok );
    TESTCHECKF( equatNovaSer.Declination().Degrees(), -14.7189444, &ok );
    TESTCHECKF( equatNovaSer.Distance(), 1., &ok );
    
    h = 16;
    m = 26;
    s = 29.19;
    cout << "raTest1: AngleHMS( " << h << ", " << m << ", " << s << " )" << endl;
    Angle raTest1( AngleHMS( h, m, s ) );
    d = -21;
    m = 13;
    s = 38.68;
    cout << "decTest1: AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    Angle decTest1( AngleDMS( d, m, s ) );
    Equatorial equatTest1( raTest1, decTest1 );
    h = 1;
    m = 14;
    s = 45.2959;
    cout << "local sidereal time: " << h << "h" << m << "m" << s << "s" << endl;
    lstHMS.Set( h, m, s );
    lst.Set( lstHMS );
    d = 35;
    m = 43;
    s = 59;
    cout << "lat: AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    Angle latTest1( AngleDMS( d, m, s ) );
    cout << "EquatorialToHorizontal( equatTest1, lst, latTest1 )" << endl;
    Horizontal horizTest1 = EquatorialToHorizontal( equatTest1, lst, latTest1 );
    AngleDMS dmsTest1Az( horizTest1.Azimuth() );
    TESTCHECK( dmsTest1Az.Positive(), true, &ok );
    TESTCHECK( dmsTest1Az.Degrees(), 275, &ok );
    TESTCHECK( dmsTest1Az.Minutes(), 50, &ok );
    TESTCHECKFE( dmsTest1Az.Seconds(), 38.9193, &ok, 1e-3 );
    AngleDMS dmsTest1Alt( horizTest1.Altitude() );
    TESTCHECK( dmsTest1Alt.Positive(), false, &ok );
    TESTCHECK( dmsTest1Alt.Degrees(), 45, &ok );
    TESTCHECK( dmsTest1Alt.Minutes(), 55, &ok );
    TESTCHECKFE( dmsTest1Alt.Seconds(), 27.7358, &ok, 1e-3 );
    TESTCHECKF( horizTest1.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    Point3D equatTest1V = equatTest1.Rectangular();
    equat2horiz = EquatorialToHorizontalMatrix( lst, latTest1 );
    Point3D horizTest1V = equat2horiz * equatTest1V;
    horizTest1.Set( horizTest1V );
    dmsTest1Az.Set( horizTest1.Azimuth() );
    TESTCHECK( dmsTest1Az.Positive(), true, &ok );
    TESTCHECK( dmsTest1Az.Degrees(), 275, &ok );
    TESTCHECK( dmsTest1Az.Minutes(), 50, &ok );
    TESTCHECKFE( dmsTest1Az.Seconds(), 38.9193, &ok, 1e-3 );
    dmsTest1Alt.Set( horizTest1.Altitude() );
    TESTCHECK( dmsTest1Alt.Positive(), false, &ok );
    TESTCHECK( dmsTest1Alt.Degrees(), 45, &ok );
    TESTCHECK( dmsTest1Alt.Minutes(), 55, &ok );
    TESTCHECKFE( dmsTest1Alt.Seconds(), 27.7358, &ok, 1e-3 );
    TESTCHECKF( horizTest1.Distance(), 1., &ok );
    cout << "HorizontalToEquatorial( horizTest1, lst, latTest1 )" << endl;
    equatTest1 = HorizontalToEquatorial( horizTest1, lst, latTest1 );
    AngleHMS hmsTest1RA( equatTest1.RightAscension() );
    TESTCHECK( hmsTest1RA.Hours(), 16, &ok );
    TESTCHECK( hmsTest1RA.Minutes(), 26, &ok );
    TESTCHECKFE( hmsTest1RA.Seconds(), 29.19, &ok, 1e-3 );
    AngleDMS dmsTest1Dec( equatTest1.Declination() );
    TESTCHECK( dmsTest1Dec.Positive(), false, &ok );
    TESTCHECK( dmsTest1Dec.Degrees(), 21, &ok );
    TESTCHECK( dmsTest1Dec.Minutes(), 13, &ok );
    TESTCHECKFE( dmsTest1Dec.Seconds(), 38.68, &ok, 1e-3 );
    TESTCHECKF( equatTest1.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    horizTest1V = horizTest1.Rectangular();
    horiz2equat = HorizontalToEquatorialMatrix( lst, latTest1 );
    equatTest1V = horiz2equat * horizTest1V;
    equatTest1.Set( equatTest1V );
    hmsTest1RA.Set( equatTest1.RightAscension() );
    TESTCHECK( hmsTest1RA.Hours(), 16, &ok );
    TESTCHECK( hmsTest1RA.Minutes(), 26, &ok );
    TESTCHECKFE( hmsTest1RA.Seconds(), 29.19, &ok, 1e-3 );
    dmsTest1Dec.Set( equatTest1.Declination() );
    TESTCHECK( dmsTest1Dec.Positive(), false, &ok );
    TESTCHECK( dmsTest1Dec.Degrees(), 21, &ok );
    TESTCHECK( dmsTest1Dec.Minutes(), 13, &ok );
    TESTCHECKF( dmsTest1Dec.Seconds(), 38.68, &ok );
    TESTCHECKFE( equatTest1.Distance(), 1., &ok, 1e-3 );

    h = 16;
    m = 26;
    s = 29.399;
    cout << "raTest2: AngleHMS( " << h << ", " << m << ", " << s << " )" << endl;
    Angle raTest2( AngleHMS( h, m, s ) );
    d = -21;
    m = 13;
    s = 38.39;
    cout << "decTest2: AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    Angle decTest2( AngleDMS( d, m, s ) );
    Equatorial equatTest2( raTest2, decTest2 );
    d = 23;
    m = 26;
    s = 15.791;
    cout << "obliquity: AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    obliquity.Set( AngleDMS( d, m, s ) );
    cout << "EquatorialToEcliptical( equatTest2, obliquity )" << endl;
    Ecliptical ecliptTest2 = EquatorialToEcliptical( equatTest2, obliquity );
    AngleDMS dmsTest2Long( ecliptTest2.Longitude() );
    TESTCHECK( dmsTest2Long.Positive(), true, &ok );
    TESTCHECK( dmsTest2Long.Degrees(), 248, &ok );
    TESTCHECK( dmsTest2Long.Minutes(), 17, &ok );
    TESTCHECKFE( dmsTest2Long.Seconds(), 30.82, &ok, 1e-3 );
    AngleDMS dmsTest2Lat( ecliptTest2.Latitude() );
    TESTCHECK( dmsTest2Lat.Positive(), true, &ok );
    TESTCHECK( dmsTest2Lat.Degrees(), 0, &ok );
    TESTCHECK( dmsTest2Lat.Minutes(), 27, &ok );
    TESTCHECKFE( dmsTest2Lat.Seconds(), 57.59, &ok, 1e-3 );
    TESTCHECKF( ecliptTest2.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    Point3D equatTest2V = equatTest2.Rectangular();
    equat2eclipt = EquatorialToEclipticalMatrix( obliquity );
    Point3D ecliptTest2V = equat2eclipt * equatTest2V;
    ecliptTest2.Set( ecliptTest2V );
    dmsTest2Long.Set( ecliptTest2.Longitude() );
    TESTCHECK( dmsTest2Long.Positive(), true, &ok );
    TESTCHECK( dmsTest2Long.Degrees(), 248, &ok );
    TESTCHECK( dmsTest2Long.Minutes(), 17, &ok );
    TESTCHECKFE( dmsTest2Long.Seconds(), 30.82, &ok, 1e-3 );
    dmsTest2Lat.Set( ecliptTest2.Latitude() );
    TESTCHECK( dmsTest2Lat.Positive(), true, &ok );
    TESTCHECK( dmsTest2Lat.Degrees(), 0, &ok );
    TESTCHECK( dmsTest2Lat.Minutes(), 27, &ok );
    TESTCHECKFE( dmsTest2Lat.Seconds(), 57.59, &ok, 1e-3 );
    TESTCHECKF( ecliptTest2.Distance(), 1., &ok );
    cout << "EclipticalToEquatorial( ecliptTest2, obliquity )" << endl;
    equatTest2 = EclipticalToEquatorial( ecliptTest2, obliquity );
    AngleHMS hmsTest2RA( equatTest2.RightAscension() );
    TESTCHECK( hmsTest2RA.Hours(), 16, &ok );
    TESTCHECK( hmsTest2RA.Minutes(), 26, &ok );
    TESTCHECKFE( hmsTest2RA.Seconds(), 29.399, &ok, 1e-3 );
    AngleDMS dmsTest2Dec( equatTest2.Declination() );
    TESTCHECK( dmsTest2Dec.Positive(), false, &ok );
    TESTCHECK( dmsTest2Dec.Degrees(), 21, &ok );
    TESTCHECK( dmsTest2Dec.Minutes(), 13, &ok );
    TESTCHECKFE( dmsTest2Dec.Seconds(), 38.39, &ok, 1e-3 );
    TESTCHECKF( equatTest2.Distance(), 1., &ok );
    cout << "Same test, using points and matrices" << endl;
    ecliptTest2V = ecliptTest2.Rectangular();
    eclipt2equat = EclipticalToEquatorialMatrix( obliquity );
    equatTest2V = eclipt2equat * ecliptTest2V;
    equatTest2.Set( equatTest2V );
    hmsTest2RA.Set( equatTest2.RightAscension() );
    TESTCHECK( hmsTest2RA.Hours(), 16, &ok );
    TESTCHECK( hmsTest2RA.Minutes(), 26, &ok );
    TESTCHECKFE( hmsTest2RA.Seconds(), 29.399, &ok, 1e-3 );
    dmsTest2Dec.Set( equatTest2.Declination() );
    TESTCHECK( dmsTest2Dec.Positive(), false, &ok );
    TESTCHECK( dmsTest2Dec.Degrees(), 21, &ok );
    TESTCHECK( dmsTest2Dec.Minutes(), 13, &ok );
    TESTCHECKF( dmsTest2Dec.Seconds(), 38.39, &ok );
    TESTCHECKFE( equatTest2.Distance(), 1., &ok, 1e-3 );


    if ( ok )
        cout << "AstroCoordTransformations PASSED." << endl << endl;
    else
        cout << "AstroCoordTransformations FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

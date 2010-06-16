/*
  MayanLongCountCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  MayanLongCountCalendar class, which defines the Mayan long count calendar.
*/


#include "MayanLongCountCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


static const long s_mayanLongCountEpoch = 584283;


//=============================================================================


void 
MayanLongCountCalendar::JulianDayToLongCount( long julianDay,
                                              int * pKin, int * pUinal,
                                              int * pTun, int * pKatun,
                                              int * pBaktun, int * pPictun,
                                              int * pCalabtun,
                                              int * pKinchiltun,
                                              int * pAlautun )
{
    long d = julianDay - s_mayanLongCountEpoch;
    long kin;
    DivModP( d, (long)KinInUinal, &d, &kin );
    long uinal;
    DivModP( d, (long)UinalInTun, &d, &uinal );
    long tun;
    DivModP( d, (long)TunInKatun, &d, &tun );
    long katun;
    DivModP( d, (long)KatunInBaktun, &d, &katun );
    long baktun;
    DivModP( d, (long)BaktunInPictun, &d, &baktun );
    long pictun;
    DivModP( d, (long)PictunInCalabtun, &d, &pictun );
    long calabtun;
    DivModP( d, (long)CalabtunInKinchiltun, &d, &calabtun );
    long kinchiltun;
    long alautun;
    DivModP( d, (long)KinchiltunInAlautun, &alautun, &kinchiltun );
    if ( pKin )
        *pKin = (int)kin;
    if ( pUinal )
        *pUinal = (int)uinal;
    if ( pTun )
        *pTun = (int)tun;
    if ( pKatun )
        *pKatun = (int)katun;
    if ( pBaktun )
        *pBaktun = (int)baktun;
    if ( pPictun )
        *pPictun = (int)pictun;
    if ( pCalabtun )
        *pCalabtun = (int)calabtun;
    if ( pKinchiltun )
        *pKinchiltun = (int)kinchiltun;
    if ( pAlautun )
        *pAlautun = (int)alautun;
}

//-----------------------------------------------------------------------------

long 
MayanLongCountCalendar::LongCountToJulianDay( int kin, int uinal, int tun,
                                              int katun, int baktun,
                                              int pictun, int calabtun,
                                              int kinchiltun,
                                              int alautun )
{
    long jd = s_mayanLongCountEpoch
            +  kin
            +  KinInUinal * (uinal
            +  UinalInTun * (tun
            +  TunInKatun * (katun
            +  KatunInBaktun * (baktun
            +  BaktunInPictun * (pictun
            +  PictunInCalabtun * (calabtun
            +  CalabtunInKinchiltun * kinchiltun
            +  KinchiltunInAlautun * (long)alautun))))));
    return jd;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

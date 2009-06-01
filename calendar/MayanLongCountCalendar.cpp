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


static const int s_mayanLongCountEpoch = 584283;


//=============================================================================


void 
MayanLongCountCalendar::JulianDayToLongCount( int julianDay,
                                              int * pKin, int * pUinal,
                                              int * pTun, int * pKatun,
                                              int * pBaktun, int * pPictun,
                                              int * pCalabtun,
                                              int * pKinchiltun,
                                              int * pAlautun )
{
    int rem;
    int calabtun;
    DivModP( julianDay - s_mayanLongCountEpoch, 57600000, &calabtun, &rem );
    int pictun;
    DivModP( rem, 2880000, &pictun, &rem );
    int baktun;
    DivModP( rem, 144000, &baktun, &rem );
    int katun;
    DivModP( rem, 7200, &katun, &rem );
    int tun;
    DivModP( rem, 360, &tun, &rem );
    int uinal;
    int kin;
    DivModP( rem, 20, &uinal, &kin );
    if ( pKin )
        *pKin = kin;
    if ( pUinal )
        *pUinal = uinal;
    if ( pTun )
        *pTun = tun;
    if ( pKatun )
        *pKatun = katun;
    if ( pBaktun )
        *pBaktun = baktun;
    if ( pPictun )
        *pPictun = pictun;
    if ( pCalabtun )
        *pCalabtun = calabtun;
    if ( pKinchiltun )
        *pKinchiltun = 0;
    if ( pAlautun )
        *pAlautun = 0;
}

//-----------------------------------------------------------------------------

int 
MayanLongCountCalendar::LongCountToJulianDay( int kin, int uinal, int tun,
                                              int katun, int baktun,
                                              int pictun, int calabtun,
                                              int /*kinchiltun*/,
                                              int /*alautun*/ )
{
    int jd = s_mayanLongCountEpoch  +  kin  +  uinal * 20  +  tun * 360
            +  katun * 7200  +  baktun * 144000
            +  pictun * 2880000  +  calabtun * 57600000;
    return jd;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

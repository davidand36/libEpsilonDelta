#ifndef MAYANLONGCOUNTCALENDAR_HPP
#define MAYANLONGCOUNTCALENDAR_HPP
/*
  MayanLongCountCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  MayanLongCountCalendar class, which defines the Mayan long count calendar.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MayanLongCountCalendar
{
public:
    static void JulianDayToLongCount( long julianDay,
                                      int * pKin, int * pUinal, int * pTun,
                                      int * pKatun, int * pBaktun,
                                      int * pPictun = 0, int * pCalabtun = 0,
                                      int * pKinchiltun = 0, int * pAlautun = 0 );
    static long LongCountToJulianDay( int kin, int uinal, int tun,
                                      int katun, int baktun,
                                      int pictun = 0, int calabtun = 0,
                                      int kinchiltun = 0, int alautun = 0 );
    static const int KinInUinal = 20;
    static const int UinalInTun = 18;
    static const int TunInKatun = 20;
    static const int KatunInBaktun = 20;
    static const int BaktunInPictun = 20;
    static const int PictunInCalabtun = 20;
    static const int CalabtunInKinchiltun = 20;
    static const int KinchiltunInAlautun = 20;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MAYANLONGCOUNTCALENDAR_HPP

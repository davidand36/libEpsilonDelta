#ifndef MAYANLONGCOUNTCALENDAR_HPP
#define MAYANLONGCOUNTCALENDAR_HPP
/*
  MayanLongCountCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  MayanLongCountCalendar class, which defines the Mayan long count calendar.

  The long count is one of the three predominant calendars of the Mayan
  civilization, which has lasted for thousands of years, notably flourishing
  from about 250 to 900 C.E. It is the calendar used to track long periods
  of time.

  It is essentially a base-twenty positional notation, with one
  exception: there are 20 k'in (days) in one uinal, but only 18 winal (or
  uinal) in one tun, so that a tun is roughly a solar year. There are then 20
  tun in a k'atun, 20 k'atun in a b'ak'tun, and 20 b'ak'tun in a pictun, with
  even longer units occasionally referenced that would cover billions of years.
  Note that the Mayans were ahead of their European contemporaries not only in
  developing positional notation but also in their use of zero.
  (The recent New Age proposition that the Mayans predicted the end of the
  world at the end of b'ak'tun 12 is both a misinterpretation of Mayan beliefs
  and a prediction that we shall soon see to be false.)

  The epoch of the Mayan long count is generally accepted as that given by
  the Goodman-Martinez-Thompson correlation, Julian Day 584283, i.e.,
  11 August 3114 B.C.E. (Gregorian).
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

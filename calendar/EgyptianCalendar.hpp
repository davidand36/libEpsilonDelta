#ifndef EGYPTIANCALENDAR_HPP
#define EGYPTIANCALENDAR_HPP
/*
  EgyptianCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class EgyptianCalendar, which defines the Egyptian calendar.

  This is the principle calendar that was in use for thousands of years not
  only in ancient Egypt but as far away as Babylonia and Persia. It was also
  used for astronomical ephemerides by Copernicus and other astronomers,
  no doubt in part because of its simplicity and also because it was
  the calendar of the Alexandrian astronomer Ptolemy.

  Although the structure remained constant, the naming of months changed over
  time. Here we use the Hellenized names used at the time of Ptolemy. In
  the Old Kingdom the months were numbered within each of the three seasons,
  Axt (inundation), Prt (growth), and Smw (harvest). Later month names were
  used as follows:
  Old Kingdom | Middle Kingdom | New Kingdom    | Greek
  ------------|----------------|----------------|-----------
  1st of Axt  | Tekh           | Dhwt           | Thoth
  2nd of Axt  | Menhet         | Pa-n-ip.t      | Phaophi
  3rd of Axt  | Hwt-hwr        | Hwt-hwr        | Athyr
  4th of Axt  | Ka-hr-ka       | Ka-hr-ka       | Cholak
  1st of Prt  | Sf-bdt         | Ta-'b          | Tybi
  2nd of Prt  | Rekh wer       | Mḫyr           | Mechir
  3rd of Prt  | Rekh neds      | Pa-n-amn-htp.w | Phamenoth
  4th of Prt  | Renwet         | pa-n-mn.t      | Pharmouthi
  1st of Smw  | Hnsw           | Pa-n-ḫns.w     | Pachon
  2nd of Smw  | Hnd-htj        | Pa-n-in.t      | Paoni
  3rd of Smw  | Ipt-hmt        | Ipip           | Epiphi
  4th of Smw  | Wep-renpet     | Msw-r'         | Mesore

  Strictly speaking, the Egyptian calendar is neither lunar nor solar: Both
  because the the synodic lunar month is only about 29-1/2 days and because
  of the epigomenae, the New Moon would have occurred at different days in
  different months of different years. And because the sidereal and tropical
  years are about 365-1/4 days, the heliacal rising of Sirius and the annual
  flooding of the Nile would also have drifted through the Egyptian calendar
  year with a cycle of about 1460 years.

  The epoch used here is that adopted by Ptolemy, the Nabonassar era, is
  Julian Day 1448638, corresponding to 18 Feb 747 B.C.E. (Gregorian).
  Obviously the calendar was in use well before this.
*/


#include "Assert.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class EgyptianCalendar
{
public:
    static void JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear );
    static long DMYToJulianDay( int day, int month, long year );
    static int MonthsInYear( long year = 0 );
    static int DaysInMonth( int month, long year = 0 );
    static const std::string & MonthName( int month, long year );
    static const std::string & MonthName( int month );

    enum EMonth
    { Thoth = 1, Phaophi, Athyr, Choiak, Tybi, Mechir, Phamenoth,
      Pharmuthi, Pachon, Payni, Epiphi, Mesori, Epagomenae };
};


//*****************************************************************************


inline
int
EgyptianCalendar::DaysInMonth( int month, long /*year*/ )
{
    Assert( (month > 0) && (month <= MonthsInYear( )) );
    return  ( (month < 13)  ?  30  :  5 );
}

//-----------------------------------------------------------------------------

inline
int
EgyptianCalendar::MonthsInYear( long /*year*/ )
{
    return 13;  //including the Epagomenae.
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EGYPTIANCALENDAR_HPP

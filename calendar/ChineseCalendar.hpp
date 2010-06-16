#ifndef CHINESECALENDAR_HPP
#define CHINESECALENDAR_HPP
/*
  ChineseCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class ChineseCalendar, which defines the Chinese calendar.

  The Chinese calendar is used in much of East Asia, particularly for festivals
  and holidays. Historical usage dates from about 1384 B.C., though Chinese 
  tradition is that the calendar was invented by the Yellow Emperor, Huang Di,
  in 2637 B.C. (For civil purposes, the Gregorian calendar has been used since
  1912 A.D.)
  There have been a number of calendar reforms. The calendar implemented here
  is the modern one, developed by Jesuit missionaries in 1645 A.D. It is an
  astronomical calendar, in that the true longitudes of the Sun and Moon are
  used. (Originally, motions of the mean Sun and Moon were used. The reform of
  619 A.D. introduced the true Moon, but retained the mean Sun.)
  The Chinese calendar is a lunisolar calendar, meaning that months are based
  on actual lunar phases (beginning with the New Moon), but leap months are
  intercalated so that the Winter Solstice always falls in the eleventh month.
  NOTES:
  1. Years are not generally counted in a linear fashion, as here.
     Traditionally, the year count began in the year following the death of
     the previous emperor, though some emperors divided their reigns into
     regnal periods (nien hao). In addition, years are also denoted by their
     position in 10- and 12-year cycles, which together constitute a 60-year
     cycle. There is some disagreement about numbering these cycles, however.
     Here we adopt the convention that begins the first cycle in 2637 B.C.
     There are also 60-day and 60-month cycles.
  2. Aside from the matter of the year discussed in Note 1, the common
     representation of a Chinese date is
     { int day, int month, bool leap, long year },
     where month ranges from 1 to 12, and there may be two months in a year
     with the same number, the second of which is a leap month. I denote this
     representation as DMLY.
  3. An internal representation which seems to be easier to work with
     computationally is what I call DMYL:
     { int day, int month, long year, int leapMonth },
     where month ranges from 1 to 12, or 1 to 13 in years that contain a leap
     month, and leapMonth is the leapMonth for that year. (leapMonth = LMNone
     if there is no leap month, and leapMonth = LMUnknown if it has not yet
     been determined.)
*/


#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class ChineseCalendar
{
public:
    static void JulianDayToDMYL( long julianDay,
                                 int * pDay, int * pMonth, long * pYear,
                                 int * pLeapMonth );
    static void JulianDayToDMLY( long julianDay,
                                 int * pDay, int * pMonth, bool * pLeap,
                                 long * pYear );
    static long DMYToJulianDay( int day, int month, long year );
    static long DMLYToJulianDay( int day, int month, bool leap, long year );

    static bool Valid( int day, int month, long year, int leapMonth );
    static long MakeValid( int * pDay, int * pMonth, long * pYear,
                          int * pLeapMonth, DateFixup::EMethod fixupMethod );
    static int LeapMonth( long year );
    static int DaysInMonth( int month, long year );

    static void SolarTerms( long julianDay,
                            int * pMajorTerm, int * pMinorTerm );
    static long JDofNextSolarTerm( long julianDay, int term, bool major = true );

    static long SexagesimalToLinear( int cyclical, long cycle );
    static void LinearToSexagesimal( long linear,
                                     int * pCyclical, long * pCycle );
    static void StemAndBranch( int cyclical, int * pCelestialStem,
                               int * pTerrestrialBranch );
    static int StemToElement( int celestialStem );
    static int DayCyclical( long julianDay );
    static int MonthCyclical( int month, long year );
    static void DayStemAndBranch( long julianDay, int * pCelestialStem,
                                 int * pTerrestrialBranch );
    static void MonthStemAndBranch( int month, long year,
                                    int * pCelestialStem,
                                    int * pTerrestrialBranch );
    static void YearStemAndBranch( long year,
                                    int * pCelestialStem,
                                    int * pTerrestrialBranch );

    static const std::string & MajorSolarTermName( int term,
                                                   bool english = false );
    static const std::string & MinorSolarTermName( int term,
                                                   bool english = false );
    static const std::string & CelestialStemName( int stem,
                                                  bool english = false );
    static const std::string & TerrestrialBranchName( int branch,
                                                      bool english = false );
    static const std::string & ElementName( int element,
                                            bool english = false );
    
    
#ifdef DEBUG
    static bool Test( );
#endif

    enum { LMUnknown = 99, LMNone = 100 };
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CHINESECALENDAR_HPP

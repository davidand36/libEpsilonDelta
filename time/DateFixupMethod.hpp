#ifndef DATEFIXUPMETHOD_HPP
#define DATEFIXUPMETHOD_HPP
/*
  DateFixupMethod.hpp
  Copyright (C) 2007 David M. Anderson

  Enumeration for specifying the desired approach to fixing invalid dates.
  NOTES:
  1. It is not always obvious how to make an invalid date valid, so
     alternatives are provided.
     The Clamp method simply moves the month and then the day into the valid
     ranges for each. So, in the Gregorian calendar, 31 November 2000 becomes
     30 November 2000, -1 December 2000 becomes 1 December 2000, and
     32/14/2000 becomes 31/12/2000.
     The Carry method carries excesses or deficiencies in months into the year,
     and excesses or deficiencies in days into the month. So 31 November 2000
     becomes 1 December 2000, -1 December 2000 becomes 29 November 2000, and
     32/13/2000 becomes 32/1/2001 and then 1/2/2001.
     If the date is invalid because of an entry error, Clamp is probably the
     appropriate fixup method. On the other hand, if a valid date has been
     incremented or decremented out of range, Carry is probably appropriate.
  2. The enumeration is defined as a bitfield to allow for a possible 
     combination of methods. It may sometimes be desired only to clamp the day,
     so that, for example, 31/14/1999 becomes 31/2/2000 and then 29/2/2000.
  3. Other options, such as ClampMonth, although defined, are for internal 
     use and do not make sense as fixup methods. In particular, the algorithms 
     are free to assume that if carry is specified for a particular unit of 
     time, it applies to all larger units. Equivalently, if clamp is
     specified for a particular unit, it applies to all smaller units.
  4. It seems unwise for the library to attempt to offer other methods, such
     as a way to advance from the last day of one month to the last of the
     next, especially since this sort of thing can be implemented reasonably
     easily using methods already provided by the library.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace DateFixup
{                                                         //namespace DateFixup

//*****************************************************************************


enum EMethod 
{ 
    Carry = 0, 

    ClampDay       = (1 << 0),
    ClampMonth     = (1 << 1),
    ClampWeek      = (1 << 1),
    ClampYear      = (1 << 2),

    Clamp          = 0xFF
};


//*****************************************************************************

}                                                         //namespace DateFixup

}                                                      //namespace EpsilonDelta

#endif //DATEFIXUPMETHOD_HPP

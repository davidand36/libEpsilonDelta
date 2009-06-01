#ifndef MODIFIEDJULIANDAY_HPP
#define MODIFIEDJULIANDAY_HPP
/*
  ModifiedJulianDay.hpp
  Copyright (C) 2007 David M. Anderson

  Routines to convert between regular and Modified Julian Day.
  NOTES:
  1. See DateJD.hpp for an explanation of Julian Day.
  1. Modified Julian Day (MJD) is an abbreviated form, useful from about 1859
     to 2131.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


double JulianDayToMJD( double julianDay );
double MJDToJulianDay( double modifiedJulianDay );

#ifdef DEBUG
bool TestMJD( );
#endif


//*****************************************************************************


inline 
double 
JulianDayToMJD( double julianDay )
{
    return  julianDay - 2400000.5;
}

//-----------------------------------------------------------------------------

inline 
double 
MJDToJulianDay( double modifiedJulianDay )
{
    return  modifiedJulianDay + 2400000.5;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MODIFIEDJULIANDAY_HPP

#ifndef FRENCHREVOLUTIONARYDATE_HPP
#define FRENCHREVOLUTIONARYDATE_HPP
/*
  FrenchRevolutionaryDate.hpp
  Copyright (C) 2007 David M. Anderson

  FrenchRevolutionaryDate class, representing a date in the French
  Revolutionary calendar.
*/


#include "DateDMY.hpp"
#include "FrenchRevolutionaryCalendar.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FrenchRevolutionaryDate
    :   public DateDMY< FrenchRevolutionaryCalendar >
{
public:
    explicit FrenchRevolutionaryDate( bool today = false );
    explicit FrenchRevolutionaryDate( int julianDay );
    FrenchRevolutionaryDate( int day, int month, int year );
    explicit FrenchRevolutionaryDate( const DateJD & date );
    FrenchRevolutionaryDate( const FrenchRevolutionaryDate & date );
    virtual ~FrenchRevolutionaryDate( );
    virtual std::string ToString( const std::string & format ) const;
    
    virtual FrenchRevolutionaryDate & operator=( const DateJD & rhs );
    virtual FrenchRevolutionaryDate & operator=(
        const DateDMY< FrenchRevolutionaryCalendar > & rhs );

    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef class FrenchRevolutionaryCalendar Calendar;
};


//.............................................................................

#ifdef DEBUG
bool TestFrenchRevolutionaryDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FRENCHREVOLUTIONARYDATE_HPP

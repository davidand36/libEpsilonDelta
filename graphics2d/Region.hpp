#ifndef REGION_HPP
#define REGION_HPP
/*
  Region.hpp
  Copyright (C) 2007 David M. Anderson

  Region abstract base class: a two-dimensional region that can contain a point.
*/


#include "Point2.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Region
{
public:
    virtual ~Region( ) { };

    virtual bool Contains( const Point2I & point ) const = 0;

    virtual bool operator==( const Region & rhs ) const = 0;
    bool operator!=( const Region & rhs ) const;
};


//*****************************************************************************


inline
bool 
Region::operator!=( const Region & rhs ) const
{
    return ! (*this == rhs);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //REGION_HPP

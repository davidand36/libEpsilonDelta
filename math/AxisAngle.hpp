#ifndef AXISANGLE_HPP
#define AXISANGLE_HPP
/*
  AxisAngle.hpp
  Copyright (C) 2007 David M. Anderson

  AxisAngle templete class: represents a 3-D rotation.
  NOTES:
  1. The axis should be normalized. By default, this is assumed. If you are
     not sure, set normalized = false. This will result in some additional
     computation.
*/


#include "Vector3.hpp"
#include "Angle.hpp"
#include "JSON.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
class AxisAngle
{
public:
    AxisAngle( );
    AxisAngle( const Vector3<T> & axis, const Angle & angle,
               bool normalized = true );
    void Set( const Vector3<T> & axis, const Angle & angle,
              bool normalized = true );
    const Vector3<T> & Axis( ) const;
    Angle GetAngle( ) const;

private:
    Vector3<T> m_axis;
    Angle m_angle;
};

//.............................................................................

template <typename T>
std::string ToJSON( const AxisAngle<T> & axisAngle );
template <typename T>
void FromJSON( const std::string & json, AxisAngle<T> * pAxisAngle );

//=============================================================================

typedef AxisAngle< float >  AxisAngleF;
typedef AxisAngle< double >  AxisAngleD;


//*****************************************************************************


template <typename T>
inline
AxisAngle<T>::AxisAngle( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline
AxisAngle<T>::AxisAngle( const Vector3<T> & axis, const Angle & angle,
                         bool normalized )
    :   m_axis( axis ),
        m_angle( angle )
{
    if ( ! normalized )
        m_axis.Normalize( );
}

//=============================================================================

template <typename T>
inline
void 
AxisAngle<T>::Set( const Vector3<T> & axis, const Angle & angle,
                   bool normalized )
{
    m_axis = axis;
    m_angle = angle;
    if ( ! normalized )
        m_axis.Normalize( );
}

//=============================================================================

template <typename T>
inline
const Vector3<T> & 
AxisAngle<T>::Axis( ) const
{
    return m_axis;
}

//-----------------------------------------------------------------------------

template <typename T>
inline
Angle 
AxisAngle<T>::GetAngle( ) const
{
    return m_angle;
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const AxisAngle<T> & axisAngle )
{
    JSONObject jsonObj;
    jsonObj[ "axis" ] = ToJSON( axisAngle.Axis() );
    jsonObj[ "angle" ] = ToJSON( axisAngle.GetAngle() );
    return ToJSON( jsonObj );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, AxisAngle<T> * pAxisAngle )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    Vector3<T> axis;
    Angle angle;
    FromJSON( jsonObj[ "axis" ], &axis );
    FromJSON( jsonObj[ "angle" ], &angle );
    pAxisAngle->Set( axis, angle );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //AXISANGLE_HPP

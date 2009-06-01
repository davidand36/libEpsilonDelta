#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP
/*
  Polynomial.hpp
  Copyright (C) 2007 David M. Anderson

  Polynomial template class: representing a polynomial over any type (field).
  NOTES:
  1. The (first, last) constructor and Set() function take a pair of iterators,
     typically into an array or vector, and copies from [first,last[.
     So the polynomial
     c[0] + c[1]*x + c[2]*x^2 + ... + c[n]*x^n
     can be constructed with Polynomial( c, c+n+1 ),
     or Polynomial( c.begin(), c.end() ).
  2. The (degree, c0 ...) constructor and Set method take the degree,
     followed by the (degree+1) coefficients. Only certain built-in types can
     be used as arguments in functions of this sort. In particular, because of
     implicit conversions, floats generally cannot be. So with PolynomialF or
     PolynomialCD, for example, you can pass in double arguments and they
     will be converted to float or complex<double>, but you cannot pass in
     float or complex<double> coefficients directly.
  3. Trim removes high-order coefficients that are zero, so that the Degree()
     of the resulting polynomial is its true degree.
  4. operator[] provides unchecked access.
     Coefficient() asserts that the power (index) is in the proper range.
  5. operator()( arg ) evaluates the polynomial at x=arg and returns the
     result.
     operator()( arg, pValue, pDerivative ) evaluates the polynomial and its
     derivative at x=arg.
     These operators make Polynomial a function object (functor) for use in
     root finding, etc.
  6. The monomial functions multiply or divide by the polynomial (x - c0).
     The remainder of DivModMonomial will actually be a scalar, not a monomial.
  7. The template subclass PolynomialR represents polynomials over reals,
     so T is typically a floating- or fixed-point type.
     The template subclass PolynomialC represents polynomials over complex
     numbers, so T is typically std::complex<F>, where F is floating-point.
  8. Most algorithms are from W.H. Press, et al., "Numerical Recipes in C".
*/


#include "Exception.hpp"
#include "Assert.hpp"
#include "RootFinder.hpp"
#include "JSON.hpp"
#include <vector>
#include <algorithm>
#include <cstdarg>
#include <complex>
#include <string>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

template <typename T>
class Polynomial;
template <typename T>
Polynomial<T> operator*( const Polynomial<T> & rhs,
                         const Polynomial<T> & lhs );
template <typename T>
std::ostream & operator<<( std::ostream & out, const Polynomial<T> & poly );
template <typename T>
std::string ToJSON( const Polynomial<T> & poly );
template <typename T>
void FromJSON( const std::string & json, Polynomial<T> * pPoly );

//*****************************************************************************


template <typename T>
class Polynomial
{
public:
    Polynomial( );
    template <typename In> Polynomial( In first, In last );
    template <typename X> Polynomial( int degree, X c0 ... );
    virtual ~Polynomial( );
    template <typename In> void Set( In first, In last );
    template <typename X> void Set( int degree, X c0 ... );
    void Trim( );
    int Degree( ) const;
    T operator[]( int power ) const;
    T Coefficient( int power ) const;
    bool operator==( const Polynomial & rhs ) const;
    T operator()( T arg ) const;
    void operator()( T arg, T * pValue, T * pDerivative ) const;
    Polynomial & operator+=( const Polynomial & rhs );
    Polynomial & operator-=( const Polynomial & rhs );
    Polynomial & operator*=( T s );
    Polynomial & operator*=( const Polynomial & rhs );
    Polynomial & operator/=( T s );
    Polynomial & operator/=( const Polynomial & rhs );
    Polynomial & MultiplyByMonomial( T c0 );
    Polynomial & DivideByMonomial( T c0 );
    void DivMod( const Polynomial & divisor,
                 Polynomial * pQuotient, Polynomial * pRemainder );
    void DivModMonomial( T c0, Polynomial * pQuotient, T * pRemainder );
    Polynomial Derivative( ) const;

    friend
    Polynomial operator*<>( const Polynomial & rhs, const Polynomial & lhs );

protected:
    std::vector<T> m_coeffs;

    typedef typename std::vector<T>::iterator TVIterator;
    typedef typename std::vector<T>::size_type TVSizeType;

    friend std::string ToJSON<>( const Polynomial & poly );
    friend void FromJSON<>( const std::string & json, Polynomial * pPoly );
};

//.............................................................................

template <typename T>
Polynomial<T> operator+( const Polynomial<T> & lhs,
                         const Polynomial<T> & rhs );
template <typename T>
Polynomial<T> operator-( const Polynomial<T> & lhs,
                         const Polynomial<T> & rhs );
template <typename T>
Polynomial<T> operator*( const Polynomial<T> & lhs,
                         const Polynomial<T> & rhs );
template <typename T>
Polynomial<T> operator/( const Polynomial<T> & lhs,
                         const Polynomial<T> & rhs );

//*****************************************************************************


template <typename T>
class PolynomialR
    :   public Polynomial<T>
{
public:
    PolynomialR( );
    template <typename In> PolynomialR( In first, In last );
    template <typename X> PolynomialR( int degree, X c0 ... );
    PolynomialR( const Polynomial<T> & poly );
    virtual ~PolynomialR( );
    std::vector<T> RealRoots( T accuracy = 1e-6 ) const;
};


//*****************************************************************************


template <typename T>
class PolynomialC
    :   public Polynomial<T>
{
public:
    PolynomialC( );
    template <typename In> PolynomialC( In first, In last );
    template <typename X> PolynomialC( int degree, X c0 ... );
    PolynomialC( const Polynomial<T> & poly );
    virtual ~PolynomialC( );
    std::vector<T> Roots( ) const;

private:
    T Laguerre( T x ) const;  //Laguerre's method finds a root near x.
};


//*****************************************************************************


typedef PolynomialR<float>          PolynomialF;
typedef PolynomialR<double>         PolynomialD;
typedef PolynomialR<long double>    PolynomialLD;

typedef PolynomialC< std::complex<float> >          PolynomialCF;
typedef PolynomialC< std::complex<double> >         PolynomialCD;
typedef PolynomialC< std::complex<long double> >    PolynomialCLD;


//*****************************************************************************


#ifdef DEBUG
bool TestPolynomial( );
#endif


//*****************************************************************************


template <typename T>
Polynomial<T>::Polynomial( )
    :   m_coeffs( 1 )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T> template <typename In>
Polynomial<T>::Polynomial( In first, In last )
    :   m_coeffs( first, last )
{
    if ( m_coeffs.size() == 0 )
        m_coeffs.assign( 1, 0 );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T> template <typename X>
Polynomial<T>::Polynomial( int degree, X c0 ... )
{
    m_coeffs.reserve( degree + 1 );
    m_coeffs.push_back( c0 );
    std::va_list args;
    va_start( args, c0 );
    for ( int i = 1; i <= degree; ++i )
    {
        X c = va_arg( args, X );
        m_coeffs.push_back( c );
    }
    va_end( args );
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T>::~Polynomial( )
{
}

//=============================================================================

template <typename T> template <typename In>
void 
Polynomial<T>::Set( In first, In last )
{
    m_coeffs.assign( first, last );
    if ( m_coeffs.size() == 0 )
        m_coeffs.assign( 1, 0 );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T> template <typename X>
void 
Polynomial<T>::Set( int degree, X c0 ... )
{
    m_coeffs.clear( );
    m_coeffs.reserve( degree + 1 );
    m_coeffs.push_back( c0 );
    std::va_list args;
    va_start( args, c0 );
    for ( int i = 1; i <= degree; ++i )
    {
        X c = va_arg( args, X );
        m_coeffs.push_back( c );
    }
    va_end( args );
}

//=============================================================================

template <typename T>
void 
Polynomial<T>::Trim( )
{
    TVIterator p = m_coeffs.end();
    while ( p != m_coeffs.begin() )
        if ( *--p != T() )
            break;
    m_coeffs.erase( ++p, m_coeffs.end() );
}

//=============================================================================

template <typename T>
int 
Polynomial<T>::Degree( ) const
{
    return m_coeffs.size() - 1;
}

//=============================================================================

template <typename T>
T 
Polynomial<T>::operator[]( int power ) const
{
    return m_coeffs[ power ];
}

//-----------------------------------------------------------------------------

template <typename T>
T 
Polynomial<T>::Coefficient( int power ) const
{
    Assert( (0 <= power) && (power <= Degree()) );
    return m_coeffs[ power ];
}

//=============================================================================

template <typename T>
bool 
Polynomial<T>::operator==( const Polynomial & rhs ) const
{
    return (m_coeffs == rhs.m_coeffs);
}

//=============================================================================

template <typename T>
T 
Polynomial<T>::operator()( T arg ) const
{
    int i = Degree();
    T value = m_coeffs[i];
    while ( i > 0 )
        value  = value * arg  +  m_coeffs[--i];
    return value;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Polynomial<T>::operator()( T arg, T * pValue, T * pDerivative ) const
{
    int i = Degree();
    T value = m_coeffs[i];
    T deriv = 0;
    while ( i > 0 )
    {
        deriv = deriv * arg  +  value;
        value  = value * arg  +  m_coeffs[--i];
    }
    if ( pValue )
        *pValue = value;
    if ( pDerivative )
        *pDerivative = deriv;
}

//=============================================================================

template <typename T>
Polynomial<T> & 
Polynomial<T>::operator+=( const Polynomial & rhs )
{
    TVSizeType rhsSize = rhs.m_coeffs.size();
    if ( m_coeffs.size() < rhsSize )
        m_coeffs.resize( rhsSize );
    for ( int i = rhsSize - 1; i >= 0; --i )
        m_coeffs[i] += rhs.m_coeffs[i];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> & 
Polynomial<T>::operator-=( const Polynomial & rhs )
{
    TVSizeType rhsSize = rhs.m_coeffs.size();
    if ( m_coeffs.size() < rhsSize )
        m_coeffs.resize( rhsSize );
    for ( int i = rhsSize - 1; i >= 0; --i )
        m_coeffs[i] -= rhs.m_coeffs[i];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> &
Polynomial<T>::operator*=( T s )
{
    int deg = Degree();
    for ( int i = 0; i <= deg; ++i )
        m_coeffs[i] *= s;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> & 
Polynomial<T>::operator*=( const Polynomial & rhs )
{
    //This time it is less efficient to do the operation in place
    return *this = *this * rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> &
Polynomial<T>::operator/=( T s )
{
    Assert( s != T() );
    int deg = Degree();
    for ( int i = 0; i <= deg; ++i )
        m_coeffs[i] /= s;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> & 
Polynomial<T>::operator/=( const Polynomial & rhs )
{
    Polynomial quot;
    DivMod( rhs, &quot, 0 );
    return *this = quot;
}

//=============================================================================

template <typename T>
Polynomial<T> & 
Polynomial<T>::MultiplyByMonomial( T c0 )
{
    int oldSize = m_coeffs.size();
    m_coeffs.resize( oldSize + 1 );
    m_coeffs[ oldSize ] = m_coeffs[ oldSize - 1 ];
    for ( int i = oldSize - 1; i >= 1; --i )
        m_coeffs[i] = m_coeffs[ i - 1 ] - m_coeffs[i] * c0;
    m_coeffs[0] *= -c0;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> & 
Polynomial<T>::DivideByMonomial( T c0 )
{
    Polynomial quot;
    DivModMonomial( c0, &quot, 0 );
    return *this = quot;
}

//-----------------------------------------------------------------------------

template <typename T>
void 
Polynomial<T>::DivMod( const Polynomial & divisor,
                       Polynomial * pQuotient, Polynomial * pRemainder )
{
    int numDeg = Degree();
    int denDeg = divisor.Degree();
    Polynomial quot;
    Polynomial rem = *this;
    if ( denDeg <= numDeg )
    {
        quot.m_coeffs.resize( numDeg - denDeg + 1 );
        for ( int i = numDeg - denDeg; i >= 0; --i )
        {
            quot.m_coeffs[i] = rem.m_coeffs[denDeg + i]
                                    / divisor.m_coeffs[denDeg];
            for ( int j = denDeg + i - 1; j >= i; --j )
                rem.m_coeffs[j] -= quot.m_coeffs[i] * divisor.m_coeffs[j - i];
        }
        rem.m_coeffs[denDeg] = 0;
    }
    if ( pQuotient )
        *pQuotient = quot;
    if ( pRemainder )
    {
        TVIterator p( rem.m_coeffs.begin() + std::min( denDeg, numDeg ) + 1 );
        while ( p != rem.m_coeffs.begin() )
            if ( *--p != T() )
                break;
        *pRemainder = Polynomial( rem.m_coeffs.begin(), ++p );
    }
}

//-----------------------------------------------------------------------------

template <typename T>
void 
Polynomial<T>::DivModMonomial( T c0, Polynomial * pQuotient, T * pRemainder )
{
    int i = Degree();
    Polynomial quot;
    quot.m_coeffs.resize( i );
    T rem = m_coeffs[i--];
    while ( i >= 0 )
    {
        T t = m_coeffs[i];
        quot.m_coeffs[i--] = rem;
        rem = t + rem * c0;
    }
    if ( pQuotient )
        *pQuotient = quot;
    if ( pRemainder )
        *pRemainder = rem;
}

//=============================================================================

template <typename T>
Polynomial<T>
Polynomial<T>::Derivative( ) const
{
    int deg = Degree();
    Polynomial deriv;
    deriv.m_coeffs.resize( deg );
    for ( int i = 1; i <= deg; ++i )
        deriv.m_coeffs[ i - 1 ] = i * m_coeffs[i];
    return deriv;
}

//=============================================================================

template <typename T>
Polynomial<T> 
operator+( const Polynomial<T> & lhs, const Polynomial<T> & rhs )
{
    Polynomial<T> p = lhs;
    return p += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> 
operator-( const Polynomial<T> & lhs, const Polynomial<T> & rhs )
{
    Polynomial<T> p = lhs;
    return p -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> 
operator*( const Polynomial<T> & lhs, const Polynomial<T> & rhs )
{
    int lhsSize = lhs.m_coeffs.size();
    int rhsSize = rhs.m_coeffs.size();
    Polynomial<T> prod;
    prod.m_coeffs.resize( lhsSize + rhsSize - 1 );
    for ( int i = 0; i < lhsSize; ++i )
        for ( int j = 0; j < rhsSize; ++j )
            prod.m_coeffs[ i + j ] += lhs.m_coeffs[i] * rhs.m_coeffs[j];
    return prod;
}

//-----------------------------------------------------------------------------

template <typename T>
Polynomial<T> 
operator/( const Polynomial<T> & lhs, const Polynomial<T> & rhs )
{
    Polynomial<T> quot;
    lhs.DivMod( rhs, &quot, 0 );
    return quot;
}

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Polynomial<T> & poly )
{
    out << "( ";
    for ( int i = 0; i <= poly.Degree(); ++i )
    {
        if ( i == 0 )
            out << poly[0];
        else if ( i == 1 )
            out << " + " << poly[1] << "x";
        else
            out << " + " << poly[i] << "x^" << i;
    }
    return out << " )";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Polynomial<T> & poly )
{
    return ToJSON( poly.m_coeffs );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Polynomial<T> * pPoly )
{
    FromJSON( json, &(pPoly->m_coeffs) );
}


//*****************************************************************************


template <typename T> 
PolynomialR<T>::PolynomialR( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T> template <typename In> 
PolynomialR<T>::PolynomialR( In first, In last )
    :   Polynomial<T>( first, last )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T> template <typename X>
PolynomialR<T>::PolynomialR( int degree, X c0 ... )
{
    this->m_coeffs.clear( );
    this->m_coeffs.reserve( degree + 1 );
    this->m_coeffs.push_back( static_cast< T >( c0 ) );
    std::va_list args;
    va_start( args, c0 );
    for ( int i = 1; i <= degree; ++i )
    {
        X c = va_arg( args, X );
        this->m_coeffs.push_back( static_cast< T >( c ) );
    }
    va_end( args );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
PolynomialR<T>::PolynomialR( const Polynomial<T> & poly )
    :   Polynomial<T>( poly )
{
}

//-----------------------------------------------------------------------------

template <typename T>
PolynomialR<T>::~PolynomialR( )
{
}

//=============================================================================

template <typename T>
std::vector<T>
PolynomialR<T>::RealRoots( T accuracy ) const
{
    std::vector<T> roots;
    int degree = this->Degree();
    roots.reserve( degree );
    if ( degree == 0 )
        return roots;
    if ( degree == 1 )
        return RootFinder::Linear( this->m_coeffs[0], this->m_coeffs[1] );
    if ( degree == 2 )
        return RootFinder::Quadratic( this->m_coeffs[0], this->m_coeffs[1],
                                      this->m_coeffs[2] );
    if ( degree == 3 )
        return RootFinder::Cubic( this->m_coeffs[0], this->m_coeffs[1],
                                  this->m_coeffs[2], this->m_coeffs[3] );

    PolynomialR poly = *this;   //Need a copy for deflating
    while ( poly.Degree() > 0 )
    {
        T x0 = -0.01;   //bracket 0; want smallest roots first
        T x1 = 0.01;
        bool ok = RootFinder::BracketRoot( poly, &x0, &x1 );
        if ( ! ok )
            break;
        T root;
        ok = RootFinder::NewtonRaphson( poly, &root, x0, x1, accuracy );
        if ( ! ok )
            break;
        //polish root of deflated poly on full poly
        if ( fabs( root ) < 1. )
        {
            x0 = root - 0.0001;
            x1 = root + 0.0001;
        }
        else
        {
            x0 = root * 0.9999;
            x1 = root * 1.0001;
        }
        ok = RootFinder::BracketRoot( *this, &x0, &x1 );
        if ( ! ok )
            break;
        ok = RootFinder::NewtonRaphson( *this, &root, x0, x1, accuracy );
        if ( ! ok )
            break;

        roots.push_back( root );
        poly.DivideByMonomial( root );  //deflation
    }
    
    return roots;
}


//*****************************************************************************


template <typename T> 
PolynomialC<T>::PolynomialC( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T> template <typename In> 
PolynomialC<T>::PolynomialC( In first, In last )
    :   Polynomial<T>( first, last )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T> template <typename X>
PolynomialC<T>::PolynomialC( int degree, X c0 ... )
{
    this->m_coeffs.clear( );
    this->m_coeffs.reserve( degree + 1 );
    this->m_coeffs.push_back( c0 );
    std::va_list args;
    va_start( args, c0 );
    for ( int i = 1; i <= degree; ++i )
    {
        X c = va_arg( args, X );
        this->m_coeffs.push_back( c );
    }
    va_end( args );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
PolynomialC<T>::PolynomialC( const Polynomial<T> & poly )
    :   Polynomial<T>( poly )
{
}

//-----------------------------------------------------------------------------

template <typename T>
PolynomialC<T>::~PolynomialC( )
{
}

//=============================================================================

template <typename T>
std::vector<T>
PolynomialC<T>::Roots( ) const
{
    std::vector<T> roots;
    int degree = this->Degree();
    roots.reserve( degree );
    PolynomialC poly = *this;   //Need a copy for deflating
    while ( poly.Degree() > 0 )
    {
        T x = 0.;   //Want smallest roots first.
        T root = poly.Laguerre( x );
        if ( fabs( root.imag() ) < 1.e-14 * fabs( root.real() ) )
            root = root.real(); //If nearly real, assume real.
        root = Laguerre( root ); //Polish on full poly
        roots.push_back( root );
        poly.DivideByMonomial( root );  //deflation
    }
    return roots;
}

//.............................................................................

template <typename T>
T 
PolynomialC<T>::Laguerre( T x ) const
{
    typedef typename T::value_type  Real;
    static const Real fractions[ 8 ]
            = { 0.5, 0.25, 0.75, 0.125, 0.375, 0.625, 0.875, 1.0 };
    int degree = this->Degree();
    Real absx = abs( x );
    for ( int i = 0; i < 89; ++i )
    {
        int j = degree;
        T v = this->m_coeffs[j];
        T d = 0.;
        T dd = 0.;
        Real e = abs( v );
        while ( j > 0 )
        {
            dd = x * dd  +  d;
            d = x * d  +  v;
            v = x * v  +  this->m_coeffs[--j];
            e = absx * e  +  abs( v );
        }
        dd *= 2.;  //At this point, v = poly(x), d = deriv, dd = 2nd deriv.
        e *= std::numeric_limits< Real >::epsilon();
        if ( abs( v ) <= e )
            return x;   //At a root.
        T g = d / v;
        T gSqr = g * g;
        T h = gSqr  -  dd / v;
        T sq = sqrt( T(degree - 1.) * (T(degree) * h  -  gSqr) );
        T gp = g + sq;
        T gm = g - sq;
        Real absGp = abs( gp );
        Real absGm = abs( gm );
        T denom = gp;
        Real absDenom = absGp;
        if ( absGm > absGp )
        {
            denom = gm;
            absDenom = absGm;
        }
        T dx = (absDenom > 0.)  ?  (T(degree) / denom)
                //To avoid pathological cases, choose point outside unit circle
                :  polar( absx + 1., Real( i ) );
        T estimate = x - dx;
        if ( x == estimate )
            return x;   //Converged.
        if ( i % 10 == 9 )
            x -= dx * fractions[ i / 10 ];  //To avoid limit cycles
        else
            x = estimate;
    }
    throw Exception( "Laguerre failed to converge." );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //POLYNOMIAL_HPP

#ifndef SMARTPTR_HPP
#define SMARTPTR_HPP
/*
  SmartPtr.hpp
  Copyright (C) 2007 David M. Anderson

  Smart pointer template classes: reference-counted pointer-like object.
  NOTES:
  1. As smart pointers move from Boost into std::tr1, this should be revisited.
*/


#if defined( USE_BOOST )

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
using boost::scoped_ptr;
using boost::scoped_array;
using boost::shared_ptr;
using boost::shared_array;
using boost::weak_ptr;
using boost::dynamic_pointer_cast;

#elif 0 //This is the way it should be:

#include <tr1/memory>
using std::tr1::scoped_ptr;
using std::tr1::scoped_array;
using std::tr1::shared_ptr;
using std::tr1::shared_array;
using std::tr1::weak_ptr;
using std::tr1::dynamic_pointer_cast;

#elif 1 //!!!GCC headers incomplete?

#include <tr1/memory>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::dynamic_pointer_cast;
using boost::scoped_ptr;
using boost::scoped_array;
using boost::shared_array;

#else
#error "Boost library required"
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG
bool TestSmartPtr( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta


#endif //SMARTPTR_HPP

#ifndef SMARTPTR_HPP
#define SMARTPTR_HPP
/*
  SmartPtr.hpp
  Copyright (C) 2007 David M. Anderson

  Smart pointer template classes: reference-counted pointer-like object.
  NOTES:
  1. This is no longer needed, now that shared_ptr is part of std::tr1.
     I'm keeping it just for the test/demonstration.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG
bool TestSmartPtr( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta


#endif //SMARTPTR_HPP

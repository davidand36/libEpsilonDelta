/*
  Mouse.cpp
  Copyright (C) 2009 David M. Anderson

  Mouse class: a standard mouse.
*/


#include "Mouse.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MouseImpl
{
public:
    MouseImpl( );
    ~MouseImpl( );

    int NumButtons( ) const;
    bool ButtonDown( int button ) const;

    int NumPointers( ) const;
    Point2I Pointer( int index ) const;
    
private:
#ifdef USE_SDL
    static const int MaxButtons = 5;   //SDL defines five buttons.
#endif
};


//*****************************************************************************


Mouse::Mouse( const std::string & name )
    :   InputDevice( InputDevice::Mouse, name ),
        m_pImpl( new MouseImpl )
{
}

//-----------------------------------------------------------------------------

Mouse::~Mouse( )
{
}

//=============================================================================

int 
Mouse::NumButtons( ) const
{
    return m_pImpl->NumButtons();
}

//-----------------------------------------------------------------------------

bool 
Mouse::ButtonDown( int button ) const
{
    return m_pImpl->ButtonDown( button );
}

//=============================================================================

int 
Mouse::NumPointers( ) const
{
    return m_pImpl->NumPointers();
}

//-----------------------------------------------------------------------------

Point2I 
Mouse::Pointer( int index ) const
{
    return m_pImpl->Pointer( index );
}


//*****************************************************************************


#ifdef USE_SDL

//=============================================================================


MouseImpl::MouseImpl( )
{
}

//-----------------------------------------------------------------------------

MouseImpl::~MouseImpl( )
{
}

//=============================================================================

int 
MouseImpl::NumButtons( ) const
{
    return MaxButtons;
}

//-----------------------------------------------------------------------------

bool 
MouseImpl::ButtonDown( int button ) const
{
    //Note that SDL starts its mouse button definitions at 1, but
    // the bit masks effectively start with (1<<0).
    if ( (button < 0) || (button >= MaxButtons) )
        throw std::out_of_range( "Mouse::ButtonDown()" );
    Uint8 buttonState = ::SDL_GetMouseState( 0, 0 );
    return ((buttonState & (1 << button)) != 0);
}

//=============================================================================

int 
MouseImpl::NumPointers( ) const
{
    return 1;
}

//-----------------------------------------------------------------------------

Point2I 
MouseImpl::Pointer( int index ) const
{
    if ( index != 0 )
        throw std::out_of_range( "Mouse::Pointer()" );
    int x, y;
    ::SDL_GetMouseState( &x, &y );
    return Point2I( x, y );
}

//=============================================================================

#endif //USE_SDL


//*****************************************************************************

}                                                      //namespace EpsilonDelta

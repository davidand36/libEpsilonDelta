#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
/*
  Exception.hpp
  Copyright (C) 2007 David M. Anderson

  Exception class:  Base exception class for my library.
  NOTES:
  1. Applications may want to catch specific subclasses close to the source,
     and also catch everything with something like:
         int main( int argc, char ** argv )
         {
             try
             {
                 //system-specific startup
                 int res = Main( argc, argv );
                 //system-specific cleanup
                 return res;
             }
             catch ( Exception & except )
             {
                 cerr << except.Description( ) << endl;
                 //or display dialog box or other appropriate output
             }
             catch ( std::exception & except )
             {
                 Exception exc( except );
                 cerr << exc.Description( ) << endl;
             }
             catch ( ... )
             {
                 Exception exc( "Unknown exception!" );
                 cerr << exc.Description( ) << endl;
             }
         }
*/


#include <stdexcept>
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Exception
{
public:
    explicit Exception( const std::string & description );
    explicit Exception( const char * description );
    explicit Exception( const std::exception & exceptn );
    virtual ~Exception( );
    virtual std::string Description( ) const;

protected:
    std::string m_description;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EXCEPTION_HPP

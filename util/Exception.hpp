#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
/*
  Exception.hpp
  Copyright (C) 2007 David M. Anderson

  Exception class:  Base exception class for my library.
  LogicError class: Programmer errors.
  RuntimeError class: Exceptions due to bad data, I/O problems, etc.
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
  2. These classes are derived from std::exception, std::logic_error,
     and std::runtime_error.
*/


#include <stdexcept>
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Exception
    :   public virtual std::exception
{
public:
    explicit Exception( const std::string & description );
    explicit Exception( const std::exception & exceptn );
    virtual ~Exception( ) throw ();
    virtual void SetDescription( const std::string & description );
    virtual std::string Description( ) const;
    virtual const char* what() const throw ();

protected:
    std::string m_description;
};


//*****************************************************************************


class LogicError
    :   public Exception,
        public std::logic_error
{
public:
    explicit LogicError( const std::string & description );
};


//*****************************************************************************


class RuntimeError
    :   public Exception,
        public std::runtime_error
{
public:
    explicit RuntimeError( const std::string & description );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EXCEPTION_HPP

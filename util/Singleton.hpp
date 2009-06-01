#ifndef SINGLETON_HPP
#define SINGLETON_HPP
/*
  Singleton.hpp
  Copyright (C) 2007 David M. Anderson

  Singleton template class.
  NOTES:
  1. To make a class a singleton, just define it like this:
     class Foo
         :    public Singleton< Foo >
     {
     public:
         ...
     private:
         Foo( );
         ~Foo( );
         ...

         friend class Singleton< Foo >;
     };

  2. Access to the single instance is provided through Instance(),
     which returns a reference to the instance.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename T >
class Singleton
{
public:
    static T & Instance( );

protected:
    Singleton( ) { }
    virtual ~Singleton( ) { }

private:
    //Following methods not defined:
    Singleton( const Singleton & );
    Singleton & operator=( const Singleton & );
};


//#############################################################################


template < typename T >
T &
Singleton< T >::Instance( )
{
    static T instance;
    return instance;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SINGLETON_HPP

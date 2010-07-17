/*
  JSON.cpp
  Copyright (C) 2007 David M. Anderson

  Routines for serializing and deserializing basic C++ type objects in
  JSON (JavaScript Object Notation).
*/


#include "JSON.hpp"
#include "JSONException.hpp"
#include "StdIO.hpp"
#include "StdLib.hpp"
#include "UnicodeUtil.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#include <tr1/array>
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


string 
ToJSON( bool b )
{
    return ( b  ?  "true"  :  "false" );
}

//-----------------------------------------------------------------------------

string
ToJSON( char i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( unsigned char i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( short i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( unsigned short i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( int i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( unsigned int i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( long i )
{
    return IntToString( i );
}

//.............................................................................

string
ToJSON( unsigned long i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( long long i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( unsigned long long i )
{
    return IntToString( static_cast< long >( i ) );
}

//.............................................................................

string
ToJSON( float r )
{
    const int BuffSize = 14;    //-d.ddddde-037
    char buff[ BuffSize ];
    snprintf( buff, BuffSize, "%+.5e", r );
    string json( buff );
    //Make sure exponent is two digits. (MS VC++)
    int expPos = json.length() - 3;
    if ( json[expPos] == '0' )
        json.erase( expPos, 1 );
    return json;
}

//.............................................................................

string
ToJSON( double r )
{
    const int BuffSize = 23;    //-d.dddddddddddddde-307
    char buff[ BuffSize ];
    snprintf( buff, BuffSize, "%+.14e", r );
    string json( buff );
    //Make sure exponent is three digits. (GCC)
    int expPos = json.length() - 3;
    if ( (json[expPos] == '+') || (json[expPos] == '-') )
         json.insert( expPos + 1, "0" );
    return json;
}

//.............................................................................

string
ToJSON( long double r )
{
    const int BuffSize = 27;    //-d.ddddddddddddddddde-4931
    char buff[ BuffSize ];
    snprintf( buff, BuffSize, "%+.17Le", r );
    return string( buff );
}

//-----------------------------------------------------------------------------

string 
ToJSON( const char * s )
{
    if ( s )
        return ToJSON( string( s ) );
    else
        return "null";
}

//.............................................................................

string
ToJSON( const string & s )
{
    string st = s;
    AddQuotes( &st, '\"', '\\' );
    Replace( &st, "\b", "\\b" );
    Replace( &st, "\f", "\\f" );
    Replace( &st, "\n", "\\n" );
    Replace( &st, "\r", "\\r" );
    Replace( &st, "\t", "\\t" );
    return st;
}

//.............................................................................

string
ToJSON( const wstring & s )
{
    string st = EncodeUTF8( s );
    return ToJSON( st );
}

//-----------------------------------------------------------------------------

string
ToJSON( const JSONObject & o )
{
    string json = "{\n";
    int i = 0;
    for ( JSONObject::const_iterator p = o.begin();
          p != o.end(); ++p, ++i )
    {
        if ( i > 0 )
            json += ",\n";
        json += ToJSON( p->first ) + ": " + p->second;
    }
    json += "\n}";
    return json;
}

//=============================================================================

namespace
{

enum JSONType
{
    JSON_Null,
    JSON_Bool,
    JSON_Number,
    JSON_String,
    JSON_Array,
    JSON_Object,
    JSON_Unknown
};

JSONType JSONTypeOf( char firstChar );
void NextJSONElement( const string & json, int * pStart, int * pEnd );

}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, bool * pB )
{
    string js = Trim( json );
    if ( js == "true" )
        *pB = true;
    else if ( js == "false" )
        *pB = false;
    else
        throw JSONException( "Invalid JSON boolean" );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, char * pC )
{
    long int i = strtol( json.c_str(), 0, 0 );
    *pC = static_cast< char >( i );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, unsigned char * pC )
{
    long int i = strtol( json.c_str(), 0, 0 );
    *pC = static_cast< char >( i );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, short * pI )
{
    long int i = strtol( json.c_str(), 0, 0 );
    *pI = static_cast< int8_t >( i );
}

//.............................................................................

void 
FromJSON( const string & json, unsigned short * pI )
{
    unsigned long int i = strtoul( json.c_str(), 0, 0 );
    *pI = static_cast< uint8_t >( i );
}

//.............................................................................

void 
FromJSON( const string & json, int * pI )
{
    long int i = strtol( json.c_str(), 0, 0 );
    *pI = static_cast< int16_t >( i );
}

//.............................................................................

void 
FromJSON( const string & json, unsigned int * pI )
{
    unsigned long int i = strtoul( json.c_str(), 0, 0 );
    *pI = static_cast< uint16_t >( i );
}

//.............................................................................

void 
FromJSON( const string & json, long * pI )
{
    long int i = strtol( json.c_str(), 0, 0 );
    *pI = static_cast< int32_t >( i );
}

//.............................................................................

void 
FromJSON( const string & json, unsigned long * pI )
{
    unsigned long int i = strtoul( json.c_str(), 0, 0 );
    *pI = static_cast< uint32_t >( i );
}

//.............................................................................

void 
FromJSON( const string & json, long long * pI )
{
    long long int i = strtoll( json.c_str(), 0, 0 );
    *pI = static_cast< int64_t >( i );
}

//.............................................................................

void 
FromJSON( const string & json, unsigned long long * pI )
{
    unsigned long long int i = strtoull( json.c_str(), 0, 0 );
    *pI = static_cast< uint64_t >( i );
}

//.............................................................................

void 
FromJSON( const std::string & json, wchar_t * pC )
{
    long int i = strtol( json.c_str(), 0, 0 );
    *pC = static_cast< wchar_t >( i );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, float * pR )
{
    *pR = (float)strtof( json.c_str(), 0 );
}

//.............................................................................

void 
FromJSON( const string & json, double * pR )
{
    *pR = strtod( json.c_str(), 0 );
}

//.............................................................................

void 
FromJSON( const string & json, long double * pR )
{
    *pR = strtold( json.c_str(), 0 );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, string * pS )
{
    string st = Trim( json );
    Replace( &st, "\\b", "\b" );
    Replace( &st, "\\f", "\f" );
    Replace( &st, "\\n", "\n" );
    Replace( &st, "\\r", "\r" );
    Replace( &st, "\\t", "\t" );
    RemoveQuotes( &st, '\"', '\\' );
    *pS = st;
}

//.............................................................................

void 
FromJSON( const string & json, std::wstring * pS )
{
    string st;
    FromJSON( json, &st );
    *pS = DecodeUTF8( st );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, JSONObject * pO )
{
    pO->clear( );
    string st = Trim( json );
    int i = 0;
    int end = st.length();
    if ( end == 0 )
        throw JSONException( "Empty JSON object" );
    if ( st[i] != '{' )
        throw JSONException( "Invalid JSON object" );
    while ( (++i < end) && isspace( st[i] ) )
        ;
    if ( st[i] == '}' )
        return;
    while ( true )
    {
        int b = i;
        int e = end;
        NextJSONElement( st, &b, &e );
        i = e;
        string key;
        FromJSON( st.substr( b, (e - b) ), &key );
        while ( (i < end) && isspace( st[i] ) )
            ++i;
        if ( i >= end )
            throw JSONException( "Unterminated JSON object" );
        if ( st[i] != ':' )
            throw JSONException( "Invalid JSON object" );
        if ( ++i >= end )
            throw JSONException( "Unterminated JSON object" );
        b = i;
        e = end;
        NextJSONElement( st, &b, &e );
        i = e;
        string value = st.substr( b, (e - b) );
        (*pO)[ key ] = value;
        while ( (i < end) && isspace( st[i] ) )
            ++i;
        if ( i >= end )
            throw JSONException( "Unterminated JSON object" );
        if ( st[i] == '}' )
            return;
        if ( st[i] != ',' )
            throw JSONException( "Invalid JSON object" );
        if ( ++i >= end )
            throw JSONException( "Unterminated JSON object" );
    }
}

//=============================================================================

namespace
{                                                                   //namespace

//.............................................................................

JSONType 
JSONTypeOf( char firstChar )
{
    switch ( firstChar )
    {
    case 'n':
        return JSON_Null;
    case 't':
    case 'f':
        return JSON_Bool;
    case '\"':
        return JSON_String;
    case '[':
        return JSON_Array;
    case '{':
        return JSON_Object;
    default:
        if ( isdigit( firstChar ) || (firstChar == '+') || (firstChar == '-')
             || (firstChar == '.') )
            return JSON_Number;
    }
    return JSON_Unknown;
}

//-----------------------------------------------------------------------------

void 
NextJSONElement( const string & json, int * pStart, int * pEnd )
{
    int i = *pStart;
    int end = *pEnd;
    while ( (i < end) && isspace( json[i] ) )
        ++i;
    *pStart = i;
    if ( i >= end )
        throw JSONException( "Empty JSON element" );
    switch ( JSONTypeOf( json[i] ) )
    {
    case JSON_Null:
        i += 4;     //strlen("null")
        if ( i > end )
            throw JSONException( "Invalid JSON null" );
        *pEnd = i;
        return;

    case JSON_Bool:
        if ( json[i] == 't' )
            i += 4; //strlen("true")
        else
            i += 5; //strlen("false")
        if ( i > end )
            throw JSONException( "Invalid JSON boolean" );
        *pEnd = i;
        return;

    case JSON_String:
        while ( ++i < end )
        {
            if ( json[i] == '\"' )
            {
                *pEnd = i + 1;
                return;
            }
            else if ( json[i] == '\\' )
            {
                if ( ++i >= end )
                    throw JSONException( "Unterminated JSON string" );
            }
        }
        throw JSONException( "Unterminated JSON string" );

    case JSON_Number:
    {
        string sub = json.substr( i, end );
        const char * pB = sub.c_str();
        char * pE = 0;
        strtold( pB, &pE );
        *pEnd = i + (pE - pB);
        return;
    }

    case JSON_Array:
        ++i;
        while ( (i < end) && isspace( json[i] ) )
            ++i;
        if ( i >= end )
            throw JSONException( "Unterminated JSON array" );
        if ( json[i] == ']' )
        {
            *pEnd = i + 1;
            return;
        }
        while ( true )
        {
            int b = i;
            int e = end;
            NextJSONElement( json, &b, &e );
            i = e;
            while ( (i < end) && isspace( json[i] ) )
                ++i;
            if ( i >= end )
                throw JSONException( "Unterminated JSON array" );
            if ( json[i] == ']' )
            {
                *pEnd = i + 1;
                return;
            }
            if ( json[i] != ',' )
                throw JSONException( "Invalid JSON array" );
            if ( ++i >= end )
                throw JSONException( "Unterminated JSON array" );
        }

    case JSON_Object:
        ++i;
        while ( (i < end) && isspace( json[i] ) )
            ++i;
        if ( i >= end )
            throw JSONException( "Unterminated JSON object" );
        if ( json[i] == '}' )
        {
            *pEnd = i + 1;
            return;
        }
        while ( true )
        {
            int b = i;
            int e = end;
            NextJSONElement( json, &b, &e );
            i = e;
            while ( (i < end) && isspace( json[i] ) )
                ++i;
            if ( i >= end )
                throw JSONException( "Unterminated JSON object" );
            if ( json[i] != ':' )
                throw JSONException( "Invalid JSON object" );
            if ( ++i >= end )
                throw JSONException( "Unterminated JSON object" );
            b = i;
            e = end;
            NextJSONElement( json, &b, &e );
            i = e;
            while ( (i < end) && isspace( json[i] ) )
                ++i;
            if ( i >= end )
                throw JSONException( "Unterminated JSON object" );
            if ( json[i] == '}' )
            {
                *pEnd = i + 1;
                return;
            }
            if ( json[i] != ',' )
                throw JSONException( "Invalid JSON object" );
            if ( ++i >= end )
                throw JSONException( "Unterminated JSON object" );
        }

    case JSON_Unknown:
        throw JSONException( "Invalid JSON element" );
    }
}

//.............................................................................

}                                                                   //namespace

//-----------------------------------------------------------------------------

void 
SplitJSONArray( const string & json, vector< string > * pStringVec )
{
    pStringVec->clear( );
    string st = Trim( json );
    int i = 0;
    int end = st.length();
    if ( end == 0 )
        throw JSONException( "Empty JSON array" );
    if ( st[i] != '[' )
        throw JSONException( "Invalid JSON array" );
    while ( (++i < end) && isspace( json[i] ) )
        ;
    if ( json[i] == ']' )
        return;
    while ( true )
    {
        int b = i;
        int e = end;
        NextJSONElement( json, &b, &e );
        i = e;
        pStringVec->push_back( st.substr( b, (e - b) ) );
        while ( (i < end) && isspace( json[i] ) )
            ++i;
        if ( i >= end )
            throw JSONException( "Unterminated JSON array" );
        if ( json[i] == ']' )
            return;
        if ( json[i] != ',' )
            throw JSONException( "Invalid JSON array" );
        if ( ++i >= end )
            throw JSONException( "Unterminated JSON array" );
    }
}

//=============================================================================


#ifdef DEBUG

//.............................................................................

class A
{
public:
    void Set( char ch, int i, long l, float f, string s[3], bool b[5] );
    char Character( ) const;
    int Integer( ) const;
    long Long( ) const;
    float Real( ) const;
    string ArrayString( int i ) const;
    int BoolVectorSize( ) const;
    bool VectorBool( int i ) const;

private:
    char    m_character;
    int     m_integer;
    long    m_long;
    float   m_real;
    array< string, 3 >  m_stringArray;
    vector< bool >  m_boolVector;

    friend string ToJSON( const A & a );
    friend void FromJSON( const string & json, A * pA );
};

//.............................................................................

void
A::Set(char ch, int i, long l, float f, string s[3], bool b[5] )
{
    m_character = ch;
    m_integer = i;
    m_long = l;
    m_real = f;
    for ( int j = 0; j < 3; ++j )
        m_stringArray[j] = s[j];
    m_boolVector.reserve( 5 );
    for ( int j = 0; j < 5; ++j )
        m_boolVector.push_back( b[j] );
}

//.............................................................................

char 
A::Character( ) const
{
    return m_character;
}

//.............................................................................

int 
A::Integer( ) const
{
    return m_integer;
}

//.............................................................................

long
A::Long( ) const
{
    return m_long;
}

//.............................................................................

float 
A::Real( ) const
{
    return m_real;
}

//.............................................................................

string
A::ArrayString( int i ) const
{
    return m_stringArray[i];
}

//.............................................................................

int 
A::BoolVectorSize( ) const
{
    return m_boolVector.size();
}

//.............................................................................

bool 
A::VectorBool( int i ) const
{
    return m_boolVector[i];
}

//.............................................................................

string 
ToJSON( const A & a )
{
    JSONObject jsonObj;
    jsonObj[ "character" ] = ToJSON( a.m_character );
    jsonObj[ "integer" ] = ToJSON( a.m_integer );
    jsonObj[ "long" ] = ToJSON( a.m_long );
    jsonObj[ "real" ] = ToJSON( a.m_real );
    jsonObj[ "stringArray" ] = ToJSON( a.m_stringArray );
    jsonObj[ "boolVector" ] = ToJSON( a.m_boolVector );
    return ToJSON( jsonObj );
}

//.............................................................................

void 
FromJSON( const string & json, A * pA )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    FromJSON( jsonObj[ "character" ], &(pA->m_character) );
    FromJSON( jsonObj[ "integer" ], &(pA->m_integer) );
    FromJSON( jsonObj[ "long" ], &(pA->m_long) );
    FromJSON( jsonObj[ "real" ], &(pA->m_real) );
    FromJSON( jsonObj[ "stringArray" ], &(pA->m_stringArray) );
    FromJSON( jsonObj[ "boolVector" ], &(pA->m_boolVector) );
}

//-----------------------------------------------------------------------------

struct S
{
    array< A, 2 >   m_aa;
};

//.............................................................................

string
ToJSON( const S & s )
{
    JSONObject jsonObj;
    jsonObj[ "aa" ] = ToJSON( s.m_aa );
    return ToJSON( jsonObj );
}

//.............................................................................

void 
FromJSON( const string & json, S * pS )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    FromJSON( jsonObj[ "aa" ], &(pS->m_aa) );
}

//-----------------------------------------------------------------------------

bool
TestJSON( )
{
    bool ok = true;
    cout << "Testing JSON" << endl;

    string s0[] = { "bir", "\tiki", "üç\n" };
    bool b0[5] = { true, false, true, false, true };
    string s1[] = { "uno\n", "\"dos\"", "tres\\" };
    bool b1[5] = { false, false, true, true, false };
    cout << "s0 = { " << s0[0] << ", " << s0[1] << ", " << s0[2] << " }"
         << endl;
    cout << "b0 = { { true, false, true, false, true }" << endl;
    cout << "s1 = { " << s1[0] << ", " << s1[1] << ", " << s1[2] << " }"
         << endl;
    cout << "b1 = { false, false, true, true, false }" << endl;
    cout << "s.m_aa[0].Set( 'd', 137, 1000L, -1.61803f, s0, b0 )" << endl;
    cout << "s.m_aa[1].Set( 'm', -1, 1e10f, s1, b1 )" << endl;
    S s;
    s.m_aa[0].Set( 'd', 137, 1000L, -1.61803f, s0, b0 );
    s.m_aa[1].Set( 'm', -1, -1000L, 1e10f, s1, b1 );
    TESTCHECK( ToJSON( s ),
               string( "{\n"
                       "\"aa\": [ "
                       "{\n"
                       "\"boolVector\": [ true, false, true, false, true ],\n"
                       "\"character\": 100,\n"
                       "\"integer\": 137,\n"
                       "\"long\": 1000,\n"
                       "\"real\": -1.61803e+00,\n"
                       "\"stringArray\": [ \"bir\", \"\\tiki\", \"üç\\n\" ]\n"
                       "}, "
                       "{\n"
                       "\"boolVector\": [ false, false, true, true, false ],\n"
                       "\"character\": 109,\n"
                       "\"integer\": -1,\n"
                       "\"long\": -1000,\n"
                       "\"real\": +1.00000e+10,\n"
                       "\"stringArray\": [ \"uno\\n\", \"\\\"dos\\\"\","
                       " \"tres\\\\\" ]\n"
                       "}"
                       " ]\n"
                       "}" ),
                       &ok );

    S sf;
    cout << "FromJSON( ToJSON( s ), &sf )" << endl;
    FromJSON( ToJSON( s ), &sf );
    TESTCHECK( sf.m_aa[0].Character(), 100, &ok );
    TESTCHECK( sf.m_aa[0].Integer(), 137, &ok );
    TESTCHECK( sf.m_aa[0].Long(), 1000L, &ok );
    TESTCHECKF( sf.m_aa[0].Real(), -1.61803f, &ok );
    TESTCHECK( sf.m_aa[0].ArrayString( 0 ), string( "bir" ), &ok );
    TESTCHECK( sf.m_aa[0].ArrayString( 1 ), string( "\tiki" ), &ok );
    TESTCHECK( sf.m_aa[0].ArrayString( 2 ), string( "üç\n" ), &ok );
    TESTCHECK( sf.m_aa[0].BoolVectorSize(), 5, &ok );
    TESTCHECK( sf.m_aa[0].VectorBool( 0 ), true, &ok );
    TESTCHECK( sf.m_aa[0].VectorBool( 1 ), false, &ok );
    TESTCHECK( sf.m_aa[0].VectorBool( 2 ), true, &ok );
    TESTCHECK( sf.m_aa[0].VectorBool( 3 ), false, &ok );
    TESTCHECK( sf.m_aa[0].VectorBool( 4 ), true, &ok );
    TESTCHECK( sf.m_aa[1].Character(), 109, &ok );
    TESTCHECK( sf.m_aa[1].Integer(), -1, &ok );
    TESTCHECK( sf.m_aa[1].Long(), -1000L, &ok );
    TESTCHECK( sf.m_aa[1].Real(), 1e10f, &ok );
    TESTCHECK( sf.m_aa[1].ArrayString( 0 ), string( "uno\n" ), &ok );
    TESTCHECK( sf.m_aa[1].ArrayString( 1 ), string( "\"dos\"" ), &ok );
    TESTCHECK( sf.m_aa[1].ArrayString( 2 ), string( "tres\\" ), &ok );
    TESTCHECK( sf.m_aa[1].BoolVectorSize(), 5, &ok );
    TESTCHECK( sf.m_aa[1].VectorBool( 0 ), false, &ok );
    TESTCHECK( sf.m_aa[1].VectorBool( 1 ), false, &ok );
    TESTCHECK( sf.m_aa[1].VectorBool( 2 ), true, &ok );
    TESTCHECK( sf.m_aa[1].VectorBool( 3 ), true, &ok );
    TESTCHECK( sf.m_aa[1].VectorBool( 4 ), false, &ok );
    
    if ( ok )
        cout << "JSON PASSED." << endl << endl;
    else
        cout << "JSON FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG

//*****************************************************************************

}                                                      //namespace EpsilonDelta

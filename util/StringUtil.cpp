/*
  StringUtil.cpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for strings (C and C++).
*/


#include "StringUtil.hpp"
#include "StdInt.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Array.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


string 
IntToString( int i, int width, char comma, bool zeroFill, bool showSign )
{
    return IntToBasicString< char, char_traits< char >, allocator< char > >
            ( i, width, comma, zeroFill, showSign );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

wstring 
IntToWString( int i, int width, char comma, bool zeroFill, bool showSign )
{
    return IntToBasicString< wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
            ( i, width, comma, zeroFill, showSign );
}

//-----------------------------------------------------------------------------

string 
RealToString( double r, int width, int decimals, char point, char comma,
              bool zeroFill, bool showSign )
{
    return RealToBasicString< char, char_traits< char >, allocator< char > >
            ( r, width, decimals, point, comma, zeroFill, showSign );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

wstring 
RealToWString( double r, int width, int decimals, char point, char comma,
               bool zeroFill, bool showSign )
{
    return RealToBasicString< wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
            ( r, width, decimals, point, comma, zeroFill, showSign );
}

//-----------------------------------------------------------------------------

string 
OrdinalToString( int i, int width, char comma )
{
    string intStr = IntToString( i, width - 2, comma );
    int r = i % 100;
    if ( (r >= 4) && (r <= 20) )
        return (intStr + "th");
    else
    {
        r = r % 10;
        if ( r == 1)
            return (intStr + "st");
        else if ( r == 2)
            return (intStr + "nd");
        else if ( r == 3)
            return (intStr + "rd");
        else
            return (intStr + "th");
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

wstring 
OrdinalToWString( int i, int width, char comma )
{
    wstring intStr = IntToWString( i, width - 2, comma );
    int r = i % 100;
    if ( (r >= 4) && (r <= 20) )
        return (intStr + L"th");
    else
    {
        r = r % 10;
        if ( r == 1)
            return (intStr + L"st");
        else if ( r == 2)
            return (intStr + L"nd");
        else if ( r == 3)
            return (intStr + L"rd");
        else
            return (intStr + L"th");
    }
}

//=============================================================================

#ifdef DEBUG

namespace
{
bool TestNumbersToString( );
}


bool
TestStringUtil( )
{
    bool ok = true;
    cout << "Testing StringUtil" << endl;

    char testPChar[ 100 ] = "ABC\xC7 abc\xE7 123 !@#";
    cout << "testPChar=\"" << testPChar << "\"" << endl;
    TESTCHECK( ToUpper( testPChar ), "ABC\xC7 ABC\xC7 123 !@#", &ok );
    TESTCHECK( ToLower( testPChar ), "abc\xE7 abc\xE7 123 !@#", &ok );
    string testString = "xyz\xFC XYZ\xDC 789 &*(";
    cout << "testString=\"" << testString << "\"" << endl;
    TESTCHECK( *ToUpper( &testString ), string( "XYZ\xDC XYZ\xDC 789 &*(" ),
               &ok );
    TESTCHECK( *ToLower( &testString ), string( "xyz\xFC xyz\xFC 789 &*(" ),
               &ok );
    const string cString1 = "xyz\xFC XYZ\xDC 789 &*(";
    cout << "cString1=\"" << cString1 << "\"" << endl;
    TESTCHECK( ToUpper( cString1 ), string( "XYZ\xDC XYZ\xDC 789 &*(" ), &ok );
    TESTCHECK( ToLower( cString1 ), string( "xyz\xFC xyz\xFC 789 &*(" ), &ok );

    wchar_t testPWChar[ 100 ] = L"ABC\xC7\x11E abc\xE7\x11F 123 !@#";
    wcout << L"testPWChar=\"" << testPWChar << L"\"" << endl;
    TESTCHECK( ToUpper( testPWChar ),
                  L"ABC\xC7\x11E ABC\xC7\x11E 123 !@#", &ok );
    TESTCHECK( ToLower( testPWChar ),
                  L"abc\xE7\x11F abc\xE7\x11F 123 !@#", &ok );
    wstring testWString = L"xyz\xFC\x15F XYZ\xDC\x15E 789 &*(";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    TESTCHECK( *ToUpper( &testWString ),
                wstring( L"XYZ\xDC\x15E XYZ\xDC\x15E 789 &*(" ), &ok );
    TESTCHECK( *ToLower( &testWString ),
                wstring( L"xyz\xFC\x15F xyz\xFC\x15F 789 &*(" ), &ok );
    const wstring cWString1 = L"xyz\xFC\x15F XYZ\xDC\x15E 789 &*(";
    wcout << L"cWString1=\"" << cWString1 << L"\"" << endl;
    TESTCHECK( ToUpper( cWString1 ),
                wstring( L"XYZ\xDC\x15E XYZ\xDC\x15E 789 &*(" ), &ok );
    TESTCHECK( ToLower( cWString1 ),
                wstring( L"xyz\xFC\x15F xyz\xFC\x15F 789 &*(" ), &ok );

    TESTCHECK( CompareNoCase( "mNoP", "MnOp" ), 0, &ok );
    TESTCHECK( CompareNoCase( "MnOp", "qRsT" ), -1, &ok );
    TESTCHECK( CompareNoCase( "mNoP", "QrSt" ), -1, &ok );
    TESTCHECK( CompareNoCase( "uVwX", "UvW" ), 1, &ok );
    TESTCHECK( CompareNoCase( "UvWx", "uVw" ), 1, &ok );
    TESTCHECK( CompareNoCase( string( "mNoP" ), string( "MnOp" ) ), 0, &ok );
    TESTCHECK( CompareNoCase( string( "MnOp" ), string( "qRsT" ) ), -1, &ok );
    TESTCHECK( CompareNoCase( string( "mNoP" ), string( "QrSt" ) ), -1, &ok );
    TESTCHECK( CompareNoCase( string( "uVwX" ), string( "UvW" ) ), 1, &ok );
    TESTCHECK( CompareNoCase( string( "UvWx" ), string( "uVw" ) ), 1, &ok );
    TESTCHECK( CompareNoCase( L"mNoP", L"MnOp" ), 0, &ok );
    TESTCHECK( CompareNoCase( L"MnOp", L"qRsT" ), -1, &ok );
    TESTCHECK( CompareNoCase( L"mNoP", L"QrSt" ), -1, &ok );
    TESTCHECK( CompareNoCase( L"uVwX", L"UvW" ), 1, &ok );
    TESTCHECK( CompareNoCase( L"UvWx", L"uVw" ), 1, &ok );
    TESTCHECK( CompareNoCase( wstring( L"mNoP" ), wstring( L"MnOp" ) ), 0,
               &ok );
    TESTCHECK( CompareNoCase( wstring( L"MnOp" ), wstring( L"qRsT" ) ), -1,
               &ok );
    TESTCHECK( CompareNoCase( wstring( L"mNoP" ), wstring( L"QrSt" ) ), -1,
               &ok );
    TESTCHECK( CompareNoCase( wstring( L"uVwX" ), wstring( L"UvW" ) ), 1,
               &ok );
    TESTCHECK( CompareNoCase( wstring( L"UvWx" ), wstring( L"uVw" ) ), 1,
               &ok );

    strcpy( testPChar, "  \tabcd \t  " );
    cout << "testPChar=\"" << testPChar << "\"" << endl;
    TESTCHECK( TrimLeading( testPChar ), "abcd \t  ", &ok );
    TESTCHECK( TrimTrailing( testPChar ), "abcd", &ok );
    strcpy( testPChar, "  \tabcd \t  " );
    cout << "testPChar=\"" << testPChar << "\"" << endl;
    TESTCHECK( Trim( testPChar ), "abcd", &ok );
    strcpy( testPChar, "" );
    cout << "testPChar=\"" << testPChar << "\"" << endl;
    TESTCHECK( TrimLeading( testPChar ), "", &ok );
    TESTCHECK( TrimTrailing( testPChar ), "", &ok );
    TESTCHECK( Trim( testPChar ), "", &ok );

    wcscpy( testPWChar, L"  \tabcd \t  " );
    wcout << L"testPChar=\"" << testPWChar << L"\"" << endl;
    TESTCHECK( TrimLeading( testPWChar ), L"abcd \t  ", &ok );
    TESTCHECK( TrimTrailing( testPWChar ), L"abcd", &ok );
    wcscpy( testPWChar, L"  \tabcd \t  " );
    wcout << L"testPChar=\"" << testPWChar << L"\"" << endl;
    TESTCHECK( Trim( testPWChar ), L"abcd", &ok );
    wcscpy( testPWChar, L"" );
    wcout << L"testPChar=\"" << testPWChar << L"\"" << endl;
    TESTCHECK( TrimLeading( testPWChar ), L"", &ok );
    TESTCHECK( TrimTrailing( testPWChar ), L"", &ok );
    TESTCHECK( Trim( testPWChar ), L"", &ok );

    testString = "  \twxyz \t  ";
    cout << "testString=\"" << testString << "\"" << endl;
    const string cString2 = testString;
    cout << "cString2=\"" << cString2 << "\"" << endl;
    TESTCHECK( *TrimLeading( &testString ), string( "wxyz \t  " ), &ok );
    TESTCHECK( TrimLeading( cString2 ), string( "wxyz \t  " ), &ok );
    TESTCHECK( *TrimTrailing( &testString ), string( "wxyz" ), &ok );
    TESTCHECK( TrimTrailing( cString2 ), string( "  \twxyz" ), &ok );
    testString = "  \twxyz \t  ";
    cout << "testString=\"" << testString << "\"" << endl;
    TESTCHECK( *Trim( &testString ), string( "wxyz" ), &ok );
    TESTCHECK( Trim( cString2 ), string( "wxyz" ), &ok );
    testString = "";
    cout << "testString=\"" << testString << "\"" << endl;
    const string cString3 = testString;
    cout << "cString3=\"" << cString3 << "\"" << endl;
    TESTCHECK( *TrimLeading( &testString ), string( "" ), &ok );
    TESTCHECK( TrimLeading( cString3 ), string( "" ), &ok );
    TESTCHECK( *TrimTrailing( &testString ), string( "" ), &ok );
    TESTCHECK( TrimTrailing( cString3 ), string( "" ), &ok );
    TESTCHECK( *Trim( &testString ), string( "" ), &ok );
    TESTCHECK( Trim( cString3 ), string( "" ), &ok );

    testWString = L"  \twxyz \t  ";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    const wstring cWString2 = testWString;
    wcout << L"cWString2=\"" << cWString2 << L"\"" << endl;
    TESTCHECK( *TrimLeading( &testWString ), wstring( L"wxyz \t  " ), &ok );
    TESTCHECK( TrimLeading( cWString2 ), wstring( L"wxyz \t  " ), &ok );
    TESTCHECK( *TrimTrailing( &testWString ), wstring( L"wxyz" ), &ok );
    TESTCHECK( TrimTrailing( cWString2 ), wstring( L"  \twxyz" ), &ok );
    testWString = L"  \twxyz \t  ";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    TESTCHECK( *Trim( &testWString ), wstring( L"wxyz" ), &ok );
    TESTCHECK( Trim( cWString2 ), wstring( L"wxyz" ), &ok );
    testWString = L"";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    const wstring cWString3 = testWString;
    wcout << L"cWString3=\"" << cWString3 << L"\"" << endl;
    TESTCHECK( *TrimLeading( &testWString ), wstring( L"" ), &ok );
    TESTCHECK( TrimLeading( cWString3 ), wstring( L"" ), &ok );
    TESTCHECK( *TrimTrailing( &testWString ), wstring( L"" ), &ok );
    TESTCHECK( TrimTrailing( cWString3 ), wstring( L"" ), &ok );
    TESTCHECK( *Trim( &testWString ), wstring( L"" ), &ok );
    TESTCHECK( Trim( cWString3 ), wstring( L"" ), &ok );

    const string quoteTest = "He said, 'It/'s '', not \".'";
    testString = quoteTest;
    cout << "testString=\"" << testString << "\"" << endl;
    const string cString4 = testString;
    cout << "cString4=\"" << cString4 << "\"" << endl;
    TESTCHECK( *AddQuotes( &testString, '\'' ),
               string( "'He said, ''It/''s '''', not \".'''" ), &ok );
    TESTCHECK( AddQuotes( cString4, '\'' ),
               string( "'He said, ''It/''s '''', not \".'''" ), &ok );
    const string cString5 = testString;
    cout << "cString5=\"" << cString5 << "\"" << endl;
    TESTCHECK( *RemoveQuotes( &testString, '\'' ), quoteTest, &ok );
    TESTCHECK( RemoveQuotes( cString5, '\'' ), quoteTest, &ok );
    testString = quoteTest;
    TESTCHECK( *AddQuotes( &testString, '\'', '/' ),
               string( "'He said, /'It///'s /'/', not \"./''" ), &ok );
    TESTCHECK( AddQuotes( cString4, '\'', '/' ),
               string( "'He said, /'It///'s /'/', not \"./''" ), &ok );
    TESTCHECK( RemoveQuotes( testString, '\'', '/' ), quoteTest, &ok );
    TESTCHECK( *RemoveQuotes( &testString, '\'', '/' ), quoteTest, &ok );

    const wstring quoteTestW = L"He said, 'It/'s '', not \".'";
    testWString = quoteTestW;
    wcout << L"testString=\"" << testWString << L"\"" << endl;
    const wstring cWString4 = testWString;
    wcout << L"cWString4=\"" << cWString4 << L"\"" << endl;
    TESTCHECK( *AddQuotes( &testWString, L'\'' ),
               wstring( L"'He said, ''It/''s '''', not \".'''" ), &ok );
    TESTCHECK( AddQuotes( cWString4, L'\'' ),
               wstring( L"'He said, ''It/''s '''', not \".'''" ), &ok );
    const wstring cWString5 = testWString;
    wcout << L"cWString5=\"" << cWString5 << L"\"" << endl;
    TESTCHECK( *RemoveQuotes( &testWString, L'\'' ), quoteTestW, &ok );
    TESTCHECK( RemoveQuotes( cWString5, L'\'' ), quoteTestW, &ok );
    testWString = quoteTestW;
    TESTCHECK( *AddQuotes( &testWString, L'\'', L'/' ),
               wstring( L"'He said, /'It///'s /'/', not \"./''" ), &ok );
    TESTCHECK( AddQuotes( cWString4, L'\'', L'/' ),
               wstring( L"'He said, /'It///'s /'/', not \"./''" ), &ok );
    TESTCHECK( RemoveQuotes( testWString, L'\'', L'/' ), quoteTestW, &ok );
    TESTCHECK( *RemoveQuotes( &testWString, L'\'', L'/' ), quoteTestW, &ok );

    testString = "to be or not to be";
    cout << "testString=\"" << testString << "\"" << endl;
    const string cString6 = testString;
    cout << "cString6=\"" << cString6 << "\"" << endl;
    TESTCHECK( *Replace( &testString, "to be", "to be or not to be" ),
               string( "to be or not to be or not to be or not to be" ), &ok );
    TESTCHECK( Replace( cString6, "to be", "to be or not to be" ),
               string( "to be or not to be or not to be or not to be" ), &ok );
    TESTCHECK( *Replace( &testString, "To be", "to be or not to be" ),
               string( "to be or not to be or not to be or not to be" ), &ok );
    TESTCHECK( Replace( cString6, "To be", "to be or not to be" ),
               string( "to be or not to be" ), &ok );

    testWString = L"to be or not to be";
    wcout << L"testString=\"" << testWString << L"\"" << endl;
    const wstring cWString6 = testWString;
    wcout << L"cWString6=\"" << cWString6 << L"\"" << endl;
    TESTCHECK( *Replace( &testWString, L"to be", L"to be or not to be" ),
               wstring( L"to be or not to be or not to be or not to be" ),
               &ok );
    TESTCHECK( Replace( cWString6, L"to be", L"to be or not to be" ),
               wstring( L"to be or not to be or not to be or not to be" ),
               &ok );
    TESTCHECK( *Replace( &testWString, L"To be", L"to be or not to be" ),
               wstring( L"to be or not to be or not to be or not to be" ),
               &ok );
    TESTCHECK( Replace( cWString6, L"To be", L"to be or not to be" ),
               wstring( L"to be or not to be" ), &ok );

    testString = ":abc : def:";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':' )" << endl;
    vector< string > parts = Split( testString, ':' );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[2], string( " def" ), &ok );
    TESTCHECK( parts[3], string( "" ), &ok );
    cout << "Split( testString, ',' )" << endl;
    parts = Split( testString, ',' );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], testString, &ok );
    testString = "";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':' )" << endl;
    parts = Split( testString, ':' );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );

    testWString = L":abc : def:";
    wcout << L"testString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':' )" << endl;
    vector< wstring > partsW = Split( testWString, L':' );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" def" ), &ok );
    TESTCHECK( partsW[3], wstring( L"" ), &ok );
    wcout << L"Split( testWString, L',' )" << endl;
    partsW = Split( testWString, L',' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    testWString = L"";
    wcout << L"testString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':' )" << endl;
    partsW = Split( testWString, L':' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testString = ":abc/' : 'def/:ghi/'':''''";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'' )" << endl;
    parts = Split( testString, ':', '\'' );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[1], string( "abc/ : def/" ), &ok );
    TESTCHECK( parts[2], string( "ghi/" ), &ok );
    TESTCHECK( parts[3], string( "'" ), &ok );
    cout << "Split( testString, ':', '\"' )" << endl;
    parts = Split( testString, ':', '\"' );
    TESTCHECK( parts.size(), 5, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[2], string( " 'def/" ), &ok );
    TESTCHECK( parts[3], string( "ghi/''" ), &ok );
    TESTCHECK( parts[4], string( "''''" ), &ok );
    cout << "Split( testString, ',', '\'' )" << endl;
    parts = Split( testString, ',', '\'' );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( ":abc/ : def/:ghi/:'" ), &ok );
    cout << "Split( testString, ',', '\"' )" << endl;
    parts = Split( testString, ',', '\"' );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], testString, &ok );
    testString = "";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'' )" << endl;
    parts = Split( testString, ':', '\'' );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );

    testString = ":abc/' : 'def/:ghi/'':''''";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', true )" << endl;
    parts = Split( testString, ':', '\'', true );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[1], string( "abc/' : 'def/" ), &ok );
    TESTCHECK( parts[2], string( "ghi/''" ), &ok );
    TESTCHECK( parts[3], string( "''''" ), &ok );
    cout << "Split( testString, ':', '\"', true )" << endl;
    parts = Split( testString, ':', '\"', true );
    TESTCHECK( parts.size(), 5, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[2], string( " 'def/" ), &ok );
    TESTCHECK( parts[3], string( "ghi/''" ), &ok );
    TESTCHECK( parts[4], string( "''''" ), &ok );
    cout << "Split( testString, ',', '\'', true )" << endl;
    parts = Split( testString, ',', '\'', true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], testString, &ok );
    cout << "Split( testString, ',', '\"', true )" << endl;
    parts = Split( testString, ',', '\"', true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], testString, &ok );
    testString = "";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', true )" << endl;
    parts = Split( testString, ':', '\'', true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''''";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'' )" << endl;
    partsW = Split( testWString, L':', L'\'' );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc/ : def/" ), &ok );
    TESTCHECK( partsW[2], wstring( L"ghi/" ), &ok );
    TESTCHECK( partsW[3], wstring( L"'" ), &ok );
    wcout << L"Split( testWString, L':', L'\"' )" << endl;
    partsW = Split( testWString, L':', L'\"' );
    TESTCHECK( partsW.size(), 5, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/" ), &ok );
    TESTCHECK( partsW[3], wstring( L"ghi/''" ), &ok );
    TESTCHECK( partsW[4], wstring( L"''''" ), &ok );
    wcout << L"Split( testWString, L',', L'\'' )" << endl;
    partsW = Split( testWString, L',', L'\'' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc/ : def/:ghi/:'" ), &ok );
    wcout << L"Split( testWString, L',', L'\"' )" << endl;
    partsW = Split( testWString, L',', L'\"' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    testWString = L"";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'' )" << endl;
    partsW = Split( testWString, L':', L'\'' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''''";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', true )" << endl;
    partsW = Split( testWString, L':', L'\'', true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc/' : 'def/" ), &ok );
    TESTCHECK( partsW[2], wstring( L"ghi/''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''''" ), &ok );
    wcout << L"Split( testWString, L':', L'\"', true )" << endl;
    partsW = Split( testWString, L':', L'\"', true );
    TESTCHECK( partsW.size(), 5, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/" ), &ok );
    TESTCHECK( partsW[3], wstring( L"ghi/''" ), &ok );
    TESTCHECK( partsW[4], wstring( L"''''" ), &ok );
    wcout << L"Split( testWString, L',', L'\'', true )" << endl;
    partsW = Split( testWString, L',', L'\'', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    wcout << L"Split( testWString, L',', L'\"', true )" << endl;
    partsW = Split( testWString, L',', L'\"', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    testWString = L"";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', true )" << endl;
    partsW = Split( testWString, L':', L'\'', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testString = ":abc/' : 'def/:ghi/'':''";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', '/' )" << endl;
    parts = Split( testString, ':', '\'', '/' );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[1], string( "abc' " ), &ok );
    TESTCHECK( parts[2], string( " def:ghi'" ), &ok );
    TESTCHECK( parts[3], string( "" ), &ok );
    cout << "Split( testString, ':', '\"', '/' )" << endl;
    parts = Split( testString, ':', '\"', '/' );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[2], string( " 'def:ghi''" ), &ok );
    TESTCHECK( parts[3], string( "''" ), &ok );
    cout << "Split( testString, ',', '\'', '/' )" << endl;
    parts = Split( testString, ',', '\'', '/' );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( ":abc' : def:ghi':" ), &ok );
    cout << "Split( testString, ',', '\"', '/' )" << endl;
    parts = Split( testString, ',', '\"', '/' );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( ":abc' : 'def:ghi'':''" ), &ok );
    testString = "";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', '/' )" << endl;
    parts = Split( testString, ':', '\'', '/' );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );

    testString = ":abc/' : 'def/:ghi/'':''";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', '/', true )" << endl;
    parts = Split( testString, ':', '\'', '/', true );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[1], string( "abc' " ), &ok );
    TESTCHECK( parts[2], string( " 'def:ghi''" ), &ok );
    TESTCHECK( parts[3], string( "''" ), &ok );
    cout << "Split( testString, ':', '\"', '/', true )" << endl;
    parts = Split( testString, ':', '\"', '/', true );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[2], string( " 'def:ghi''" ), &ok );
    TESTCHECK( parts[3], string( "''" ), &ok );
    cout << "Split( testString, ',', '\'', '/', true )" << endl;
    parts = Split( testString, ',', '\'', '/', true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( ":abc' : 'def:ghi'':''" ), &ok );
    cout << "Split( testString, ',', '\"', '/', true )" << endl;
    parts = Split( testString, ',', '\"', '/', true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( ":abc' : 'def:ghi'':''" ), &ok );
    testString = "";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', '/', true )" << endl;
    parts = Split( testString, ':', '\'', '/', true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );

    testString = ":abc/' : 'def/:ghi/'':''";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', '/', false, true )" << endl;
    parts = Split( testString, ':', '\'', '/', false, true );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[1], string( "abc/' " ), &ok );
    TESTCHECK( parts[2], string( " def/:ghi/'" ), &ok );
    TESTCHECK( parts[3], string( "" ), &ok );
    cout << "Split( testString, ':', '\"', '/', false, true )" << endl;
    parts = Split( testString, ':', '\"', '/', false, true );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[2], string( " 'def/:ghi/''" ), &ok );
    TESTCHECK( parts[3], string( "''" ), &ok );
    cout << "Split( testString, ',', '\'', '/', false, true )" << endl;
    parts = Split( testString, ',', '\'', '/', false, true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( ":abc/' : def/:ghi/':" ), &ok );
    cout << "Split( testString, ',', '\"', '/', false, true )" << endl;
    parts = Split( testString, ',', '\"', '/', false, true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( ":abc/' : 'def/:ghi/'':''" ), &ok );
    testString = "";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', '/', false, true )" << endl;
    parts = Split( testString, ':', '\'', '/', false, true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );

    testString = ":abc/' : 'def/:ghi/'':''";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', '/', true, true )" << endl;
    parts = Split( testString, ':', '\'', '/', true, true );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[1], string( "abc/' " ), &ok );
    TESTCHECK( parts[2], string( " 'def/:ghi/''" ), &ok );
    TESTCHECK( parts[3], string( "''" ), &ok );
    cout << "Split( testString, ':', '\"', '/', true, true )" << endl;
    parts = Split( testString, ':', '\"', '/', true, true );
    TESTCHECK( parts.size(), 4, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[2], string( " 'def/:ghi/''" ), &ok );
    TESTCHECK( parts[3], string( "''" ), &ok );
    cout << "Split( testString, ',', '\'', '/', true, true )" << endl;
    parts = Split( testString, ',', '\'', '/', true, true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], testString, &ok );
    cout << "Split( testString, ',', '\"', '/', true, true )" << endl;
    parts = Split( testString, ',', '\"', '/', true, true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], testString, &ok );
    testString = "";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, ':', '\'', '/', true, true )" << endl;
    parts = Split( testString, ':', '\'', '/', true, true );
    TESTCHECK( parts.size(), 1, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );

    testString = "abc123def123abc";
    cout << "testString=\"" << testString << "\"" << endl;
    cout << "Split( testString, string(\"123\") )" << endl;
    parts = Split( testString, string( "123" ) );
    TESTCHECK( parts.size(), 3, &ok );
    TESTCHECK( parts[0], string( "abc" ), &ok );
    TESTCHECK( parts[1], string( "def" ), &ok );
    TESTCHECK( parts[2], string( "abc" ), &ok );
    cout << "Split( testString, string(\"abc\") )" << endl;
    parts = Split( testString, string( "abc" ) );
    TESTCHECK( parts.size(), 3, &ok );
    TESTCHECK( parts[0], string( "" ), &ok );
    TESTCHECK( parts[1], string( "123def123" ), &ok );
    TESTCHECK( parts[2], string( "" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/' )" << endl;
    partsW = Split( testWString, L':', L'\'', L'/' );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc' " ), &ok );
    TESTCHECK( partsW[2], wstring( L" def:ghi'" ), &ok );
    TESTCHECK( partsW[3], wstring( L"" ), &ok );
    wcout << L"Split( testWString, L':', L'\"', L'/' )" << endl;
    partsW = Split( testWString, L':', L'\"', L'/' );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def:ghi''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
    wcout << L"Split( testWString, L',', L'\'', L'/' )" << endl;
    partsW = Split( testWString, L',', L'\'', L'/' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc' : def:ghi':" ), &ok );
    wcout << L"Split( testWString, L',', L'\"', L'/' )" << endl;
    partsW = Split( testWString, L',', L'\"', L'/' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc' : 'def:ghi'':''" ), &ok );
    testWString = L"";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/' )" << endl;
    partsW = Split( testWString, L':', L'\'', L'/' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', true )" << endl;
    partsW = Split( testWString, L':', L'\'', L'/', true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc' " ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def:ghi''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
    wcout << L"Split( testWString, L':', L'\"', L'/', true )" << endl;
    partsW = Split( testWString, L':', L'\"', L'/', true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def:ghi''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
    wcout << L"Split( testWString, L',', L'\'', L'/', true )" << endl;
    partsW = Split( testWString, L',', L'\'', L'/', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc' : 'def:ghi'':''" ), &ok );
    wcout << L"Split( testWString, L',', L'\"', L'/', true )" << endl;
    partsW = Split( testWString, L',', L'\"', L'/', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc' : 'def:ghi'':''" ), &ok );
    testWString = L"";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', true )" << endl;
    partsW = Split( testWString, L':', L'\'', L'/', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', false, true )" << endl;
    partsW = Split( testWString, L':', L'\'', L'/', false, true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc/' " ), &ok );
    TESTCHECK( partsW[2], wstring( L" def/:ghi/'" ), &ok );
    TESTCHECK( partsW[3], wstring( L"" ), &ok );
    wcout << L"Split( testWString, L':', L'\"', L'/', false, true )" << endl;
    partsW = Split( testWString, L':', L'\"', L'/', false, true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/:ghi/''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
    wcout << L"Split( testWString, L',', L'\'', L'/', false, true )" << endl;
    partsW = Split( testWString, L',', L'\'', L'/', false, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc/' : def/:ghi/':" ), &ok );
    wcout << L"Split( testWString, L',', L'\"', L'/', false, true )" << endl;
    partsW = Split( testWString, L',', L'\"', L'/', false, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc/' : 'def/:ghi/'':''" ), &ok );
    testWString = L"";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', false, true )" << endl;
    partsW = Split( testWString, L':', L'\'', L'/', false, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', true, true )" << endl;
    partsW = Split( testWString, L':', L'\'', L'/', true, true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc/' " ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/:ghi/''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
    wcout << L"Split( testWString, L':', L'\"', L'/', true, true )" << endl;
    partsW = Split( testWString, L':', L'\"', L'/', true, true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/:ghi/''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
    wcout << L"Split( testWString, L',', L'\'', L'/', true, true )" << endl;
    partsW = Split( testWString, L',', L'\'', L'/', true, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    wcout << L"Split( testWString, L',', L'\"', L'/', true, true )" << endl;
    partsW = Split( testWString, L',', L'\"', L'/', true, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    testWString = L"";
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', true, true )" << endl;
    partsW = Split( testWString, L':', L'\'', L'/', true, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L"abc123def123abc";
    wcout << L"testString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, wstring(L\"123\") )" << endl;
    partsW = Split( testWString, wstring( L"123" ) );
    TESTCHECK( partsW.size(), 3, &ok );
    TESTCHECK( partsW[0], wstring( L"abc" ), &ok );
    TESTCHECK( partsW[1], wstring( L"def" ), &ok );
    TESTCHECK( partsW[2], wstring( L"abc" ), &ok );
    wcout << L"Split( testString, wstring(L\"abc\") )" << endl;
    partsW = Split( testWString, wstring( L"abc" ) );
    TESTCHECK( partsW.size(), 3, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"123def123" ), &ok );
    TESTCHECK( partsW[2], wstring( L"" ), &ok );

    if ( ! TestNumbersToString( ) )
        ok = false;

    if ( ok )
        cout << "StringUtil PASSED." << endl << endl;
    else
        cout << "StringUtil FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

namespace
{                                                                   //namespace

bool 
TestNumbersToString( )
{
    bool ok = true;

    TESTCHECK( IntToString( 137 ), string( "137" ), &ok );
    TESTCHECK( IntToString( 137, 2 ), string( "137" ), &ok );
    TESTCHECK( IntToString( 137, 5 ), string( "  137" ), &ok );
    TESTCHECK( IntToString( 137, 5, ',' ), string( "  137" ), &ok );
    TESTCHECK( IntToString( 137, 5, 0, true ), string( "00137" ), &ok );
    TESTCHECK( IntToString( 137, 5, ',', true ), string( "00137" ), &ok );
    TESTCHECK( IntToString( 137, 5, 0, true, true ), string( "+0137" ),
               &ok );
    TESTCHECK( IntToString( 137, 5, ',', true, true ), string( "+0137" ),
               &ok );
    TESTCHECK( IntToString( 137, 5, 0, false, true ), string( "+ 137" ),
               &ok );
    TESTCHECK( IntToString( 137, 5, ',', false, true ), string( "+ 137" ),
               &ok );
    TESTCHECK( IntToString( -137 ), string( "-137" ), &ok );
    TESTCHECK( IntToString( -137, 2 ), string( "-137" ), &ok );
    TESTCHECK( IntToString( -137, 5 ), string( "- 137" ), &ok );
    TESTCHECK( IntToString( -137, 5, ',' ), string( "- 137" ), &ok );
    TESTCHECK( IntToString( -137, 5, 0, true ), string( "-0137" ), &ok );
    TESTCHECK( IntToString( -137, 5, ',', true ), string( "-0137" ), &ok );
    TESTCHECK( IntToString( -137, 5, 0, true, true ), string( "-0137" ),
               &ok );
    TESTCHECK( IntToString( -137, 5, ',', true, true ), string( "-0137" ),
               &ok );
    TESTCHECK( IntToString( -137, 5, 0, false, true ), string( "- 137" ),
               &ok );
    TESTCHECK( IntToString( -137, 5, ',', false, true ), string( "- 137" ),
               &ok );
    TESTCHECK( IntToString( 1729 ), string( "1729" ), &ok );
    TESTCHECK( IntToString( 1729, 2 ), string( "1729" ), &ok );
    TESTCHECK( IntToString( 1729, 6 ), string( "  1729" ), &ok );
    TESTCHECK( IntToString( 1729, 6, ',' ), string( " 1,729" ), &ok );
    TESTCHECK( IntToString( 1729, 6, 0, true ), string( "001729" ), &ok );
    TESTCHECK( IntToString( 1729, 6, '.', true ), string( "01.729" ), &ok );
    TESTCHECK( IntToString( 1729, 6, 0, true, true ), string( "+01729" ),
               &ok );
    TESTCHECK( IntToString( 1729, 6, ',', true, true ), string( "+1,729" ),
               &ok );
    TESTCHECK( IntToString( 1729, 6, 0, false, true ), string( "+ 1729" ),
               &ok );
    TESTCHECK( IntToString( 1729, 6, ',', false, true ), string( "+1,729" ),
               &ok );
    TESTCHECK( IntToString( -1729 ), string( "-1729" ), &ok );
    TESTCHECK( IntToString( -1729, 2 ), string( "-1729" ), &ok );
    TESTCHECK( IntToString( -1729, 6 ), string( "- 1729" ), &ok );
    TESTCHECK( IntToString( -1729, 6, ',' ), string( "-1,729" ), &ok );
    TESTCHECK( IntToString( -1729, 6, 0, true ), string( "-01729" ), &ok );
    TESTCHECK( IntToString( -1729, 6, ',', true ), string( "-1,729" ), &ok );
    TESTCHECK( IntToString( -1729, 6, 0, true, true ), string( "-01729" ),
               &ok );
    TESTCHECK( IntToString( -1729, 6, ',', true, true ), string( "-1,729" ),
               &ok );
    TESTCHECK( IntToString( -1729, 6, 0, false, true ), string( "- 1729" ),
               &ok );
    TESTCHECK( IntToString( -1729, 6, ',', false, true ), string( "-1,729" ),
               &ok );
    TESTCHECK( IntToString( 0, 0, 0, false, false ), string( "0" ), &ok );
    TESTCHECK( IntToString( 0, 0, ',', false, false ), string( "0" ), &ok );
    TESTCHECK( IntToString( 0, 0, 0, false, true ), string( "+0" ), &ok );
    TESTCHECK( IntToString( -0, 0, 0, false, true ), string( "+0" ), &ok );

    TESTCHECK( IntToWString( 137 ), wstring( L"137" ), &ok );
    TESTCHECK( IntToWString( 137, 2 ), wstring( L"137" ), &ok );
    TESTCHECK( IntToWString( 137, 5 ), wstring( L"  137" ), &ok );
    TESTCHECK( IntToWString( 137, 5, ',' ), wstring( L"  137" ), &ok );
    TESTCHECK( IntToWString( 137, 5, 0, true ), wstring( L"00137" ), &ok );
    TESTCHECK( IntToWString( 137, 5, ',', true ), wstring( L"00137" ), &ok );
    TESTCHECK( IntToWString( 137, 5, 0, true, true ), wstring( L"+0137" ),
               &ok );
    TESTCHECK( IntToWString( 137, 5, ',', true, true ), wstring( L"+0137" ),
               &ok );
    TESTCHECK( IntToWString( 137, 5, 0, false, true ), wstring( L"+ 137" ),
               &ok );
    TESTCHECK( IntToWString( 137, 5, ',', false, true ), wstring( L"+ 137" ),
               &ok );
    TESTCHECK( IntToWString( -137 ), wstring( L"-137" ), &ok );
    TESTCHECK( IntToWString( -137, 2 ), wstring( L"-137" ), &ok );
    TESTCHECK( IntToWString( -137, 5 ), wstring( L"- 137" ), &ok );
    TESTCHECK( IntToWString( -137, 5, ',' ), wstring( L"- 137" ), &ok );
    TESTCHECK( IntToWString( -137, 5, 0, true ), wstring( L"-0137" ), &ok );
    TESTCHECK( IntToWString( -137, 5, ',', true ), wstring( L"-0137" ), &ok );
    TESTCHECK( IntToWString( -137, 5, 0, true, true ), wstring( L"-0137" ),
               &ok );
    TESTCHECK( IntToWString( -137, 5, ',', true, true ), wstring( L"-0137" ),
               &ok );
    TESTCHECK( IntToWString( -137, 5, 0, false, true ), wstring( L"- 137" ),
               &ok );
    TESTCHECK( IntToWString( -137, 5, ',', false, true ), wstring( L"- 137" ),
               &ok );
    TESTCHECK( IntToWString( 1729 ), wstring( L"1729" ), &ok );
    TESTCHECK( IntToWString( 1729, 2 ), wstring( L"1729" ), &ok );
    TESTCHECK( IntToWString( 1729, 6 ), wstring( L"  1729" ), &ok );
    TESTCHECK( IntToWString( 1729, 6, ',' ), wstring( L" 1,729" ), &ok );
    TESTCHECK( IntToWString( 1729, 6, 0, true ), wstring( L"001729" ), &ok );
    TESTCHECK( IntToWString( 1729, 6, '.', true ), wstring( L"01.729" ),
                &ok );
    TESTCHECK( IntToWString( 1729, 6, 0, true, true ), wstring( L"+01729" ),
               &ok );
    TESTCHECK( IntToWString( 1729, 6, ',', true, true ), wstring( L"+1,729" ),
               &ok );
    TESTCHECK( IntToWString( 1729, 6, 0, false, true ), wstring( L"+ 1729" ),
               &ok );
    TESTCHECK( IntToWString( 1729, 6, ',', false, true ),
                wstring( L"+1,729" ), &ok );
    TESTCHECK( IntToWString( -1729 ), wstring( L"-1729" ), &ok );
    TESTCHECK( IntToWString( -1729, 2 ), wstring( L"-1729" ), &ok );
    TESTCHECK( IntToWString( -1729, 6 ), wstring( L"- 1729" ), &ok );
    TESTCHECK( IntToWString( -1729, 6, ',' ), wstring( L"-1,729" ), &ok );
    TESTCHECK( IntToWString( -1729, 6, 0, true ), wstring( L"-01729" ), &ok );
    TESTCHECK( IntToWString( -1729, 6, ',', true ), wstring( L"-1,729" ),
               &ok );
    TESTCHECK( IntToWString( -1729, 6, 0, true, true ), wstring( L"-01729" ),
               &ok );
    TESTCHECK( IntToWString( -1729, 6, ',', true, true ),
               wstring( L"-1,729" ), &ok );
    TESTCHECK( IntToWString( -1729, 6, 0, false, true ), wstring( L"- 1729" ),
               &ok );
    TESTCHECK( IntToWString( -1729, 6, ',', false, true ),
               wstring( L"-1,729" ), &ok );
    TESTCHECK( IntToWString( 0, 0, 0, false, false ), wstring( L"0" ), &ok );
    TESTCHECK( IntToWString( 0, 0, ',', false, false ), wstring( L"0" ),
               &ok );
    TESTCHECK( IntToWString( 0, 0, 0, false, true ), wstring( L"+0" ), &ok );
    TESTCHECK( IntToWString( -0, 0, 0, false, true ), wstring( L"+0" ), &ok );

    TESTCHECK( RealToString( 137.036 ), string( "137" ), &ok );
    TESTCHECK( RealToString( 137.036, 2 ), string( "137" ), &ok );
    TESTCHECK( RealToString( 137.036, 8 ), string( "     137" ), &ok );
    TESTCHECK( RealToString( 137.036, 8, 2 ), string( "  137.04" ), &ok );
    TESTCHECK( RealToString( 137.036, 8, 2, '.', ',' ), string( "  137.04" ),
               &ok );
    TESTCHECK( RealToString( 137.036, 8, 2, '.', 0, true ),
               string( "00137.04" ), &ok );
    TESTCHECK( RealToString( 137.036, 8, 2, ',', '.', true ),
               string( "00137,04" ), &ok );
    TESTCHECK( RealToString( 137.036, 8, 2, '.', 0, true, true ),
               string( "+0137.04" ), &ok );
    TESTCHECK( RealToString( 137.036, 8, 2, '.',',', true, true ),
               string( "+0137.04" ), &ok );
    TESTCHECK( RealToString( 137.036, 8, 2, '.', 0, false, true ),
               string( "+ 137.04" ), &ok );
    TESTCHECK( RealToString( 137.036, 8, 2, '.', ',', false, true ),
               string( "+ 137.04" ), &ok );
    TESTCHECK( RealToString( -137.036 ), string( "-137" ), &ok );
    TESTCHECK( RealToString( -137.036, 2 ), string( "-137" ), &ok );
    TESTCHECK( RealToString( -137.036, 2, 0, '.', ',' ), string( "-137" ),
               &ok );
    TESTCHECK( RealToString( -137.036, 8 ), string( "-    137" ), &ok );
    TESTCHECK( RealToString( -137.036, 8, 0, '.', ',' ), string( "-    137" ),
               &ok );
    TESTCHECK( RealToString( -137.036, 8, 2 ), string( "- 137.04" ), &ok );
    TESTCHECK( RealToString( -137.036, 8, 2, '.', ',' ), string( "- 137.04" ),
               &ok );
    TESTCHECK( RealToString( -137.036, 8, 2, '.', 0, true ),
               string( "-0137.04" ), &ok );
    TESTCHECK( RealToString( -137.036, 8, 2, '.', ',', true ),
               string( "-0137.04" ), &ok );
    TESTCHECK( RealToString( -137.036, 8, 2, '.', 0, true, true ),
               string( "-0137.04" ), &ok );
    TESTCHECK( RealToString( -137.036, 8, 2, '.', ',', true, true ),
               string( "-0137.04" ), &ok );
    TESTCHECK( RealToString( -137.036, 8, 2, '.', 0, false, true ),
               string( "- 137.04" ), &ok );
    TESTCHECK( RealToString( -137.036, 8, 2, '.', ',', false, true ),
               string( "- 137.04" ), &ok );
    TESTCHECK( RealToString( 1729.036 ), string( "1729" ), &ok );
    TESTCHECK( RealToString( 1729.036, 2 ), string( "1729" ), &ok );
    TESTCHECK( RealToString( 1729.036, 9 ), string( "     1729" ), &ok );
    TESTCHECK( RealToString( 1729.036, 9, 2 ), string( "  1729.04" ), &ok );
    TESTCHECK( RealToString( 1729.036, 9, 2, '.', ',' ), string( " 1,729.04" ),
               &ok );
    TESTCHECK( RealToString( 1729.036, 9, 2, '.', 0, true ),
               string( "001729.04" ), &ok );
    TESTCHECK( RealToString( 1729.036, 9, 2, '.', ',', true ),
               string( "01,729.04" ), &ok );
    TESTCHECK( RealToString( 1729.036, 9, 2, '.', 0, true, true ),
               string( "+01729.04" ), &ok );
    TESTCHECK( RealToString( 1729.036, 9, 2, '.', ',', true, true ),
               string( "+1,729.04" ), &ok );
    TESTCHECK( RealToString( 1729.036, 9, 2, '.', 0, false, true ),
               string( "+ 1729.04" ), &ok );
    TESTCHECK( RealToString( 1729.036, 9, 2, '.', ',', false, true ),
               string( "+1,729.04" ), &ok );
    TESTCHECK( RealToString( -1729.036 ), string( "-1729" ), &ok );
    TESTCHECK( RealToString( -1729.036, 2 ), string( "-1729" ), &ok );
    TESTCHECK( RealToString( -1729.036, 2, 0, '.', ',' ), string( "-1,729" ),
               &ok );
    TESTCHECK( RealToString( -1729.036, 9 ), string( "-    1729" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 0, '.', ',' ),
               string( "-   1,729" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 2 ), string( "- 1729.04" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 2, '.', ',' ),
               string( "-1,729.04" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 2, '.', 0, true ),
               string( "-01729.04" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 2, '.', ',', true ),
               string( "-1,729.04" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 2, '.', 0, true, true ),
               string( "-01729.04" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 2, '.', ',', true, true ),
               string( "-1,729.04" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 2, '.', 0, false, true ),
               string( "- 1729.04" ), &ok );
    TESTCHECK( RealToString( -1729.036, 9, 2, '.', ',', false, true ),
               string( "-1,729.04" ), &ok );
    TESTCHECK( RealToString( 0., 0, 0, '.', 0, false, false ), string( "0" ),
               &ok );
    TESTCHECK( RealToString( 0., 0, 0, '.', 0, false, true ), string( "+0" ),
               &ok );
    TESTCHECK( RealToString( -0., 0, 0, '.', 0, false, true ), string( "+0" ),
               &ok );
    TESTCHECK( RealToString( 0., 0, 2, '.', 0, false, false ),
               string( "0.00" ), &ok );
    TESTCHECK( RealToString( 0., 0, 2, '.', 0, false, true ),
               string( "+0.00" ), &ok );
    TESTCHECK( RealToString( 0., 0, 0, '.', ',', false, false ), string( "0" ),
               &ok );
    TESTCHECK( RealToString( 0., 0, 0, '.', ',', false, true ), string( "+0" ),
               &ok );
    TESTCHECK( RealToString( -0., 0, 0, '.', ',', false, true ),
               string( "+0" ), &ok );
    TESTCHECK( RealToString( 0., 0, 2, '.', ',', false, false ),
               string( "0.00" ), &ok );
    TESTCHECK( RealToString( 0., 0, 2, '.', ',', false, true ),
               string( "+0.00" ), &ok );

    TESTCHECK( RealToWString( 137.036 ), wstring( L"137" ), &ok );
    TESTCHECK( RealToWString( 137.036, 2 ), wstring( L"137" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8 ), wstring( L"     137" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8, 2 ), wstring( L"  137.04" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8, 2, '.', ',' ),
                wstring( L"  137.04" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8, 2, '.', 0, true ),
               wstring( L"00137.04" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8, 2, ',', '.', true ),
               wstring( L"00137,04" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8, 2, '.', 0, true, true ),
               wstring( L"+0137.04" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8, 2, '.',',', true, true ),
               wstring( L"+0137.04" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8, 2, '.', 0, false, true ),
               wstring( L"+ 137.04" ), &ok );
    TESTCHECK( RealToWString( 137.036, 8, 2, '.', ',', false, true ),
               wstring( L"+ 137.04" ), &ok );
    TESTCHECK( RealToWString( -137.036 ), wstring( L"-137" ), &ok );
    TESTCHECK( RealToWString( -137.036, 2 ), wstring( L"-137" ), &ok );
    TESTCHECK( RealToWString( -137.036, 2, 0, '.', ',' ), wstring( L"-137" ),
               &ok );
    TESTCHECK( RealToWString( -137.036, 8 ), wstring( L"-    137" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 0, '.', ',' ),
                wstring( L"-    137" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 2 ), wstring( L"- 137.04" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 2, '.', ',' ),
                wstring( L"- 137.04" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 2, '.', 0, true ),
               wstring( L"-0137.04" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 2, '.', ',', true ),
               wstring( L"-0137.04" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 2, '.', 0, true, true ),
               wstring( L"-0137.04" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 2, '.', ',', true, true ),
               wstring( L"-0137.04" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 2, '.', 0, false, true ),
               wstring( L"- 137.04" ), &ok );
    TESTCHECK( RealToWString( -137.036, 8, 2, '.', ',', false, true ),
               wstring( L"- 137.04" ), &ok );
    TESTCHECK( RealToWString( 1729.036 ), wstring( L"1729" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 2 ), wstring( L"1729" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 9 ), wstring( L"     1729" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 9, 2 ), wstring( L"  1729.04" ),
                &ok );
    TESTCHECK( RealToWString( 1729.036, 9, 2, '.', ',' ),
                wstring( L" 1,729.04" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 9, 2, '.', 0, true ),
               wstring( L"001729.04" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 9, 2, '.', ',', true ),
               wstring( L"01,729.04" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 9, 2, '.', 0, true, true ),
               wstring( L"+01729.04" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 9, 2, '.', ',', true, true ),
               wstring( L"+1,729.04" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 9, 2, '.', 0, false, true ),
               wstring( L"+ 1729.04" ), &ok );
    TESTCHECK( RealToWString( 1729.036, 9, 2, '.', ',', false, true ),
               wstring( L"+1,729.04" ), &ok );
    TESTCHECK( RealToWString( -1729.036 ), wstring( L"-1729" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 2 ), wstring( L"-1729" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 2, 0, '.', ',' ),
                wstring( L"-1,729" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9 ), wstring( L"-    1729" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 0, '.', ',' ),
               wstring( L"-   1,729" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 2 ), wstring( L"- 1729.04" ),
                &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 2, '.', ',' ),
               wstring( L"-1,729.04" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 2, '.', 0, true ),
               wstring( L"-01729.04" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 2, '.', ',', true ),
               wstring( L"-1,729.04" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 2, '.', 0, true, true ),
               wstring( L"-01729.04" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 2, '.', ',', true, true ),
               wstring( L"-1,729.04" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 2, '.', 0, false, true ),
               wstring( L"- 1729.04" ), &ok );
    TESTCHECK( RealToWString( -1729.036, 9, 2, '.', ',', false, true ),
               wstring( L"-1,729.04" ), &ok );
    TESTCHECK( RealToWString( 0., 0, 0, '.', 0, false, false ),
                wstring( L"0" ), &ok );
    TESTCHECK( RealToWString( 0., 0, 0, '.', 0, false, true ),
                wstring( L"+0" ), &ok );
    TESTCHECK( RealToWString( -0., 0, 0, '.', 0, false, true ),
                wstring( L"+0" ), &ok );
    TESTCHECK( RealToWString( 0., 0, 2, '.', 0, false, false ),
               wstring( L"0.00" ), &ok );
    TESTCHECK( RealToWString( 0., 0, 2, '.', 0, false, true ),
               wstring( L"+0.00" ), &ok );
    TESTCHECK( RealToWString( 0., 0, 0, '.', ',', false, false ),
                wstring( L"0" ), &ok );
    TESTCHECK( RealToWString( 0., 0, 0, '.', ',', false, true ),
                wstring( L"+0" ), &ok );
    TESTCHECK( RealToWString( -0., 0, 0, '.', ',', false, true ),
               wstring( L"+0" ), &ok );
    TESTCHECK( RealToWString( 0., 0, 2, '.', ',', false, false ),
               wstring( L"0.00" ), &ok );
    TESTCHECK( RealToWString( 0., 0, 2, '.', ',', false, true ),
               wstring( L"+0.00" ), &ok );

    TESTCHECK( OrdinalToString( 100 ), string( "100th" ), &ok );
    TESTCHECK( OrdinalToString( 101 ), string( "101st" ), &ok );
    TESTCHECK( OrdinalToString( 102, 7 ), string( "  102nd" ), &ok );
    TESTCHECK( OrdinalToString( 103 ), string( "103rd" ), &ok );
    TESTCHECK( OrdinalToString( 104 ), string( "104th" ), &ok );
    TESTCHECK( OrdinalToString( 111 ), string( "111th" ), &ok );
    TESTCHECK( OrdinalToString( 112 ), string( "112th" ), &ok );
    TESTCHECK( OrdinalToString( 113 ), string( "113th" ), &ok );
    TESTCHECK( OrdinalToString( 114 ), string( "114th" ), &ok );
    TESTCHECK( OrdinalToString( 191 ), string( "191st" ), &ok );
    TESTCHECK( OrdinalToString( 192 ), string( "192nd" ), &ok );
    TESTCHECK( OrdinalToString( 193 ), string( "193rd" ), &ok );
    TESTCHECK( OrdinalToString( 194 ), string( "194th" ), &ok );
    TESTCHECK( OrdinalToString( 100, 0, ',' ), string( "100th" ), &ok );
    TESTCHECK( OrdinalToString( 101, 0, ',' ), string( "101st" ), &ok );
    TESTCHECK( OrdinalToString( 102, 7, ',' ), string( "  102nd" ), &ok );
    TESTCHECK( OrdinalToString( 103, 0, ',' ), string( "103rd" ), &ok );
    TESTCHECK( OrdinalToString( 104, 0, ',' ), string( "104th" ), &ok );
    TESTCHECK( OrdinalToString( 111, 0, ',' ), string( "111th" ), &ok );
    TESTCHECK( OrdinalToString( 112, 0, ',' ), string( "112th" ), &ok );
    TESTCHECK( OrdinalToString( 113, 0, ',' ), string( "113th" ), &ok );
    TESTCHECK( OrdinalToString( 114, 0, ',' ), string( "114th" ), &ok );
    TESTCHECK( OrdinalToString( 191, 0, ',' ), string( "191st" ), &ok );
    TESTCHECK( OrdinalToString( 192, 0, ',' ), string( "192nd" ), &ok );
    TESTCHECK( OrdinalToString( 193, 0, ',' ), string( "193rd" ), &ok );
    TESTCHECK( OrdinalToString( 194, 0, ',' ), string( "194th" ), &ok );
    TESTCHECK( OrdinalToString( 2000100 ), string( "2000100th" ), &ok );
    TESTCHECK( OrdinalToString( 2000101 ), string( "2000101st" ), &ok );
    TESTCHECK( OrdinalToString( 2000102, 13 ), string( "    2000102nd" ), &ok );
    TESTCHECK( OrdinalToString( 2000103 ), string( "2000103rd" ), &ok );
    TESTCHECK( OrdinalToString( 2000104 ), string( "2000104th" ), &ok );
    TESTCHECK( OrdinalToString( 2000111 ), string( "2000111th" ), &ok );
    TESTCHECK( OrdinalToString( 2000112 ), string( "2000112th" ), &ok );
    TESTCHECK( OrdinalToString( 2000113 ), string( "2000113th" ), &ok );
    TESTCHECK( OrdinalToString( 2000114 ), string( "2000114th" ), &ok );
    TESTCHECK( OrdinalToString( 2000191 ), string( "2000191st" ), &ok );
    TESTCHECK( OrdinalToString( 2000192 ), string( "2000192nd" ), &ok );
    TESTCHECK( OrdinalToString( 2000193 ), string( "2000193rd" ), &ok );
    TESTCHECK( OrdinalToString( 2000194 ), string( "2000194th" ), &ok );
    TESTCHECK( OrdinalToString( 2000100, 0, ',' ), string( "2,000,100th" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000101, 0, ',' ), string( "2,000,101st" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000102, 13, ',' ), string( "  2,000,102nd" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000103, 0, ',' ), string( "2,000,103rd" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000104, 0, ',' ), string( "2,000,104th" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000111, 0, ',' ), string( "2,000,111th" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000112, 0, ',' ), string( "2,000,112th" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000113, 0, ',' ), string( "2,000,113th" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000114, 0, ',' ), string( "2,000,114th" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000191, 0, ',' ), string( "2,000,191st" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000192, 0, ',' ), string( "2,000,192nd" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000193, 0, ',' ), string( "2,000,193rd" ),
               &ok );
    TESTCHECK( OrdinalToString( 2000194, 0, ',' ), string( "2,000,194th" ),
               &ok );

    TESTCHECK( OrdinalToWString( 100 ), wstring( L"100th" ), &ok );
    TESTCHECK( OrdinalToWString( 101 ), wstring( L"101st" ), &ok );
    TESTCHECK( OrdinalToWString( 102, 7 ), wstring( L"  102nd" ), &ok );
    TESTCHECK( OrdinalToWString( 103 ), wstring( L"103rd" ), &ok );
    TESTCHECK( OrdinalToWString( 104 ), wstring( L"104th" ), &ok );
    TESTCHECK( OrdinalToWString( 111 ), wstring( L"111th" ), &ok );
    TESTCHECK( OrdinalToWString( 112 ), wstring( L"112th" ), &ok );
    TESTCHECK( OrdinalToWString( 113 ), wstring( L"113th" ), &ok );
    TESTCHECK( OrdinalToWString( 114 ), wstring( L"114th" ), &ok );
    TESTCHECK( OrdinalToWString( 191 ), wstring( L"191st" ), &ok );
    TESTCHECK( OrdinalToWString( 192 ), wstring( L"192nd" ), &ok );
    TESTCHECK( OrdinalToWString( 193 ), wstring( L"193rd" ), &ok );
    TESTCHECK( OrdinalToWString( 194 ), wstring( L"194th" ), &ok );
    TESTCHECK( OrdinalToWString( 100, 0, ',' ), wstring( L"100th" ), &ok );
    TESTCHECK( OrdinalToWString( 101, 0, ',' ), wstring( L"101st" ), &ok );
    TESTCHECK( OrdinalToWString( 102, 7, ',' ), wstring( L"  102nd" ), &ok );
    TESTCHECK( OrdinalToWString( 103, 0, ',' ), wstring( L"103rd" ), &ok );
    TESTCHECK( OrdinalToWString( 104, 0, ',' ), wstring( L"104th" ), &ok );
    TESTCHECK( OrdinalToWString( 111, 0, ',' ), wstring( L"111th" ), &ok );
    TESTCHECK( OrdinalToWString( 112, 0, ',' ), wstring( L"112th" ), &ok );
    TESTCHECK( OrdinalToWString( 113, 0, ',' ), wstring( L"113th" ), &ok );
    TESTCHECK( OrdinalToWString( 114, 0, ',' ), wstring( L"114th" ), &ok );
    TESTCHECK( OrdinalToWString( 191, 0, ',' ), wstring( L"191st" ), &ok );
    TESTCHECK( OrdinalToWString( 192, 0, ',' ), wstring( L"192nd" ), &ok );
    TESTCHECK( OrdinalToWString( 193, 0, ',' ), wstring( L"193rd" ), &ok );
    TESTCHECK( OrdinalToWString( 194, 0, ',' ), wstring( L"194th" ), &ok );
    TESTCHECK( OrdinalToWString( 2000100 ), wstring( L"2000100th" ), &ok );
    TESTCHECK( OrdinalToWString( 2000101 ), wstring( L"2000101st" ), &ok );
    TESTCHECK( OrdinalToWString( 2000102, 13 ), wstring( L"    2000102nd" ),
                &ok );
    TESTCHECK( OrdinalToWString( 2000103 ), wstring( L"2000103rd" ), &ok );
    TESTCHECK( OrdinalToWString( 2000104 ), wstring( L"2000104th" ), &ok );
    TESTCHECK( OrdinalToWString( 2000111 ), wstring( L"2000111th" ), &ok );
    TESTCHECK( OrdinalToWString( 2000112 ), wstring( L"2000112th" ), &ok );
    TESTCHECK( OrdinalToWString( 2000113 ), wstring( L"2000113th" ), &ok );
    TESTCHECK( OrdinalToWString( 2000114 ), wstring( L"2000114th" ), &ok );
    TESTCHECK( OrdinalToWString( 2000191 ), wstring( L"2000191st" ), &ok );
    TESTCHECK( OrdinalToWString( 2000192 ), wstring( L"2000192nd" ), &ok );
    TESTCHECK( OrdinalToWString( 2000193 ), wstring( L"2000193rd" ), &ok );
    TESTCHECK( OrdinalToWString( 2000194 ), wstring( L"2000194th" ), &ok );
    TESTCHECK( OrdinalToWString( 2000100, 0, ',' ), wstring( L"2,000,100th" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000101, 0, ',' ), wstring( L"2,000,101st" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000102, 13, ',' ),
                wstring( L"  2,000,102nd" ), &ok );
    TESTCHECK( OrdinalToWString( 2000103, 0, ',' ), wstring( L"2,000,103rd" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000104, 0, ',' ), wstring( L"2,000,104th" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000111, 0, ',' ), wstring( L"2,000,111th" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000112, 0, ',' ), wstring( L"2,000,112th" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000113, 0, ',' ), wstring( L"2,000,113th" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000114, 0, ',' ), wstring( L"2,000,114th" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000191, 0, ',' ), wstring( L"2,000,191st" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000192, 0, ',' ), wstring( L"2,000,192nd" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000193, 0, ',' ), wstring( L"2,000,193rd" ),
               &ok );
    TESTCHECK( OrdinalToWString( 2000194, 0, ',' ), wstring( L"2,000,194th" ),
               &ok );

    return ok;
}

}                                                                   //namespace

#endif //DEBUG

//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  StringUtil.cpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for strings (C and C++).
*/


#include "StringUtil.hpp"
#include "StdInt.hpp"
#include "CodePointData.hpp"
#include "StdLib.hpp"
#include <cstring>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


string 
IntToString( long i, int width, char comma, bool zeroFill, bool showSign )
{
    return IntToBasicString< char, char_traits< char >, allocator< char > >
            ( i, width, comma, zeroFill, showSign );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

wstring 
IntToWString( long i, int width, char comma, bool zeroFill, bool showSign )
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
OrdinalToString( long i, int width, char comma )
{
    string intStr = IntToString( i, width - 2, comma );
    int r = (int)(abs( i ) % 100L);
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
OrdinalToWString( long i, int width, char comma )
{
    wstring intStr = IntToWString( i, width - 2, comma );
    int r = (int)(abs( i ) % 100L);
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

namespace
{                                                                   //namespace

//.............................................................................

enum LineBreakPairAction
{
    LBPA_Prohibited             = 0,
    LBPA_Required               = 1,
    LBPA_Allowed                = 2,
    LBPA_AllowedWithPenalty     = 3,
    LBPA_AllowedWithSpaces      = 4,
    LBPA_ProhibitedWithSpaces   = 5,
    LBPA_SkipCombining          = 6
};
uint8_t s_LBActionTable[ 32 ][ 32 ]
= {
     /* BK CR LF CM WJ ZW GL SP B2 BA BB HY CB CL CP EX IN NS OP QU IS NU PO PR SY AL H2 H3 ID JL JV JT*/
/*BK*/ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*CR*/ { 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*LF*/ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*CM*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 4, 4, 0, 4, 3, 3, 0, 4, 3, 3, 3, 3, 3, 3 },
/*WJ*/ { 0, 0, 0, 6, 0, 0, 4, 0, 4, 4, 4, 4, 4, 0, 0, 0, 4, 4, 4, 4, 0, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4 },
/*ZW*/ { 0, 0, 0, 2, 5, 0, 2, 0, 2, 2, 2, 2, 2, 5, 5, 5, 2, 2, 2, 2, 5, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2 },
/*GL*/ { 0, 0, 0, 6, 0, 0, 4, 0, 4, 4, 4, 4, 4, 0, 0, 0, 4, 4, 4, 4, 0, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4 },
/*SP*/ { 0, 0, 0, 2, 0, 0, 2, 0, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2 },
/*B2*/ { 0, 0, 0, 6, 0, 0, 4, 0, 0, 4, 3, 4, 2, 0, 0, 0, 3, 4, 3, 4, 0, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3 },
/*BA*/ { 0, 0, 0, 6, 0, 0, 3, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 4, 3, 4, 0, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3 },
/*BB*/ { 0, 0, 0, 6, 0, 0, 4, 0, 4, 4, 4, 4, 2, 0, 0, 0, 4, 4, 4, 4, 0, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4 },
/*HY*/ { 0, 0, 0, 6, 0, 0, 3, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 4, 3, 4, 0, 4, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3 },
/*CB*/ { 0, 0, 0, 6, 0, 0, 4, 0, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 4, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2 },
/*CL*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 0, 3, 4, 0, 3, 4, 4, 0, 3, 3, 3, 3, 3, 3, 3 },
/*CP*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 0, 3, 4, 0, 4, 4, 4, 0, 4, 3, 3, 3, 3, 3, 3 },
/*EX*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 4, 3, 4, 0, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3 },
/*IN*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 3, 4, 0, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3 },
/*NS*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 4, 3, 4, 0, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3 },
/*OP*/ { 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
/*QU*/ { 0, 0, 0, 6, 0, 0, 4, 0, 4, 4, 4, 4, 4, 0, 0, 0, 4, 4, 0, 4, 0, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4 },
/*IS*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 4, 3, 4, 0, 4, 3, 3, 0, 4, 3, 3, 3, 3, 3, 3 },
/*NU*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 4, 4, 0, 4, 4, 4, 0, 4, 3, 3, 3, 3, 3, 3 },
/*PO*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 4, 4, 4, 0, 4, 3, 3, 0, 4, 3, 3, 3, 3, 3, 3 },
/*PR*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 4, 4, 4, 0, 4, 3, 3, 0, 4, 4, 4, 4, 4, 4, 4 },
/*SY*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 3, 4, 3, 4, 0, 4, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3 },
/*AL*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 4, 4, 0, 4, 3, 3, 0, 4, 3, 3, 3, 3, 3, 3 },
/*H2*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 3, 4, 0, 3, 4, 3, 0, 3, 3, 3, 3, 3, 4, 4 },
/*H3*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 3, 4, 0, 3, 4, 3, 0, 3, 3, 3, 3, 3, 3, 4 },
/*ID*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 3, 4, 0, 3, 4, 3, 0, 3, 3, 3, 3, 3, 3, 3 },
/*JL*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 3, 4, 0, 3, 4, 3, 0, 3, 4, 4, 3, 4, 4, 3 },
/*JV*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 3, 4, 0, 3, 4, 3, 0, 3, 3, 3, 3, 3, 4, 4 },
/*JT*/ { 0, 0, 0, 6, 0, 0, 4, 0, 3, 4, 3, 4, 2, 0, 0, 0, 4, 4, 3, 4, 0, 3, 4, 3, 0, 3, 3, 3, 3, 3, 3, 4 }
};

#ifdef DEBUG

string lineBreakClassAbbrs[ NumLineBreakClasses ]
= {
    "BK",   //LBC_MandatoryBreak,
    "CR",   //LBC_CarriageReturn,
    "LF",   //LBC_LineFeed,
    "CM",   //LBC_CombiningMark,
    "WJ",   //LBC_WordJoiner,
    "ZW",   //LBC_ZeroWidthSpace,
    "GL",   //LBC_Glue,
    "SP",   //LBC_Space,
    "B2",   //LBC_BreakOpportunityBeforeAndAfter,
    "BA",   //LBC_BreakAfter,
    "BB",   //LBC_BreakBefore,
    "HY",   //LBC_Hyphen,
    "CB",   //LBC_ContingentBreakOpportunity,
    "CL",   //LBC_ClosePunctuation,
    "CP",   //LBC_CloseParenthesis,
    "EX",   //LBC_ExclamationInterrogation,
    "IN",   //LBC_Inseparable,
    "NS",   //LBC_Nonstarter,
    "OP",   //LBC_OpenPunctuation,
    "QU",   //LBC_Quotation,
    "IS",   //LBC_InfixNumericSeperator,
    "NU",   //LBC_Numeric,
    "PO",   //LBC_PosfixNumeric,
    "PR",   //LBC_PrefixNumeric,
    "SY",   //LBC_SymbolsAllowingBreakAfter,
    "AL",   //LBC_Alphabetic,
    "H2",   //LBC_HangulLVSyllable,
    "H3",   //LBC_HangulLVTSyllable,
    "ID",   //LBC_Ideographic,
    "JL",   //LBC_HangulLJamo,
    "JV",   //LBC_HangulVJamo,
    "JT",   //LBC_HangulTJamo,
    "NL",   //LBC_NextLine,
    "SG",   //LBC_Surrogate,
    "AI",   //LBC_Ambiguous,
    "SA",   //LBC_ComplexContextDependent,
    "XX"    //LBC_UnknownLineBreak
};

#endif

//.............................................................................

}                                                                   //namespace

//-----------------------------------------------------------------------------

void 
DetermineLineBreakOpportunities( const wstring & text,
                                 vector< LineBreakAction > * pActions )
{
    if ( ! pActions )
        return;
    pActions->clear( );
    if ( text.empty() )
        return;
    pActions->resize( text.size() );
    wchar_t ch = text[ 0 ];
    Assert( ch <= MaximumCodePoint );
    LineBreakClass lbc0 = (LineBreakClass)codePointLineBreakClasses[ ch ];
#if 0   //This reflects the sample code in Section 7.4, but not the test cases in LineBreakTest.txt.
    if ( lbc0 == LBC_Space )
        lbc0 = LBC_WordJoiner;
#endif

    for ( int i = 0; i < text.size() - 1; ++i )
    {
        ch = text[ i + 1 ];
        Assert( ch <= MaximumCodePoint );
        LineBreakClass lbc1 = (LineBreakClass)codePointLineBreakClasses[ ch ];
        LineBreakPairAction action
                = (LineBreakPairAction)s_LBActionTable[ lbc0 ][ lbc1 ];
        if ( (lbc1 == LBC_Space) && (action != LBPA_Required) )
        {
            pActions->at( i ) = LBA_Prohibited; //No break before space
            continue;   //Maintain previous lbc0
        }
        if ( action == LBPA_AllowedWithSpaces )
        {
            LineBreakClass lbcPrev = (LineBreakClass)codePointLineBreakClasses[ text[ i ] ];
            if ( lbcPrev == LBC_Space )
                pActions->at( i ) = LBA_Allowed;
            else
                pActions->at( i ) = LBA_Prohibited;
        }
        else if ( action == LBPA_ProhibitedWithSpaces )
        {
            LineBreakClass lbcPrev = (LineBreakClass)codePointLineBreakClasses[ text[ i ] ];
            if ( lbcPrev == LBC_Space )
                pActions->at( i ) = LBA_Prohibited;
            else
                pActions->at( i ) = LBA_Allowed;
        }
        else if ( action == LBPA_SkipCombining )
        {
            LineBreakClass lbcPrev = (LineBreakClass)codePointLineBreakClasses[ text[ i ] ];
            if ( (lbcPrev == LBC_Space) && (lbc0 != LBC_OpenPunctuation) )
                pActions->at( i ) = LBA_Allowed;
            else
                pActions->at( i ) = LBA_Prohibited;
            continue;
        }
        else
        {
            pActions->at( i ) = (LineBreakAction)action;
        }
        lbc0 = lbc1;
    }
    //Set up to break at end only after BK, CR or LF.
    LineBreakClass lbc1 = LBC_ZeroWidthSpace;
    pActions->at( text.size() - 1 )
            = (LineBreakAction)s_LBActionTable[ lbc0 ][ lbc1 ];
}

//=============================================================================

#ifdef DEBUG

namespace
{
bool TestNumbersToString( );
}

//-----------------------------------------------------------------------------

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
#if WCOUT_AVAILABLE
    wcout << L"testPWChar=\"" << testPWChar << L"\"" << endl;
#endif    
    TESTCHECK( ToUpper( testPWChar ),
                  L"ABC\xC7\x11E ABC\xC7\x11E 123 !@#", &ok );
    TESTCHECK( ToLower( testPWChar ),
                  L"abc\xE7\x11F abc\xE7\x11F 123 !@#", &ok );
    wstring testWString = L"xyz\xFC\x15F XYZ\xDC\x15E 789 &*(";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
#endif
    TESTCHECK( *ToUpper( &testWString ),
                wstring( L"XYZ\xDC\x15E XYZ\xDC\x15E 789 &*(" ), &ok );
    TESTCHECK( *ToLower( &testWString ),
                wstring( L"xyz\xFC\x15F xyz\xFC\x15F 789 &*(" ), &ok );
    const wstring cWString1 = L"xyz\xFC\x15F XYZ\xDC\x15E 789 &*(";
#if WCOUT_AVAILABLE
    wcout << L"cWString1=\"" << cWString1 << L"\"" << endl;
#endif
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
#if WCOUT_AVAILABLE
    wcout << L"testPChar=\"" << testPWChar << L"\"" << endl;
#endif
    TESTCHECK( TrimLeading( testPWChar ), L"abcd \t  ", &ok );
    TESTCHECK( TrimTrailing( testPWChar ), L"abcd", &ok );
    wcscpy( testPWChar, L"  \tabcd \t  " );
#if WCOUT_AVAILABLE
    wcout << L"testPChar=\"" << testPWChar << L"\"" << endl;
#endif
    TESTCHECK( Trim( testPWChar ), L"abcd", &ok );
    wcscpy( testPWChar, L"" );
#if WCOUT_AVAILABLE
    wcout << L"testPChar=\"" << testPWChar << L"\"" << endl;
#endif
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
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
#endif
    const wstring cWString2 = testWString;
#if WCOUT_AVAILABLE
    wcout << L"cWString2=\"" << cWString2 << L"\"" << endl;
#endif
    TESTCHECK( *TrimLeading( &testWString ), wstring( L"wxyz \t  " ), &ok );
    TESTCHECK( TrimLeading( cWString2 ), wstring( L"wxyz \t  " ), &ok );
    TESTCHECK( *TrimTrailing( &testWString ), wstring( L"wxyz" ), &ok );
    TESTCHECK( TrimTrailing( cWString2 ), wstring( L"  \twxyz" ), &ok );
    testWString = L"  \twxyz \t  ";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
#endif
    TESTCHECK( *Trim( &testWString ), wstring( L"wxyz" ), &ok );
    TESTCHECK( Trim( cWString2 ), wstring( L"wxyz" ), &ok );
    testWString = L"";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
#endif
    const wstring cWString3 = testWString;
#if WCOUT_AVAILABLE
    wcout << L"cWString3=\"" << cWString3 << L"\"" << endl;
#endif
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
#if WCOUT_AVAILABLE
    wcout << L"testString=\"" << testWString << L"\"" << endl;
#endif
    const wstring cWString4 = testWString;
#if WCOUT_AVAILABLE
    wcout << L"cWString4=\"" << cWString4 << L"\"" << endl;
#endif
    TESTCHECK( *AddQuotes( &testWString, L'\'' ),
               wstring( L"'He said, ''It/''s '''', not \".'''" ), &ok );
    TESTCHECK( AddQuotes( cWString4, L'\'' ),
               wstring( L"'He said, ''It/''s '''', not \".'''" ), &ok );
    const wstring cWString5 = testWString;
#if WCOUT_AVAILABLE
    wcout << L"cWString5=\"" << cWString5 << L"\"" << endl;
#endif
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
#if WCOUT_AVAILABLE
    wcout << L"testString=\"" << testWString << L"\"" << endl;
#endif
    const wstring cWString6 = testWString;
#if WCOUT_AVAILABLE
    wcout << L"cWString6=\"" << cWString6 << L"\"" << endl;
#endif
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
#if WCOUT_AVAILABLE
    wcout << L"testString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':' )" << endl;
#endif
    vector< wstring > partsW = Split( testWString, L':' );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" def" ), &ok );
    TESTCHECK( partsW[3], wstring( L"" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',' )" << endl;
#endif
    partsW = Split( testWString, L',' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    testWString = L"";
#if WCOUT_AVAILABLE
    wcout << L"testString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':' )" << endl;
#endif
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
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'' )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'' );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc/ : def/" ), &ok );
    TESTCHECK( partsW[2], wstring( L"ghi/" ), &ok );
    TESTCHECK( partsW[3], wstring( L"'" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L':', L'\"' )" << endl;
#endif
    partsW = Split( testWString, L':', L'\"' );
    TESTCHECK( partsW.size(), 5, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/" ), &ok );
    TESTCHECK( partsW[3], wstring( L"ghi/''" ), &ok );
    TESTCHECK( partsW[4], wstring( L"''''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\'' )" << endl;
#endif
    partsW = Split( testWString, L',', L'\'' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc/ : def/:ghi/:'" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\"' )" << endl;
#endif
    partsW = Split( testWString, L',', L'\"' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    testWString = L"";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'' )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''''";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc/' : 'def/" ), &ok );
    TESTCHECK( partsW[2], wstring( L"ghi/''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L':', L'\"', true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\"', true );
    TESTCHECK( partsW.size(), 5, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/" ), &ok );
    TESTCHECK( partsW[3], wstring( L"ghi/''" ), &ok );
    TESTCHECK( partsW[4], wstring( L"''''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\'', true )" << endl;
#endif
    partsW = Split( testWString, L',', L'\'', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\"', true )" << endl;
#endif
    partsW = Split( testWString, L',', L'\"', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    testWString = L"";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', true )" << endl;
#endif
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
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/' )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', L'/' );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc' " ), &ok );
    TESTCHECK( partsW[2], wstring( L" def:ghi'" ), &ok );
    TESTCHECK( partsW[3], wstring( L"" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L':', L'\"', L'/' )" << endl;
#endif
    partsW = Split( testWString, L':', L'\"', L'/' );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def:ghi''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\'', L'/' )" << endl;
#endif
    partsW = Split( testWString, L',', L'\'', L'/' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc' : def:ghi':" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\"', L'/' )" << endl;
#endif
    partsW = Split( testWString, L',', L'\"', L'/' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc' : 'def:ghi'':''" ), &ok );
    testWString = L"";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/' )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', L'/' );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', L'/', true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc' " ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def:ghi''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L':', L'\"', L'/', true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\"', L'/', true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def:ghi''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\'', L'/', true )" << endl;
#endif
    partsW = Split( testWString, L',', L'\'', L'/', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc' : 'def:ghi'':''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\"', L'/', true )" << endl;
#endif
    partsW = Split( testWString, L',', L'\"', L'/', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc' : 'def:ghi'':''" ), &ok );
    testWString = L"";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', L'/', true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', false, true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', L'/', false, true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc/' " ), &ok );
    TESTCHECK( partsW[2], wstring( L" def/:ghi/'" ), &ok );
    TESTCHECK( partsW[3], wstring( L"" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L':', L'\"', L'/', false, true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\"', L'/', false, true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/:ghi/''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\'', L'/', false, true )" << endl;
#endif
    partsW = Split( testWString, L',', L'\'', L'/', false, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc/' : def/:ghi/':" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\"', L'/', false, true )" << endl;
#endif
    partsW = Split( testWString, L',', L'\"', L'/', false, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L":abc/' : 'def/:ghi/'':''" ), &ok );
    testWString = L"";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', false, true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', L'/', false, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L":abc/' : 'def/:ghi/'':''";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', true, true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', L'/', true, true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[1], wstring( L"abc/' " ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/:ghi/''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L':', L'\"', L'/', true, true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\"', L'/', true, true );
    TESTCHECK( partsW.size(), 4, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );
    TESTCHECK( partsW[2], wstring( L" 'def/:ghi/''" ), &ok );
    TESTCHECK( partsW[3], wstring( L"''" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\'', L'/', true, true )" << endl;
#endif
    partsW = Split( testWString, L',', L'\'', L'/', true, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testWString, L',', L'\"', L'/', true, true )" << endl;
#endif
    partsW = Split( testWString, L',', L'\"', L'/', true, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], testWString, &ok );
    testWString = L"";
#if WCOUT_AVAILABLE
    wcout << L"testWString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, L':', L'\'', L'/', true, true )" << endl;
#endif
    partsW = Split( testWString, L':', L'\'', L'/', true, true );
    TESTCHECK( partsW.size(), 1, &ok );
    TESTCHECK( partsW[0], wstring( L"" ), &ok );

    testWString = L"abc123def123abc";
#if WCOUT_AVAILABLE
    wcout << L"testString=\"" << testWString << L"\"" << endl;
    wcout << L"Split( testWString, wstring(L\"123\") )" << endl;
#endif
    partsW = Split( testWString, wstring( L"123" ) );
    TESTCHECK( partsW.size(), 3, &ok );
    TESTCHECK( partsW[0], wstring( L"abc" ), &ok );
    TESTCHECK( partsW[1], wstring( L"def" ), &ok );
    TESTCHECK( partsW[2], wstring( L"abc" ), &ok );
#if WCOUT_AVAILABLE
    wcout << L"Split( testString, wstring(L\"abc\") )" << endl;
#endif
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

//-----------------------------------------------------------------------------

bool 
TestLineBreakOpportunities( const std::string & testFileText )
{
    bool ok = true;
    cout << "Testing LineBreakOpportunities" << endl;

    vector< string > lines = Split( testFileText, '\n' );

    int numLines = (int)lines.size();
    int linesTested = 0;
    int linesFailed = 0;

    for ( int i = 0; i < numLines; ++i )
    {
        string & line = lines[ i ];
        size_t poundPos = line.find( '#' );
        if ( poundPos != string::npos )
            line.erase( poundPos );
        Trim( &line );
        if ( line.empty() )
            continue;
        vector< string > fields = Split( line, ' ' );
        Assert( fields.size() > 1 );
        if ( fields.size() <= 1 )
            continue;

#if 0
        cout << "Testing: " << line << "" << endl;
#endif

        wstring testString;
        vector< bool > expectedBreaks;
        wchar_t maxCP = 0;
        for ( int j = 1; j < (int)fields.size(); ++j ) //skip initial '÷'
        {
            if ( fields[ j ].empty() )
                break;
            if ( (j % 2) == 0 )
            {
                if ( fields[ j ] == "÷" )
                    expectedBreaks.push_back( true );
                else if ( fields[ j ] == "×" )
                    expectedBreaks.push_back( false );
                else
                    Assert( 0 && "Expected ÷ or ×" );
            }
            else
            {
                wchar_t cp = strtol( fields[ j ].c_str(), 0, 16 );
                testString += cp;
                if ( cp > maxCP )
                    maxCP = cp;
            }
        }
        Assert( testString.size() == expectedBreaks.size() );
        if ( maxCP > MaximumCodePoint )
            continue;

        vector< LineBreakAction > actions;
        DetermineLineBreakOpportunities( testString, &actions );
        vector< bool > computedBreaks;
        for ( int j = 0; j < (int)actions.size(); ++j )
        {
            bool breakAllowed = (actions[ j ] != LBA_Prohibited);
            computedBreaks.push_back( breakAllowed );
        }
        bool computedIsExpected = true;
        //skip final value, because expected value is always true, not ours
        for ( int j = 0; j < (int)computedBreaks.size() - 1; ++j )
        {
            if ( computedBreaks[ j ] != expectedBreaks[ j ] )
            {
                computedIsExpected = false;
#if 1
                cout << "Non-match: " << j << " computed=" << computedBreaks[ j ]
                     << " expected=" << expectedBreaks[ j ] << endl;
#endif
            }
        }
        if ( ! computedIsExpected )
        {
            ok = false;
            cout << line << " FAILED" << endl;
            ++linesFailed;
        }
        ++linesTested;
    }

    cout << linesTested << " lines tested. " << linesFailed << " failed" << endl;

    if ( ok )
        cout << "LineBreakOpportunities PASSED." << endl << endl;
    else
        cout << "LineBreakOpportunities FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG

//*****************************************************************************

}                                                      //namespace EpsilonDelta

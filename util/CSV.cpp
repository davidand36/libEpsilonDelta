/*
  CSV.cpp
  Copyright (C) 2007 David M. Anderson

  Routines for parsing and generating CSV (comma-separated values) format
  strings.
*/


#include "CSV.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestCSV( )
{
    bool ok = true;
    cout << "Testing CSV" << endl;

    string csvText1 = "1997, Ford,E350,\" ac, abs, moon \" , 3000.00\n"
            "1999,Chevy,\"Venture \"\"Extended Edition\"\"\",,\t4900.00\n"
            "1996,Jeep,Grand Cherokee , \"MUST SELL!\n"
            "air, moon roof, loaded\", 4799.00 ";
    cout << "csvText=\"" << csvText1 << "\"" << endl;

    cout << "SplitCSV( csvText1 )" << endl;
    vector< vector< string > > table1 = SplitCSV( csvText1 );
    TESTCHECK( table1.size(), 3, &ok );
    TESTCHECK( table1[0].size(), 5, &ok );
    TESTCHECK( table1[0][3], string( " ac, abs, moon " ), &ok );
    TESTCHECK( table1[1].size(), 5, &ok );
    TESTCHECK( table1[1][2], string( "Venture \"Extended Edition\"" ), &ok );
    TESTCHECK( table1[1][3], string( "" ), &ok );
    TESTCHECK( table1[2].size(), 5, &ok );
    TESTCHECK( table1[2][0], string( "1996" ), &ok );
    TESTCHECK( table1[2][3], string( "MUST SELL!\nair, moon roof, loaded" ),
               &ok );
    TESTCHECK( table1[2][4], string( "4799.00" ), &ok );

    cout << "SplitCSV( csvText1, true )" << endl;
    vector< vector< string > > table2 = SplitCSV( csvText1, true );
    TESTCHECK( table2.size(), 3, &ok );
    TESTCHECK( table2[0].size(), 5, &ok );
    TESTCHECK( table2[0][3], string( "ac, abs, moon" ), &ok );
    TESTCHECK( table2[1].size(), 5, &ok );
    TESTCHECK( table2[1][2], string( "Venture \"Extended Edition\"" ), &ok );
    TESTCHECK( table2[1][3], string( "" ), &ok );
    TESTCHECK( table2[2].size(), 5, &ok );
    TESTCHECK( table2[2][0], string( "1996" ), &ok );
    TESTCHECK( table2[2][3], string( "MUST SELL!\nair, moon roof, loaded" ),
               &ok );
    TESTCHECK( table2[2][4], string( "4799.00" ), &ok );

    wstring csvWText1 = L"1997, Ford,E350,\" ac, abs, moon \" , 3000.00\n"
            L"1999,Chevy,\"Venture \"\"Extended Edition\"\"\",,\t4900.00\n"
            L"1996,Jeep,Grand Cherokee , \"MUST SELL!\n"
            L"air, moon roof, loaded\", 4799.00 ";
    wcout << L"csvText=\"" << csvWText1 << L"\"" << endl;

    wcout << L"SplitCSV( csvWText1 )" << endl;
    vector< vector< wstring > > tableW1 = SplitCSV( csvWText1 );
    TESTCHECK( tableW1.size(), 3, &ok );
    TESTCHECK( tableW1[0].size(), 5, &ok );
    TESTCHECK( tableW1[0][3], wstring( L" ac, abs, moon " ), &ok );
    TESTCHECK( tableW1[1].size(), 5, &ok );
    TESTCHECK( tableW1[1][2], wstring( L"Venture \"Extended Edition\"" ),
               &ok );
    TESTCHECK( tableW1[1][3], wstring( L"" ), &ok );
    TESTCHECK( tableW1[2].size(), 5, &ok );
    TESTCHECK( tableW1[2][0], wstring( L"1996" ), &ok );
    TESTCHECK( tableW1[2][3],
               wstring( L"MUST SELL!\nair, moon roof, loaded" ), &ok );
    TESTCHECK( tableW1[2][4], wstring( L"4799.00" ), &ok );

    wcout << L"SplitCSV( csvWText1, true )" << endl;
    vector< vector< wstring > > tableW2 = SplitCSV( csvWText1, true );
    TESTCHECK( tableW2.size(), 3, &ok );
    TESTCHECK( tableW2[0].size(), 5, &ok );
    TESTCHECK( tableW2[0][3], wstring( L"ac, abs, moon" ), &ok );
    TESTCHECK( tableW2[1].size(), 5, &ok );
    TESTCHECK( tableW2[1][2], wstring( L"Venture \"Extended Edition\"" ),
               &ok );
    TESTCHECK( tableW2[1][3], wstring( L"" ), &ok );
    TESTCHECK( tableW2[2].size(), 5, &ok );
    TESTCHECK( tableW2[2][0], wstring( L"1996" ), &ok );
    TESTCHECK( tableW2[2][3],
               wstring( L"MUST SELL!\nair, moon roof, loaded" ), &ok );
    TESTCHECK( tableW2[2][4], wstring( L"4799.00" ), &ok );

    cout << "CreateCSV( table1 )" << endl;
    string csvText2 = CreateCSV( table1 );
    string csvText3 = "\"1997\",\"Ford\",\"E350\",\" ac, abs, moon \",\"3000.00\"\n"
            "\"1999\",\"Chevy\",\"Venture \"\"Extended Edition\"\"\",\"\",\"4900.00\"\n"
            "\"1996\",\"Jeep\",\"Grand Cherokee\",\"MUST SELL!\n"
            "air, moon roof, loaded\",\"4799.00\"";
    TESTCHECK( csvText2, csvText3, &ok );

    cout << "SplitCSV( csvText2 )" << endl;
    vector< vector< string > > table3 = SplitCSV( csvText2 );
    TESTCHECK( table3.size(), 3, &ok );
    TESTCHECK( table3[0].size(), 5, &ok );
    TESTCHECK( table3[1].size(), 5, &ok );
    TESTCHECK( table3[2].size(), 5, &ok );
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 5; ++j )
            TESTCHECK( table3[i][j], table1[i][j], &ok );

    wcout << L"CreateCSV( tableW1 )" << endl;
    wstring csvWText2 = CreateCSV( tableW1 );
    wstring csvWText3 = L"\"1997\",\"Ford\",\"E350\",\" ac, abs, moon \",\"3000.00\"\n"
            L"\"1999\",\"Chevy\",\"Venture \"\"Extended Edition\"\"\",\"\",\"4900.00\"\n"
            L"\"1996\",\"Jeep\",\"Grand Cherokee\",\"MUST SELL!\n"
            L"air, moon roof, loaded\",\"4799.00\"";
    TESTCHECK( csvWText2, csvWText3, &ok );

    wcout << L"SplitCSV( csvWText2 )" << endl;
    vector< vector< wstring > > tableW3 = SplitCSV( csvWText2 );
    TESTCHECK( tableW3.size(), 3, &ok );
    TESTCHECK( tableW3[0].size(), 5, &ok );
    TESTCHECK( tableW3[1].size(), 5, &ok );
    TESTCHECK( tableW3[2].size(), 5, &ok );
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 5; ++j )
            TESTCHECK( tableW3[i][j], tableW1[i][j], &ok );

    if ( ok )
        cout << "CSV PASSED." << endl << endl;
    else
        cout << "CSV FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

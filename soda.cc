#include <uC++.h>
#include <uFile.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "MPRNG.h"
#include "q1tallyVotes.h"
#include "q1voter.h"
#include "q1printer.h"

using namespace std;

MPRNG mprng; // global instance of MPRNG

/**
 * @fn      Voter::main
 * @brief   Main loop of Voter
 */
void Voter::main()
{
    printer->print( id , Voter::Start );
    yield( mprng( 19 ) );
    const bool ballot = (mprng( 1 ) == 1);
    const bool res = tallier->vote( id , ballot );
    printer->print( id , Voter::Finished , res );
}

/**
 * @fn          printUsageMsg
 * @brief       Prints an appropriate usage message for the process
 */
void printUsageMsg()
{
    cout << "Usage: ./vote  Voters (> 0 & V mod G = 0, default 6)  Group (> 0 & odd, default 3)  Seed (> 0)" << endl;
}

void uMain::main()
{
    const int noArgs = 1;
    const int voterPos = 1 , groupPos = 2 , seedPos = 3;
    const int voterMin = 1 , groupMin = 1 , seedMin = 1;
    int voterValue = 6 , groupValue = 3 , seedValue = time( NULL );

    switch ( argc )
    {
        case seedPos + 1 :
        {
            seedValue = atoi( argv[seedPos] );

            if ( seedValue < seedMin )
            {
                cout << "Invalid value [" << argv[seedPos] << "] specified for seed" << endl;
                printUsageMsg();
                return;
            } // if
        } // case seedPos + 1
        case groupPos + 1 :
        {
            groupValue = atoi( argv[groupPos] );

            if ( groupValue < groupMin || groupValue % 2 != 1 )
            {
                cout << "Invalid value [" << argv[groupPos] << "] specified for group quantity" << endl;
                printUsageMsg();
                return;
            } // if
        } // case groupPos + 1
        case voterPos + 1 :
        {
            voterValue = atoi( argv[voterPos] );

            if ( voterValue < voterMin || voterValue % groupValue != 0 )
            {
                cout << "Invalid value [" << argv[voterPos] << "] specified for voter quantity" << endl;
                printUsageMsg();
                return;
            } // if
        } // case voterPos + 1
        case noArgs :
        {
            break;
        } // case noArgs
        default :
        {
            cout << "Invalid number of arguments" << endl;
            printUsageMsg();
            return;
        } // default
    } // switch

    mprng.seed( seedValue );
    Printer * printer = new Printer( voterValue );
    TallyVotes * tallier = new TallyVotes( groupValue , *printer );
    Voter ** voters = (Voter**)malloc( sizeof(Voter*) * voterValue );

    for ( int i = 0 ; i < voterValue ; i += 1 )
        voters[i] = new Voter( i , *tallier , *printer );

    for ( int i = 0 ; i < voterValue ; i += 1 )
        delete voters[i];

    delete voters;
    delete tallier;
    delete printer;

    uRetCode = 0;
}

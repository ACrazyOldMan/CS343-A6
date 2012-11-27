#include <uC++.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "MPRNG.h"
#include "config.h"
#include "Printer.h"
#include "NameServer.h"
#include "Bank.h"
#include "WATCardOffice.h"
#include "Student.h"

using namespace std;

MPRNG mprng; // global instance of MPRNG

/**
 * @fn      Student::main
 * @brief   Main loop of Student
 */
void Student::main()
{
}

/**
 * @fn          printUsageMsg
 * @brief       Prints an appropriate usage message for the process
 */
void printUsageMsg()
{
    cout << "./soda [ config-file [ random-seed ( > 0 ) ] ]" << endl;
}

void uMain::main()
{
    const int noArgs = 1;
    const int filePos = 1 , seedPos = 2;
    const int seedMin = 1;
    int seedValue = time( NULL );
    string configFile = "soda.config";

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
        case filePos + 1 :
        {
            configFile = argv[filePos];
        } // case filePos + 1
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

    ConfigParams params;
    processConfigFile( configFile.c_str() , params );
    mprng.seed( seedValue );
    Printer * printer = new Printer( params.numStudents , params.numVendingMachines , params.numCouriers );
    NameServer * nameServer = new NameServer( *printer , params.numVendingMachines , params.numStudents );
    Bank * bank = new Bank( params.numStudents );
    WATCardOffice * cardOffice = new WATCardOffice( *printer , *bank , params.numCouriers );
    Student ** students = (Student**)malloc( sizeof(Student*) * params.numStudents );

    for ( unsigned int i = 0 ; i < params.numStudents ; i += 1 )
        students[i] = new Student( *printer , *nameServer , *cardOffice , i , params.maxPurchases );

    for ( unsigned int i = 0 ; i < params.numStudents ; i += 1 )
        delete students[i];

    delete students;
    delete cardOffice;
    delete bank;
    delete nameServer;
    delete printer;

    uRetCode = 0;
}

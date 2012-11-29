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
#include "WATCard.h"
#include "VendingMachine.h"
#include "States.h"
#include "Parent.h"

using namespace std;

MPRNG mprng; // global instance of MPRNG

/**
 * @fn      Courier::main
 * @brief   Main loop of Courier
 */
void WATCardOffice::Courier::main()
{
    printer->print( Printer::Courier , id , Starting );

    while ( true )
    {
        _Accept( ~Courier )
        {
            break;
        }
        else
        {
            Job * job = office->requestWork();
            unsigned int sID = job->args.sID;
            unsigned int amount = job->args.amount;
            WATCard * card = job->args.card;
            printer->print( Printer::Courier , id , BeginTransfer , sID , amount );
            bank->withdraw( sID , amount );
            printer->print( Printer::Courier , id , EndTransfer , sID , amount );

            // test for lost card
            if ( mprng( 5 ) == 0 )
            {
                job->result.exception( new Lost() );
                delete card;
            }
            else
            {
                card->deposit( amount );
                job->result.delivery( card );
            } // if
        } // _Accept
    } // while

    printer->print( Printer::Courier , id , Finished );
}

/**
 * @fn      Parent::main
 * @brief   Main loop of Parent
 */
void Parent::main()
{
    printer->print( Printer::Parent , Starting );

    while ( true )
    {
        // accept destructor so task exits gracefully; meanwhile, deposit gifts at random
        _Accept( ~Parent )
        {
            break;
        }
        else
        {
            yield( delay );
            unsigned int sID = mprng( studentQuantity - 1 );
            unsigned int amount = mprng( 1 , 3 );
            printer->print( Printer::Parent , Deposit , sID , amount );
            bank->deposit( sID , amount );
        } // _Accept
    } // while

    printer->print( Printer::Parent , Finished );
}

/**
 * @fn      Student::main
 * @brief   Main loop of Student
 */
void Student::main()
{
    VendingMachine::Flavours flavour = (VendingMachine::Flavours)mprng( 3 );
    unsigned int totalPurchases = mprng( 1 , maxPurchases );
    printer->print( Printer::Student , id , Starting , flavour , totalPurchases );
    unsigned int purchaseCounter = 0;
    FWATCard fCard = office->create( id , 5 );
    VendingMachine * machine = server->getMachine( id );
    printer->print( Printer::Student , id , SelectingMachine , machine->getID() );

    while ( purchaseCounter < totalPurchases )
    {
        yield( mprng( 1 , 10 ) );

        // keep trying to buy soda until successful
        while ( true )
        {
            try
            {
                WATCard * card = fCard();
                VendingMachine::Status status = machine->buy( flavour , *card );

                switch ( status )
                {
                    case VendingMachine::BUY :
                    {
                        printer->print( Printer::Student , id , BoughtSoda , card->getBalance() );
                        purchaseCounter += 1;
                        continue;
                    } // case VendingMachine::BUY
                    case VendingMachine::STOCK :
                    {
                        machine = server->getMachine( id );
                        printer->print( Printer::Student , id , SelectingMachine , machine->getID() );
                        break;
                    } // case VendingMachine::STOCK
                    case VendingMachine::FUNDS :
                    {
                        printer->print( Printer::Student , id , LostCard );
                        fCard = office->transfer( id , machine->cost() + 5 , card );
                        break;
                    } // case VendingMachine::FUNDS
                } // switch
            }
            catch ( WATCardOffice::Lost ) // WATCard lost
            {
                fCard = office->create( id , 5 );
            } // try
        } // while
    } // while

    printer->print( Printer::Student , id , Finished );
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
    Bank * bank = new Bank( params.numStudents );
    Parent * parent = new Parent( *printer , *bank , params.numStudents , params.parentalDelay );
    NameServer * server = new NameServer( *printer , params.numVendingMachines , params.numStudents );
    WATCardOffice * office = new WATCardOffice( *printer , *bank , params.numCouriers );
    VendingMachine ** machines = (VendingMachine**)malloc( sizeof(VendingMachine*) * params.numVendingMachines );
    Student ** students = (Student**)malloc( sizeof(Student*) * params.numStudents );

    for ( unsigned int i = 0 ; i < params.numVendingMachines ; i += 1 )
        machines[i] = new VendingMachine( *printer , *server , i , params.sodaCost , params.maxStockPerFlavour );

    for ( unsigned int i = 0 ; i < params.numStudents ; i += 1 )
        students[i] = new Student( *printer , *server , *office , i , params.maxPurchases );

    for ( unsigned int i = 0 ; i < params.numStudents ; i += 1 )
        delete students[i];

    for ( unsigned int i = 0 ; i < params.numVendingMachines ; i += 1 )
        delete machines[i];

    delete students;
    delete machines;
    delete office;
    delete server;
    delete parent;
    delete bank;
    delete printer;

    uRetCode = 0;
}

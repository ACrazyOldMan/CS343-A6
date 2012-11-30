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
#include "BottlingPlant.h"
#include "Truck.h"

using namespace std;

MPRNG mprng; // global instance of MPRNG

/**
 * @fn      Truck::main
 * @brief   Main loop of Truck
 */
void Truck::main()
{
    printer->print( Printer::Truck , Starting );
    VendingMachine ** machines = server->getMachineList();
    unsigned int cargo[FLAVOUR_QUANTITY];
    unsigned int totalCargo = 0;
    yield( mprng( 1 , 10 ) );

    while ( !plant->getShipment( cargo ) )
    {
        for ( unsigned int i = 0 ; i < FLAVOUR_QUANTITY ; i += 1 )
            totalCargo += cargo[i];

        printer->print( Printer::Truck , Pickup , totalCargo );

        for ( unsigned int i = 0 ; i < machineQuantity ; i += 1 )
        {
            if ( totalCargo == 0 )
                break;

            VendingMachine * vm = machines[i];
            printer->print( Printer::Truck , BeginDelivery , vm->getID() , totalCargo );
            unsigned int totalMissing = 0; // total number of bottles not filled
            unsigned int * inventory = vm->inventory();

            for ( unsigned int j = 0 ; j < FLAVOUR_QUANTITY ; j += 1 )
            {
                unsigned int inv = inventory[j];
                unsigned int miss = maxStock - inv;

                // check if enough cargo to fill all
                if ( miss > cargo[j] )
                {
                    inventory[j] += cargo[j];
                    totalCargo -= cargo[j];
                    cargo[j] = 0;
                    totalMissing += maxStock - inventory[j];
                }
                else
                {
                    inventory[j] = maxStock;
                    totalCargo -= miss;
                    cargo[j] -= miss;
                } // if
            } // for

            vm->restocked();

            if ( totalMissing > 0 )
                printer->print( Printer::Truck , CannotFill , vm->getID() , totalMissing );

            printer->print( Printer::Truck , EndDelivery , vm->getID() , totalCargo );
        } // for

        totalCargo = 0;
    } // while

    printer->print( Printer::Truck , Finished );
}

/**
 * @fn      BottlingPlant::main
 * @brief   Main loop of BottlingPlant
 */
void BottlingPlant::main()
{
    printer->print( Printer::BottlingPlant , Starting );
    Truck * truck = new Truck( *printer , *server , *this , machineQuantity , maxStock );

    while ( true )
    {
        // accept destructor so task exits gracefully; meanwhile, keep producing soda
        _Accept( ~BottlingPlant )
        {
            break;
        }
        else
        {
            totalProduced = 0;

            for ( unsigned int i = 0 ; i < FLAVOUR_QUANTITY ; i += 1 )
            {
                production[i] = mprng( maxShipped );
                totalProduced += production[i];
            } // for

            yield( delay );
            printer->print( Printer::BottlingPlant , GeneratingSoda , totalProduced );

            _Accept( getShipment )
            {
                printer->print( Printer::BottlingPlant , Pickup );
            } // _Accept
        } // _Accept
    } // while

    isShuttingDown = true;
    _Accept( getShipment );
    delete truck;
    printer->print( Printer::BottlingPlant , Finished );
}

/**
 * @fn      Courier::main
 * @brief   Main loop of Courier
 */
void WATCardOffice::Courier::main()
{
    printer->print( Printer::Courier , id , Starting );

    while ( true )
    {
        // accept destructor so task exits gracefully; meanwhile, keep working on jobs
        _Accept( ~Courier )
        {
            break;
        }
        else
        {
            Job * job = office->requestWork();

            // if no job and returns, means office is closing
            if ( job == NULL )
                break;

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

            delete job;
            printer->print( Printer::WATCardOffice , CourierComplete );
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

    while ( true )
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
                        goto NextBottle;
                    } // case VendingMachine::BUY
                    case VendingMachine::STOCK :
                    {
                        machine = server->getMachine( id );
                        printer->print( Printer::Student , id , SelectingMachine , machine->getID() );
                        break;
                    } // case VendingMachine::STOCK
                    case VendingMachine::FUNDS :
                    {
                        fCard = office->transfer( id , machine->cost() + 5 , card );
                        break;
                    } // case VendingMachine::FUNDS
                } // switch
            }
            catch ( WATCardOffice::Lost ) // WATCard lost
            {
                printer->print( Printer::Student , id , LostCard );
                fCard = office->create( id , 5 );
            } // try
        } // while

        NextBottle :

        if ( purchaseCounter == totalPurchases )
        {
            delete fCard();
            break;
        }
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
    WATCardOffice * office = new WATCardOffice( *printer , *bank , params.numCouriers );
    NameServer * server = new NameServer( *printer , params.numVendingMachines , params.numStudents );
    VendingMachine ** machines = (VendingMachine**)malloc( sizeof(VendingMachine*) * params.numVendingMachines );

    for ( unsigned int i = 0 ; i < params.numVendingMachines ; i += 1 )
        machines[i] = new VendingMachine( *printer , *server , i , params.sodaCost , params.maxStockPerFlavour );

    BottlingPlant * plant = new BottlingPlant( *printer , *server , params.numVendingMachines , params.maxShippedPerFlavour , params.maxStockPerFlavour , params.timeBetweenShipments );
    Student ** students = (Student**)malloc( sizeof(Student*) * params.numStudents );

    for ( unsigned int i = 0 ; i < params.numStudents ; i += 1 )
        students[i] = new Student( *printer , *server , *office , i , params.maxPurchases );

    for ( unsigned int i = 0 ; i < params.numStudents ; i += 1 )
        delete students[i];

    delete students;
    delete plant;

    for ( unsigned int i = 0 ; i < params.numVendingMachines ; i += 1 )
        delete machines[i];

    delete machines;
    delete server;
    delete office;
    delete parent;
    delete bank;
    delete printer;

    uRetCode = 0;
}

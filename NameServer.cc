#include "NameServer.h"
#include "States.h"
#include "VendingMachine.h"
#include "Printer.h"

/**
 * @brief       Constructor for NameServer
 * @param[in]   prt                 Printer reference
 * @param[in]   numVendingMachines  Number of machines
 * @param[in]   numStudents         Number of students
 */
NameServer::NameServer( Printer &prt , unsigned int numVendingMachines , unsigned int numStudents )
        : printer( &prt ), machineQuantity( numVendingMachines ), counter( 0 )
{
    machines = (VendingMachine**)malloc( sizeof(VendingMachine*) * numVendingMachines );
    assignedMachines = (unsigned int*)malloc( sizeof(unsigned int) * numStudents );

    for ( unsigned int i = 0 ; i < numStudents ; i += 1 )
        assignedMachines[i] = i % numVendingMachines;
}

/**
 * @brief       Destructor for NameServer
 */
NameServer::~NameServer()
{
    delete machines;
    delete assignedMachines;
}

/**
 * @fn          NameServer::registerVM
 * @brief       Register vending machine
 * @param[in]   vm      Vending machine reference
 */
void NameServer::registerVM( VendingMachine * vm )
{
    machines[counter] = vm;
}

/**
 * @fn          NameServer::getMachine
 * @brief       Get vending machine for student
 * @param[in]   id      Student ID
 * @return      Vending machine reference
 */
VendingMachine * NameServer::getMachine( unsigned int id )
{
    sID = id;
    return machines[assignedMachines[id]];
}

/**
 * @fn          NameServer::getMachineList
 * @brief       Get list of all vending machines
 * @return      Vending machine references
 */
VendingMachine ** NameServer::getMachineList()
{
    return machines;
}

/**
 * @fn      NameServer::main
 * @brief   Main loop of NameServer
 */
void NameServer::main()
{
    printer->print( Printer::NameServer , Starting );

    while ( true )
    {
        // accept destructor so task exits gracefully; meanwhile, perform various functionalities
        _Accept( ~NameServer )
        {
            break;
        }
        or _When( counter < machineQuantity ) _Accept( registerVM )
        {
            printer->print( Printer::NameServer , RegisterMachine , counter );
            counter += 1;
        }
        or _Accept( getMachineList )
        {
        }
        or _Accept( getMachine )
        {
            printer->print( Printer::NameServer , NewMachine , sID , assignedMachines[sID] );
            assignedMachines[sID] = (assignedMachines[sID] + 1) % machineQuantity;
        } // _Accept
    } // while

    printer->print( Printer::NameServer , Finished );
}

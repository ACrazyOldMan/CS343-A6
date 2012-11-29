#include "Truck.h"

Truck::Truck( Printer &prt , NameServer &nameServer , BottlingPlant &plant , unsigned int numVendingMachines , unsigned int maxStockPerFlavour )
        : printer( &prt ), server( &nameServer ), plant( &plant ), machineQuantity( numVendingMachines ), maxStock( maxStockPerFlavour )
{
}

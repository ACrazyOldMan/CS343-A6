#include "BottlingPlant.h"
#include "VendingMachine.h"

/**
 * @brief       Constructor for BottlingPlant
 * @param[in]   prt                     Printer reference
 * @param[in]   nameServer              NameServer reference
 * @param[in]   numVendingMachines      Number of vending machines
 * @param[in]   maxShippedPerFlavour    Maximum bottles shipped per flavour per run
 * @param[in]   maxStockPerFlavour      Maximum inventory stock per flavour
 * @param[in]   timeBetweenShipments    Delay between each run
 */
BottlingPlant::BottlingPlant( Printer &prt , NameServer &nameServer , unsigned int numVendingMachines , unsigned int maxShippedPerFlavour , unsigned int maxStockPerFlavour , unsigned int timeBetweenShipments )
        : printer( &prt ), server( &nameServer ), machineQuantity( numVendingMachines ), maxShipped( maxShippedPerFlavour ), maxStock( maxStockPerFlavour ), delay( timeBetweenShipments ), isShuttingDown( false )
{
    production = (unsigned int*)malloc( sizeof(unsigned int) * FLAVOUR_QUANTITY );
}

/**
 * @brief       Destructor for BottlingPlant
 */
BottlingPlant::~BottlingPlant()
{
    delete production;
}

/**
 * @fn              BottlingPlant::getShipment
 * @brief           Get new shipment
 * @param[in,out]   cargo       Storage for new shipment
 * @return          True if plant shutting down and no new shipment
 */
bool BottlingPlant::getShipment( unsigned int cargo[] )
{
    if ( isShuttingDown )
        return true;

    for ( unsigned int i = 0 ; i < FLAVOUR_QUANTITY ; i += 1 )
        cargo[i] = production[i];

    return false;
}

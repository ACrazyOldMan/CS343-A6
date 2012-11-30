#ifndef BottlingPlant_H
#define BottlingPlant_H

#include <uC++.h>

class Printer;
_Task NameServer;

_Task BottlingPlant
{
    public:

    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments );
    ~BottlingPlant();

    bool getShipment( unsigned int cargo[] );

    private:

    Printer * printer; // Printer reference
    NameServer * server;// NameServer reference
    unsigned int machineQuantity;// number of vending machines
    unsigned int maxShipped;// maximum bottles shipped per run
    unsigned int maxStock;// maximum stock in machine inventories per flavour
    unsigned int delay;// time between runs
    unsigned int * production;// storage for bottles produced each run
    unsigned int totalProduced;// total number of bottles produced per run
    bool isShuttingDown;// flag to indicate plant is shutting down

    void main();

};

#endif // BottlingPlant_H

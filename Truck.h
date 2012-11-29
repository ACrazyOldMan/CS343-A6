#ifndef Truck_H
#define Truck_H

#include <uC++.h>

class Printer;
_Task NameServer;
_Task BottlingPlant;

_Task Truck
{
    public:

    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour );

    private:

    Printer * printer; // Printer reference
    NameServer * server;// NameServer reference
    BottlingPlant * plant;// BottlingPlant reference
    unsigned int machineQuantity;// number of vending machines
    unsigned int maxStock;// maximum machine inventory per flavour

    void main();

};

#endif // Truck_H

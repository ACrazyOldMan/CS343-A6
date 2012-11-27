#ifndef Truck_H
#define Truck_H

#include <uC++.h>

class Printer;
class NameServer;
class BottlingPlant;

_Task Truck
{
    public:

    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour );

    private:

    void main();

};

#endif // Truck_H

#ifndef BottlingPlant_H
#define BottlingPlant_H

#include <uC++.h>

class Printer;
class NameServer;

_Task BottlingPlant
{
    public:

    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments );

    bool getShipment( unsigned int cargo[] );

    private:

    void main();

};

#endif // BottlingPlant_H

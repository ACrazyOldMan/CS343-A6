#ifndef VendingMachine_H
#define VendingMachine_H

#include <uC++.h>

class Printer;
class WATCard;
_Task NameServer;

_Task VendingMachine
{
    public:

    // flavours of soda
    enum Flavours
    {
        Apple = 0,
        Orange,
        Grape,
        Cranberry
    };

    // purchase status: successful buy, out of stock, insufficient funds
    enum Status
    {
        BUY,
        STOCK,
        FUNDS
    };

    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour );

    Status buy( Flavours flavour, WATCard &card );
    unsigned int * inventory();
    void restocked();

    _Nomutex unsigned int cost();
    _Nomutex unsigned int getID();

    private:

    void main();

};

#endif // VendingMachine_H

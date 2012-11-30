#ifndef VendingMachine_H
#define VendingMachine_H

#include <uC++.h>

#define FLAVOUR_QUANTITY    4

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
    ~VendingMachine();

    Status buy( Flavours flavour, WATCard &card );
    unsigned int * inventory();
    void restocked();

    _Nomutex unsigned int cost();
    _Nomutex unsigned int getID();

    private:

    Printer * printer; // Printer reference
    NameServer * server;// NameServer reference
    unsigned int id;// machine ID
    unsigned int sodaCost;// soda cost
    unsigned int maxStock;// max stock per flavour
    unsigned int * sodaInventory;// currently available stock per flavour
    bool isRestocking;// flag to indicate restocking event
    unsigned int flavourPurchased;// stores information about purchased soda; allows for more concurrent processing

    void main();

};

#endif // VendingMachine_H

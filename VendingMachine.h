#ifndef VendingMachine_H
#define VendingMachine_H

#include <uC++.h>

class Printer;
class NameServer;
class WATCard;

_Task VendingMachine
{
    public:

    enum Flavours
    {
    };          // flavours of soda (YOU DEFINE)

    enum Status
    {
        BUY,
        STOCK,
        FUNDS
    };          // purchase status: successful buy, out of stock, insufficient funds

    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour );

    Status buy( Flavours flavour, WATCard &card );
    unsigned int * inventory();
    void restocked();

    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();

    private:

    void main();

};

#endif // VendingMachine_H

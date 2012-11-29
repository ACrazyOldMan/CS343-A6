#ifndef NameServer_H
#define NameServer_H

#include <uC++.h>

class Printer;
_Task VendingMachine;

_Task NameServer
{
    public:

    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );

    void registerVM( VendingMachine * vendingmachine );
    VendingMachine * getMachine( unsigned int id );
    VendingMachine ** getMachineList();

    private:

    void main();

};

#endif // NameServer_H

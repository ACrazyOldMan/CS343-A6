#ifndef NameServer_H
#define NameServer_H

#include <uC++.h>

class Printer;
_Task VendingMachine;

_Task NameServer
{
    public:

    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );

    void registerVM( VendingMachine * vm );
    VendingMachine * getMachine( unsigned int id );
    VendingMachine ** getMachineList();

    private:

    Printer * printer; // Printer reference
    unsigned int machineQuantity;// total number of machines
    VendingMachine ** machines;// list of references to all machines
    unsigned int counter;// counter to keep track of number of machines registered
    unsigned int * assignedMachines;// assigned machines for each student
    unsigned int sID;// stored student ID for last machine request; used to delegate more processing to main

    void main();

};

#endif // NameServer_H

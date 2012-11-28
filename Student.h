#ifndef Student_H
#define Student_H

#include "NameServer.h"
#include "WATCardOffice.h"

class Printer;

_Task Student
{
    public:

    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases );

    private:

    Printer * printer; // reference to Printer
    NameServer * server;// reference to NameServer
    WATCardOffice * office;// reference to WATCardOffice
    unsigned int id;// student ID
    unsigned int maxPurchases;// maximum number of purchases that can be made

    void main();

};

#endif // Student_H

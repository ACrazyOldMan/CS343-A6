#ifndef Student_H
#define Student_H

class Printer;
class NameServer;
class WATCardOffice;

_Task Student
{
    public:

    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases );

    private:

    void main();

};

#endif // Student_H

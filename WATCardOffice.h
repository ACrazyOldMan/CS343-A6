#ifndef WATCardOffice_H
#define WATCardOffice_H

#include <uC++.h>
#include <queue>
#include "WATCard.h"

class Printer;
class Bank;

_Task WATCardOffice
{
    private:

    struct Args
    {
        unsigned int sID; // ID of student for which the current Job applies
        unsigned int amount;// amount to transfer from bank
        WATCard * card;// card to transfer funds into

        Args( unsigned int sID , unsigned int amount , WATCard * card )
        : sID( sID ) , amount( amount ) , card( card )
        {
        }
    };

    struct Job // marshalled arguments and return future
    {
        Args args; // call arguments
        FWATCard result;// return future

        Job( Args args )
        : args( args )
        {
        }
    };

    public:

    _Event Lost
    {
    };

    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );

    FWATCard create( unsigned int sid, unsigned int amount );
    FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job * requestWork();

    private:

    _Task Courier // communicator with bank
    {
        public:

        Courier( unsigned int id );

        private:

        unsigned int id; // courier ID

        void main();
    };

    static WATCardOffice * office; // WATCardOffice reference
    static Printer * printer;// Printer reference
    static Bank * bank;// Bank reference

    unsigned int courierQuantity;// number of couriers
    std::queue< Job* > jobQueue;// queue of jobs for couriers

    void main();

};

#endif // WATCardOffice_H

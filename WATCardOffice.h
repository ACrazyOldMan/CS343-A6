#ifndef WATCardOffice_H
#define WATCardOffice_H

#include <uC++.h>
#include "WATCard.h"

class Printer;
class Bank;

_Task WATCardOffice
{
    struct Job
    {                // marshalled arguments and return future
        Args args;// call arguments (YOU DEFINE "Args")
        FWATCard result;// return future

        Job( Args args ) : args( args )
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

    _Task Courier
    {
    };          // communicates with bank

    void main();

};

#endif // WATCardOffice_H

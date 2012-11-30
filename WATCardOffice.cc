#include "WATCardOffice.h"
#include "Printer.h"
#include "Bank.h"
#include "WATCard.h"

WATCardOffice * WATCardOffice::office = NULL;
Printer * WATCardOffice::printer = NULL;
Bank * WATCardOffice::bank = NULL;
unsigned int WATCardOffice::courierCount = 0;

/**
 * @brief       Constructor for Parent
 * @param[in]   prt             Printer reference
 * @param[in]   bank            Bank reference
 * @param[in]   numCouriers     Number of couriers
 */
WATCardOffice::WATCardOffice( Printer &prt , Bank &bank , unsigned int numCouriers )
        : courierQuantity( numCouriers ), isClosing( false )
{
    WATCardOffice::office = this;
    WATCardOffice::printer = &prt;
    WATCardOffice::bank = &bank;
}

/**
 * @brief       Constructor for Courier
 */
WATCardOffice::Courier::Courier()
{
    id = courierCount;
    courierCount += 1;
}

/**
 * @fn          WATCardOffice::create
 * @brief       Create new WATCard
 * @param[in]   sid         Student ID
 * @param[in]   amount      Transfer amount
 * @return      Future WATCard
 */
FWATCard WATCardOffice::create( unsigned int sid , unsigned int amount )
{
    return transfer( sid , amount , new WATCard() );
}

/**
 * @fn          WATCardOffice::transfer
 * @brief       Transfer money to WATCard
 * @param[in]   sid         Student ID
 * @param[in]   amount      Transfer amount
 * @param[in]   card        WATCard reference
 * @return      Future WATCard
 */
FWATCard WATCardOffice::transfer( unsigned int sid , unsigned int amount , WATCard *card )
{
    Job * job = new Job( Args( sid , amount , card ) );
    jobQueue.push( job );
    return job->result;
}

/**
 * @fn          WATCardOffice::requestWork
 * @brief       Work request for couriers
 * @return      Next job
 */
WATCardOffice::Job * WATCardOffice::requestWork()
{
    // if office closing, tell each courier no more jobs left so they can finish
    if ( isClosing )
    {
        courierCount -= 1;
        return NULL;
    } // if

    return jobQueue.front();
}

/**
 * @fn      WATCardOffice::main
 * @brief   Main loop of WATCardOffice
 */
void WATCardOffice::main()
{
    printer->print( Printer::WATCardOffice , Starting );
    Courier ** couriers = (Courier**)malloc( sizeof(Courier*) * courierQuantity );

    for ( unsigned int i = 0 ; i < courierQuantity ; i += 1 )
        couriers[i] = new Courier();

    while ( true )
    {
        // accept destructor so task exits gracefully; meanwhile, perform various functionalities
        _Accept( ~WATCardOffice )
        {
            isClosing = true;
            break;
        }
        or _When( jobQueue.size() > 0 ) _Accept( requestWork )
        {
            jobQueue.pop();
        }
        or _Accept( create )
        {
            Job * added = jobQueue.back();
            printer->print( Printer::WATCardOffice , CardCreation , added->args.sID , added->args.amount );
        }
        or _Accept( transfer )
        {
            Job * added = jobQueue.back();
            printer->print( Printer::WATCardOffice , EndTransfer , added->args.sID , added->args.amount );
        } // _Accept
    } // while

    // make sure no couriers are stuck waiting for jobs
    while ( courierCount > 0 )
    {
        _Accept( requestWork )
        {
        }
        else
        {
        }
    } // while

    for ( unsigned int i = 0 ; i < courierQuantity ; i += 1 )
        delete couriers[i];

    delete couriers;
    printer->print( Printer::WATCardOffice , Finished );
}

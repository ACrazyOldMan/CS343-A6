#include "VendingMachine.h"
#include "NameServer.h"
#include "WATCard.h"
#include "Printer.h"
#include "States.h"

/**
 * @brief       Constructor for VendingMachine
 * @param[in]   prt                 Printer reference
 * @param[in]   nameServer          NameServer reference
 * @param[in]   id                  Machine ID
 * @param[in]   sodaCost            Cost per soda
 * @param[in]   maxStockPerFlavour  Maximum inventory stock per flavour
 */
VendingMachine::VendingMachine( Printer &prt , NameServer &nameServer , unsigned int id , unsigned int sodaCost , unsigned int maxStockPerFlavour )
        : printer( &prt ), server( &nameServer ), id( id ), sodaCost( sodaCost ), maxStock( maxStockPerFlavour ), isRestocking( false ), flavourPurchased( -1 )
{
    sodaInventory = (unsigned int*)malloc( sizeof(unsigned int) * FLAVOUR_QUANTITY );

    for ( unsigned int i = 0 ; i < FLAVOUR_QUANTITY ; i += 1 )
        sodaInventory[i] = 0;
}

/**
 * @fn          VendingMachine::buy
 * @brief       Buy soda if possible
 * @param[in]   flavour     Soda flavour
 * @param[in]   card        WATCard for funds
 * @return      Transaction status
 */
VendingMachine::Status VendingMachine::buy( Flavours flavour , WATCard &card )
{
    if ( sodaInventory[flavour] == 0 )
        return STOCK;

    if ( card.getBalance() < sodaCost )
        return FUNDS;

    card.withdraw( sodaCost );
    flavourPurchased = flavour;
    return BUY;
}

/**
 * @fn          VendingMachine::inventory
 * @brief       Get reference to machine inventory
 * @return      Inventory reference
 */
unsigned int * VendingMachine::inventory()
{
    return sodaInventory;
}

/**
 * @fn          VendingMachine::restocked
 * @brief       Call to finish restocking
 */
void VendingMachine::restocked()
{
}

/**
 * @fn          VendingMachine::cost
 * @brief       Get soda cost
 * @return      Soda cost
 */
_Nomutex unsigned int VendingMachine::cost()
{
    return sodaCost;
}

/**
 * @fn          VendingMachine::getID
 * @brief       Get machine ID
 * @return      Machine ID
 */
_Nomutex unsigned int VendingMachine::getID()
{
    return id;
}

/**
 * @fn      VendingMachine::main
 * @brief   Main loop of VendingMachine
 */
void VendingMachine::main()
{
    printer->print( Printer::Vending , id , Starting , sodaCost );
    server->registerVM( this );

    while ( true )
    {
        // accept destructor so task exits gracefully; meanwhile, perform various functionalities
        _Accept( ~VendingMachine )
        {
            break;
        }
        or _When( isRestocking ) _Accept( restocked )
        {
            printer->print( Printer::Vending , id , EndReloading );
            isRestocking = false;
        }
        or _Accept( inventory )
        {
            printer->print( Printer::Vending , id , BeginReloading );
            isRestocking = true;
        }
        or _Accept( buy )
        {
            if ( flavourPurchased != (unsigned int)-1 )
            {
                sodaInventory[flavourPurchased] -= 1;
                printer->print( Printer::Vending , id , BoughtSoda , flavourPurchased , sodaInventory[flavourPurchased] );
                flavourPurchased = -1;
            }
        } // _Accept
    } // while

    printer->print( Printer::Vending , id , Finished );
}

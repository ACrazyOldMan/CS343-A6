#include "WATCard.h"

/**
 * @brief       Constructor for WATCard
 */
WATCard::WATCard()
        : balance( 0 )
{
}

/**
 * @fn          WATCard::deposit
 * @brief       Deposit funds into balance
 * @param[in]   amount      Amount to deposit
 */
void WATCard::deposit( unsigned int amount )
{
    balanceLock.acquire();
    balance += amount;
    balanceLock.release();
}

/**
 * @fn          WATCard::withdraw
 * @brief       Withdraw funds from balance
 * @param[in]   amount      Amount to withdraw
 */
void WATCard::withdraw( unsigned int amount )
{
    balanceLock.acquire();
    balance -= amount;
    balanceLock.release();
}

/**
 * @fn          WATCard::getBalance
 * @brief       Get current balance info
 * @return      Current balance
 */
unsigned int WATCard::getBalance()
{
    balanceLock.acquire();
    unsigned int bal = balance;
    balanceLock.release();
    return bal;
}

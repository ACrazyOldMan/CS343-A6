#include "Bank.h"

/**
 * @brief       Constructor for Bank
 */
Bank::Bank( unsigned int numStudents )
{
    balances = (unsigned int*)malloc( sizeof(unsigned int) * numStudents );
    enoughFunds = (uCondition*)malloc( sizeof(uCondition) * numStudents );
    currentWithdrawal = (unsigned int*)malloc( sizeof(unsigned int) * numStudents );

    for ( unsigned int i = 0 ; i < numStudents ; i += 1 )
    {
        balances[i] = 0;
    } // for
}

/**
 * @fn          Bank::deposit
 * @brief       Deposit funds into specified balance
 * @param[in]   id          Student ID
 * @param[in]   amount      Amount to deposit
 */
void Bank::deposit( unsigned int id , unsigned int amount )
{
    balances[id] += amount;

    if ( balances[id] >= currentWithdrawal[id] )
        enoughFunds[id].signal();
}

/**
 * @fn          Bank::withdraw
 * @brief       Withdraw funds from specified balance
 * @param[in]   id          Student ID
 * @param[in]   amount      Amount to withdraw
 */
void Bank::withdraw( unsigned int id , unsigned int amount )
{
    if ( balances[id] < amount )
    {
        currentWithdrawal[id] = amount;
        enoughFunds[id].wait();
    } // if

    balances[id] -= amount;
}

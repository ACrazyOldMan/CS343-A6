#ifndef Bank_H
#define Bank_H

#include <uC++.h>

_Monitor Bank
{
    public:

        Bank( unsigned int numStudents );

        void deposit( unsigned int id , unsigned int amount );
        void withdraw( unsigned int id , unsigned int amount );

    private:

        unsigned int * balances; // bank balances for each student
        uCondition ** enoughFunds; // conditions for waiting until enough funds present for account; greater concurrency than external scheduling since can selectively choose which task to block/unblock
        unsigned int * currentWithdrawal; // amount being blocked on for current withdrawal

};

#endif // Bank_H

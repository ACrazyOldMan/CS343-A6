#ifndef WATCard_H
#define WATCard_H

#include <uC++.h>
#include <uFuture.h>

class WATCard
{
    public:

        WATCard();

        void deposit( unsigned int amount );
        void withdraw( unsigned int amount );
        unsigned int getBalance();

    private:

        unsigned int balance; // balance of card
        uOwnerLock balanceLock; // prevent simultaneous access to card balance

        // prevent copying
        WATCard( const WATCard & );
        WATCard &operator=( const WATCard & );

};

typedef Future_ISM< WATCard * > FWATCard;     // future WATCard pointer

#endif // WATCard_H

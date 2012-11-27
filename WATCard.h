#ifndef WATCard_H
#define WATCard_H

#include <uFuture.h>

class WATCard
{
    public:

        WATCard();

        void deposit( unsigned int amount );
        void withdraw( unsigned int amount );
        unsigned int getBalance();

    private:

        // prevent copying
        WATCard( const WATCard & );
        WATCard &operator=( const WATCard & );

};

typedef Future_ISM< WATCard * > FWATCard;     // future WATCard pointer

#endif // WATCard_H

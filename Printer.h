#ifndef Printer_H
#define Printer_H

#include <uC++.h>
#include <string>
#include "States.h"

_Monitor Printer
{
    public:

        enum Kind
        {
            Parent = 0,
            WATCardOffice,
            NameServer,
            Truck,
            BottlingPlant,
            Student,
            Vending,
            Courier
        };

        Printer( unsigned int numStudents , unsigned int numVendingMachines , unsigned int numCouriers );
        ~Printer();

        void print( Kind kind , States state );
        void print( Kind kind , States state , unsigned int value1 );
        void print( Kind kind , States state , unsigned int value1 , unsigned int value2 );
        void print( Kind kind , unsigned int lid , States state );
        void print( Kind kind , unsigned int lid , States state , unsigned int value1 );
        void print( Kind kind , unsigned int lid , States state , unsigned int value1 , unsigned int value2 );

    private:

        unsigned int studentQuantity; // number of students
        unsigned int machineQuantity; // number of vending machines
        unsigned int courierQuantity; // number of couriers
        States * stateStorage; // storage for task states
        unsigned int * valueStorage; // storage for task values

        std::string i2s( unsigned int i ) const;
        States getState( Kind kind , unsigned int lid );

};

#endif // Printer_H

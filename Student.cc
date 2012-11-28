#include "Student.h"

/**
 * @brief       Constructor for Student
 * @param[in]   prt             Reference to Printer
 * @param[in]   nameServer      Reference to NameServer
 * @param[in]   cardOffice      Reference to WATCardOffice
 * @param[in]   id              Student ID
 * @param[in]   maxPurchases    Maximum number of purchases that can be made
 */
Student::Student( Printer &prt , NameServer &nameServer , WATCardOffice &cardOffice , unsigned int id , unsigned int maxPurchases )
        : printer( &prt ), server( &nameServer ), office( &cardOffice ), id( id ), maxPurchases( maxPurchases )
{
}

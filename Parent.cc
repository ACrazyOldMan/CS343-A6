#include "Parent.h"

/**
 * @brief       Constructor for Parent
 * @param[in]   prt             Printer reference
 * @param[in]   bank            Bank reference
 * @param[in]   numStudents     Number of students
 * @param[in]   parentalDelay   Delay for depositing gifts
 */
Parent::Parent( Printer &prt , Bank &bank , unsigned int numStudents , unsigned int parentalDelay )
        : printer( &prt ), bank( &bank ), studentQuantity( numStudents ), delay( parentalDelay )
{
}

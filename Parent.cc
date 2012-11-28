#include "Parent.h"

Parent::Parent( Printer &prt , Bank &bank , unsigned int numStudents , unsigned int parentalDelay )
        : printer( &prt ), bank( &bank ), studentQuantity( numStudents ), delay( parentalDelay )
{
}

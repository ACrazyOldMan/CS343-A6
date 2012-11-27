#ifndef Parent_H
#define Parent_H

#include <uC++.h>

class Printer;
class Bank;

_Task Parent
{
    public:

    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );

    private:

    void main();

};

#endif // Parent_H

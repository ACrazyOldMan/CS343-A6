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

    Printer * printer; // reference to Printer
    Bank * bank;//reference to Bank
    unsigned int studentQuantity;// number of students
    unsigned int delay;// delay for depositing gifts

    void main();

};

#endif // Parent_H

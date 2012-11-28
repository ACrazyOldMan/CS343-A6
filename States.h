#ifndef States_H
#define States_H

enum States
{
    Starting = 'S',
    Finished = 'F',
    Deposit = 'D',
    CourierComplete = 'W',
    CreationComplete = 'C',
    RegisterMachine = 'R',
    NewMachine = 'N',
    Pickup = 'P',
    BeginDelivery = 'd',
    CannotFill = 'U',
    EndDelivery = 'D',
    GeneratingSoda = 'G',
    SelectingMachine = 'V',
    BoughtSoda = 'B',
    LostCard = 'L',
    BeginReloading = 'r',
    EndReloading = 'R',
    BeginTransfer = 't',
    EndTransfer = 'T',
};

#endif // States_H

#include "VendingMachine.h"

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour)
  : mPrinter(prt),
    mNameServer(nameServer),
    mId(id),
    mSodaCost(sodaCost),
    mMaxStockPerFlavour(maxStockPerFlavour)
{

}

VendingMachine::Status
VendingMachine::buy(VendingMachine::Flavours flavour, WATCard &card)
{
  return VendingMachine::BUY;
}

unsigned int *
VendingMachine::inventory()
{
  return NULL;
}

void
VendingMachine::restocked()
{

}

unsigned int
VendingMachine::cost()
{
  return 0;
}

unsigned int
VendingMachine::getID()
{
  return 0;
}

#include <uC++.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "Printer.h"

using namespace std;

#define COLUMN_WIDTH        8
#define SINGLETON_QUANTITY  5

/**
 * @brief       Constructor for Printer
 * @param[in]   numStudents             Number of students
 * @param[in]   numVendingMachines      Number of vending machines
 * @param[in]   numCouriers             Number of couriers
 */
Printer::Printer( unsigned int numStudents , unsigned int numVendingMachines , unsigned int numCouriers )
        : studentQuantity( numStudents ), machineQuantity( numVendingMachines ), courierQuantity( numCouriers )
{
    const unsigned int totalQuantity = (SINGLETON_QUANTITY + numStudents + numVendingMachines + numCouriers);
    stateStorage = (States*)malloc( sizeof(States) * totalQuantity );
    valueStorage = (unsigned int*)malloc( sizeof(unsigned int) * totalQuantity * 2 );

    for ( unsigned int i = 0 ; i < totalQuantity ; i += 1 )
    {
        string s;

        if ( i >= SINGLETON_QUANTITY + numStudents + numVendingMachines ) // couriers
        {
            s = "Cour" + i2s( i - SINGLETON_QUANTITY - numStudents - numVendingMachines );
        }
        else if ( i >= SINGLETON_QUANTITY + numStudents ) // vending machines
        {
            s = "Mach" + i2s( i - SINGLETON_QUANTITY - numStudents );
        }
        else if ( i >= SINGLETON_QUANTITY ) // students
        {
            s = "Stud" + i2s( i - SINGLETON_QUANTITY );
        }
        else // singletons
        {
            switch ( i )
            {
                case Parent :
                {
                    s = "Parent";
                    break;
                } // case Parent
                case WATCardOffice :
                {
                    s = "WATOff";
                    break;
                } // case WATCardOffice
                case NameServer :
                {
                    s = "Names";
                    break;
                } // case NameServer
                case Truck :
                {
                    s = "Truck";
                    break;
                } // case Truck
                case BottlingPlant :
                {
                    s = "Plant";
                    break;
                } // case BottlingPlant
            } // switch
        } // if

        stateStorage[i] = Finished;
        cout << setw( COLUMN_WIDTH ) << left << s;
    } // for

    cout << endl;

    for ( unsigned int i = 0 ; i < totalQuantity ; i += 1 )
        cout << setw( COLUMN_WIDTH ) << left << "*******";

    cout << endl;
}

/**
 * @brief       Destructor for Printer
 */
Printer::~Printer()
{
    delete stateStorage;
    delete valueStorage;

    cout << "***********************\n" << endl;
}

/**
 * @fn          Printer::print
 * @brief       Print state information about specific kind of task
 * @param[in]   kind    Task kind
 * @param[in]   state   Task state
 */
void Printer::print( Kind kind , States state )
{
    print( kind , 0 , state );
}

/**
 * @fn          Printer::print
 * @brief       Print state information about specific kind of task
 * @param[in]   kind    Task kind
 * @param[in]   state   Task state
 * @param[in]   value1  First value printed with state
 */
void Printer::print( Kind kind , States state , unsigned int value1 )
{
    print( kind , 0 , state , value1 );
}

/**
 * @fn          Printer::print
 * @brief       Print state information about specific kind of task
 * @param[in]   kind    Task kind
 * @param[in]   state   Task state
 * @param[in]   value1  First value printed with state
 * @param[in]   value2  Second value printed with state
 */
void Printer::print( Kind kind , States state , unsigned int value1 , unsigned int value2 )
{
    print( kind , 0 , state , value1 , value2 );
}

/**
 * @fn          Printer::print
 * @brief       Print state information about specific kind of task
 * @param[in]   kind    Task kind
 * @param[in]   lid     Local ID number
 * @param[in]   state   Task state
 */
void Printer::print( Kind kind , unsigned int lid , States state )
{
    const unsigned int totalQuantity = SINGLETON_QUANTITY + studentQuantity + machineQuantity + courierQuantity;
    States prevState = getState( kind , lid );

    if ( prevState != Finished )
    {
        for ( unsigned int i = 0 ; i < totalQuantity ; i += 1 )
        {
            cout << setw( COLUMN_WIDTH ) << left;
            prevState = stateStorage[i];

            if ( prevState == Finished )
            {
                cout << "";
            }
            else
            {
                string s = "";
                s += prevState;

                if ( i >= SINGLETON_QUANTITY + studentQuantity + machineQuantity ) // couriers
                {
                    switch ( prevState )
                    {
                        case BeginTransfer :
                        case EndTransfer :
                        {
                            s += i2s( valueStorage[i * 2] ) + "," + i2s( valueStorage[i * 2 + 1] );
                            break;
                        } // case BeginTransfer, EndTransfer
                        default :
                        {
                        } // default
                    } // switch
                }
                else if ( i >= SINGLETON_QUANTITY + studentQuantity ) // vending machines
                {
                    switch ( prevState )
                    {
                        case Starting :
                        {
                            s += i2s( valueStorage[i * 2] );
                            break;
                        } // case Starting
                        case BoughtSoda :
                        {
                            s += i2s( valueStorage[i * 2] ) + "," + i2s( valueStorage[i * 2 + 1] );
                            break;
                        } // case BoughtSoda
                        default :
                        {
                        } // default
                    } // switch
                }
                else if ( i >= SINGLETON_QUANTITY ) // students
                {
                    switch ( prevState )
                    {
                        case SelectingMachine :
                        case BoughtSoda :
                        {
                            s += i2s( valueStorage[i * 2] );
                            break;
                        } // case SelectingMachine, BoughtSoda
                        case Starting :
                        {
                            s += i2s( valueStorage[i * 2] ) + "," + i2s( valueStorage[i * 2 + 1] );
                            break;
                        } // case Starting
                        default :
                        {
                        } // default
                    } // switch
                }
                else // singletons
                {
                    switch ( i )
                    {
                        case Parent :
                        {
                            switch ( prevState )
                            {
                                case Deposit :
                                {
                                    s += i2s( valueStorage[i * 2] ) + "," + i2s( valueStorage[i * 2 + 1] );
                                    break;
                                } // case Deposit
                                default :
                                {
                                } // default
                            } // switch

                            break;
                        } // case Parent
                        case WATCardOffice :
                        {
                            switch ( prevState )
                            {
                                case CardCreation :
                                case EndTransfer :
                                {
                                    s += i2s( valueStorage[i * 2] ) + "," + i2s( valueStorage[i * 2 + 1] );
                                    break;
                                } // case CreationComplete, EndTransfer
                                default :
                                {
                                } // default
                            } // switch

                            break;
                        } // case WATCardOffice
                        case NameServer :
                        {
                            switch ( prevState )
                            {
                                case RegisterMachine :
                                {
                                    s += i2s( valueStorage[i * 2] );
                                    break;
                                } // case RegisterMachine
                                case NewMachine :
                                {
                                    s += i2s( valueStorage[i * 2] ) + "," + i2s( valueStorage[i * 2 + 1] );
                                    break;
                                } // case NewMachine
                                default :
                                {
                                } // default
                            } // switch

                            break;
                        } // case NameServer
                        case Truck :
                        {
                            switch ( prevState )
                            {
                                case Pickup :
                                {
                                    s += i2s( valueStorage[i * 2] );
                                    break;
                                } // case Pickup
                                case BeginDelivery :
                                case CannotFill :
                                case EndDelivery :
                                {
                                    s += i2s( valueStorage[i * 2] ) + "," + i2s( valueStorage[i * 2 + 1] );
                                    break;
                                } // case BeginDelivery, CannotFill, EndDelivery
                                default :
                                {
                                } // default
                            } // switch

                            break;
                        } // case Truck
                        case BottlingPlant :
                        {
                            switch ( prevState )
                            {
                                case GeneratingSoda :
                                {
                                    s += i2s( valueStorage[i * 2] );
                                    break;
                                } // case GeneratingSoda
                                default :
                                {
                                } // default
                            } // switch

                            break;
                        } // case BottlingPlant
                    } // switch
                } // if

                cout << s;
                stateStorage[i] = Finished;
            } // if
        } // for

        cout << endl;
    } // if

    unsigned int storageIndex;

    switch ( kind )
    {
        case Student :
        {
            storageIndex = SINGLETON_QUANTITY + lid;
            break;
        } // case Student
        case Vending :
        {
            storageIndex = SINGLETON_QUANTITY + studentQuantity + lid;
            break;
        } // case Vending
        case Courier :
        {
            storageIndex = SINGLETON_QUANTITY + studentQuantity + machineQuantity + lid;
            break;
        } // case Courier
        default :
        {
            storageIndex = kind;
        } // default
    } // switch

    if ( state == Finished )
    {
        for ( unsigned int i = 0 ; i < totalQuantity ; i += 1 )
        {
            cout << setw( COLUMN_WIDTH ) << left;

            if ( i == storageIndex )
            {
                string s = "";
                s += Finished;
                cout << s;
            }
            else
            {
                cout << "...";
            } // if
        } // for

        cout << endl;
    } // if

    stateStorage[storageIndex] = state;
}

/**
 * @fn          Printer::print
 * @brief       Print state information about specific kind of task
 * @param[in]   kind    Task kind
 * @param[in]   lid     Local ID number
 * @param[in]   state   Task state
 * @param[in]   value1  First value printed with state
 */
void Printer::print( Kind kind , unsigned int lid , States state , unsigned int value1 )
{
    print( kind , lid , state );

    switch ( kind )
    {
        case Student :
        {
            valueStorage[(SINGLETON_QUANTITY + lid) * 2] = value1;
            break;
        } // case Student
        case Vending :
        {
            valueStorage[(SINGLETON_QUANTITY + studentQuantity + lid) * 2] = value1;
            break;
        } //case Vending
        case Courier :
        {
            valueStorage[(SINGLETON_QUANTITY + studentQuantity + machineQuantity + lid) * 2] = value1;
            break;
        } // case Courier
        default :
        {
            valueStorage[kind * 2] = value1;
        } // default
    } // switch
}

/**
 * @fn          Printer::print
 * @brief       Print state information about specific kind of task
 * @param[in]   kind    Task kind
 * @param[in]   lid     Local ID number
 * @param[in]   state   Task state
 * @param[in]   value1  First value printed with state
 * @param[in]   value2  Second value printed with state
 */
void Printer::print( Kind kind , unsigned int lid , States state , unsigned int value1 , unsigned int value2 )
{
    print( kind , lid , state , value1 );

    switch ( kind )
    {
        case Student :
        {
            valueStorage[(SINGLETON_QUANTITY + lid) * 2 + 1] = value2;
            break;
        } // case Student
        case Vending :
        {
            valueStorage[(SINGLETON_QUANTITY + studentQuantity + lid) * 2 + 1] = value2;
            break;
        } //case Vending
        case Courier :
        {
            valueStorage[(SINGLETON_QUANTITY + studentQuantity + machineQuantity + lid) * 2 + 1] = value2;
            break;
        } // case Courier
        default :
        {
            valueStorage[kind * 2 + 1] = value2;
        } // default
    } // switch
}

/**
 * @fn          Printer::i2s
 * @brief       Converts unsigned int to string
 * @param[in]   i   Integer to convert
 */
string Printer::i2s( unsigned int i ) const
{
    stringstream ss;
    ss << i;
    return string( ss.str() );
}

/**
 * @fn          Printer::getState
 * @brief       Gets state for specified Kind and ID
 * @param[in]   kind    Task kind
 * @param[in]   lid     Local ID number
 */
States Printer::getState( Kind kind , unsigned int lid )
{
    switch ( kind )
    {
        case Student :
            return stateStorage[SINGLETON_QUANTITY + lid];
        case Vending :
            return stateStorage[SINGLETON_QUANTITY + studentQuantity + lid];
        case Courier :
            return stateStorage[SINGLETON_QUANTITY + studentQuantity + machineQuantity + lid];
        default :
            return stateStorage[kind];
    } // switch
}

#include <uC++.h>
#include <iostream>
#include <limits>					// numeric_limits
#include "config.h"

using namespace std;

static bool comments( ifstream & in , string & name )
{
    for ( ; ; )
    {
        in >> name;

        if ( in.fail() )
            return true;

        if ( name.substr( 0 , 1 ) != "#" )
            break;

        in.ignore( numeric_limits< int >::max() , '\n' ); // ignore remainder of line
    } // for

    return false;
} // comments

// Process the configuration file to set the parameters of the simulation.
void processConfigFile( const char * configFile , ConfigParms & params )
{
    const unsigned int Parmnum = 9;

    struct
    {
            const char *name;
            bool used;
            unsigned int *value;
    }static parms[Parmnum] = { { "SodaCost" , false , &params.sodaCost } , { "NumStudents" , false , &params.numStudents } , { "MaxPurchases" , false , &params.maxPurchases } , { "NumVendingMachines" , false , &params.numVendingMachines } , { "MaxStockPerFlavour" , false , &params.maxStockPerFlavour } , { "MaxShippedPerFlavour" , false , &params.maxShippedPerFlavour } , { "TimeBetweenShipments" , false , &params.timeBetweenShipments } , { "ParentalDelay" , false , &params.parentalDelay } , { "NumCouriers" , false , &params.numCouriers } , };

    string name;
    unsigned int value , posn;
    unsigned int numOfParm = 0;

    try
    {
        ifstream in( configFile );			// open the configuration file for input

        for ( unsigned int cnt = 0 ; cnt < Parmnum ; cnt += 1 )
        {	// parameter names can appear in any order
            if ( comments( in , name ) )
                break;		// eof ?

            for ( posn = 0; posn < Parmnum && name != parms[posn].name ; posn += 1 )
            {
            } // linear search

            if ( posn == Parmnum )
                break;			// configuration not found ?

            if ( parms[posn].used )
                break;		// duplicate configuration ?

            in >> value;

            if ( in.fail() )
                break;

            in.ignore( numeric_limits< int >::max() , '\n' ); // ignore remainder of line
            numOfParm += 1;
            parms[posn].used = true;
            *parms[posn].value = value;
        } // for

        if ( numOfParm != Parmnum )
        {
            cerr << "Error: file \"" << configFile << "\" is corrupt." << endl;
            exit( EXIT_FAILURE );
        } // if

        // ! eof ?
        if ( !comments( in , name ) )
        {
            cerr << "Error: file \"" << configFile << "\" has extraneous data." << endl;
            exit( EXIT_FAILURE );
        } // if
    }
    catch ( uFile::Failure )
    {
        cerr << "Error: could not open input file \"" << configFile << "\"" << endl;
        exit( EXIT_FAILURE );
    } // try
} // processConfigFile

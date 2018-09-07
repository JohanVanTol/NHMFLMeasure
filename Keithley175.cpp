//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Keithley175.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//////////////////////////////////////////////////////////////////////////
//
//	class K175    A Keithley 175 Multimeter
//				  Only a simple reading command is provided,
//				  ReadVoltage(), but this may be used also for
//				  current or resistance measurements
//

// The default constructor. //only the IEEEdevice constructor is called
K175::K175(int _addr)
			: IEEEdevice(_addr)
{}

// ReadVoltage()
//  reads the currently read value, in the current range
//  This is not a timed measurement so beware..
//
//  Also for testing purposes this routine returns a default value
//  of around 8.7 if the Multimeters address is zero.
double K175::ReadVoltage()
{
	// Read the value, but if the address is 0 returns a random double
	if (address != 0)
        status = GPIBReceive(address,response, 32);
		else return 8.7 + 0.001 * double(rand())/RAND_MAX;

	if (status != 0)     // On error
	{
		ErrorCode = 1;
		sprintf(ErrorMessage,"Failure reading K175 at address %d", address);
	}
	return atof(response+4);  // The first four characters of the response
									  // are letters indicating the mode
}

//
int K175::Test()
{
	if (address == 0) return 0;  //Do nothing and leave function if address = 0

    status = GPIBReceive(address,response, 32);
       //read device

	if (status != 0)     										// On error
	{
		ErrorCode = status;
		sprintf(ErrorMessage,"IEEE error %d :Failure reading K175 at address %d",
											status, address);
		return ErrorCode;
	}

	return 0;														// All ok !
}


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

void WaitMillisecs(int ms);   
#include "HP34401A.h"
HP34401A::HP34401A(int _addr)
    : IEEEdevice(_addr)  // Constructor
{

}

int HP34401A::Test()
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    sprintf(command,"*IDN?");     // ask ID string
   	status = GPIBSendCommand(address, command);
    if (status == 0)
   	{
		WaitBeforeRead();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status == 0)
		  return strncmp(response,"HEWLETT-PACKARD,34401A",22);
	}
	ErrorCode = status;
	strcpy(ErrorMessage,"IEEE error in sending to HP");
	return -1;
}

double HP34401A::ReadValue()
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	sprintf(command,"READ?");     // ask ID string
	status = GPIBSendCommand(address, command);
	if (status == 0)
	{
 //		WaitBeforeRead();
		WaitMillisecs(2);
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status == 0)
          return atof(response);
    }
	ErrorCode = status;
    strcpy(ErrorMessage,"IEEE error in sending to HP");
    return -999.0;

}

double HP34401A::Read()
{
	return ReadValue();
}

double HP34401A::ReadVoltage()
{
	return ReadValue();
}

//---------------------------------------------------------------------------
#pragma package(smart_init)

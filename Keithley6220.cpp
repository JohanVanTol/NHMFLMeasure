//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "Keithley6220.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

K6220::K6220(int _addr)
			: IEEEdevice(_addr)
{
	MeasCurrent = 1e-6;
	ComplianceVoltage = 0.1;
}

int K6220::Test()
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	sprintf(command,"*IDN?");     // ask ID string
	status = GPIBSendCommand(address, command);
	if (status == 0)
	{
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status == 0)
		  return strncmp(response,"KEITHLEY INSTRUMENTS INC.,MODEL 622",34);
	}
	ErrorCode = status;
	strcpy(ErrorMessage,"IEEE error in sending to Keithley 6220");
	return -1;
}

int K6220::SetCurrent(double _curr)
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	sprintf(command,"SOURce:CURRent %e", _curr);     // ask ID string
	status = GPIBSendCommand(address, command);
//	ErrorMessage = command;
	if (status == 0)
		return status;
	  else
	  {
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to Keithley 6220");
		return -1;
	  }
}

double K6220::ReadCurrent()
{
	if (address == 0) return 0.0;
	sprintf(command,"SOURce:CURR?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to Keithley 6220");
		return -1.0;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
		  else
		  {
			  return atof(response);
		  }
	  }
}


int K6220::SetCompliance(double _comp)
{
	if (address == 0 ) return -1;
	if (_comp < 0.1) _comp = 0.1;
	if (_comp > 10 ) _comp = 10;    // actual maximum is 105 V but we'll keep it at 10 V
	sprintf(command,"SOURce:CURR:COMP %e", _comp);
	status = GPIBSendCommand(address, command);
	if (status == 0)
		return status;
	  else
	  {
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to Keithley 6220");
		return -1;
	  }

}


double K6220::ReadCompliance()
{
	if (address == 0) return 0.0;
	sprintf(command,"SOURce:CURR:COMP?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to Current source");
		return -1.0;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
		  else
		  {
			  return atof(response);
		  }
	  }
}

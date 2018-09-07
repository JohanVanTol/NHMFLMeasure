//---------------------------------------------------------------------------

#include <vcl.h>
#include "SMB100A.h"
#pragma hdrstop



//---------------------------------------------------------------------------

#pragma package(smart_init)


//////////////////////////////////////////////////////////////////////////
//
//	class SMB100A    Rohde & Schwartz 2 GHz signal generator
//
//
//  The status byte is of the following format:
//  Only the bits 4 to 6 are used (of 0..7)
//  Bit 4 MAV  Message available  (value 16 ..)
//
//  The answers from the SMB100A contain HEADERS if they are not
//  switched off by a HEADERS:OFF command. So I will test every answer
//  whether it contains a header or not before reading it

SMB100A::SMB100A(int addr)
		: IEEEdevice(addr)
{}

//  Test sets the device in remote and asks for the ID
//  If results OK returns 0 otherwise returns current bus status
int SMB100A::Test()
{
	if (address == 0) return 0;
    SetRemote(address);

	sprintf(command,"*IDN?");
    status = GPIBSendCommand(address,command);
	ErrorCode = status;
	if (status != 0)
	{
		sprintf(ErrorMessage,"IEEE error talking to SMB100A at address %d", address);
	}

    status = GPIBReceive(address, response, MaxResponse -1);
 	if (status != 0)
	{
		sprintf(ErrorMessage,"IEEE error listening to SMB100A at address %d", address);
        ErrorCode = status;
		return ErrorCode;
	}
        // now test for correct response:  ROHDE&SCHWARZ,SMB100A,826045/013,2.02

	if (strncmp(response,"Rohde&Schwarz,SMB100A",19) !=0)
	{
		sprintf(ErrorMessage,"Incorrect ID for SMB100A at address %d", address);
        ErrorCode = 200;
	}

	return ErrorCode;
}

int SMB100A::SetFreq(double MHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"SOUR:FREQ %f MHZ", MHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMB100A::SetStartFreq(double MHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"SOUR:FREQ:START %f MHz", MHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMB100A::SetStopFreq(double MHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"SOUR:FREQ:STOP %fMHz", MHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMB100A::SetAFfreq(double kHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"SOUR:LFO %f kHz", kHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMB100A::SetStep(double kHz)
{
	if (address == 0) return 0;
	sprintf(command,"SWE:RES");
	status = GPIBSendCommand(address, command);
	sprintf(command,"SOUR:SWE:FREQ:MODE MAN");
	status = GPIBSendCommand(address, command);
	sprintf(command,"SOUR:SWE:STEP:LIN %f kHz", kHz);
	status = GPIBSendCommand(address, command);
	return status;
}
/*
int SMB100A::Step(double MHz)
{
	if (address == 0) return 0;
	sprintf(command,"FREQ:MAN %f MHz", MHz);
	status = GPIBSendCommand(address, command);
	return status;
}
*/

int SMB100A::Step()
{
	if (address == 0) return 0;
	sprintf(command,"FREQ:MAN UP");
	status = GPIBSendCommand(address, command);
	return status;
}

int SMB100A::SetPower(double P)
{
	if (address == 0) return 0;
	sprintf(command,":POW %f",P);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting Power of SMB100A");
	}
	return ErrorCode;
}

int SMB100A::SetAM(int on,double Mod)
{
	if (address == 0) return 0;
	sprintf(command,"AM:DEPTh:LIN %5.1f", Mod);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
			strcpy(ErrorMessage,"IEEE error setting AM Depth of SMB100A");
			return ErrorCode = status;
	}

	switch (on)
	{
		case 1:
			sprintf(command,"AM:SOUR INT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "AM:STAT ON");
			break;
		case 2:
			sprintf(command,"AM:SOUR EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"AM:EXT:COUP AC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "AM:STAT ON");
			break;
		case 3:
			sprintf(command,"AM:SOUR EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"AM:EXT:COUP DC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "AM:STAT ON");
			break;
		case 4:
			sprintf(command,"AM:SOUR INT,EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"AM:EXT:COUP AC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "AM:STAT ON");
			break;
		case 5:
			sprintf(command,"AM:SOUR INT,EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"AM:EXT:COUP DC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "AM:STAT ON");
			break;
		default: sprintf(command,"AM:STAT OFF");break;
	}
	status = GPIBSendCommand(address, command);

	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting AM state of SMB100A");
	}
	return ErrorCode;
}

int SMB100A::SetFM(int on,double Mod)
{
	if (address == 0) return 0;
	sprintf(command,"FM:DEV %5.1f", Mod);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
			strcpy(ErrorMessage,"IEEE error setting AM Depth of SMB100A");
			return ErrorCode = status;
	}

	switch (on)
	{
		case 1:
			sprintf(command,"FM:SOUR INT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "FM:STAT ON");
			break;
		case 2:
			sprintf(command,"FM:SOUR EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"FM:EXT:COUP AC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "FM:STAT ON");
			break;
		case 3:
			sprintf(command,"FM:SOUR EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"FM:EXT:COUP DC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "FM:STAT ON");
			break;
		case 4:
			sprintf(command,"FM:SOUR INT,EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"FM:EXT:COUP AC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "FM:STAT ON");
			break;
		case 5:
			sprintf(command,"FM:SOUR INT,EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"FM:EXT:COUP DC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "FM:STAT ON");
			break;
		default: sprintf(command,"FM:STAT OFF");break;
	}
	status = GPIBSendCommand(address, command);

	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting AM state of SMB100A");
	}
	return ErrorCode;
}

int SMB100A::SetPM(int on,double Mod)          //Set phase modulation
{
	if (address == 0) return 0;
	sprintf(command,"PM:DEV %6.2f", Mod);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
			strcpy(ErrorMessage,"IEEE error setting AM Depth of SMB100A");
			return ErrorCode = status;
	}

	switch (on)
	{
		case 1:
			sprintf(command,"PM:SOUR INT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "PM:STAT ON");
			break;
		case 2:
			sprintf(command,"PM:SOUR EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"PM:EXT:COUP AC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "PM:STAT ON");
			break;
		case 3:
			sprintf(command,"PM:SOUR EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"PM:EXT:COUP DC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "PM:STAT ON");
			break;
		case 4:
			sprintf(command,"PM:SOUR INT,EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"PM:EXT:COUP AC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "PM:STAT ON");
			break;
		case 5:
			sprintf(command,"PM:SOUR INT,EXT");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command,"PM:EXT:COUP DC");
			status = GPIBSendCommand(address, command);
			if (status != 0)
			{
				strcpy(ErrorMessage,"IEEE error setting AM source of SMB100A");
				return ErrorCode = status;
			}
			sprintf(command, "PM:STAT ON");
			break;
		default: sprintf(command,"PM:STAT OFF");break;
	}
	status = GPIBSendCommand(address, command);

	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting AM state of SMB100A");
	}
	return ErrorCode;
}

int SMB100A::RFon()
{
	if (address == 0) return 0;
	status = GPIBSendCommand(address, "OUTPut:STATe ON");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error turning rf on ");
	}
	return ErrorCode;
}

int SMB100A::RFoff()
{
	if (address == 0) return 0;
	status = GPIBSendCommand(address, "OUTP:STAT OFF");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error turning rf on ");
	}
	return ErrorCode;
}

double SMB100A::GetFreq()    // returns the frequency in MHz
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "SOURCE:FREQ?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
 //			if (strncmp(response,"RF ",2)==0)
//			   return atof(response+3)/1e6;
			 return atof(response)/1e6;     // in MHz
      }
}

double SMB100A::GetPower()    // returns the power level in dBm
{
    if (address == 0) return -200.0;
	status = GPIBSendCommand(address, ":POWER?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
        return -200.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -200.0;
          else
 //           if (strncmp(response,"LEVEL:OFF",9)==0) return -150.0;
 //             else if (strncmp(response,"LEVEL",5)==0)
 //                      return atof(response+5);
			return atof(response);     // in dBm
      }
}

double SMB100A::GetAFfreq()    // returns the Auxiliry Frequency in kHz
{
	if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "LFO:FREQ?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1.0;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
 //		if (strncmp(response,"AF:OFF",6)==0) return 0.0;
		if (status != 0) return -1.0;
 //		  else if (strncmp(response,"LFO:FREQ",8)==0)
 //			return atof(response+8)/1000.0;
			 else return atof(response)/1000.0;     // in kHz
	  }
}

double SMB100A::GetStartFreq()    // returns the STart frequency in MHz
{
	if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "SOURCE:FREQ:START?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1.0;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
//		  else if (strncmp(response,"SOUR:FREQ:START",15)==0)
//			return atof(response+15)/1e6;
			 else return atof(response)/1e6;     // in MHz
	  }
}

double SMB100A::GetStopFreq()    // returns the END frequency in MHz
{
	if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "SOUR:FREQ:STOP?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1.0;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
//		  else if (strncmp(response,"RF:STOP",7)==0)
//			return atof(response+7)/1e6;
			 else return atof(response)/1e6;     // in MHz
	  }
}

double SMB100A::GetStep()    // returns the frequency in kHz
{
	if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "SOUR:SWE:STEP?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1.0;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
		 else return atof(response)/1e3;     // in kHz
	  }
}

int SMB100A::GetStepTime()    // returns the dwell time
{
	if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "SWE:POW:DWEL?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1.0;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		 else return int(atof(response));     // in seconds
	  }
}

int SMB100A::GetSweepType()    // returns the frequency in MHz
{
	if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "SWE:FREQ:MODE?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		  else if (strncmp(response,"MAN",3)==0) return 0;
			 else if (strncmp(response,"AUTO",4)==0) return 1;
				 else return 2;
	  }
}

int SMB100A::GetAM(double* mod)
{
	if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "AM:STAT?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);

		if (status != 0) return -1;

		if (response[0] == '0' ) return 0;    //No AM modulation

		status = GPIBSendCommand(address, "AM:DEPT:LIN?");
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
			return -1;
		}
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
			else *mod = atof(response)/1000;

		status = GPIBSendCommand(address, "AM:SOUR?");
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
			return -1;
		}
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		int returnValue = 0;
		if (strncmp(response,"INT",3)==0) returnValue = 1;
		if (strncmp(response,"EXT",3)==0) returnValue = 2;
		if (strncmp(response,"INT,EXT",7)==0) returnValue = 4;

		if (returnValue > 1)
		{
			status = GPIBSendCommand(address, "AM:EXT:COUP?");
			if (status != 0)
			{
				ErrorCode = status;
				strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
				return -1;
			}
			WaitUntilMessageAvailable();
			status = GPIBReceive(address, response, MaxResponse-1);
			if (status != 0) return -1;
			if (strncmp(response,"DC",2) == 0) returnValue += 1;
		}
		return returnValue;
	  }

//	return 0;
}

int SMB100A::GetFM(double* mod)
{
	if (address == 0) return -1;
	status = GPIBSendCommand(address, "FM:STAT?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);

		if (status != 0) return -1;

		if (response[0] == '0' ) return 0;     // No FM modulation

		status = GPIBSendCommand(address, "FM:DEV?");
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
			return -1;
		}
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
			else *mod = atof(response)/1000;

		status = GPIBSendCommand(address, "FM:SOUR?");
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
			return -1;
		}
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		int returnValue = 0;
		if (strncmp(response,"INT",3)==0) returnValue = 1;
		if (strncmp(response,"EXT",3)==0) returnValue = 2;
		if (strncmp(response,"INT,EXT",7)==0) returnValue = 4;

		if (returnValue > 1)
		{
			status = GPIBSendCommand(address, "FM:EXT:COUP?");
			if (status != 0)
			{
				ErrorCode = status;
				strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
				return -1;
			}
			WaitUntilMessageAvailable();
			status = GPIBReceive(address, response, MaxResponse-1);
			if (status != 0) return -1;
			if (strncmp(response,"DC",2) == 0) returnValue += 1;
		}
		return returnValue;
	  }

//	return 0;
}

int SMB100A::GetPM(double* mod)
{
	if (address == 0) return -1;
	status = GPIBSendCommand(address, "PM:STAT?");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
		return -1;
	}
	  else
	  {
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);

		if (status != 0) return -1;

		if (response[0] == '0' ) return 0;   // No PM modulation

		status = GPIBSendCommand(address, "PM:DEV?");
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
			return -1;
		}
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
			else *mod = atof(response)/1000;


		status = GPIBSendCommand(address, "PM:SOUR?");
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
			return -1;
		}
		WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		int returnValue = 0;
		if (strncmp(response,"INT",3)==0) returnValue = 1;
		if (strncmp(response,"EXT",3)==0) returnValue = 2;
		if (strncmp(response,"INT,EXT",7)==0) returnValue = 4;

		if (returnValue > 1)
		{
			status = GPIBSendCommand(address, "PM:EXT:COUP?");
			if (status != 0)
			{
				ErrorCode = status;
				strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
				return -1;
			}
			WaitUntilMessageAvailable();
			status = GPIBReceive(address, response, MaxResponse-1);
			if (status != 0) return -1;
			if (strncmp(response,"DC",2) == 0) returnValue += 1;
		}
		return returnValue;
	  }

//	return 0;
}

int SMB100A::IsError(char* message, int maxlength)
{
    if (address == 0) return -1;
	status = GPIBSendCommand(address, "SYSTEM:ERR?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMB100A");
        return -1;
	}
      else
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
           else strncpy(message, response, maxlength);
		if (strncmp(response,"0,",2) == 0) return 0;
		  else return 1;
}

int SMB100A::SetSweepType(int type)
{
	if (address == 0) return 0;
	switch (type)
    {
		case 1: sprintf(command,"SOUR:FREQ:MODE SWE");break;
		case 2: sprintf(command,"SOUR:FREQ:SWE:EXEC");break;
		default: sprintf(command,"SOUR:FREQ:MODE CW");break;
    }
	status = GPIBSendCommand(address, command);
	return status;
}

int SMB100A::SetStepTime(int ms)
{
	if (address == 0) return 0;
	sprintf(command,"SWE:DWEL %d ms", ms);      // range 2 ms -100 s
	status = GPIBSendCommand(address, command);
	return status;
}

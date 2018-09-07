//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#pragma hdrstop
#include <stdio.h>
#include <math.h>
#include "RS232Dev.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#include "wsc.h"
void WaitMillisecs(int ms);              // Defined in RS232Test.cpp

int GetChar(int Port, int MilliSecs);    // Defined in RS232Test.cpp
										// A version of SioGetc that includes
										// waiting for a character to be
										// available

RS232Device::RS232Device(int _Port)
	: Port(_Port)
{
	MaxLength = 256;
	command = new char[MaxLength];
	response = new char[MaxLength];
	ErrorString = new char[MaxLength];
	Error = 0;
	InitPort(Port);
}

RS232Device::~RS232Device()
{
	delete[] command;
	delete[] response;
	delete[] ErrorString;
}

int RS232Device::Reset(int _Port, int _ML)
{
	Port = _Port;
	MaxLength = _ML;
	command = new char[MaxLength];
	response = new char[MaxLength];
	return 0;
}

int RS232Device::InitPort(int TrBuf, int RcBuf)
{
	if (SioReset(Port, TrBuf, RcBuf) < 0) return -1;
	if (SetBaud(Baud9600) < 0) return -1;
	return SioRTS(Port,'S');
}

int RS232Device::SetBaud(int Baud)
{
	return SioBaud(Port, Baud);
}

int RS232Device::FreePort()
{
	return SioDone(Port);
}

int RS232Device::Send(int delay)
{
	SioTxClear(Port);
	SioRxClear(Port);
	SioRTS(Port,'S');
	for (int i=0;i<strlen(command);i++)
	{
		if (SioPutc(Port, command[i]) < 0) return -1;
		WaitMillisecs(delay);
	}
	return 0;
}

int RS232Device::Read(int delay)
{
	int j=0;
	char c;
	while (( (c = GetChar(Port,delay)) != WSC_NO_DATA) && (c != 13))
	{
	   response[j++]  = c;
	}
	response[j] = 0;
	return 0;
}

/////////////////////////////////////////////////
//
//  Virtual function for the RS232Device class.
//  each device should have its own test
//
int RS232Device::Test()
{
	return 0;
}
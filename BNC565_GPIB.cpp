//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop
#include <stdio.h>
#include "BNC565_GPIB.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

BNC565::BNC565(int _addr)
	: IEEEdevice(_addr)
{

}

BNC565::Test()
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	sprintf(command,"*IDN?");     // ask ID string
	status = GPIBSendCommand(address, command);
	if (status == 0)
	{
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status == 0)
		  return strncmp(response,"565-8",5);
	}
	ErrorCode = status;
	strcpy(ErrorMessage,"IEEE error in sending to BNC565 pulser");
	return -1;

}

int BNC565::Set(int ch, int del, int width)
{
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	double w = (double)width * 0.000000001;
	sprintf(command,":PULSE%1d:WIDTH %-10.8f",ch, w);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
//	stat = Read(100);

	double d = (double)del * 0.000000001;
	sprintf(command,":PULSE%1d:DEL %-12.9f",ch, d);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
//	stat = Read(100);
	return status;
}

int BNC565::SetRate(int rate)
{
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	double r = (double)rate * 0.000000001;
	sprintf(command,":RATE %-10.7f", r);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
	return status;

}

int BNC565::SetPeriod(int SRT)
{
// The Period is the SRT in microseconds
// The device needs the number in seconds
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	double r = (double)SRT * 0.000001;            // microsec to seconds
	sprintf(command,":PULSE0:PER %-10.6f", r);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
	return status;
}

int BNC565::SetDelay(int ch, int del)
{
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	double d = (double)del * 0.000000001;
	sprintf(command,":PULSE%1d:DEL %-12.9f",ch, d);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
	return status;
}

int BNC565::SetDelay(int ch, long long del)
{
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	double d = (double)del * 0.000000001;
	sprintf(command,":PULSE%1d:DEL %-12.9f",ch, d);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
	return status;
}

int BNC565::SetWidth(int ch, int wid)
{
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	double w = (double)wid * 0.000000001;
	sprintf(command,":PULSE%1d:WIDTH %-12.10f",ch, w);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
	return status;
}

int BNC565::SetWidth(int ch, long long wid)
{
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	double w = (double)wid * 0.000000001;
	sprintf(command,":PULSE%1d:WIDTH %-12.10f",ch, w);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
	return status;
}

int BNC565::Enable(int ch)
{
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	sprintf(command,":PULSE%1d:STATE ON",ch);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
	return status;
}

int BNC565::Disable(int ch)
{
	if (address == 0) return 0;
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	sprintf(command,":PULSE%1d:STATE OFF",ch);
	strcat(command, endch);
	status = GPIBSendCommand(address, command);
	return status;
}

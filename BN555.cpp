//---------------------------------------------------------------------------


#pragma hdrstop
#include <stdio.h>
#include <string.h>
#include "BN555.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
BN555::BN555(int _port)
	: RS232Device(_port)
{}

int BN555::Set(int ch, int del, int width)
{
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	int stat = 0;
	double w = (double)width * 0.00000001  ;
	sprintf(command,":PULSE%1d:WIDTH %-10.8f",ch, w);
	strcat(command, endch);
	stat = Send(10);
//	stat = Read(100);

	double d = (double)del * 0.00000001;
	sprintf(command,":PULSE%1d:DEL %-10.8f",ch, d);
	strcat(command, endch);
	stat =Send(10);
//	stat = Read(100);
	return stat;
}

int BN555::SetRate(int rate)
{
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	double r = (double)rate * 0.00000001;
	sprintf(command,":RATE %-10.7f", r);
	strcat(command, endch);
	int stat =Send();
	return stat;

}

int BN555::SetDelay(int ch, int del)
{
	char endch[3];
	endch[0] = 13;
	endch[1] = 10;
	endch[2] = 0;
	int stat = 0;
	double d = (double)del * 0.00000001;
	sprintf(command,":PULSE%1d:DEL %-10.8f",ch, d);
	strcat(command, endch);
	stat =Send(5);
//	stat = Read(100);
	return stat;
}

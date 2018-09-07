//---------------------------------------------------------------------------

#ifndef SMB100AH
#define SMB100AH
#include "GPIB_NatlInstrum.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class SMB100A : public IEEEdevice   //Rohde & Schwartz Signal generator
{
	public:
		SMB100A(int addr);
		int SetFreq(double MHz);
		int SetStep(double kHz);
		int SetPower(double P);
		int SetAFfreq(double kHz);
		int SetSweepType(int type);
		int SetStartFreq(double MHz);
		int SetStopFreq(double MHz);
		int SetStepTime(int ms);
		int SetFM(int on,double Mod);
		int SetAM(int on,double Mod);
		int SetPM(int on,double Mod);
//		           int Step(double MHz);
		int Step();
		int RFon();
		int RFoff();
		int Test();
		double GetFreq();
		double GetPower();
		double GetAFfreq();
		double GetStartFreq();
		double GetStopFreq();
		double GetStep();
		int GetStepTime();
		int GetSweepType();
		int GetAM(double*);
		int GetFM(double*);
		int GetPM(double*);
		int IsError(char* message, int maxlength);
};

//---------------------------------------------------------------------------
#endif

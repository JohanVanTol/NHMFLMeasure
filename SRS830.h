//---------------------------------------------------------------------------

#ifndef SRS830H
#define SRS830H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "GPIB_NatlInstrum.h"
//---------------------------------------------------------------------------
class SRS830 : public IEEEdevice
{
	public:
		SRS830(int _addr);
		int SetRemote(int remote=1);
        double ReadOutput(int channel);
        int ReadXY(double *x, double *y);
		double GetSignalPhase();
		int SetDAC(int mV);
        int SetDAC(int n, int mV);
		double ReadADC(int channel);

        double SetModulationFreq(double f);
        int SetModulationPhase(double ph);
        double SetModulationAmp(double amp);

		int GetSensitivity(char* sensitivity);
		int GetTimeConstant(char* tc);
		double GetRefFrequency();
		double GetPhase();
        double GetRefAmp();
        
		double MeasFilterFrequency();
		void WriteAllParameters(char* LockinPar) const;
		int UpdateParameters();
		int Test();
    	int MeasSensitivity();
        double RealSensitivity();
        int SetSensitivity(int sens);
		int MeasTimeConstant();
		int MeasTimeConstant(double *tc);
		int MeasSlope();
		int MeasDynRes();
		int MeasFilterMode();
        int GetFilters(char* line, int MaxChar);

	private:
		double Phase;                  //Phase in degrees
		int Sensitivity;               //Sensitivity code 0:10nV,...,15:3V
		int TimeConstant;              //Time constant code 0:1ms,...,13:3ks
		double ReferenceFrequency;     //Reference Frequency in Hz
		int Slope;                     //OutputFilter. 0:6dB/oct, 1:12dB/oct
		int DynamicReserve;            //0:HI STAB 20dB 1:NORM 40dB 2:HI RES 60dB
		double FilterFrequency;			 //Input FilterFrequency in Hz
		int FilterMode;					 //0:FLAT, 1:NOTCH, 2:LOW-PASS, 3:BANDPASS

		void AsciiTimeConstant(int TcCode, char* tc) const;
		void AsciiSensitivity(int SensCode, char* sens) const;
};
#endif

//---------------------------------------------------------------------------

#ifndef Keithley6220H
#define Keithley6220H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "GPIB_NatlInstrum.h"
//---------------------------------------------------------------------------
class K6220 : public IEEEdevice
{
	public:
		K6220(int _addr);
		int SetCurrent(double _curr);
		double ReadCurrent();
		int SetCompliance(double _comp);
		double ReadCompliance();
//		double Read();
		int Test();
		void SetDefaultCurr(double _curr) {MeasCurrent = _curr;}
		double GetDefaultCurr() {return MeasCurrent;}
		void SetDefaultComp(double _compV) {ComplianceVoltage = _compV;}
		double GetDefaultComp() {return ComplianceVoltage;}

	private:
		double MeasCurrent;
		double ComplianceVoltage;
};
#endif

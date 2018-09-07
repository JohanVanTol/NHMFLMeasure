#ifndef Keithley197H
#define Keithley197H

#include "GPIB_NatlInstrum.h"
//---------------------------------------------------------------------------
class K197 : public IEEEdevice
{
	public:
		K175(int _addr);
		double ReadVoltage();
		int Test();
};
#endif

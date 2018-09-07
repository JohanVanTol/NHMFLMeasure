//---------------------------------------------------------------------------
#ifndef Keithley175H
#define Keithley175H

#include "GPIB_NatlInstrum.h"
//---------------------------------------------------------------------------
class K175 : public IEEEdevice
{
	public:
		K175(int _addr);
		double ReadVoltage();
		int Test();
};
#endif
 
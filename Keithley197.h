//---------------------------------------------------------------------------
#ifndef Keithley197H
#define Keithley197H

#include "GPIB_NatlInstrum.h"
//---------------------------------------------------------------------------
class K197 : public IEEEdevice
{
	public:
		K197(int _addr);
		double Read();
		int Test();
};
#endif
 
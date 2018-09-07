//---------------------------------------------------------------------------

#ifndef BNC565_GPIBH
#define BNC565_GPIBH
//---------------------------------------------------------------------------
#include "GPIB_NatlInstrum.h"

class BNC565 : public IEEEdevice
{
    public:
		BNC565(int _addr);  // Constructor
		int Test();
		int Set(int ch, int del, int wid);
		int SetDelay(int ch, int del);
		int SetDelay(int ch, long long del);
		int SetWidth(int ch, int wid);
		int SetWidth(int ch, long long wid);
		int SetPeriod(int SRT);
		int SetRate(int rate);
		int Enable(int ch);
		int Disable(int ch);
//		int Trigger();
//		int TriggerMode(int mode);
//		int Stop();
//		int Clear(int trace);
};

#endif

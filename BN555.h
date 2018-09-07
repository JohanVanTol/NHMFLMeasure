//---------------------------------------------------------------------------

#ifndef BN555H
#define BN555H
//---------------------------------------------------------------------------
#include "GPIB_NatlInstrum.h"
#include "RS232Dev.h"

class BN555 : public RS232Device
{
	public:
		BN555(int _port);
		int Set(int ch, int del, int wid);
		int SetDelay(int ch, int del);
		int SetRate(int rate);
};




#endif

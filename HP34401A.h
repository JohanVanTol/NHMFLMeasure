//---------------------------------------------------------------------------
#ifndef HP34401AH
#define HP34401AH
//---------------------------------------------------------------------------
#include "mydata.h"
#include "GPIB_NatlInstrum.h"

class HP34401A : public IEEEdevice
{
    public:
        HP34401A(int _addr);  // Constructor
        int Test();
		double ReadValue();
		double Read();
		double ReadVoltage();
};
//---------------------------------------------------------------------------
#endif
 
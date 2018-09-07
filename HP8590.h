//---------------------------------------------------------------------------

#ifndef HP8590H
#define HP8590H
//---------------------------------------------------------------------------
#include "mydata.h"
#include "GPIB_NatlInstrum.h"

class HP8590 : public IEEEdevice
{
    public:
        HP8590(int _addr);  // Constructor
        int GetData(int channel, DataArray *NewData);
        int Simulate(int npts, DataArray *Data);
        double ResBW();     // returns Resolution bandwidth in kHz
        double VideoBW();   // returns video bandwidth in kHz
        int Test();
        int Trigger();
        int TriggerMode(int mode);
        int Stop();
        int Clear(int trace);
        int SetAver();
        int Local();
        int Remote();
};


#endif

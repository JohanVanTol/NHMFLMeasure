//---------------------------------------------------------------------------
#ifndef HP54845AH
#define HP54845AH
//---------------------------------------------------------------------------
#include "mydata.h"
#include "GPIB_NatlInstrum.h"

class HP54845A : public IEEEdevice
{
    public:
        HP54845A(int _addr);  // Constructor
        ~HP54845A();          // Destructor
        int GetData(int channel, DataArray *NewData);
        int GetData(int channel, int channel2, DataArray *NewData);
        int Simulate(int npts, DataArray *Data);
        int Test();
        int Run();
        int Stop();
        int Clear();
        int GetAverage();
        int SetAverage(int nAver);
        int GetCount();
        int GatedValues(int nVal, DataPoint *P, double *T1, double *T2, int chan=1);
        int RetrieveBuffer(int chan);
        int GetGateValues(int nVal, DataPoint *P, double *T1, double *T2, int chan=1);
        int RetrieveOffsets();
        int RetrieveIncrements();
        int RetrieveScaleSettings();
		int SetDefaultFormat();
		int SetDelay(double _delay);

    private:
        char **databuff;
        int *ndat;
        double *XO;
        double *YO;
        double *XI;
        double *YI;
};
#endif
 
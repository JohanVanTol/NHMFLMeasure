//---------------------------------------------------------------------------
#ifndef TekScopeH
#define TekScopeH
//---------------------------------------------------------------------------
#include "mydata.h"
#include "GPIB_NatlInstrum.h"

class Tek7000 : public IEEEdevice
{
	public:
		Tek7000(int _addr);  // Constructor
		~Tek7000();          // Destructor
		int GetData(int channel, DataArray *NewData);
		int GetData(int channel, int channel2, DataArray *NewData);
		int Simulate(int npts, DataArray *Data);
		int Test();
		int Run();
		int Stop();
		int Clear();
		int GetAverage();
		int SetAverage(int nAver);
		int SetMathAverage(int ch, int nAver);		
		int GetCount();
		int GatedValues(int nVal, DataPoint *P, double *T1, double *T2, int chan=1);
		int RetrieveBuffer(int chan);
		int GetGateValues(int nVal, DataPoint *P, double *T1, double *T2, int chan=1);
		int RetrieveOffsets(int mmode = 0);
		int RetrieveIncrements();
		int RetrieveScaleSettings(int mmode = 0);
		int SetDefaultFormat();
		int SetDelay(double _delay);
		int Header(int on);

	private:
		char **databuff;
		int *ndat;
		double *XO;
		double *YO;
		double *XI;
		double *YI;
};
#endif

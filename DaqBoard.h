//---------------------------------------------------------------------------

#ifndef DaqBoardH
#define DaqBoardH
#include <StdCtrls.hpp>

#include "DaqX.h"
#include "Daqroutines.h"
//---------------------------------------------------------------------------
// The IOTechDaqBoard, depemding on the version, has the following
// inputs and outputs
// nADC ADC channels (16 for DaqBoard 500, DaqBoard 1000)
// nDig Digital I/O channels (24 for DaqBoard 500, DaqBoard 1000)
// nDaC DAC outputs (2)
// nCtr Counter input channels (1 for DaqBoard 500, 4 for DaqBoard 1000)
// nTmr Pulse output channels ( 2 )

// Implementation of the ADC channels is given in the ADC class

class ADC
{
    private:
        double MaxVolt;
        double scale;
        double offset;
        DaqAdcGain gain;
        DWORD flag;

        void ReCalcScale();

    public:
        ADC();
        ~ADC();
        int SetGain(DaqAdcGain _Gn);
        void SetFlag(DWORD _fl) {flag = _fl;}
        DWORD GetFlag() {return flag;}
        DaqAdcGain GetGain() {return gain;}
        double GetValue(WORD reading);
};


class IOTechDaqBoard
{
    public:
        IOTechDaqBoard();
        ~IOTechDaqBoard();

        int Connect();
        int IsOpen();
        int Close();

        double SetScale(int ch, int fraction_of_FS);
        double AdcRead(int ch, int nsample);
        int StandardInit(int _nch=4, int nsamp = 100);
        int Trigger();
        int Read(double* results);
        int SetGain(int ch_index, DaqAdcGain gn);
        DaqAdcGain GetGain(int ch_index);
        DWORD GetFlag(int ch_index);

        int ResetTransBuf(int nfloats);
        double ReadChannel(int ch);           // simple read routine  of channel ch
        double ReadChannel(int ch, int aver); // return average of aver readings of one channel
        double* ReadChannels(int first, int last); // returns array of doubles representing a series of channels
        double* ReadChannels(int first, int last, int aver);//returns average of n readings of one channel
        int SetName(char* ch);
        int ChannelInit(int _nch, int *chan);
        ADC operator[](i) {return ch[i];}
        int SetAcquisition(double f, int nscans);
        int GetValues(int nch, double* val);
        int WaitToComplete(int milli=25);
        int GetnActiveChannels() const {return nchan;}
        int Arm();
        int DisArm();
		int SetIO(char A, char B, char C);
		int InitDAC(int i=0);
		int SetDAC(int ch, int value);

    private:
        DWORD *flags;
        DaqAdcGain *gains;
        DaqHandleT  handle;
        int nchan;
        int nsample;
        int nADC;               // Total number of DAC ports
        ADC *ch;
        DWORD *channels;          // array of channels to read
        double adcfreq;
        char DaqName[30];
        float MaxVolt;
        WORD *transbuf;      // 16 bit constant (here unsigned short)

};


#endif

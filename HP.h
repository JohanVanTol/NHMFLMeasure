//---------------------------------------------------------------------------
#ifndef HPH
#define HPH
//---------------------------------------------------------------------------
class HP54845A : public IEEEdevice
{
    public:
        HP54845A(int _addr);  // Constructor
        int GetData(int channel, DataArray *NewData);
        int Simulate(int npts, DataArray *Data);
        int Test();
};
#endif
 
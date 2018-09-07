//---------------------------------------------------------------------------
#ifndef IsoBusDevicesH
#define IsoBusDevicesH

class IsoBusDevice
{
    protected:
        int Port;
        int Address;
        char* command;
        char* response;
        char* ErrorString;
        int Error;
        int MaxLength;
    public:
        IsoBusDevice(int _Port, int _isobus);
        ~IsoBusDevice();
        int Reset(int _Port, int _isobus, int _ML = 256);
        int SetBaud(int _Baud=9600);
        int GetPort() {return Port;}
        int GetAddress() {return Address;}
        int InitPort(int tbuf = 256, int rBuf = 256);
        int FreePort();
        int Send(int delay=20);
        int Read(int delay=1000);
        int virtual Test();

};

class IB_ITC503: public IsoBusDevice
{
    public:
        IB_ITC503(int Port, int Address);
        int SetRemote(int mode);
        int SetTemperature(double T);
        double GetTemperature(int sensor);
        int SetAutoPID(int on);
        int SetAutoHeater(int on);
        int SetAutoFlow(int on);
        int SetHeater(double manheat);
        int SetFlow(double valve);
        double GetValve();
        double GetPID(int param);
        double GetHeater(int mode);
        int GetAutoHeater();
        int GetAutoValve();
        int GetAutoMode();
        int GetStatus(char* StatStr);
        bool AutoPID(const char *StatStr);
        bool AutoPID();
        bool AutoHeater(const char *StatStr);
        bool AutoHeater();
        bool AutoFlow(const char *StatStr);
        bool AutoFlow();
        int ControlChannel(const char *StatStr);
        int SetControlChannel(int chan);
        int SetP(double P);
        int SetI(double I);
        int SetD(double D);
        int SetAuto(int a);
        int Test();
};

class IB_ILM211: public IsoBusDevice
{
    public:
        IB_ILM211(int Port, int Address);
        int SetRemote(int mode);
        double GetHe();
        double GetN2();
        int Test();
};

class IB_IPS20: public IsoBusDevice
{
    private:
        double AperT;
        double MaxSpeed;
        double MaxCurrent;
        double SafeSpeed;
    public:
        IB_IPS20(int Port, int Address);
        int SetRemote(int mode);
        double GetParameter(int n);
        double GetSpeed();
        double GetField();
        double GetCurrent();
        double GetTargetField();
        int GetStatus(char* Stat);
        int Activity(const char* Stat);
        int Quenched(const char* Stat);
        int SpeedLimited(const char* Stat);
        int SetField(double B);
        int SetCurrent(double Curr);
        int SetFieldSweepSpeed(double mT_per_s);
        int SetAmpSweepSpeed(double AperMin);
        int SetAction(int act);
        int SetExtendedResolution(int on);
        bool Sweeping();
        double ApT() {return AperT;}
        double GetSafeSpeed() {return SafeSpeed;}
        double GetMaxSpeed() {return MaxSpeed;}
        double GetMaxCurrent() { return MaxCurrent;}
        int Test();
};

class IB_IPS120: public IsoBusDevice
{
    private:
        double AperT;
        double MaxSpeed;
        double MaxCurrent;
        double SafeSpeed;
        double Hysteresis; // in seconds (in mT per mT/s sweeprate)

    public:
        IB_IPS120(int Port, int Address);
        int SetRemote(int mode);
        double GetParameter(int n);
        double GetSpeed();
        double GetAccurateSpeed();
        double GetField();
        double GetCurrent();
        double GetTargetField();
        int GetStatus(char* Stat);
        int Activity(const char* Stat);
        int Heater(const char* Stat);
        int Heater();
        int Quenched(const char* Stat);
        int SpeedLimited(const char* Stat);
        int SetField(double B);
        int SetCurrent(double Curr);
        int SetFieldSweepSpeed(double mT_per_s);
        int SetAmpSweepSpeed(double AperMin);
        int SetAction(int act);
        int SetExtendedResolution(int on);
        double ApT() {return AperT;}
        double GetSafeSpeed() {return SafeSpeed;}
        double GetMaxSpeed() {return MaxSpeed;}
        double GetMaxCurrent() { return MaxCurrent;}
        bool Sweeping(const char* Stat);
        bool Sweeping();
        int SetHeater(int on);
        int Test();
        void SetHysteresis(const double Hys) {Hysteresis = Hys;}
        double GetHysteresis() {return Hysteresis;} 
};
//---------------------------------------------------------------------------
class IB_IPS180: public IsoBusDevice
{
    private:
        double AperT;
        double MaxSpeed;
        double MaxCurrent;
        double SafeSpeed;
    public:
        IB_IPS180(int Port, int Address);
        int SetRemote(int mode);
        double GetParameter(int n);
        double GetSpeed();
        double GetField();
        double GetCurrent();
        double GetTargetField();
        int GetStatus(char* Stat);
        int Activity(const char* Stat);
        int Heater(const char* Stat);
        int Heater();
        int Quenched(const char* Stat);
        int SpeedLimited(const char* Stat);
        int SetField(double B);
        int SetCurrent(double Curr);
        int SetFieldSweepSpeed(double mT_per_s);
        int SetAmpSweepSpeed(double AperMin);
        int SetAction(int act);
        int SetExtendedResolution(int on);
        double ApT() {return AperT;}
        double GetSafeSpeed() {return SafeSpeed;}
        double GetMaxSpeed() {return MaxSpeed;}
        double GetMaxCurrent() { return MaxCurrent;}
        bool Sweeping(const char* Stat);
        bool Sweeping();
        int SetHeater(int on);
        int Test();
};
//---------------------------------------------------------------------------
#endif

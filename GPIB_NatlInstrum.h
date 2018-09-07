#ifndef GPIBNatInstrum_H
#define GPIBNatInstrum_H
//#include "mydata.h"
//  First the additional function declarations:

bool LoadGPIBDll(void);   // Loads the gpib-32.dll
void FreeGPIBDll(void);   // Unloads the gpib-32.dll
int GPIBInterfaceClear(int _gpib = 0);
int GPIBSendCommand(short address, const char* buffer);
int GPIBSendCommand_NoSetRemote(short address, const char* buffer);
int GPIBGetStatusByte();
int GPIBGetErrorByte();
int GPIBGetError(char* Errormessage);
int GPIBGetLengthCount();
int GPIBReceive(short addr, char* Buffer, int BufLength=72);
int GPIBStatusLine(int i, char* line);
short GPIBGetDeviceStatus(int addr);
int GPIBSetTimeOut(int index);
void GPIBSetEosMode(int n);
void GPIBSetEotMode(int n);
void SetRemote(short addr);
void SetLocal(short addr);
int GPIBFindAllListeners(short* addr);
void GPIBGetID(int address, char* deviceID, int MaxLength);

//////////////////////////////////////////////////////////////////////////
//
//  Header file for the IEEE devices  Last revised 23-2-1996
//
//	Contains the definitions for the classes. Each device has its
// own class. The following classes are defined
//  -	IEEEdevice		 The base class of all devices
//	 -		K175  			Keithley 175 autoranging multimeter
//	 -		K195A          Keithley 195A multimeter
//	 -		K196           Keithley 196 multimeter 6.5 digits
//	 -		EGG5210        EG&G 5210 Lock-in amplifier
//	 -		G6062A         Gigatronics 6062A frequency synthesizer
//  -   ITC503         Oxford Instruments 'Intelligent' Temperature Controller ITC503
//  - 	PS120				Oxford Instruments power supply PS120-10

///////////////////////////////////////////////////////////////////////////
//
//	  The class IEEEdevice is the base class of the devices. It contains
//	  the parameters used in the GPIB protocol and some general routines
//
const int MaxCommand = 80;
const int MaxResponse = 254;
const int MaxErrorMessage = 128;

class IEEEdevice
{
	public:
		IEEEdevice(int addr);
		void SetAddress(const int _address) {address = short(_address);}
		int GetAddress() const {return address;}
		int ReadyToRead();
		int WaitBeforeRead();
		int WaitUntilMessageAvailable();
		int MessageAvailable();
		int CommandPerformed();
		int ErrorTest(char *message);
        int ErrorTest(char *message, int maxlen);
//
		int virtual Test();

// The data members of the IEEEdevice are protected: they can be accessed with functions
// of derived classes.

	protected:
		short address;                     // IEEE address
		int status;                      // Status indicates if GPIB commands were
													//		(in)correctly executed
		int length;                      // length of response from device
		unsigned char poll;              // poll byte, response from a serial poll
		char command[MaxCommand];                // command contains ASCII string to be send
													//     to the device
		char response[MaxResponse];               // response contains ASCII response from device
		char ErrorMessage[MaxErrorMessage];          // String to contain errormessage
		int ErrorCode;                   // Code indicating occurrence of errors
};

//class K175 : public IEEEdevice
//{
//	public:
//		K175(int _addr);
//		double ReadVoltage();
//		int Test();
//};

class K195A : public IEEEdevice
{
	public:
		K195A(int _addr);
		double ReadVoltage();
		int Test();
};

class K196 : public IEEEdevice
{
	public:
		K196(int _addr);
		double ReadVoltage();
		int Test();
};

class G6062A : public IEEEdevice
{
	public:
		G6062A(int addr);
		int SetFreq(double MHz);
		int SetStep(double kHz);
		int SetPower(int P);
		int SetFM(int on,double Mod);
		int SetAM(int on,double Mod);
		int Step();
		int RFon();
		int RFoff();
		int Test();
};

class SMY02 : public IEEEdevice   //Rohde & Schwartz Signal generator
{
	public:
		SMY02(int addr);
		int SetFreq(double MHz);
		int SetStep(double kHz);
		int SetPower(double P);
        int SetAFfreq(double kHz);
        int SetSweepType(int type);
          // Sets internal modul frequency
		int SetStartFreq(double MHz);
		int SetStopFreq(double MHz);
        int SetStepTime(int ms);
		int SetFM(int on,double Mod);
		int SetAM(int on,double Mod);
		int SetPM(int on,double Mod);
		int Step();
		int RFon();
		int RFoff();
		int Test();
        double GetFreq();
        double GetPower();
        double GetAFfreq();
        double GetStartFreq();
        double GetStopFreq();
        double GetStep();
        int GetStepTime();
        int GetSweepType();
        int GetAM(double*);
        int GetFM(double*);
        int GetPM(double*);
        int IsError(char* message, int maxlength);
};

class AR100W1000A :public IEEEdevice
{
    public:
        AR100W1000A(int _addr);
        int SetPowerStatus(int stat);
        int GetPowerStatus();
        int IsOperationComplete();
        int SetStandBy();
        int SetOperate();
        double GetGain();
		int SetGain(double gain);
        double GetFPower();
        double GetRPower();
        int GetMode();
        int GetALCParameters(double* ThreeReals);
        int SetALCMode(int mode);
        int SetThreshold(double th);
        int SetDetectorGain(double dg);
        int SetResponse(int resp);
        int Test();
};

class EGG5210 : public IEEEdevice
{
	public:
		EGG5210(int _addr);
		double ReadOutput(int channel);
		double GetSignalPhase();
		int SetDAC(int mV);
		double ReadADC(int channel);

		char* GetSensitivity(char* sensitivity);
		char* GetTimeConstant(char* tc);
		double GetRefFrequency();
		double GetPhase();
		double MeasFilterFrequency();
		void WriteAllParameters(char* LockinPar) const;
		int UpdateParameters();
		int Test();

	private:
		double Phase;                  //Phase in degrees
		int Sensitivity;               //Sensitivity code 0:10nV,...,15:3V
		int TimeConstant;              //Time constant code 0:1ms,...,13:3ks
		double ReferenceFrequency;     //Reference Frequency in Hz
		int Slope;                     //OutputFilter. 0:6dB/oct, 1:12dB/oct
		int DynamicReserve;            //0:HI STAB 20dB 1:NORM 40dB 2:HI RES 60dB
		double FilterFrequency;			 //Input FilterFrequency in Hz
		int FilterMode;					 //0:FLAT, 1:NOTCH, 2:LOW-PASS, 3:BANDPASS

		int MeasSensitivity();
		int MeasTimeConstant();
		int MeasSlope();
		int MeasDynRes();
		int MeasFilterMode();
		void AsciiTimeConstant(int TcCode, char* tc) const;
		void AsciiSensitivity(int SensCode, char* sens) const;
};

class ITC503 : public IEEEdevice
{
	public:
		ITC503(int _addr);
		int SetTemperature(double T);
		double GetTemperature(int sensor=0);
		double GetSetTemperature();
		int SetRemote(int remote=3);     //Sets remote or local mode
		int Test();
	private:
		int CurrentSensor;
		double SetPoint;
		double SwitchTemperature;
};

class IPS120 : public IEEEdevice
{
	public:
		IPS120(int _addr);
		double GetDemandField();
		double GetTargetField();
		double GetMeasuredCurrent();
        double GetFieldSweepRate();
		double GetBperA() {return BperA;}
		double GetLmagnet() {return Lmagnet;}
		double GetMaxCurrent() {return MaxCurrent;}
		double GetMaxSpeed() {return MaxSpeed;}
		int SetTargetField(double B);
		int SetFieldSweepRate(double rate);
		int Zero();
		int Hold();
		int GotoSetPoint();
		int SetRemote(int remote=3);     //Sets remote or local mode
		int Test();
		int IsSweeping();

	private:
		double BperA;   // Calibration factor in T/A
		double Lmagnet; // Selfinductance of magnet in H
		double MaxSpeed; // Maximum sweepspeed in T/min
		double MaxCurrent; // Maximum current
};

class SRS245 : public IEEEdevice
{
    public:
        SRS245(int _addr);            // Constructor with IEEE address
        int Set(int AnalogPort, double value);
        double Get(int i);
        int SetNout(int i);
        int GetNout() {return Nout;}
        int SetB(int i, int TTLvalue);
        bool GetB(int i);

	private:
		int Nout;
};
/*
class SRS830 : public IEEEdevice
{
	public:
		SRS830(int _addr);
		int SetRemote(int remote=1);
		double ReadOutput(int channel);
		int ReadXY(double *x, double *y);
		double GetSignalPhase();
		int SetDAC(int mV);
		int SetDAC(int n, int mV);
		double ReadADC(int channel);

		double SetModulationFreq(double f);
		int SetModulationPhase(double ph);
		double SetModulationAmp(double amp);

		int GetSensitivity(char* sensitivity);
		int GetTimeConstant(char* tc);
		double GetRefFrequency();
		double GetPhase();
		double GetRefAmp();

		double MeasFilterFrequency();
		void WriteAllParameters(char* LockinPar) const;
		int UpdateParameters();
		int Test();
		int MeasSensitivity();
		double RealSensitivity();
		int SetSensitivity(int sens);
		int MeasTimeConstant();
		int MeasTimeConstant(double *tc);
		int MeasSlope();
		int MeasDynRes();
		int MeasFilterMode();
		int GetFilters(char* line, int MaxChar);

	private:
		double Phase;                  //Phase in degrees
		int Sensitivity;               //Sensitivity code 0:10nV,...,15:3V
		int TimeConstant;              //Time constant code 0:1ms,...,13:3ks
		double ReferenceFrequency;     //Reference Frequency in Hz
		int Slope;                     //OutputFilter. 0:6dB/oct, 1:12dB/oct
		int DynamicReserve;            //0:HI STAB 20dB 1:NORM 40dB 2:HI RES 60dB
		double FilterFrequency;			 //Input FilterFrequency in Hz
		int FilterMode;					 //0:FLAT, 1:NOTCH, 2:LOW-PASS, 3:BANDPASS

		void AsciiTimeConstant(int TcCode, char* tc) const;
		void AsciiSensitivity(int SensCode, char* sens) const;
};
*/
#endif

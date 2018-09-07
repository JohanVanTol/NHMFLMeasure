//
//
//
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <math.h>
#include "IsoBusDevices.h"
#include "wsc.h"
void WaitMillisecs(int ms);              // Defined in RS232Test.cpp

int GetChar(int Port, int MilliSecs);    // Defined in RS232Test.cpp
										// A version of SioGetc that includes
										// waiting for a character to be
										// available

//---------------------------------------------------------------------------
#pragma package(smart_init)
//

IsoBusDevice::IsoBusDevice(int _Port, int _Address)
	: Port(_Port), Address(_Address)
{
	MaxLength = 256;
	command = new char[MaxLength];
	response = new char[MaxLength];
	ErrorString = new char[MaxLength];
	Error = 0;
}

IsoBusDevice::~IsoBusDevice()
{
	delete[] command;
	delete[] response;
}

int IsoBusDevice::Reset(int _Port, int _Address, int _ML)
{
	Port = _Port;
	Address = _Address;
	MaxLength = _ML;
	command = new char[MaxLength];
    response = new char[MaxLength];
    return 0;
}

int IsoBusDevice::InitPort(int TrBuf, int RcBuf)
{
    if (SioReset(Port, TrBuf, RcBuf) < 0) return -1;
    if (SetBaud(Baud9600) < 0) return -1;
	return SioRTS(Port,'S');
}

int IsoBusDevice::SetBaud(int Baud)
{
    return SioBaud(Port, Baud);
}

int IsoBusDevice::FreePort()
{
	return SioDone(Port);
}

int IsoBusDevice::Send(int delay)
{
    for (int i=0;i<strlen(command);i++)
	{
        if (SioPutc(Port, command[i]) < 0) return -1;
		WaitMillisecs(delay);
    }
	return 0;
}

int IsoBusDevice::Read(int delay)
{
    int j=0;
    char c;
    while (( (c = GetChar(Port,delay)) != WSC_NO_DATA) && (c != 13))
	{
       response[j++]  = c;
    }
    response[j] = 0;
	return 0;
}

/////////////////////////////////////////////////
//
//  Virtual function for the IsoBusDevice class.
//  each device should have its own test
//
int IsoBusDevice::Test()
{
	return 0;
}

//**********************************************************
//**********************************************************
//
//  DEFINITIONS OF IB_ITC503
//
//**********************************************************

///////////////////////////////////////
//
//  Constructor
//
// Parameters
//      _Port    :  The serial Port (ex COM1=0, COM2=1, etc)
//      _Address :  The Isobus address (1..9)
//
IB_ITC503::IB_ITC503(int _Port, int _Address)
    : IsoBusDevice(_Port,_Address)
{}

///////////////////////////////////////
//
//  SetRemote
//
//  Parameters
//    mode:   0 local & locked
//            1 Remote & locked
//            2 Local & unlocked
//            3 Remote & unlocked
//
//  Return value
//            0    OK
//            -1   Error
//
int IB_ITC503::SetRemote(int mode)
{
    if ( (mode<0) || (mode>3) ) mode = 0;
    sprintf(command,"@%1dC%1d\r", Address, mode);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'C') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   SetTemperature
//      Sets the temperature (in K)
//
//  Parameters
//    T:  The 'SET' temperature of the ITC503
//
//  Return value
//            0    OK
//            -1   Error
//
int IB_ITC503::SetTemperature(double T)
{
    if (SetRemote(3) < 0) return -1;

	if (T >= 100.0)            //Set temp in right format
		sprintf(command,"@%1dT%5.1f\r",Address, T);
	  else
		if (T >= 10.00)
		  sprintf(command,"@%1dT%5.2f\r",Address, T);
		 else
          sprintf(command,"@%1dT%5.3f\r", Address, T);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'T') return -1;

    return 0;
}

/////////////////////////////////////////////
//
//   GetTemperature
//      returns measured or settemperature(in K)
//
//  Parameters
//    sensor:  0: The 'SET' temperature of the ITC503
//             1,2,3 : The measured T of channel 1,2,or 3
//
//  Return value
//            double : The temperature
//            or < 0 : Error
//
double IB_ITC503::GetTemperature(int sensor)
{
	if (Port < 0) return -1.0;           //if not connected return
	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dR%1d\r",Address, sensor);
    if (Send() < 0) return -1.0;
    if (Read() < 0) return -1.0;
    if (response[0] != 'R') return -1.0;
    return atof(response+1);
}

/////////////////////////////////////////////
//
//   GetPID
//      returns the P, I, or D parameter
//
//  Parameters
//    param :  0: Returns the Proportionality parameter P
//            1: Returns the Proportionality parameter I
//            2: Returns the Proportionality parameter D
//
//  Return value
//            double : P, I, or D
//            or < 0 : Error
//
double IB_ITC503::GetPID(int param)
{
	if (Port < 0) return -1.0;           //if not connected return
	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dR%1d\r",Address, param + 8);
    if (Send() < 0) return -1.0;
    if (Read() < 0) return -1.0;
    if (response[0] != 'R') return -1.0;
    return atof(response+1);
}

/////////////////////////////////////////////
//
//   GetHeater
//      returns the Heater output in % or Volts
//
//  Parameters
//    param :  0: Returns the Output in % of Max current
//             1: Returns the Output in V
//
//  Return value
//            double : Heater output
//            or < 0 : Error
//
double IB_ITC503::GetHeater(int mode)
{
	if (Port < 0) return -1.0;           //if not connected return
	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dR%1d\r",Address, mode + 5);
    if (Send() < 0) return -1.0;
    if (Read() < 0) return -1.0;
    if (response[0] != 'R') return -1.0;
	return atof(response+1);
}

/////////////////////////////////////////////
//
//   GetHeater
//      returns the Valve flow in %
//
//  Return value
//            double : Valve state %
//            or < 0 : Error
//
double IB_ITC503::GetValve()
{
	if (Port < 0) return -1.0;           //if not connected return
	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dR%1d\r",Address, 7);
    if (Send() < 0) return -1.0;
    if (Read() < 0) return -1.0;
    if (response[0] != 'R') return -1.0;
    return atof(response+1);
}

/////////////////////////////////////////////
//
//   SetAutoPID
//      Set AUTO PID on or off
//
//  Return value
//            int : 0 OK,  <0 Error
//
int IB_ITC503::SetAutoPID(int on)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    if (on > 0) on = 1;
    sprintf(command,"@%1dL%1d\r",Address, on);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'L') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   SetAutoFlow
//      Set AUTO PID on or off
//
//  Return value
//            int : 0 OK,  <0 Error
//
int IB_ITC503::SetAutoFlow(int on)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    if (on > 0) on = 1;
    if (AutoHeater()) on += 1;
    sprintf(command,"@%1dA%1d\r",Address, on);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'A') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   SetAuto
//      Set AUTO Heater and flow
//
//  Parameter on.
//      Should be 0 for manual heater, manual flow
//      Should be 1 for auto heater, manual flow
//      Should be 2 for manual heater, auto flow
//      Should be 3 for auto heater, auto flow
//
//  Return value
//            int : 0 OK,  <0 Error
//
int IB_ITC503::SetAuto(int on)



{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    if ((on < 0)|| (on >3)) on = 0;
    sprintf(command,"@%1dA%1d\r",Address, on);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'A') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   SetP
//      Set Proportionality value P
//
//  Parameter double : the value to be set
//  Return value
//            int : 0 OK,  <0 Error
//
int IB_ITC503::SetP(double P)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dP%f\r",Address, P);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'P') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   SetI
//      Set integration  value I (in minutes)
//
//  Parameter double : the value to be set
//  Return value
//            int : 0 OK,  <0 Error
//
int IB_ITC503::SetI(double I)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dI%f\r",Address, I);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'I') return -1;
    return 0;
}
/////////////////////////////////////////////
//
//   SetD
//      Set Differential  value D
//
//  Parameter double : the value to be set
//  Return value
//            int : 0 OK,  <0 Error
//
int IB_ITC503::SetD(double D)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dD%f\r",Address, D);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'D') return -1;
    return 0;
}
/////////////////////////////////////////////
//
//   SetFlow
//      Set Flow (percentage with 0.1% resolution)
//
//  Parameter double : the value to be set
//  Return value
//            int : 0 OK,  <0 Error
//
int IB_ITC503::SetFlow(double F)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dG%f\r",Address, F);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'G') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   SetHeater
//      Set Heater in %
//
//  Parameter double : the value to be set
//  Return value
//            int : 0 OK,  <0 Error
//
int IB_ITC503::SetHeater(double Heat)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dO%f\r",Address, Heat);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'O') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   GetStatus
//      returns the Valve flow in %
//
//   Param
//      StatStr  The string that the ITC returns upon the X-command
//                Format  XnAnCnSnnHnLn
//              From this the Auto/manual settings can be derived
//
//  Return value
//            int : length of string OK
//              or < 0 : Error
//
int IB_ITC503::GetStatus(char* StatStr)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dX\r",Address);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'X') return -1;
    strncpy(StatStr, response, 14);
    StatStr[13] = 0;
    return strlen(StatStr);
}

bool IB_ITC503::AutoHeater(const char *StatStr)
{
    if ((StatStr[3] == '1') || (StatStr[3] == '3'))
        return true;
    return false;
}

bool IB_ITC503::AutoFlow(const char *StatStr)
{
    if (StatStr[3] < 50)
        return false;
    return true;
}

bool IB_ITC503::AutoFlow()
{
    char StatusString[14];
    int len = GetStatus(StatusString);
    if (len < 12) return false;
    if (AutoFlow(StatusString))
      return true;
    return false;
}

bool IB_ITC503::AutoPID()
{
    char StatusString[14];
    int len = GetStatus(StatusString);
    if (len < 12) return false;
    if (AutoPID(StatusString))
      return true;
    return false;
}

bool IB_ITC503::AutoHeater()
{
    char StatusString[14];
    int len = GetStatus(StatusString);
    if (len < 12) return false;
    if (AutoHeater(StatusString))
      return true;
    return false;
}

bool IB_ITC503::AutoPID(const char *StatStr)
{
    for (int i=10;i<12;i++)
        if (StatStr[i] == 'L')
            if (StatStr[i+1] == '1') return true;
    return false;
}

int IB_ITC503::ControlChannel(const char* StatStr)
{
    for (int i=8;i<10;i++)
        if (StatStr[i] == 'H')
            return int(StatStr[i+1]) -48;
}

int IB_ITC503::SetControlChannel(int chan)
{
    if (AutoHeater()) return -1;       // allow change only in Manual mode

	if (Port < 0) return -1;           // if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dH%1d\r", Address, chan);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'H') return -1;
    return 0;

}

int IB_ITC503::Test()
{
	if (Port < 0) return -1;           // if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked
    sprintf(command,"@%1dV\r", Address);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    return strncmp(response,"ITC503",6);
}


//**********************************************************
//**********************************************************
//
//  DEFINITIONS OF IB_ILM211
//
//**********************************************************

///////////////////////////////////////
//
//  Constructor
//
// Parameters
//      _Port    :  The serial Port (ex COM1=0, COM2=1, etc)
//      _Address :  The Isobus address (1..9)
//
IB_ILM211::IB_ILM211(int _Port, int _Address)
    : IsoBusDevice(_Port,_Address)
{}

///////////////////////////////////////
//
//  SetRemote
//
//  Parameters
//    mode:   0 local & locked
//            1 Remote & locked
//            2 Local & unlocked
//            3 Remote & unlocked
//
//  Return value
//            0    OK
//            -1   Error
//
int IB_ILM211::SetRemote(int mode)
{
    if ( (mode<0) || (mode>3) ) mode = 0;
    sprintf(command,"@%1dC%1d\r", Address, mode);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'C') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   GetHe()
//      returns measured He-level
//
//  Return value
//            double : The level
//            or < 0 : Error
//
double IB_ILM211::GetHe()
{
	if (Port < 0) return -1.0;           //if not connected return
//	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked
    sprintf(command,"@%1dR1\r",Address);
    if (Send() < 0) return -1.0;
    if (Read() < 0) return -1.0;
    if (response[0] != 'R') return -1.0;
    return atof(response+1)/10.0;
}

/////////////////////////////////////////////
//
//   GetN2()
//      returns measured N2-level
//
//  Return value
//            double : The level
//            or < 0 : Error
//
double IB_ILM211::GetN2()
{
	if (Port < 0) return -1.0;           //if not connected return
//	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dR2\r",Address);
    if (Send() < 0) return -1.0;
    if (Read() < 0) return -1.0;
    if (response[0] != 'R') return -1.0;
    return atof(response+1)/10.0;
}

int IB_ILM211::Test()
{
	if (Port < 0) return -1;           // if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked
    sprintf(command,"@%1dV\r", Address);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    return strncmp(response,"ILM200",6);
}

///
void WaitMillisecs(int ms)
{
    unsigned int count = GetTickCount() + ms;
    while (GetTickCount() < count) {}
    return;
}

int GetChar(int Port, int MilliSecs)
{int   Code;
 int   Flag;
 DWORD Count;
 Flag = 0;
 while(1)
   {Code = SioGetc(Port);
    if(Code!=WSC_NO_DATA) return Code;
    if(Flag==0)
       {Count = GetTickCount() + (DWORD)MilliSecs;
        Flag = 1;
       }
    if(GetTickCount()>Count) break;
   }
 return WSC_NO_DATA;
}


///////////////////////////////////////
//
//  Constructor
//
// Parameters
//      _Port    :  The serial Port (ex COM1=0, COM2=1, etc)
//      _Address :  The Isobus address (1..9)
//
IB_IPS20::IB_IPS20(int _Port, int _Address)
    : IsoBusDevice(_Port,_Address), AperT(125.2), MaxSpeed(10.0),
                SafeSpeed(2.0), MaxCurrent(12.52)
{}

///////////////////////////////////////
//
//  SetRemote
//
//  Parameters
//    mode:   0 local & locked
//            1 Remote & locked
//            2 Local & unlocked
//            3 Remote & unlocked
//
//  Return value
//            0    OK
//            -1   Error
//
int IB_IPS20::SetRemote(int mode)
{
    if ( (mode<0) || (mode>3) ) mode = 0;
    sprintf(command,"@%1dC%1d\r", Address, mode);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'C') return -1;
    return 0;
}
//////////////////////////////////////////////
//
//  GetParametersends the Rn command and reads
//                          the answer
//  n can have the following values
//  n=0     Demand current (A)
//  n=1     Measured Power Supply Voltage (V)
//  n=2     Measured Magnet Current(A)
//  n=3     unused
//  n=4     Demand current (A) (equals n=0)
//  n=5     Set Point: Target current (A)
//  n=6     Current Sweep Rate (A/min)
//  n=7     Demand field (T)
//  n=8     Set Point: Target Field (T)
//  n=9     Field Sweep Rate (T/min)
//  n=10    DAC zero offset (A)
//  n=11    Channel 1 freq./4
//  n=12    Channel 2 freq./4
//  n=13    Channel 3 freq./4
//  n=14    Demand current (A) (equals n=0)
//  n=15    Software Voltage Limit (V)
//  n=16    Persistent Magnet Current(A)
//  n=17    Trip current (A)
//  n=18    Persistent Magnet Field (T)
//  n=19    Trip Field (T)
//  n=20    Switch Heater Current (mA)
//  n=21    Safe Current Limit, most negative(A)
//  n=22    Safe Current Limit, most positive(A)
//  n=23    Lead Resistance (mOhm)
//  n=24    Magnet Inductance (H)
//
//

double IB_IPS20::GetParameter(int n)
{
    if (Port < 0) return -1.0;           //if not connected return
//	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dR%1d\r",Address, n);
    if (Send() < 0) return -1.0;
    if (Read() < 0) return -1.0;
    if (response[0] != 'R') return -1.0;
    return atof(response+1);
}

double IB_IPS20::GetSpeed()
{
    double TperMin;
	if ((TperMin = GetParameter(9)) < 0) return -1.0;
    return TperMin*1000.0/60.0;  // return mT/sec
}

double IB_IPS20::GetField()
{
    double B;
    B=GetParameter(0)/AperT;
    return B;// return T
}

double IB_IPS20::GetTargetField()
{
    double B;
    B=GetParameter(8);
    return B;// return T
}

double IB_IPS20::GetCurrent()
{
    double Amps;
    Amps=GetParameter(0);
    return Amps;// return T
}

int IB_IPS20::SetField(double B)
{
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    double Amp = B*AperT;
    if (Amp > MaxCurrent) Amp = MaxCurrent;
    if (Amp < -1.0*MaxCurrent) Amp = -1.0*MaxCurrent;
    sprintf(command,"@%1dI%f\r",Address, B*AperT);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'I') return -1;
    return 0;
}

int IB_IPS20::SetCurrent(double Amp)
{
    if (Amp > MaxCurrent) Amp = MaxCurrent;
    if (Amp < -1.0*MaxCurrent) Amp = -1.0*MaxCurrent;
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dI%f\r",Address, Amp);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'I') return -1;
    return 0;
}

int IB_IPS20::SetAmpSweepSpeed(double AperMin)
{
    if (AperMin > MaxSpeed) AperMin = MaxSpeed;
    if (AperMin < 0.0) AperMin = 0.0;
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dS%f\r",Address, AperMin);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'S') return -1;
    return 0;
}

int IB_IPS20::SetFieldSweepSpeed(double mT_per_s)
{
    double AmpPerMinute = mT_per_s*60.0*AperT/1000.0;
    if (AmpPerMinute > SafeSpeed) AmpPerMinute = SafeSpeed;
    SetAmpSweepSpeed(AmpPerMinute);
}

int IB_IPS20::SetAction(int act)
{
    if (Port < 0) return -1.0;           //if not connected return
	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dA%1d\r",Address, act);
    if (Send() < 0) return -1.0;
    if (Read() < 0) return -1.0;
    if (response[0] != 'A') return -1.0;
    return 0;
}

/////////////////////////////////////////////
//
//   GetStatus
//      returns the Valve flow in %
//
//   Param
//      StatStr  The string that the ITC returns upon the X-command
//                Format  XnAnCnSnnHnLn
//              From this the Auto/manual settings can be derived
//
//  Return value
//            int : length of string OK
//              or < 0 : Error
//
int IB_IPS20::GetStatus(char* StatStr)  // NB Statstr should have at least
                                          // 16 elements
{
	if (Port < 0) return -1;           //if not connected return
//	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dX\r",Address);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'X')
    {     //try once more
        if (Send() < 0) return -1;
        if (Read() < 0) return -1;
        if (response[0] != 'X') return -1;
    }
    strncpy(StatStr, response, 15);
    StatStr[15] = 0;
    return strlen(StatStr);
}

int IB_IPS20::Activity(const char* Stat)
{
    return int(Stat[4])-48;
}

int IB_IPS20::Quenched(const char* Stat)
{
    if (Stat[1]=='1') return 1;
    return 0;
}

int IB_IPS20::SpeedLimited(const char* Stat)
{
    if ((Stat[11] == '2') || (Stat[11] == '3')) return 1;
    return 0;
}

int IB_IPS20::SetExtendedResolution(int on)
{
	if (Port < 0) return -1;
    if ((on < 0) || (on >1)) return -1;           //if not connected return
    sprintf(command,"@%1dQ%1d\r",Address, 4*on);
    if (Send() < 0) return -1;
    return 0;
}

bool IB_IPS20::Sweeping()
{
    char StatStr[16];
    int stat;
    if (stat=GetStatus(StatStr)==-1 ) return true;
    //  Sometimes the GetStatus doesn't work
    //  Then return true to not stop the program
    if (StatStr[11] == '0') return false;
    return true;
}

int IB_IPS20::Test()
{
	if (Port < 0) return -1;           // if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked
    sprintf(command,"@%1dV\r", Address);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (strncmp(response,"IPS20",5) == 0) return 0;
      else
      if (strncmp(response,"IPS120-10",9)==0) // if 17T sweep coil
      {
        AperT = 133.3;
        MaxSpeed = 15.0;
        MaxCurrent = 13.33;
        SafeSpeed =  5.0;
        return 0;
      }
    return 1;
}



///////////////////////////////////////
//
//  Constructor
//
// Parameters
//      _Port    :  The serial Port (ex COM1=0, COM2=1, etc)
//      _Address :  The Isobus address (1..9)
//
IB_IPS120::IB_IPS120(int _Port, int _Address)
    : IsoBusDevice(_Port,_Address), AperT(8.45952), MaxSpeed(5),
                SafeSpeed(4.23), MaxCurrent(105.744), Hysteresis(3.69)
{}

///////////////////////////////////////
//
//  SetRemote
//
//  Parameters
//    mode:   0 local & locked
//            1 Remote & locked
//            2 Local & unlocked
//            3 Remote & unlocked
//
//  Return value
//            0    OK
//            -1   Error
//
int IB_IPS120::SetRemote(int mode)
{
    if ( (mode<0) || (mode>3) ) mode = 0;
    sprintf(command,"@%1dC%1d\r", Address, mode);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'C') return -1;
    return 0;
}

/////////////////////////////////////////////////
//
//  GetParameter(int n)
//
//  Returns the following parameters
//  n=0     Demand current (A)
//  n=1     Measured Power Supply Voltage (V)
//  n=2     Measured Magnet Current(A)
//  n=3     unused
//  n=4     Demand current (A) (equals n=0)
//  n=5     Set Point: Target current (A)
//  n=6     Current Sweep Rate (A/min)
//  n=7     Demand field (T)
//  n=8     Set Point: Target Field (T)
//  n=9     Field Sweep Rate (T/min)
//  n=10    DAC zero offset (A)
//  n=11    Channel 1 freq./4
//  n=12    Channel 2 freq./4
//  n=13    Channel 3 freq./4
//  n=14    Demand current (A) (equals n=0)
//  n=15    Software Voltage Limit (V)
//  n=16    Persistent Magnet Current(A)
//  n=17    Trip current (A)
//  n=18    Persistent Magnet Field (T)
//  n=19    Trip Field (T)
//  n=20    Switch Heater Current (mA)
//  n=21    Safe Current Limit, most negative(A)
//  n=22    Safe Current Limit, most positive(A)
//  n=23    Lead Resistance (mOhm)
//  n=24    Magnet Inductance (H)

double IB_IPS120::GetParameter(int n)
{
    if (Port < 0) return -96.99;           //if not connected return
//	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dR%1d\r",Address, n);
    if (Send() < 0) return -99.99;
    if (Read() < 0) return -98.99;
    if (response[0] != 'R') return -97.99;
    return atof(response+1);
}

double IB_IPS120::GetSpeed()
{
    double TperMin;
    if ((TperMin=GetParameter(9)) < 0) return -1.0;
    return TperMin*1000.0/60.0;  // return mT/sec
}

double IB_IPS120::GetAccurateSpeed()
{
    double AperMin;
    if ((AperMin=GetParameter(6)) < 0) return -1.0;
    return AperMin*1000.0/(60.0*AperT);  // return mT/sec
}

double IB_IPS120::GetField()
{
    double B;
    B=GetParameter(7);
    return B;// return T
}

double IB_IPS120::GetTargetField()
{
    double B;
    B=GetParameter(8);
    return B;// return T
}

double IB_IPS120::GetCurrent()
{
    double Amps;
    Amps=GetParameter(0);
    return Amps;// return T
}

int IB_IPS120::SetField(double B)
{
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    if (B*AperT > MaxCurrent) B = MaxCurrent/AperT;
    if (B*AperT < -1.0*MaxCurrent) B=  -1.0*MaxCurrent/AperT;
    sprintf(command,"@%1dJ%f\r",Address, B);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'J') return -1;
    return 0;
}

int IB_IPS120::SetCurrent(double Amp)
{
    if (Amp > MaxCurrent) Amp = MaxCurrent;
    if (Amp < -1.0*MaxCurrent) Amp = -1.0*MaxCurrent;
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dI%f\r",Address, Amp);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'I') return -1;
    return 0;
}

int IB_IPS120::SetAmpSweepSpeed(double AperMin)
{
    if (AperMin > MaxSpeed) AperMin = MaxSpeed;
    if (AperMin < 0.0) AperMin = 0.0;
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dS%f\r",Address, AperMin);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'S') return -1;
    return 0;
}

int IB_IPS120::SetFieldSweepSpeed(double mT_per_s)
{
	double BPerMinute = mT_per_s*60.0/1000.0;
	if (BPerMinute > MaxSpeed/AperT ) BPerMinute = MaxSpeed/AperT;
	if (BPerMinute < 0.0) BPerMinute = 0.0;
//    if (Port < 0) return -1;           //if not connected return
//	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

	// Let change to Amp_p_min
	// for some reason rounding to an integer is somewhat vague in C++
	// the implicit casting to an integer rounds toward zero. We'll just round up
	double mApM = 1000.0*BPerMinute * AperT;

	int mAPerMinute = 1;
	if (fmod(mApM, 1.0) >= 0.5) mAPerMinute = (int)(ceil(mApM));
	  else mAPerMinute = (int)(floor(mApM));
	if (mAPerMinute < 1) mAPerMinute = 1;

	double APerMinute = mAPerMinute/1000.0;

	sprintf(command,"@%1dS%f\r",Address, APerMinute);
	if (Send() < 0) return -1;
	if (Read() < 0) return -1;
	if (response[0] != 'S') return -1;
	return 0;

//    Notice that the resolution is 0.001 A/min
//    So what is it for the field...(???)
//    For the IPS120-20 the actual step size is 15 uA corresponding
//    to roughly to 1.5 uT
}

/////////////////////////////////////////////////
//
// SetAction
//
//  0: Hold
//  1: To Set Point
//  2: To Zero
//  3: Clamp

int IB_IPS120::SetAction(int act)
{
	if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dA%1d\r",Address, act);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'A') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   GetStatus
//      returns the Valve flow in %
//
//   Param
//      StatStr  The string that the ITC returns upon the X-command
//                Format  XnAnCnSnnHnLn
//              From this the Auto/manual settings can be derived
//
//  Return value
//            int : length of string OK
//              or < 0 : Error
//
int IB_IPS120::GetStatus(char* StatStr)  // NB Statstr should have at least
                                          // 16 elements
{
	if (Port < 0) return -1;           //if not connected return
//	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dX\r",Address);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'X') return -1;
    strncpy(StatStr, response, 15);
    StatStr[15] = 0;
    return strlen(StatStr);
}

int IB_IPS120::Activity(const char* Stat)
{
    return int(Stat[4])-48;
}

int IB_IPS120::Heater(const char* Stat)
{
    return int(Stat[8])-48;
}

int IB_IPS120::Heater()
{
	char StatStr[16];

//	The 9th character if the status string has 5 possible values:
//	0 :  heater OFF, magnet at zero
//	1 :  heater ON
//	2 :  heater OFF, magnet at field
//  5 :  heater FAULT  (low current though heater)
//  8 :  no superconducting switch present

//    sprintf(command,"@%1dX\r",Address);
//    if (Send() < 0) return -1;
//    if (Read() < 0) return -1;
//    if (response[0] != 'X') return -1;
//    strncpy(StatStr, response, 15);
//    StatStr[15] = 0;
	if (GetStatus(StatStr) > 14)
		return int(StatStr[8])-48;
}

int IB_IPS120::Quenched(const char* Stat)
{
    if (Stat[1]=='1') return 1;
    return 0;
}

int IB_IPS120::SpeedLimited(const char* Stat)
{
    if ((Stat[11] == '2') || (Stat[11] == '3')) return 1;
    return 0;
}

bool IB_IPS120::Sweeping(const char* Stat)
{
    if (Stat[11] == '1') return true;
    return false;
}

bool IB_IPS120::Sweeping()
{
    char StatStr[16];
	if (Port < 0) return -1;           //if not connected return
//	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dX\r",Address);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'X') return -1;
    strncpy(StatStr, response, 15);
    StatStr[15] = 0;
    if (StatStr[11] == '0') return false;
    return true;
}

int IB_IPS120::SetExtendedResolution(int on)
{
	if (Port < 0) return -1;
    if ((on < 0) || (on >1)) return -1;           //if not connected return
    sprintf(command,"@%1dQ%1d\r",Address, 4*on);
    if (Send() < 0) return -1;
    return 0;
}

int IB_IPS120::SetHeater(int on)
{
    if ((Port<0) || (Address == 0)) return -1;
    if (SetRemote(3) < 0) return -1;
    if ((on < 0) || (on >1)) return -1;

    sprintf(command,"@%1dH%1d\r",Address,on);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'H') return -1;
    return on;
}

int IB_IPS120::Test()
{
	if (Port < 0) return -1;                   // if not connected return
	if (SetRemote(3) == -1) return -1;         // Set Remote & Unlocked
	sprintf(command,"@%1dV\r", Address);       // Request Version and ID
	if (Send() < 0) return -1;
	if (Read() < 0) return -1;
	if (strncmp(response,"IPS120-20",9) == 0)
	{
		// test for maximum current
		// Initialize at 9 T values (safe)
		// but set to 12.5 T values if maxcurrent > 100 A?


		return 0;   // Need to test
	}                                                   // More complete ID
	 else
//  change for sweepcoil power supply
//		if (strncmp(response,"IPS120-10",9)==0) // if 17T magnet

		if ((strncmp(response,"IPS180-20",9)==0) || (strncmp(response,"IPS    Ve",9)==0))// if 17T magnet
		{
			AperT = 8.0224706;
			MaxSpeed = 4.018;
			MaxCurrent = 119.535; //14.9 maximum field
//			MaxCurrent = 116.326; //14.5 maximum field
			SafeSpeed =  4.018;
			Hysteresis = 20.0;
			return 1;
		}
	  else
//  change for Hills power supply
//		if (strncmp(response,"IPS120-10",9)==0) // if 17T magnet

		if (strncmp(response,"IPS120-10",9)==0) // if 17T magnet
		{
			AperT = 8.0224706;
			MaxSpeed = 4.018;
			MaxCurrent = 119.535; //14.9 maximum field
//			MaxCurrent = 101.382; //11.98 Tesla
			SafeSpeed =  4.018;
			Hysteresis = 3.5;
			return 1;
		}
    return -1;
//  The more complete responses are:
//  IPS180-20  Version 3.03  (c) OXFORD 1994     17 T magnet power supply
//  IPS120-10  Version 3.03  (c) OXFORD 1994     17 T sweepcoil
//  IPS120-20  Version 3.07  (c) OXFORD 1996     9T magnet power supply
//                                       as well as 12.5 T power supply !!
//  IPS    Version 4.01(c) 
}
///////////////////////////////////////
//
//  Constructor
//
// Parameters
//      _Port    :  The serial Port (ex COM1=0, COM2=1, etc)
//      _Address :  The Isobus address (1..9)
//
IB_IPS180::IB_IPS180(int _Port, int _Address)
    : IsoBusDevice(_Port,_Address), AperT(8.0224706), MaxSpeed(4.018),
				SafeSpeed(4.018), MaxCurrent(119.535)
//    Note 14.9  T maximum field
{}

///////////////////////////////////////
//
//  SetRemote
//
//  Parameters
//    mode:   0 local & locked
//            1 Remote & locked
//            2 Local & unlocked
//            3 Remote & unlocked
//
//  Return value
//            0    OK
//            -1   Error
//
int IB_IPS180::SetRemote(int mode)
{
    if ( (mode<0) || (mode>3) ) mode = 0;
    sprintf(command,"@%1dC%1d\r", Address, mode);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'C') return -1;
    return 0;
}

/////////////////////////////////////////////////
//
//  GetParameter(int n)
//
//  Returns the following parameters
//  n=0     Demand current (A)
//  n=1     Measured Power Supply Voltage (V)
//  n=2     Measured Magnet Current(A)
//  n=3     unused
//  n=4     Demand current (A) (equals n=0)
//  n=5     Set Point: Target current (A)
//  n=6     Current Sweep Rate (A/min)
//  n=7     Demand field (T)
//  n=8     Set Point: Target Field (T)
//  n=9     Field Sweep Rate (T/min)
//  n=10    DAC zero offset (A)
//  n=11    Channel 1 freq./4
//  n=12    Channel 2 freq./4
//  n=13    Channel 3 freq./4
//  n=14    Demand current (A) (equals n=0)
//  n=15    Software Voltage Limit (V)
//  n=16    Persistent Magnet Current(A)
//  n=17    Trip current (A)
//  n=18    Persistent Magnet Field (T)
//  n=19    Trip Field (T)
//  n=20    Switch Heater Current (mA)
//  n=21    Safe Current Limit, most negative(A)
//  n=22    Safe Current Limit, most positive(A)
//  n=23    Lead Resistance (mOhm)
//  n=24    Magnet Inductance (H)

double IB_IPS180::GetParameter(int n)
{
    if (Port < 0) return -96.99;           //if not connected return
//	if (SetRemote(3) == -1) return -1.0; // Set Remote & Unlocked

    sprintf(command,"@%1dR%1d\r",Address, n);
    if (Send() < 0) return -99.99;
    if (Read() < 0) return -98.99;
    if (response[0] != 'R') return -97.99;
    return atof(response+1);
}

double IB_IPS180::GetSpeed()
{
    double TperMin;
    if ((TperMin=GetParameter(9)) < 0) return -1.0;
    return TperMin*1000.0/60.0;  // return mT/sec
}

double IB_IPS180::GetField()
{
    double B;
    B=GetParameter(7);
    return B;// return T
}

double IB_IPS180::GetTargetField()
{
    double B;
    B=GetParameter(8);
    return B;// return T
}

double IB_IPS180::GetCurrent()
{
    double Amps;
    Amps=GetParameter(0);
    return Amps;// return T
}

int IB_IPS180::SetField(double B)
{
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    if (B*AperT > MaxCurrent) B = MaxCurrent/AperT;
    if (B*AperT < -1.0*MaxCurrent) B=  -1.0*MaxCurrent/AperT;
    sprintf(command,"@%1dJ%f\r",Address, B);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'J') return -1;
    return 0;
}

int IB_IPS180::SetCurrent(double Amp)
{
    if (Amp > MaxCurrent) Amp = MaxCurrent;
    if (Amp < -1.0*MaxCurrent) Amp = -1.0*MaxCurrent;
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dI%f\r",Address, Amp);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'I') return -1;
    return 0;
}

int IB_IPS180::SetAmpSweepSpeed(double AperMin)
{
    if (AperMin > MaxSpeed) AperMin = MaxSpeed;
    if (AperMin < 0.0) AperMin = 0.0;
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dS%f\r",Address, AperMin);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'S') return -1;
    return 0;
}

int IB_IPS180::SetFieldSweepSpeed(double mT_per_s)
{
    double BPerMinute = mT_per_s*60.0/1000.0;
    if (BPerMinute > MaxSpeed/AperT ) BPerMinute = MaxSpeed/AperT;
    if (BPerMinute < 0.0) BPerMinute = 0.0;
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dT%f\r",Address, BPerMinute);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'T') return -1;
    return 0;
}

/////////////////////////////////////////////////
//
// SetAction
//
//  0: Hold
//  1: To Set Point
//  2: To Zero
//  3: Clamp

int IB_IPS180::SetAction(int act)
{
    if (Port < 0) return -1;           //if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dA%1d\r",Address, act);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'A') return -1;
    return 0;
}

/////////////////////////////////////////////
//
//   GetStatus
//      returns the Valve flow in %
//
//   Param
//      StatStr  The string that the ITC returns upon the X-command
//                Format  XnAnCnSnnHnLn
//              From this the Auto/manual settings can be derived
//
//  Return value
//            int : length of string OK
//              or < 0 : Error
//
int IB_IPS180::GetStatus(char* StatStr)  // NB Statstr should have at least
                                          // 16 elements
{
	if (Port < 0) return -1;           //if not connected return
//	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dX\r",Address);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'X') return -1;
    strncpy(StatStr, response, 15);
    StatStr[15] = 0;
    return strlen(StatStr);
}

int IB_IPS180::Activity(const char* Stat)
{
    return int(Stat[4])-48;
}

int IB_IPS180::Heater(const char* Stat)
{
    return int(Stat[8])-48;
}

int IB_IPS180::Heater()
{
    char StatStr[16];
    sprintf(command,"@%1dX\r",Address);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'X') return -1;
    strncpy(StatStr, response, 15);
    StatStr[15] = 0;

    return int(StatStr[8])-48;
}

int IB_IPS180::Quenched(const char* Stat)
{
    if (Stat[1]=='1') return 1;
    return 0;
}

int IB_IPS180::SpeedLimited(const char* Stat)
{
    if ((Stat[11] == '2') || (Stat[11] == '3')) return 1;
    return 0;
}

bool IB_IPS180::Sweeping(const char* Stat)
{
    if (Stat[11] == '1') return true;
    return false;
}

bool IB_IPS180::Sweeping()
{
    char StatStr[16];
	if (Port < 0) return -1;           //if not connected return
//	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked

    sprintf(command,"@%1dX\r",Address);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'X') return -1;
    strncpy(StatStr, response, 15);
    StatStr[15] = 0;
    if (StatStr[11] == '0') return false;
    return true;
}

int IB_IPS180::SetExtendedResolution(int on)
{
	if (Port < 0) return -1;
    if ((on < 0) || (on >1)) return -1;           //if not connected return
    sprintf(command,"@%1dQ%1d\r",Address, 4*on);
    if (Send() < 0) return -1;
    return 0;
}

int IB_IPS180::SetHeater(int on)
{
    if ((Port<0) || (Address == 0)) return -1;
    if (SetRemote(3) < 0) return -1;
    if ((on < 0) || (on >1)) return -1;

    sprintf(command,"@%1dH%1d\r",Address,on);
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    if (response[0] != 'H') return -1;
    return on;
}

int IB_IPS180::Test()
{
	if (Port < 0) return -1;           // if not connected return
	if (SetRemote(3) == -1) return -1; // Set Remote & Unlocked
    sprintf(command,"@%1dV\r", Address);       //Set Remote
    if (Send() < 0) return -1;
    if (Read() < 0) return -1;
    return strncmp(response,"IPS180",6);
}


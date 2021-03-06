//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "SRS860.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
SRS860::SRS860(int _addr)
	:IEEEdevice(_addr)
{}

double SRS860::ReadOutput(int channel)
{
    if (address == 0) return 0.0;
    if ((channel<1) || (channel>4)) return 0.0;
	sprintf(command,"OUTP? %1d\n",channel-1);
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
        return -1.0;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
          else
          {
			  return 1000.0*atof(response);        // in mV
          }
      }
}

int SRS860::ReadXY(double *x, double *y)
{
	if (address == 0)
	{
		*x = 0.0;
		*y = rand();
		return 0;
	}
	sprintf(command,"SNAP? 0,1\n");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
			  sscanf(response,"%lf,%lf",x,y);
			  (*x) *= 1000.0;   // return in mV
			  (*y) *= 1000.0;
              return 2;
          }
      }
}

double SRS860::GetSignalPhase()
{
	if (address == 0) return -1.0;
	sprintf(command,"OUTP? 3\n");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1.0;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
		  else
		  {
			  return atof(response);
		  }
	  }
}

int SRS860::Test()
{
	if (address == 0) return 0;
	sprintf(command,"*IDN?\n");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		  else
		  {
			  return strncmp(response,"Stanford_Research_Systems,SR8",29);
		  }
      }
}

int SRS860::SetDAC(int n, int mV)
{
	if (address == 0) return -1;
	if ((n<0) || (n>4)) return -1;
	double Volt = double(mV)/1000.0;
	sprintf(command,"AUXV%1d,%6.3f\r\n",n,Volt);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS830");
		return -1;
	}
	return 0;
}

int SRS860::SetRemote(int remote)
{
	if (address == 0) return -1;
	if ((remote<0) || (remote>2)) return -1;
	sprintf(command,"LOCL%1d\r\n",remote);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS830");
		return -1;
	}
	return 0;
}

double SRS860::GetRefFrequency()
{
	if ( address == 0 ) return -1.0;
	sprintf(command,"FREQ?\n");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1.0;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
		  else
		  {
			  return atof(response);
		  }
	  }
}

double SRS860::GetPhase()
{
	if ( address == 0 ) return -1.0;
	sprintf(command,"PHAS?\n");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1.0;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
		  else
		  {
			  return atof(response);
		  }
	  }
}

double SRS860::GetRefAmp()
{
	if ( address == 0 ) return -1.0;
	sprintf(command,"SLVL?\n");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1.0;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
		  else
		  {
			  return atof(response);
		  }
	  }
}

double SRS860::SetModulationAmp(double amp)
{
	if ( address == 0 ) return -1.0;
	if (amp<0.002) amp = 0.002;
	if (amp>5.0)   amp = 5.0;
	sprintf(command,"SLVL %5.3f\n",amp);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1.0;
	}
	else return GetRefAmp();
}

double SRS860::SetModulationFreq(double f)
{
	if ( address == 0 ) return -1;
	if (f<0) f=0.00001;
	if (f>500000) f = 500000.0;
	sprintf(command,"FREQ %6f\n",f);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1.0;
	}
	else return GetRefFrequency();
}

int SRS860::SetModulationPhase(double ph)
{
	if ( address == 0 ) return -1;
	while (ph<-180.0) ph += 360;
	while (ph>180.0) ph -= 360;
	sprintf(command,"PHAS %07.2f\n",ph);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1;
	}
	else return 0;
}


int SRS860::MeasSensitivity()
{
	if ( address == 0 ) return -1;
	sprintf(command,"SCAL?\n");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1.0;
		  else
			  return atoi(response);
	  }
}

int SRS860::SetSensitivity(int sens)
{
	if ( address == 0 ) return -1;
	if ((sens<0) || (sens>27)) return -1;
	sprintf(command,"SCAL %2d\n",sens);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1;
	}
	else return sens;
}

int SRS860::GetSensitivity(char* sens)
{
	if (address == 0) return -1;
	int SensitivityCode = MeasSensitivity();
	switch (SensitivityCode)
	{
		case 27: strcpy(sens, "  1 nV"); break;
		case 26: strcpy(sens,"  2 nV"); break;
		case 25:	strcpy(sens,"  5 nV"); break;
		case 24:	strcpy(sens," 10 nV"); break;
		case 23:	strcpy(sens," 20 nV"); break;
		case 22:	strcpy(sens," 50 nV"); break;
		case 21:	strcpy(sens,"100 nV"); break;
		case 20:	strcpy(sens,"200 nV"); break;
		case 19:	strcpy(sens,"500 nV"); break;
		case 18:	strcpy(sens,"  1 uV"); break;
		case 17:	strcpy(sens,"  2 uV"); break;
		case 16:	strcpy(sens,"  5 uV"); break;
		case 15:	strcpy(sens," 10 uV"); break;
		case 14:	strcpy(sens," 20 uV"); break;
		case 13:	strcpy(sens," 50 uV"); break;
		case 12:	strcpy(sens,"100 uV"); break;
		case 11:	strcpy(sens,"200 uV"); break;
		case 10:	strcpy(sens,"500 uV"); break;
		case  9:	strcpy(sens,"  1 mV"); break;
		case  8:	strcpy(sens,"  2 mV"); break;
		case  7:	strcpy(sens,"  5 mV"); break;
		case  6:	strcpy(sens," 10 mV"); break;
		case  5:	strcpy(sens," 20 mV"); break;
		case  4:	strcpy(sens," 50 mV"); break;
		case  3:	strcpy(sens,"100 mV"); break;
		case  2:	strcpy(sens,"200 mV"); break;
		case  1:	strcpy(sens,"500 mV"); break;
		case  0:	strcpy(sens,"  1  V"); break;

		default: strcpy(sens,"nondef"); break;
	}
	sens[6] = 0;
	return SensitivityCode;
}

double SRS860::RealSensitivity()
{
	if (address == 0) return -1.0;
	int SensitivityCode = MeasSensitivity();
	double Sreal;
	switch (SensitivityCode)
	{
		case 27: Sreal = 1e-9; break;
		case 26:	Sreal = 2e-9; break;
		case 25:	Sreal = 5e-9; break;
		case 24:	Sreal = 10e-9; break;
		case 23:	Sreal = 20e-9; break;
		case 22:	Sreal = 50e-9; break;
		case 21:	Sreal = 100e-9; break;
		case 20:	Sreal = 200e-9; break;
		case 19:	Sreal = 500e-9; break;
		case 18:	Sreal = 1e-6; break;
		case 17:	Sreal = 2e-6; break;
		case 16:	Sreal = 5e-6; break;
		case 15:	Sreal = 10e-6; break;
		case 14:	Sreal = 20e-6; break;
		case 13:	Sreal = 50e-6; break;
		case 12:	Sreal = 100e-6; break;
		case 11:	Sreal = 200e-6; break;
		case 10:	Sreal = 500e-6; break;
		case 9:	Sreal = 1e-3; break;
		case 8:	Sreal = 2e-3; break;
		case 7:	Sreal = 5e-3; break;
		case 6:	Sreal = 10e-3; break;
		case 5:	Sreal = 20e-3; break;
		case 4:	Sreal = 50e-3; break;
		case 3:	Sreal = 100e-3; break;
		case 2:	Sreal = 200e-3; break;
		case 1:	Sreal = 500e-3; break;
		case 0:	Sreal = 1.0; break;
		default: Sreal = 1e-3; break;
	}
	return Sreal;
}


int SRS860::MeasTimeConstant()
{
	if ( address == 0 ) return -1;
	sprintf(command,"OFLT?\n");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS830");
		return -1;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		  else
		  {
			  return atoi(response);
		  }
	  }
}

int SRS860::MeasTimeConstant(double *tc)
{
	if ( address == 0 ) return -1;
	sprintf(command,"OFLT?\n");
	status = GPIBSendCommand(address, command);
	int TCcode;
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS830");
		return -1;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		  else
		  {
			  TCcode = atoi(response);
		  }
	  }
	if ((TCcode < 0) || (TCcode >21))
	{
		*tc = 0.0;
		return -1;
	}
	else if (tc != NULL)
	switch (TCcode)
	{
		case 0: *tc = 1e-6; break;
		case 1: *tc = 3e-6; break;
		case 2: *tc = 1e-5; break;
		case 3: *tc = 3e-5; break;
		case 4: *tc = 1e-4; break;
		case 5: *tc = 3e-4; break;
		case 6: *tc = 1e-3; break;
		case 7: *tc = 3e-3; break;
		case 8: *tc = 1e-2; break;
		case 9: *tc = 3e-2; break;
		case 10: *tc = 1e-1; break;
		case 11: *tc = 3e-1; break;
		case 12: *tc = 1.0; break;
		case 13: *tc = 3.0; break;
		case 14: *tc = 10.0; break;
		case 15: *tc = 30.0; break;
		case 16: *tc = 100.0; break;
		case 17: *tc = 300.0; break;
		case 18: *tc = 1000.0; break;
		case 19: *tc = 3000.0; break;
		case 20: *tc = 10000.0; break;
		case 21: *tc = 30000.0; break;
	}
	return TCcode;
}

int SRS860::MeasSlope()
{
//  Gets the low-pass filter slope
//  Possible values:
//  0 : 6 dB/oct
//  1 : 12 dB/oct
//  2 : 18 dB/oct
//  3 : 24 dB/oct
	if ( address == 0 ) return -1;
	sprintf(command,"OFSL? \n");
	status = GPIBSendCommand(address, command);
	int Slope;
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		  else
		  {
			 Slope = atoi(response);
		  }
	  }
	return Slope;
}

int SRS860::MeasDynRes()
//  Gets the Dynamic Reserve.
//  There are three settings
//     0: High Reserve
//     1: Normal
//     2: Low Noise
//
{
/*	if ( address == 0 ) return -1;
	sprintf(command,"RMOD?\r\n");
	status = GPIBSendCommand(address, command);
	int DynRes;
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS860");
		return -1;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		  else
		  {
			 DynRes = atoi(response);
		  }
	  }
*/	return -1;
}

int SRS860::MeasFilterMode()
//  Gets the notch filters
//  0:  No notch filters
//  1:  60 Hz (50 Hz) notch filter
//  2:  120 Hz (100 Hz) notch filter
//  3:  both notch filters
{
/*	if ( address == 0 ) return -1;
	sprintf(command,"ILIN?\r\n");
	status = GPIBSendCommand(address, command);
	int Filter;
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS830");
		return -1;
	}
	  else
	  {
//        WaitUntilMessageAvailable();
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status != 0) return -1;
		  else
		  {
			 Filter = atoi(response);
		  }
	  }
*/    return 0;
}

int SRS860::GetFilters(char* line, int MaxChar)
{
//  Returns Input and output filters and
//  Dynamic reserve
	int Slope, DynRes, Filter;
	double tcc;
	if (MaxChar < 60) return -1;
	Slope = MeasSlope();
	MeasTimeConstant(&tcc);
	sprintf(line,"Tc %f s, Slope %2d/oct, ",tcc,6+6*Slope);


	DynRes = MeasDynRes();
	switch (DynRes)
	{
		case 0: strcat(line,"Dyn Res High "); break;
		case 1: strcat(line,"Dyn Res Normal "); break;
		case 2: strcat(line,"Dyn Res Low Noise "); break;
		default: strcat(line,"Dyn Res Undef "); break;
	}

	Filter = MeasFilterMode();
	switch (Filter)
	{
		case 0: strcat(line,".\n\0"); break;
		case 1: strcat(line,", Line filter. \n\0"); break;
		case 2: strcat(line,", 2xLine filter. \n\0"); break;
		case 3: strcat(line,", 1x, 2x Line Filter. \n\0"); break;
	}
	return Filter;
}

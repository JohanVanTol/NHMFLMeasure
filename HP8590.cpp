//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "HP8590.h"
#include "Valid.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

HP8590::HP8590(int _addr)
    : IEEEdevice(_addr)
{}

int HP8590::GetData(int mode, DataArray *NewData)
{
    DataArray *Data;
    Data = NULL;    // We don't initialize the temporary data yet

    if (address == 0)
    {
        int npoints = 401;
        Data = new DataArray(npoints);
        Simulate(npoints, Data);
        *NewData = *Data;   // if in test mode or not connected
        return Data->Getn();
    }

    double StartF, StopF, RefLevel;
    Remote();
// Set the source: not implemented yet
// Get the Start X-value
    sprintf(command,"FA?;");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              StartF = atof(response);
          }
      }

//  Get the End Frequency
    sprintf(command,"FB?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              StopF = atof(response);
          }
      }

// Get the Reference-value
    sprintf(command,"AUNITS DBM;RL?;");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              RefLevel = atof(response);
          }
      }


//  Get the Data
    int NData = 802;

    sprintf(command,"TDF B;MDS W;TRA?;");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}

    unsigned char *data;
    data = new unsigned char[NData];
    char dummy[8];

//  Here finally we read the data and store it in the character-array data
    GPIBReceive(address, data, NData);
    GPIBReceive(address, dummy, 1L);   // read closing character

    if (Data != NULL) delete Data;
    Data = new DataArray(NData);
    DataPoint Point(1);
    double dBm;
    int Level;
    for (int i=0; i<NData;i+=2)
    {
        Level = data[i];
        Level *= 256;
        Level += data[i+1];
        Point.Set(0,1e-6*(StartF + double(i) * (StopF-StartF)/802));
        dBm = (double(Level) - 8000.0)*0.01 + RefLevel;
        Point.Set(1,dBm);
        Data->Add(Point);
    }
    delete[] data;

    *NewData = *Data;
    Local();
    return NData;
}

int HP8590::Simulate(int npts, DataArray *Data)
{
// Only used for program test purposes, when device
//  is not attached to computer

    DataPoint P(2);
    for (int i=0; i<npts;i++)
    {
        P.Set(0,i);
        P.Set(1,cos(6.28*i/npts));
        Data->Add(P);
    }
    return Data->Getn();
}

int HP8590::Test()
{
//  uses the ID command, and tests for the right ID
//  The HP that we have sends back "HP8590L"

    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    Remote();
    sprintf(command,"ID;");     // ask ID string
   	status = GPIBSendCommand(address, command);
    if (status == 0)   // if send went OK
   	{
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status == 0)
        {
          return strncmp(response,"HP859",5); // only 5 first characters
            Local();
        }
    }
	ErrorCode = status;
    strcpy(ErrorMessage,"IEEE error in sending to HP");
    return -1;
}

int HP8590::Trigger()
{
//    Uses the TS command (Take Sweep)
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    sprintf(command,"TS;");     // ask ID string
   	status = GPIBSendCommand(address, command);
    Local();
    return status;
}

int HP8590::TriggerMode(int mode)
{
//    Selects single or continuous mode
//      Mode = 0   single sweep
//      mode = 1   cont sweep

    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    switch (mode)
    {
        case 0:
            sprintf(command,"SNGLS;"); break; //single sweep
        case 1:
            sprintf(command,"CONTS;"); break; //continuous sweep
        default:
            sprintf(command,"CONTS;"); break; //continuous sweep

    }
    Remote();
// Single trigger
   	status = GPIBSendCommand(address, command);
    Local();
    return status;
}

int HP8590::Stop()
{
    return -1;
}

int HP8590::Local()
{
    SetLocal(address);
    return 0;
}


int HP8590::Remote()
{
    SetRemote(address);
    return 0;
}

int HP8590::Clear(int trace)
{
//  uses the CLRW  (clear write) command
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    Remote();
    switch (trace)
    {
        case 0:
            sprintf(command,"CLRW TRA;"); break; //single sweep
        case 1:
            sprintf(command,"CLRW TRB;"); break; //continuous sweep
        case 2:
            sprintf(command,"CLRW TRC;"); break; //continuous sweep
        default:
            sprintf(command,"CLRW TRA;"); break; //single sweep
    }

    status = GPIBSendCommand(address, command);

    Local();
    return status;
}


double HP8590::ResBW()    // returns Resolution Bandwidth in kHz
{
    if (address == 0) return 0.0;

    int Hertz;
    AnsiString Resp;

    sprintf(command,"RB?;");
    status = GPIBSendCommand(address, command);
    if (status == 0)   // if send went OK
   	{
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status == 0)
        {
          Resp = response;
          if (*(Resp.AnsiLastChar()) == '\r') *(Resp.AnsiLastChar()) = 0;
          if ( ValidInt(Resp, &Hertz));
          return double(Hertz)/1000.0;
        }
    }
	ErrorCode = status;
    strcpy(ErrorMessage,"IEEE error in sending to HP");
    return status;
}

double HP8590::VideoBW()    // return Video Bandwidth in kHz
{
    if (address == 0) return 0.0;

    int BandWidth;
    AnsiString Resp;

    sprintf(command,"VB?;");
    status = GPIBSendCommand(address, command);
    if (status == 0)   // if send went OK
   	{
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status == 0)
        {
          Resp = response;  // the response ends with a /r character
          if (*(Resp.AnsiLastChar()) == '\r') *(Resp.AnsiLastChar()) = 0;
          if ( ValidInt(Resp, &BandWidth));
          return (double)BandWidth/1000.0;
        }
    }
	ErrorCode = status;
    strcpy(ErrorMessage,"IEEE error in sending to HP");
    return status;
}


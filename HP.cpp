//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HP.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
/////////////////////////////////////////////
//
//   HP Infinium Oscilloscope HP54854A
//
HP54845A::HP54845A(int _addr)
    : IEEEdevice(_addr)
{}

int HP54845A::GetData(int channel, DataArray *NewData)
{
    DataArray *Data;
    Data = NULL;    // We don't initialize the temporary data yet

    if (address == 0)
    {
        int npoints = 100;
        Data = new DataArray(npoints);
        Simulate(npoints, Data);
        *NewData = *Data;   // if in test mode or not connected
        return Data->Getn();
    }
    double XOrigin, YOrigin, XIncrem, YIncrem;

// Set the source
    if ((channel > 0) && (channel<5))
        sprintf(command,":WAVeform:SOURce CHANnel%1d",channel);
      else
        if ((channel > 4) && (channel<9))
         sprintf(command,":WAVeform:SOURce WMEMory%1d",channel);
         else
             if ((channel > 8) && (channel<12))
            sprintf(command,":WAVeform:SOURce FUNCtion%1d",channel);
              else return -1;

   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
//  Set the desired bytorder: MSBFirst(default) or LSBFirst
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
    sprintf(command,":WAVeform:BYTeorder LSBFirst");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
//
//  Set the desired format: Options are
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
    sprintf(command,":WAVeform:FORMat WORD");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
// Get the Start X-value
    sprintf(command,":WAVeform:XORigin?");
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
              XOrigin = atof(response);
          }
      }

//  Get the start of Y value
   sprintf(command,":WAVeform:YORigin?");
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
              YOrigin = atof(response);
          }
      }

//  Get the X-value increment
    sprintf(command,":WAVeform:XINCrement?");
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
              XIncrem = atof(response);
          }
      }

// Get the Y-value increment
    sprintf(command,":WAVeform:YINCrement?");
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
              YIncrem = atof(response);
          }
      }

//  Get the Data
    sprintf(command,":WAVeform:DATA?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}

//  Find the # character:
    char dummy[256];
    while ((response[0] != '#') && (status == 0))
                 status = GPIBReceive(address, response, 1L);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}

//  Get the length of the header containg the number of bytes
    GPIBReceive(address, dummy, 1L);
    long HeaderLength = atol(dummy);
    GPIBReceive(address, dummy, HeaderLength);
    long NData = atol(dummy);

    char *data;
    data = new char[NData];
//  Here finally we read the data and store it in the character-array data
    GPIBReceive(address, data, NData);
    GPIBReceive(address, dummy, 1L);   // read closing character

    short *PShort;            // We have to transform to 16-bit integer (short)
    PShort = (short*)data;
    if (Data != NULL) delete Data;
    Data = new DataArray(NData);
    DataPoint Point(1);
    for (int i=0; i<NData;i+=2)
    {
        Point.Set(0,XOrigin+(i/2)*XIncrem);                   // REVIEW  !!
        Point.Set(1,YOrigin+(*(PShort+i/2))*YIncrem);     // REVIEW  !!
//        Point.Set(1,YOrigin+data[i]*YIncrem);
          Data->Add(Point);
    }
    delete[] data;
    *NewData = *Data;
    return NData;
}

int HP54845A::Simulate(int npts, DataArray *Data)
{
    DataPoint P(2);
    for (int i=0; i<npts;i++)
    {
        P.Set(0,i);
        P.Set(1,cos(6.28*i/npts));
        Data->Add(P);
    }
    return Data->Getn();
} 
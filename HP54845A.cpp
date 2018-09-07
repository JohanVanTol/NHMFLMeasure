//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "HP54845A.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
/////////////////////////////////////////////
//
//   HP Infinium Oscilloscope HP54854A
//
HP54845A::HP54845A(int _addr)
    : IEEEdevice(_addr)
{
    XO = new double[4];
    YO = new double[4];
    XI = new double[4];
    YI = new double[4];
    ndat = new int[4];
    databuff = new char*[4];
    for (int i=0; i<4; i++)
        databuff[i] = new char[131072];

}

HP54845A::~HP54845A()
{
    delete[] XO;
    delete[] XI;
    delete[] YO;
    delete[] YI;
    delete[] ndat;
    for (int i=0; i<4; i++)
        delete[] databuff[i];
    delete[] databuff;
}

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

    char preamble[1000];
    sprintf(command,":WAVeform:PREamble?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    GPIBReceive(address, preamble, 990);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    int dumint;
    sscanf(preamble,"%d,%d,%d,%d,%lf,%lf,%d,%lf,%lf", &dumint,
            &dumint, &dumint, &dumint, &XIncrem, &XOrigin, &dumint, &YIncrem, &YOrigin);

/*    // Get the Start X-value
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
*/
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
    dummy[1] = 0;
    long HeaderLength = atol(dummy);
    GPIBReceive(address, dummy, HeaderLength);
    dummy[HeaderLength] = 0;
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

int HP54845A::GetData(int channel, int channel2, DataArray *NewData)
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
    double XOrigin, YOrigin, XIncrem, YIncrem, YOrigin2, YIncrem2;

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


    char preamble[1000];
    sprintf(command,":WAVeform:PREamble?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    GPIBReceive(address, preamble, 990);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    int dumint;
    sscanf(preamble,"%d,%d,%d,%d,%lf,%lf,%d,%lf,%lf", &dumint,
            &dumint, &dumint, &dumint, &XIncrem, &XOrigin, &dumint, &YIncrem, &YOrigin);


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
    status = GPIBReceive(address, response, 1L);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
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
    dummy[1] = 0;
    long HeaderLength = atol(dummy);
    GPIBReceive(address, dummy, HeaderLength);
    dummy[HeaderLength] = 0;
    long NData = atol(dummy);

    char *data;
    data = new char[NData];
//  Here finally we read the data and store it in the character-array data
    GPIBReceive(address, data, NData);
    GPIBReceive(address, dummy, 1L);   // read closing character

    short *PShort;            // We have to transform to 16-bit integer (short)
    PShort = (short*)data;

// Set the second source trace
    if ((channel2 > 0) && (channel2<5))
        sprintf(command,":WAVeform:SOURce CHANnel%1d",channel2);
      else
        if ((channel2 > 4) && (channel2<9))
         sprintf(command,":WAVeform:SOURce WMEMory%1d",channel2);
         else
             if ((channel2 > 8) && (channel2<12))
            sprintf(command,":WAVeform:SOURce FUNCtion%1d",channel2);
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

//    char preamble[1000];
    sprintf(command,":WAVeform:PREamble?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    GPIBReceive(address, preamble, 990);
//    int dumint;
    double x1,x2;
    sscanf(preamble,"%d,%d,%d,%d,%lf,%lf,%d,%lf,%lf", &dumint,
            &dumint, &dumint, &dumint, &x1, &x2, &dumint, &YIncrem2, &YOrigin2);

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
//    char dummy[256];
    status = GPIBReceive(address, response, 1L);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    while ((response[0] != '#') && (status == 0))
           status = GPIBReceive(address, response, 1L);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}

//  Get the length of the header containg the number of bytes
    status = GPIBReceive(address, dummy, 1L);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}    dummy[1] = 0;
    HeaderLength = atol(dummy);
    status = GPIBReceive(address, dummy, HeaderLength);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    dummy[HeaderLength] = 0;
    NData = atol(dummy);

    char *data2;
    data2 = new char[NData];
//  Here finally we read the data and store it in the character-array data
    GPIBReceive(address, data2, NData);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    GPIBReceive(address, dummy, 1L);   // read closing character
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
	}
    short *PShort2;            // We have to transform to 16-bit integer (short)
    PShort2 = (short*)data2;

    if (Data != NULL) delete Data;
    Data = new DataArray(NData,2);
    DataPoint Point(2);
    for (int i=0; i<NData;i+=2)
    {
        Point.Set(0,XOrigin+(i/2)*XIncrem);                   // REVIEW  !!
        Point.Set(1,YOrigin+(*(PShort+i/2))*YIncrem);     // REVIEW  !!
        Point.Set(2,YOrigin2+(*(PShort2+i/2))*YIncrem2);     // REVIEW  !!
//        Point.Set(1,YOrigin+data[i]*YIncrem);
          Data->Add(Point);
    }


    delete[] data;
    delete[] data2;
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

int HP54845A::Test()
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    sprintf(command,"*IDN?");     // ask ID string
   	status = GPIBSendCommand(address, command);
    if (status == 0)
   	{
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status == 0)
          return strncmp(response,"HEWLETT-PACKARD,54845A",22);
    }
	ErrorCode = status;
    strcpy(ErrorMessage,"IEEE error in sending to HP");
    return -1;

}

int HP54845A::Run()
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    sprintf(command,"RUN");     // ask ID string
   	status = GPIBSendCommand(address, command);

    return status;
}

int HP54845A::Stop()
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    sprintf(command,"STOP");     // ask ID string
   	status = GPIBSendCommand(address, command);

    return status;
}

int HP54845A::Clear()
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    sprintf(command,"CDISplay");     // ask ID string
   	status = GPIBSendCommand(address, command);

    return status;
}

int HP54845A::GetAverage()
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    int Aver;
    sprintf(command,":ACQ:AVER?");     // ask ID string
   	status = GPIBSendCommand(address, command);
    if (status == 0)
   	{
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status == 0)
           if (strncmp(response,"0",1)==0) return 1;
        //  If averaging off: return 1 (single sweep)
      }
	  else
      {
        ErrorCode = status;
        strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
    }
    sprintf(command,":ACQ:AVER:COUNT?");     // ask ID string
   	status = GPIBSendCommand(address, command);
    if (status == 0)
   	{
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status == 0)
           if (sscanf(response,"%d",&Aver)==1) return Aver;
                   else  return -1;
      }
	  else
      {
        ErrorCode = status;
        strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
    }
    return 0;
}

int HP54845A::SetAverage(int nAver)
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected

    if (nAver < 2) sprintf(command,"ACQ:AVER 0");     // ask ID string
      else
      {
        sprintf(command,"ACQ:AVER 1");
       	status = GPIBSendCommand(address, command);
        if (status != 0)
        {
            ErrorCode = status;
            strcpy(ErrorMessage,"IEEE error in sending to HP");
            return -1;
        }
        if (nAver > 4096) nAver = 4096;  // Maximum averages
        sprintf(command,"ACQ:AVER:COUNT %d",nAver);
    }
   	status = GPIBSendCommand(address, command);
    return status;
}

int HP54845A::SetDelay(double _delay)
{
 //   if (address == 0) return 0;   // return 0 = OK if specified as not connected

	sprintf(command,"TIM:DEL %9.4e", _delay);     // ask ID string

	if (address != 0) status = GPIBSendCommand(address, command);
    return status;
}

int HP54845A::GetCount()
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    int Count;
    sprintf(command,":WAV:COUNT?");     // ask number of counts
                                // Be careful: if the Waveform source
                                // is not set correctly, a zero will be returned
   	status = GPIBSendCommand(address, command);
    if (status == 0)
   	{
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status == 0)
           if (sscanf(response,"%d",&Count)==1) return Count;
               else return 0;
      }
	  else
      {
        ErrorCode = status;
        strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
    }
    return 0;    // just to get rid of compiler warning
}

int HP54845A::GatedValues(int nVal, DataPoint *P, double *T1, double *T2,  int chan)
{
    if (address == 0) return -1;

    if ((GetCount()) < GetAverage()) return -2;

//    Stop();

    double XOrigin, YOrigin, XIncrem, YIncrem;

// Set the source
    int channel = chan;   // We use Channel 1 here
    if ((channel > 0) && (channel<5))
        sprintf(command,":WAVeform:SOURce CHANnel%1d",channel);
      else
        if ((channel > 4) && (channel<9))
         sprintf(command,":WAVeform:SOURce WMEMory%1d",channel);
         else
             if ((channel > 8) && (channel<12))
            sprintf(command,":WAVeform:SOURce FUNCtion%1d",channel);
              else return -1;

   	status = GPIBSendCommand(address, command);  // Set the waveform channel
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:SOUR to HP");
        return -1;
	}

//  Set the desired bytorder: MSBFirst(default) or LSBFirst
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
    sprintf(command,":WAVeform:BYTeorder LSBFirst");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:BYT to HP");
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
		strcpy(ErrorMessage,"IEEE error in sending WAV:FORM WORD to HP");
        return -1;
	}

    // Get the Start X-value
    sprintf(command,":WAVeform:XORigin?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{                                // on error:
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to WAV:XOR? HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
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
		strcpy(ErrorMessage,"IEEE error in sending WAV:YOR to HP");
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
		strcpy(ErrorMessage,"IEEE error in sending WAV:XINC to HP");
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
		strcpy(ErrorMessage,"IEEE error in sending WAV:YINC? to HP");
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
		strcpy(ErrorMessage,"IEEE error in sending WAV:DATA? to HP");
        return -1;
	}

//  Find the # character:
    char dummy[256];
    while ((response[0] != '#') && (status == 0))
                 status = GPIBReceive(address, response, 1L);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in retrieving HP data");
        return -1;
	}

//  Get the length of the header containg the number of bytes
    GPIBReceive(address, dummy, 1L);
    dummy[1] = 0;
    long HeaderLength = atol(dummy);
    GPIBReceive(address, dummy, HeaderLength);
    dummy[HeaderLength] = 0;
    long NData = atol(dummy);

    char *data;
    data = new char[NData];
//  Here finally we read the data and store it in the character-array data
    GPIBReceive(address, data, NData);
    GPIBReceive(address, dummy, 1L);   // read closing character

    short *PShort;            // We have to transform to 16-bit integer (short)
    PShort = (short*)data;

    int nPoints = NData/2;

//    Now we can use these data. We'll keep them in integer format for the
//    moment.

//  What we want is to average the points between T1 and T2 and put these in
//  our DataPoint P.
//
//    Clear();
//    Run();  // Restart the averaging           NOT HERE !!!!

    int iF, iL, Sum;
    for (int i=0; i<nVal; i++)  // loop over the number of gates
    {
        // t = Xorigin+i*XIncrem    thus
        Sum = 0;
        iF = (T1[i] - XOrigin)/XIncrem;
        if (iF < 0) iF=0;
        if (iF >= nPoints) iF=nPoints-1;
        iL = (T2[i] - XOrigin)/XIncrem;
        if (iL < 0) iL=0;
        if (iL > nPoints) iL=nPoints;
        for (int k=iF; k<iL;k++)
                  Sum+= *(PShort+k);
        if ((iL-iF) > 0)
         P->Set(i+1, YIncrem*(double)Sum/(iL-iF)+YOrigin);
    }
//    P->Set(2,P->Get(1)-P->Get(2));
    delete[] data;
    return nPoints;
}

int HP54845A::RetrieveBuffer(int chan)
{
    if (address == 0) return -1;

// Set the source
    int channel = chan;   // the channel we want to read
    if ((channel>12) || (channel <1)) return -1; // I only implement real channels
                                            // not memory and function data
//    millis = GetTickCount();
//  Set the source

	if (channel <=4 )
		sprintf(command,":WAVeform:SOURce CHANnel%1d",channel);
	  else if ((channel > 8) && (channel<=12))
	  {
		sprintf(command,":WAVeform:SOURce FUNCtion%1d",channel);
		channel = 3;                              // NOT GENERAL, TEMPORARY TRICK
	  }

	status = GPIBSendCommand(address, command);  // Set the waveform channel
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:SOUR to HP");
        return -1;
	}

//    int millis, delay1, delay2;
    if ((GetCount()) < GetAverage()) return -2;   //


/*
//  Set the desired bytorder: MSBFirst(default) or LSBFirst
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
    sprintf(command,":WAVeform:BYTeorder LSBFirst");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:BYT to HP");
        return -1;
	}

//  Set the desired format: Options are
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
    sprintf(command,":WAVeform:FORMat WORD");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:FORM WORD to HP");
        return -1;
	}

/*    // Get the Start X-value
    sprintf(command,":WAVeform:XORigin?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{                                // on error:
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to WAV:XOR? HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              XO[channel-1] = atof(response);
          }
      }

//  Get the start of Y value
   sprintf(command,":WAVeform:YORigin?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:YOR to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              YO[channel-1] = atof(response);
          }
      }

//  Get the X-value increment
    sprintf(command,":WAVeform:XINCrement?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:XINC to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              XI[channel-1] = atof(response);
          }
      }

// Get the Y-value increment
    sprintf(command,":WAVeform:YINCrement?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:YINC? to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              YI[channel-1] = atof(response);
          }
      }
*/

//  Get the Data
    sprintf(command,":WAVeform:DATA?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:DATA? to HP");
        return -1;
	}

//  Find the # character:
    char dummy[256];
    while ((response[0] != '#') && (status == 0))
                 status = GPIBReceive(address, response, 1L);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in retrieving HP data");
        return -1;
	}

//  Get the length of the header containg the number of bytes
    GPIBReceive(address, dummy, 1L);
    dummy[1] = 0;
    long HeaderLength = atol(dummy);
    GPIBReceive(address, dummy, HeaderLength);
    dummy[HeaderLength] = 0;
//    delay1 = GetTickCount()-millis;
    long NData = atol(dummy);
    ndat[channel-1] = NData;
    if ( NData > 131072 ) NData = 131072; //(65536 * 2)
//  Here finally we read the data and store it in the character-array data
    status = GPIBReceive(address, databuff[channel-1], NData);
//    delay2 = GetTickCount() - millis;
    GPIBReceive(address, dummy, 1L);   // read closing character

    return NData;
}


int HP54845A::GetGateValues(int nVal, DataPoint *P, double *T1, double *T2, int chan)
{
	if (address == 0)
	{
		for (int i=0; i < nVal; i++) {
			P->SetY(i, 0.98);
		}
		return -1;
	}
	int index = chan-1;
    if ((index <0) || (index >3)) return -1;

    short *PShort;            // We have to transform to 16-bit integer (short)
    PShort = (short*)databuff[index];

    int nPoints = ndat[index]/2;

//    Now we can use these data. We'll keep them in integer format for the
//    moment.

//  What we want is to average the points between T1 and T2 and put these in
//  our DataPoint P.
//
//    Clear();
//    Run();  // Restart the averaging           NOT HERE !!!!

    int iF, iL, Sum;
    for (int i=0; i<nVal; i++)  // loop over the number of gates
    {
        // t = Xorigin+i*XIncrem    thus
        Sum = 0;

        iF = ceil((T1[i] - XO[index])/XI[index]);
        if (iF < 0) iF=0;
        if (iF >= nPoints) iF=nPoints-1;

        iL = floor((T2[i] - XO[index])/XI[index]);
        if (iL < 0) iL=0;
        if (iL > nPoints) iL=nPoints;

        for (int k=iF; k<=iL; k++)
                  Sum+= *(PShort+k);
        if ((iL-iF) >= 0)
         P->Set(i+1, YI[index]*(double)Sum/(1+iL-iF)+YO[index]);
    }
//    P->Set(2,P->Get(1)-P->Get(2));
    return nPoints;
}

int HP54845A::RetrieveOffsets()
{
    if (address == 0) return -1;
    for (int channel=1; channel<=4;channel++)
    {
    // Get the Start X-value
    sprintf(command,":WAVeform:XORigin?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{                                // on error:
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to WAV:XOR? HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              XO[channel-1] = atof(response);
          }
      }

//  Get the start of Y value
   sprintf(command,":WAVeform:YORigin?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:YOR to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              YO[channel-1] = atof(response);
          }
      }
    }
    return status;
}

int HP54845A::RetrieveScaleSettings()
{
    if (address == 0) return -1;

    char preamble[1000];
    int dumint;


    for (int channel=1; channel<=4;channel++)
    {
    // Get the Start X-value
        sprintf(command,":WAVeform:SOURce CHANnel%1d",channel);
   	    status = GPIBSendCommand(address, command);  // Set the waveform channel
        if (status != 0)
   	    {
		    ErrorCode = status;
		    strcpy(ErrorMessage,"IEEE error in sending WAV:SOUR to HP");
            return -1;
	    }

        sprintf(command,":WAVeform:PREamble?");
   	    status = GPIBSendCommand(address, command);
        if (status != 0)
   	    {
		    ErrorCode = status;
		    strcpy(ErrorMessage,"IEEE error in sending to HP");
            return -1;
	    }
        GPIBReceive(address, preamble, 990);
        if (status != 0)
   	    {
		    ErrorCode = status;
		    strcpy(ErrorMessage,"IEEE error in sending to HP");
            return -1;
	    }

        sscanf(preamble,"%d,%d,%d,%d,%lf,%lf,%d,%lf,%lf", &dumint,&dumint,
                &dumint, &dumint, &XI[channel-1], &XO[channel-1],
                        &dumint, &YI[channel-1], &YO[channel-1]);

    }
    return status;
}



int HP54845A::RetrieveIncrements()
{
    if (address == 0) return -1;
    for (int channel=1; channel<=4;channel++)
    {
//  Get the X-value increment
    sprintf(command,":WAVeform:XINCrement?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:XINC to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              XI[channel-1] = atof(response);
          }
      }

// Get the Y-value increment
    sprintf(command,":WAVeform:YINCrement?");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:YINC? to HP");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              YI[channel-1] = atof(response);
          }
      }
    }
    return status;
}

int HP54845A::SetDefaultFormat()
{
    if (address == 0) return -1;
//  Set the desired bytorder: MSBFirst(default) or LSBFirst
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
    sprintf(command,":WAVeform:BYTeorder LSBFirst");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:BYT to HP");
        return -1;
	}

//  Set the desired format: Options are
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
    sprintf(command,":WAVeform:FORMat WORD");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WAV:FORM WORD to HP");
        return -1;
	}

    return status;

}

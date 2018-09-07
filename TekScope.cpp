//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "TekScope.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
/////////////////////////////////////////////
//
//   HP Infinium Oscilloscope HP54854A
//
Tek7000::Tek7000(int _addr)
    : IEEEdevice(_addr)
{
	XO = new double[12];
	YO = new double[12];
	XI = new double[12];
	YI = new double[12];
    ndat = new int[4];
    databuff = new char*[4];
    for (int i=0; i<4; i++)
		databuff[i] = new char[2000000];

}

Tek7000::~Tek7000()
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

// Still to do
int Tek7000::GetData(int channel, DataArray *NewData)
{
    DataArray *Data;
    Data = NULL;    // We don't initialize the temporary data yet

    if (address == 0)
    {
        int npoints = 100;
        Data = new DataArray(npoints);
        Simulate(npoints, Data);
		*NewData = *Data;   // if in test mode or not connected
		delete Data;
		return NewData->Getn();
    }
	double XOrigin, YOrigin, XIncrem, YIncrem;

// Set the source
	if ((channel > 0) && (channel<5))
		sprintf(command,"DATa:SOUrce CH%1d",channel);
	  else
		if ((channel > 4) && (channel<9))
		 sprintf(command,"DATa:SOUrce REF%1d",channel-4);
		 else
			 if ((channel > 8) && (channel<13))
			sprintf(command,"DATa:SOUrce MATH%1d",channel-8);
			  else return -1;

	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:SOURCE to TekScope");
		return -1;
	}
		else ErrorCode = 0;
//  Set the desired output format: LSBFirst Binary
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	if (channel >8) {
		sprintf(command,"DATA:ENCdg SFPbinary");          //Lst significant byte first
	}
	  else 	sprintf(command,"DATA:ENCdg SRIBinary");          //Lst significant byte first
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:ENCdg to TekScope");
		return -1;
	}
//
//  Set the desired format: Options are 1, 2, 4, and 8
//    NOTE that not all options are valid for all types of channels
//	  for the moment we use 2. This works for the channels, but not for
//	  math channels
	if (channel >8) {
		sprintf(command,"WFMOutpre:BYT_Nr 4");          //Lst significant byte first
	}
	  else sprintf(command,"WFMOutpre:BYT_Nr 2");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending BYT_Nr to TekScope");
		return -1;
	}

//  Set the start and stop points. For now this is 1 and the aquisitionlength
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:START 1");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	int reclength = 10000;
	int dumint;
	int num_points;

	sprintf(command,"HORizontal:ACQLENGTH?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	GPIBReceive(address, response, 80);
	if (scanf(response, "%ld", &dumint) == 1)
		reclength = dumint;


	sprintf(command,"DATA:STOP %d", reclength);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}

	char preamble[1000];
	sprintf(command,"WFMOUTPRE:NR_PT?;YOFF?;YMULT?;XINCR?;PT_OFF?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WFMOutpre to TekScope");
		return -1;
	}


	GPIBReceive(address, preamble, 990);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
		return -1;
	}

	sscanf(preamble,"%d;%lf;%lf;%lf;%d", &num_points, &YOrigin, &YIncrem,
			&XIncrem, &dumint);
	XOrigin = dumint*XIncrem;

//  Get the Data
	sprintf(command,"CURVE?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending CURVE? to TekScope");
		return -1;
	}

//  Find the # character:
	char dummy[256];
	while ((response[0] != '#') && (status == 0))
				 status = GPIBReceive(address, response, 1L);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in receiving data from TekScope");
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
	float *Pfloat;

	if (channel > 8) {
		Pfloat = (float*)data;
		if (Data != NULL) delete Data;
		Data = new DataArray(NData/4);
		DataPoint Point(1);
		for (int i=0; i<NData;i+=4)
		{
			Point.Set(0,XOrigin+(i/4)*XIncrem);                   // REVIEW  !!
			Point.Set(1,(*(Pfloat+i/4) - YOrigin)*YIncrem);     // REVIEW  !!
//        Point.Set(1,YOrigin+data[i]*YIncrem);
			  Data->Add(Point);
		}

	}
	  else {
		PShort = (short*)data;
		if (Data != NULL) delete Data;
		Data = new DataArray(NData);
		DataPoint Point(1);
		for (int i=0; i<NData;i+=2)
		{
			Point.Set(0,XOrigin+(i/2)*XIncrem);                   // REVIEW  !!
			Point.Set(1,(*(PShort+i/2) - YOrigin)*YIncrem);     // REVIEW  !!
//        Point.Set(1,YOrigin+data[i]*YIncrem);
			  Data->Add(Point);
		}
	}
	delete[] data;
	*NewData = *Data;
	delete Data;
	return NData;

}

// Still to do
int Tek7000::GetData(int channel, int channel2, DataArray *NewData)
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
		sprintf(command,"DATa:SOUrce CH%1d",channel);
	  else
		if ((channel > 4) && (channel<9))
		 sprintf(command,"DATa:SOUrce REF%1d",channel);
		 else
			 if ((channel > 8) && (channel<12))
			sprintf(command,"DATa:SOUrce MATH%1d",channel);
			  else return -1;

	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:SOURCE to TekScope");
		return -1;
	}

//  Set the desired output format: LSBFirst Binary
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:ENCdg SRIBinary");          //Lst significant byte first
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:ENCdg to TekScope");
		return -1;
	}
//
//  Set the desired format: Options are 1, 2, 4, and 8
//    NOTE that not all options are valid for all types of channels
//	  for the moment we use 2. This works for the channels, but not for
//	  math channels
	sprintf(command,"WFMOutpre:BYT_Nr 2");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending BYT_Nr to TekScope");
		return -1;
	}

//  Set the start and stop points. For now this is 1 and the aquisitionlength
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:START 1");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	int reclength = 10000;
	int dumint;
	int num_points;

	sprintf(command,"HORizontal:ACQLENGTH?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	GPIBReceive(address, response, 80);
	if (scanf(response, "%ld", &dumint) == 1)
		reclength = dumint;


	sprintf(command,"DATA:STOP %d", reclength);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}

	char preamble[1000];
	sprintf(command,"WFMOUTPRE:NR_PT?;YOFF?;YMULT?;XINCR?;PT_OFF?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WFMOutpre to TekScope");
		return -1;
	}


	GPIBReceive(address, preamble, 990);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
		return -1;
	}

	sscanf(preamble,"%d;%lf;%lf;%lf;%d", &num_points, &YOrigin, &YIncrem,
			&XIncrem, &dumint);
	XOrigin = dumint*XIncrem;

//  Get the Data
	sprintf(command,"CURVE?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending CURVE? to TekScope");
		return -1;
	}


//  Find the # character:
	char dummy[256];
	while ((response[0] != '#') && (status == 0))
				 status = GPIBReceive(address, response, 1L);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in receiving data from TekScope");
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
// Set the source
	if ((channel > 0) && (channel<5))
		sprintf(command,"DATa:SOUrce CH%1d",channel2);
	  else
		if ((channel > 4) && (channel<9))
		 sprintf(command,"DATa:SOUrce REF%1d",channel2);
		 else
			 if ((channel > 8) && (channel<12))
			sprintf(command,"DATa:SOUrce MATH%1d",channel2);
			  else return -1;

	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:SOURCE to TekScope");
		return -1;
	}

//  Set the desired output format: LSBFirst Binary
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:ENCdg SRIBinary");          //Lst significant byte first
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:ENCdg to TekScope");
		return -1;
	}
//
//  Set the desired format: Options are 1, 2, 4, and 8
//    NOTE that not all options are valid for all types of channels
//	  for the moment we use 2. This works for the channels, but not for
//	  math channels
	sprintf(command,"WFMOutpre:BYT_Nr 2");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending BYT_Nr to TekScope");
		return -1;
	}

//  Set the start and stop points. For now this is 1 and the aquisitionlength
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:START 1");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}

	sprintf(command,"HORizontal:ACQLENGTH?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	GPIBReceive(address, response, 80);
	if (scanf(response, "%ld", &dumint) == 1)
		reclength = dumint;


	sprintf(command,"DATA:STOP %d", reclength);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}

	sprintf(command,"WFMOUTPRE:NR_PT?;YOFF?;YMULT?;XINCR?;PT_OFF?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WFMOutpre to TekScope");
		return -1;
	}


	GPIBReceive(address, preamble, 990);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
		return -1;
	}

	sscanf(preamble,"%d;%lf;%lf;%lf;%d", &num_points, &YOrigin2, &YIncrem2,
			&XIncrem, &dumint);
	XOrigin = dumint*XIncrem;

//  Get the Data
	sprintf(command,"CURVE?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending CURVE? to TekScope");
		return -1;
	}


//  Find the # character:
	while ((response[0] != '#') && (status == 0))
				 status = GPIBReceive(address, response, 1L);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in receiving data from TekScope");
		return -1;
	}


//  Get the length of the header containg the number of bytes
	GPIBReceive(address, dummy, 1L);
	dummy[1] = 0;
	HeaderLength = atol(dummy);
	GPIBReceive(address, dummy, HeaderLength);
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
		Point.Set(1,(*(PShort+i/2) - YOrigin)*YIncrem);        // REVIEW  !!
		Point.Set(2,(*(PShort2+i/2) - YOrigin2)*YIncrem2);        // REVIEW  !!
//        Point.Set(1,YOrigin+data[i]*YIncrem);
		  Data->Add(Point);
	}


	delete[] data;
	delete[] data2;
	*NewData = *Data;
	delete Data;
	return NData;
}

int Tek7000::Simulate(int npts, DataArray *Data)
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

int Tek7000::Test()
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	sprintf(command,"*IDN?");     // ask ID string
	status = GPIBSendCommand(address, command);
	if (status == 0)
	{
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status == 0)
		  return strncmp(response,"TEKTRONIX,DPO7",14);
	}
	ErrorCode = status;
	strcpy(ErrorMessage,"IEEE error in sending to HP");
	return -1;

}

int Tek7000::Run()
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	sprintf(command,"ACQUIRE:STATE RUN");     // ask ID string
	status = GPIBSendCommand(address, command);

	return status;
}

int Tek7000::Stop()
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	sprintf(command,"ACQUIRE:STATE STOP");
	status = GPIBSendCommand(address, command);

	return status;
}

int Tek7000::Clear()
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	sprintf(command,"DATA Init");
	status = GPIBSendCommand(address, command);

	return status;
}

int Tek7000::GetAverage()
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	int Aver;
	sprintf(command,"ACQuire:MODE?");     // ask ID string
	status = GPIBSendCommand(address, command);
	if (status == 0)
	{
		status = GPIBReceive(address, response, MaxResponse-1);
		if (status == 0)
		   if (strncmp(response,"AVER",4) != 0) return 1;
		//  If averaging off: return 1 (single sweep)
      }
	  else
      {
        ErrorCode = status;
        strcpy(ErrorMessage,"IEEE error in sending to HP");
        return -1;
    }
	sprintf(command,"ACQuire:NUMAVg?");     // ask ID string
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

int Tek7000::SetAverage(int nAver)
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected

	if (nAver < 2) sprintf(command,"ACQuire:MODE SAMPLE");     // ask ID string
      else
      {
		sprintf(command,"ACQuire:MODE AVERAGE");
       	status = GPIBSendCommand(address, command);
        if (status != 0)
        {
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending to HP");
			return -1;
		}
		if (nAver > 4096) nAver = 4096;  // Maximum averages
		sprintf(command,"ACQuire:NUMAVg %d",nAver);
	}
	status = GPIBSendCommand(address, command);
	return status;
}

int Tek7000::SetMathAverage(int ch, int nAver)
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected

	if (nAver < 2) nAver = 2;

	if (nAver > 4096) nAver = 4096;  // Maximum averages
	sprintf(command,"MATH%1d:NUMAVg %d",ch, nAver);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
		return -1;
	}
	return status;
}

int Tek7000::SetDelay(double _delay)
{
 //   if (address == 0) return 0;   // return 0 = OK if specified as not connected

	sprintf(command,"HORIZONTAL:DELAY:TIME %10.5e", _delay);     // ask ID string

	if (address != 0) status = GPIBSendCommand(address, command);
    return status;
}

int Tek7000::GetCount()
{
    if (address == 0) return 0;   // return 0 = OK if specified as not connected
    int Count;
	sprintf(command,"ACQ:NUMACq?");     // ask number of counts
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
		strcpy(ErrorMessage,"IEEE error in asking NUMACq from TekScope");
		return -1;
	}
	return 0;    // just to get rid of compiler warning
}

int Tek7000::GatedValues(int nVal, DataPoint *P, double *T1, double *T2,  int channel)
{
	if (address == 0) return -1;

	if ((GetCount()) < GetAverage()) return -2;

//    Stop();

	double XOrigin, YOrigin, XIncrem, YIncrem;


// Set the source
	if ((channel > 0) && (channel<5))
		sprintf(command,"DATa:SOUrce CH%1d",channel);
	  else
		if ((channel > 4) && (channel<9))
		 sprintf(command,"DATa:SOUrce REF%1d",channel);
		 else
			 if ((channel > 8) && (channel<12))
			sprintf(command,"DATa:SOUrce MATH%1d",channel);
			  else return -1;

	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:SOURCE to TekScope");
		return -1;
	}
//  Set the desired output format: LSBFirst Binary
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:ENCdg SRIBinary");          //Lst significant byte first
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:ENCdg to TekScope");
		return -1;
	}
//
//  Set the desired format: Options are 1, 2, 4, and 8
//    NOTE that not all options are valid for all types of channels
//	  for the moment we use 2. This works for the channels, but not for
//	  math channels
	sprintf(command,"WFMOutpre:BYT_Nr 2");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending BYT_Nr to TekScope");
		return -1;
	}

//  Set the start and stop points. For now this is 1 and the aquisitionlength
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:START 1");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	int reclength = 10000;
	int dumint;
	int num_points;

	sprintf(command,"HORizontal:ACQLENGTH?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	GPIBReceive(address, response, 80);
	if (scanf(response, "%ld", &dumint) == 1)
		reclength = dumint;


	sprintf(command,"DATA:STOP %d", reclength);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}

	char preamble[1000];
	sprintf(command,"WFMOUTPRE:NR_PT?;YOFF?;YMULT?;XINCR?;PT_OFF?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending WFMOutpre to TekScope");
		return -1;
	}


	GPIBReceive(address, preamble, 990);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to HP");
		return -1;
	}

	sscanf(preamble,"%d;%lf;%lf;%lf;%d", &num_points, &YOrigin, &YIncrem,
			&XIncrem, &dumint);
	XOrigin = dumint*XIncrem;

//  Get the Data
	sprintf(command,"CURVE?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending CURVE? to TekScope");
		return -1;
	}

//  Find the # character:
	char dummy[256];
	while ((response[0] != '#') && (status == 0))
				 status = GPIBReceive(address, response, 1L);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in receiving data from TekScope");
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
		 P->Set(i+1, YIncrem*((double)Sum/(iL-iF)-YOrigin));
	}
//    P->Set(2,P->Get(1)-P->Get(2));
	delete[] data;
    return nPoints;
}

int Tek7000::RetrieveBuffer(int chan)
{
    if (address == 0) return -1;
	int mathmode = 0;
	if (chan > 8)
	{
		mathmode = 1;
		chan -= 8;
	}

// Set the source
	int channel = chan;   // the channel we want to read
	if ((channel>4) || (channel <1)) return -1; // I only implement real channels
												// not memory and function data
//    millis = GetTickCount();
//  Set the source
	if (mathmode == 1)
	{
		sprintf(command,"DATA:SOURce MATH%1d",channel);
	}
	  else sprintf(command,"DATA:SOURce Ch%1d",channel);
	status = GPIBSendCommand(address, command);  // Set the waveform channel
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:SOURCE to TekScope");
		return -1;
	}
		else {ErrorCode = 0; strcpy(ErrorMessage,"No Error");}

//    int millis, delay1, delay2;
//	if ((GetCount()) < GetAverage()) return -2;   //


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
	sprintf(command,"CURVE?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending CURVE? to Tektronix");
		return -1;
	}

//  Find the # character:
	char dummy[256];
	while ((response[0] != '#') && (status == 0))
				 status = GPIBReceive(address, response, 1L);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in retrieving Tektronix data");
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
	if ( NData > 2000000 ) NData = 2000000; //(1M * 2)
//  Here finally we read the data and store it in the character-array data
	status = GPIBReceive(address, databuff[channel-1], NData);
//    delay2 = GetTickCount() - millis;
	GPIBReceive(address, dummy, 1L);   // read closing character

    return NData;
}


int Tek7000::GetGateValues(int nVal, DataPoint *P, double *T1, double *T2, int chan)
{
	if (address == 0)
	{
		for (int i=0; i < nVal; i++) {
			P->SetY(i, 0.98);
		}
		return -1;
	}
	int mathmode = 0;
	if (chan >8)
	{
		mathmode = 1;
		chan -= 8;
	}
	int index = chan-1;
	if ((index <0) || (index >3)) return -1;

	short *PShort;            // We have to transform to 16-bit integer (short)
	float *Pfloat;            // or a 4 byte real
	PShort = (short*)databuff[index];
	Pfloat = (float*)databuff[index];

	int nPoints = ndat[index]/2;
	if (mathmode == 1) {nPoints /= 2;}

//    Now we can use these data. We'll keep them in integer format for the
//    moment.

//  What we want is to average the points between T1 and T2 and put these in
//  our DataPoint P.
//
//    Clear();
//    Run();  // Restart the averaging           NOT HERE !!!!

	int iF, iL, Sum;
	double RealSum;

	for (int i=0; i<nVal; i++)  // loop over the number of gates
	{
		// t = Xorigin+i*XIncrem    thus
		Sum = 0;
		RealSum = 0.0;

		iF = ceil((T1[i] - XO[index])/XI[index]);
		if (iF < 0) iF=0;
		if (iF >= nPoints) iF=nPoints-1;

		iL = floor((T2[i] - XO[index])/XI[index]);
		if (iL < 0) iL=0;
		if (iL > nPoints) iL=nPoints;

		if (mathmode == 1)
		{
			for (int k=iF; k<=iL; k++)
				  RealSum+= *(Pfloat+k);
			if ((iL-iF) >= 0)
				P->Set(i+1, YI[index]*RealSum/(1+iL-iF)-YO[index]);
		}
			else
		 {		for (int k=iF; k<=iL; k++)
					  Sum+= *(PShort+k);
			if ((iL-iF) >= 0)
			P->Set(i+1, YI[index]*(double)Sum/(1+iL-iF)-YO[index]);
		 }
	}
//    P->Set(2,P->Get(1)-P->Get(2));
	return nPoints;
}

int Tek7000::RetrieveOffsets(int mathmode)
{
	if (address == 0) return -1;
	char preamble[1000];
	int reclength = 10000;
	int dumint;
	int num_points;

	for (int channel=1; channel<=4;channel++)
	{
		if (mathmode ==1)
		sprintf(command,"DATA:SOURce MATH%1d",channel);
		  else sprintf(command,"DATA:SOURce Ch%1d",channel);
		status = GPIBSendCommand(address, command);  // Set the waveform channel
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending DATA:SOURCE to TekScope");
			return  -1;
		}
//  Set the desired output format: LSBFirst Binary
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	if (mathmode == 1)
		sprintf(command,"DATA:ENCdg SFPbinary");          //Lst significant byte first
	  else sprintf(command,"DATA:ENCdg SRIBinary");          //Lst significant byte first
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:ENCdg to TekScope");
		return -1;
	}
//
//  Set the desired format: Options are 1, 2, 4, and 8
//    NOTE that not all options are valid for all types of channels
//	  for the moment we use 2. This works for the channels, but not for
//	  math channels
	if (mathmode == 1)
		sprintf(command,"WFMOutpre:BYT_Nr 4");
	  else sprintf(command,"WFMOutpre:BYT_Nr 2");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending BYT_Nr to TekScope");
		return -1;
	}

//  Set the start and stop points. For now this is 1 and the aquisitionlength
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:START 1");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}

	sprintf(command,"HORizontal:ACQLENGTH?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	GPIBReceive(address, response, 80);
	if (scanf(response, "%ld", &dumint) == 1)
		reclength = dumint;


	sprintf(command,"DATA:STOP %d", reclength);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}


		sprintf(command,"WFMOUTPRE:NR_PT?;YOFF?;YMULT?;XINCR?;PT_OFF?");
		status = GPIBSendCommand(address, command);
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending WFMOutpre to TekScope");
			return -1;
		}


		GPIBReceive(address, preamble, 990);
		if (status != 0)
		{
			ErrorCode = status;
			strcpy(ErrorMessage,"IEEE error in sending to HP");
			return -1;
		}

		sscanf(preamble,"%d;%lf;%lf;%lf;%d", &num_points, &YO[channel-1], &YI[channel-1],
			&XI[channel-1], &dumint);
		XO[channel-1] = -dumint*XI[channel-1];
		YO[channel-1] *= YI[channel-1];

	}
	return status;
}

int Tek7000::RetrieveScaleSettings(int mathmode)
{
	return RetrieveOffsets(mathmode);
/*	if (address == 0) return -1;

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
*/
}


/*
int Tek7000::RetrieveIncrements()
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
*/
int Tek7000::SetDefaultFormat()
{
	if (address == 0) return -1;
//  Set the desired bytorder: MSBFirst(default) or LSBFirst
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
//  Set the desired output format: LSBFirst Binary
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:ENCdg SRIBinary");          //Lst significant byte first
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:ENCdg to TekScope");
		return -1;
	}
//
//  Set the desired format: Options are 1, 2, 4, and 8
//    NOTE that not all options are valid for all types of channels
//	  for the moment we use 2. This works for the channels, but not for
//	  math channels
	sprintf(command,"WFMOutpre:BYT_Nr 2");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending BYT_Nr to TekScope");
		return -1;
	}

//  Set the start and stop points. For now this is 1 and the aquisitionlength
//    ASCII, BYTE, WORD (2byte), and LONG (4 byte)
	sprintf(command,"DATA:START 1");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	int reclength = 10000;
	int dumint;
	int num_points;

	sprintf(command,"HORizontal:ACQLENGTH?");
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}
	GPIBReceive(address, response, 80);
	if (scanf(response, "%ld", &dumint) == 1)
		reclength = dumint;


	sprintf(command,"DATA:STOP %d", reclength);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending DATA:START to TekScope");
		return -1;
	}

	return status;

}


int Tek7000::Header(int on)
{
	if (address == 0) return 0;   // return 0 = OK if specified as not connected
	if (on == 1) sprintf(command,"HEADER ON");
	   else sprintf(command,"HEADER OFF");
			status = GPIBSendCommand(address, command);

	return status;
}

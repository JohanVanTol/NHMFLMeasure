//---------------------------------------------------------------------------


#pragma hdrstop


#include "DaqBoard.h"
#include "string.h"

#define STARTSOURCE	DatsImmediate
#define STOPSOURCE	DatsScanCount
//---------------------------------------------------------------------------

#pragma package(smart_init)
//--------------------------------------------------
//

ADC::ADC()
{
    MaxVolt = 10.0;
    flag = DafBipolar;
    gain = DgainX1;
    ReCalcScale();
}

ADC::~ADC()
{}

void ADC::ReCalcScale()
{
    double Range = MaxVolt;
    for (int i=0; i<gain; i++)
        Range /= 2.0;

    scale = Range/(65535 - (flag&DafBipolar)*16384);
	         //offset should equal maxVolt for bipolar, 0 for unipolar
    offset = Range*(flag&DafBipolar)/2;
}

int ADC::SetGain(DaqAdcGain _Gn)
{
    gain = _Gn;
    ReCalcScale();
    return (int)gain;
}

double ADC::GetValue(WORD buf)
{
    return (double)buf * scale - offset;
}

IOTechDaqBoard::IOTechDaqBoard()
{
    nADC = 16;
    ch = new ADC[nADC];
    nsample = 1000;
    nchan = 0;
    channels = new DWORD[nADC];
    transbuf = new WORD[nADC*nsample];
    flags = new DWORD[nADC];
    gains = new DaqAdcGain[nADC];
    handle = -1;

}
//---------------------------------------------------
IOTechDaqBoard::~IOTechDaqBoard()
{
    delete [] ch;
    delete [] transbuf;
    delete [] channels;
    delete [] flags;
    delete [] gains;
}

int IOTechDaqBoard::SetName(char* nom)
{
    strcpy(DaqName, nom);
    return strlen(DaqName);
}
//---------------------------------------------------

int IOTechDaqBoard::Connect()     // Note that the name needs to be set first
{
    if ( (handle = daqOpen(DaqName)) > -1) return 0;
      else return -1;
}

int IOTechDaqBoard::IsOpen()
{
    if (handle >-1) return 1;
       else return 0;
}

int IOTechDaqBoard::Close()
{
    if (handle >-1) daqClose(handle);
    return 0;
}

int IOTechDaqBoard::StandardInit(int _nch, int _nread)
{
    nchan = _nch;
    nsample = _nread;

	//used to configure scan
    if (transbuf != NULL) delete[] transbuf;
    transbuf = new WORD[nchan * nsample];

//	gains[0]    = DgainX1;     //gain of X1
    for (int i=0; i<nchan; i++) channels[i] = i;
//	channels[0] = 0;           //select channel 0
	ch[channels[0]].SetFlag(DafBipolar);  //select Bipolar mode
	//used to monitor scan

    DaqAdcGain gains[16];
    DWORD flags[16];
    for (int i=0; i< nchan; i++)
    {
        gains[i] = ch[i].GetGain();
        flags[i] = ch[i].GetFlag();
    }
	//used for scaling to SI units
//	float maxVolt;
//	float scale[1];
//	float offset[1];
//	float conv_buffer[10];

	//this code will poll for the first compatible device
	//if the preferred device name is known, set devName equal here

    if ( handle > -1 ) //if connection not established, handle = -1
    {
         //scan set up
         // set # of scans to perform and scan mode
         daqAdcSetAcq(handle, DaamNShot, NULL, nsample);

         //Scan settings
         daqAdcSetScan(handle, channels, gains, flags, 1);

         //set scan rate
         daqAdcSetFreq(handle, 100);

         //Set buffer location, size and flag settings
         daqAdcTransferSetBuffer(handle, transbuf, 10, DatmUpdateSingle|DatmCycleOff);

         //Set to trigger immediatly
         daqSetTriggerEvent(handle, STARTSOURCE, (DaqEnhTrigSensT)NULL, channels[0], gains[0], flags[0],
		    	         DaqTypeAnalogLocal, 0, 0, DaqStartEvent);
         //Set to stop when the requested number of scans is completed
         daqSetTriggerEvent(handle, STOPSOURCE, (DaqEnhTrigSensT)NULL, channels[0], gains[0], flags[0],
				         DaqTypeAnalogLocal, 0, 0, DaqStopEvent);
         //begin data acquisition
    }
    return nchan;
}

double IOTechDaqBoard::AdcRead(int ch, int nsample)
{
    double ReturnValue = 0.0;
	//used to configure scan
	WORD buffer[10];
	DaqAdcGain  gains[1];
	DWORD       channels[1], flags[1];
	gains[0]    = DgainX1;     //gain of X1
	channels[0] = ch;           //select channel 0
	flags[0]    = DafBipolar;  //select Bipolar mode
	//used to monitor scan
	DWORD active, retCount;
	//used for scaling to SI units
	float maxVolt;
	float scale[1];
	float offset[1];
	float conv_buffer[10];

	//this code will poll for the first compatible device
	//if the preferred device name is known, set devName equal here

    if ( handle > -1 ) //if connection not established, handle = -1
    {
         //scan set up
         // set # of scans to perform and scan mode
         daqAdcSetAcq(handle, DaamNShot, NULL, nsample);

         //Scan settings
         daqAdcSetScan(handle, channels, gains, flags, 1);

         //set scan rate
         daqAdcSetFreq(handle, 100);

         //Set buffer location, size and flag settings
         daqAdcTransferSetBuffer(handle, buffer, nsample, DatmUpdateSingle|DatmCycleOff);

         //Set to trigger immediatly
         daqSetTriggerEvent(handle, STARTSOURCE, (DaqEnhTrigSensT)NULL, channels[0], gains[0], flags[0],
		    	         DaqTypeAnalogLocal, 0, 0, DaqStartEvent);
         //Set to stop when the requested number of scans is completed
         daqSetTriggerEvent(handle, STOPSOURCE, (DaqEnhTrigSensT)NULL, channels[0], gains[0], flags[0],
				         DaqTypeAnalogLocal, 0, 0, DaqStopEvent);
         //begin data acquisition

         daqAdcTransferStart(handle);
         daqAdcArm(handle);
         do
         {
			 Sleep(25);
            //transfer data into computer memory and halt acquisition when done
            daqAdcTransferGetStat(handle, &active, (ULONG*) &retCount);
         }
         while ( active & DaafAcqActive );

         //Disarm when completed
         daqAdcDisarm(handle);

         //convert the data to volts:
         //DaqBoards convert all data to an unsigned, 16-bit number (range 0 to 65535).  Zero corresponds
         //to the minimum voltage, which is -maxVolt if in bipolar mode, or zero if unipolar.
         //65535 corresponds to maxVolt if bipolar, or 2 * maxVolt if unipolar.  Note that a voltage
         //higher than the device's absolute range (+/-10V for DaqBoard2000 , +/-5V for other Daq* devices)
         //can damage the device.  Setting flags and gain settings which indicate a voltage higher than
         //the device's max, such as an unipolar, unscaled scan will result in an error before the scan
         //is performed.
         //
         //The following routine automatically determines the max voltage for the device used
         //and the proper scaling and offset factors for the polarity flags selected.

         maxVolt = 10.0;		//Max voltage for Daq2K is +/-10V

         for(WORD g = 0; g < 1; g++)
         {
	         //scale should equal maxVolt/65335 for unipolar, maxVolt/32767 for bipolar
	         scale[g] = maxVolt/(65535 - (flags[g]&DafBipolar)*16384);
	         //offset should equal maxVolt for bipolar, 0 for unipolar
	         offset[g] = maxVolt*(flags[g]&DafBipolar)/2;
         }

         for (WORD t = 0; t < retCount*1; t++)
	         conv_buffer[t] = buffer[t]*scale[t%1] - offset[t%1];

         //print scan data
         for(WORD i = 0; i < retCount; i++)
         {
	         for(WORD j = 0; j < 1; j++)
                ReturnValue += conv_buffer[i+j];

         }
         if (retCount > 0) ReturnValue /= retCount;
            else ReturnValue = 0.0;
         //close device connections
	}
    return ReturnValue;

}

double IOTechDaqBoard::ReadChannel(int ich)
{
    if ((ich <0) || (ich>15)) return -99.9;

    WORD buffer[1];
	if (daqAdcRd(handle, 0, buffer, ch[ich].GetGain(), ch[ich].GetFlag()) == DerrNoError)
    {
        return ch[ich].GetValue(buffer[0]);
    }
    else return -99.9;
}

int IOTechDaqBoard::Read(double* results)
{
    return -1;
}

int IOTechDaqBoard::SetGain(int ch_index, DaqAdcGain gn)
{
    if ((ch_index >= 0) && (ch_index < nADC))
            ch[ch_index].SetGain(gn);
    return ch_index;
}

int IOTechDaqBoard::ChannelInit(int _nch, int *chanarray)
{
    nchan = _nch;
    if (nchan == 0) return nchan;
    for (int i=0; i< nchan; i++)
    {
        channels[i] = chanarray[i];
		gains[i] = ch[chanarray[i]].GetGain();
        flags[i] = ch[chanarray[i]].GetFlag();
    }
    daqAdcSetScan(handle, channels, gains, flags, nchan);
    return nchan;
}

DaqAdcGain IOTechDaqBoard::GetGain(int ch_index)
{
	if ((ch_index > 0) && (ch_index < nADC))
		return ch[ch_index].GetGain();
	else return DgainX1;
}

DWORD IOTechDaqBoard::GetFlag(int ch_index)
{
	if ((ch_index > 0) && (ch_index < nADC))
		return ch[ch_index].GetFlag();
	else return 0;
}

int IOTechDaqBoard::SetAcquisition(double f, int nscans)
{
	if (handle < 0) return -1;
	DWORD before, after;
	before = 0;
	after = nscans;
	nsample = nscans;
	daqAdcSetAcq(handle, DaamNShot, before, after);


	DaqAdcTriggerSource StartSource = DatsSoftware;
	DaqAdcTriggerSource StopSource = DatsScanCount;

	daqSetTriggerEvent(handle, StartSource, (DaqEnhTrigSensT)NULL, channels[0], gains[0], flags[0],
						 DaqTypeAnalogLocal, 0, 0, DaqStartEvent);
//Set to stop when the requested number of scans is completed
	daqSetTriggerEvent(handle, StopSource, (DaqEnhTrigSensT)NULL, channels[0], gains[0], flags[0],
						 DaqTypeAnalogLocal, 0, 0, DaqStopEvent);

	FLOAT SetFreq = f;
	FLOAT ActualFreq = 0;

	daqAdcSetRate(handle, DarmFrequency, DaasPostTrig, SetFreq, &ActualFreq);

	ResetTransBuf(nchan*nscans);
	daqAdcTransferSetBuffer(handle, transbuf, nscans, DatmUpdateSingle|DatmCycleOff);

	daqAdcTransferStart(handle);
	daqAdcArm(handle);
	return 0;
}

int IOTechDaqBoard::Trigger()
{
	if (handle < 0) return -1;
	return daqAdcSoftTrig(handle);
}

int IOTechDaqBoard::WaitToComplete(int milli)
{
	if (handle < 0) return -1;
	DWORD active, retCount;
	do
	{
		Sleep(milli);
		//transfer data into computer memory and halt acquisition when done
		daqAdcTransferGetStat(handle, &active, (ULONG*) &retCount);
	}
	while ( active & DaafAcqActive );
	daqAdcDisarm(handle);
	return 0;
}

int IOTechDaqBoard::GetValues(int nch, double* val)
{
	if (nch != nchan) return -1;
	for (int k=0; k< nchan; k++) val[k] = 0.0;

	for (int i = 0; i < nsample; i++)
		for (int j = 0; j<nchan; j++)
			val[j] += ch[channels[j]].GetValue(transbuf[nchan*i+j]);

	for (int k=0; k< nchan; k++) val[k] /= (double)nsample;

	return nchan;
}

int IOTechDaqBoard::ResetTransBuf(int nfloat)
{
	if (transbuf != NULL) delete[] transbuf;
	transbuf = new WORD[nfloat];
	return nfloat;
//    for (int i=0; i<nfloat; i++) transbuf[i] = 99;
}

int IOTechDaqBoard::Arm()
{
	if (handle < 0) return -1;
	daqAdcTransferStart(handle);
	daqAdcArm(handle);
	return handle;
}

int IOTechDaqBoard::DisArm()
{
	if (handle < 0) return -1;
	daqAdcDisarm(handle);
	return handle;
}

int IOTechDaqBoard::SetIO(char A, char B, char C)
{
	if (handle < 0) return -1;
	DWORD config;
	if (handle + 1)				//a return of -1 indicates failure
	{
		//set ports A, B, and C as outputs
		daqIOGet8255Conf(handle, 0 ,0, 0, 0, &config);
		//write settings to internal register

		daqIOWrite(handle, DiodtLocal8255, Diodp8255IR, 0, DioepP2, config);

		//update ports A, B, and C
		daqIOWrite(handle, DiodtLocal8255, Diodp8255A, 0, DioepP2, A);
		daqIOWrite(handle, DiodtLocal8255, Diodp8255B, 0, DioepP2, B);
		daqIOWrite(handle, DiodtLocal8255, Diodp8255C, 0, DioepP2, C);
	 }
	 return (int)A*(int)B*(int)C;
}

int IOTechDaqBoard::InitDAC(int ch)
{
	if (handle < 0) return -1;
	daqDacSetOutputMode(handle, DddtLocal, ch, DdomBipolar);
	return ch;
}


int IOTechDaqBoard::SetDAC(int ch, int value)
{
	if (handle < 0) return -1;
	daqDacWt(handle, DddtLocal, ch, (unsigned short)value);
	return 0;
}

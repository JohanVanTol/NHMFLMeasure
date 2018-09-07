/////////////////////////////////////////////////////////////////////////
//
//  IEEEdev.CPP
//		Contains the IEEE commands for the different devices used
//		A device is represented by a class which is a subclass of
// 	    the IEEEdevice class.
//
//      This file is an adaption of the devices.cpp file which was
//      intended for the Keithley IEEE card. These function definitions
//      are valid for the National Instrument GPIB card.
//      The access method chosen is the "direct access", which means that
//      the DLL file is loaded when the program is started ( the DLL used
//      is the "gpib-32.dll" ) This is necessary because the C++ Builder
//      is not a real Borland C/C++ compiler and the supplied object file
//      for Borland C/C++ does not work for the Pascal-based C++ Builder.
//
//      General commands of the serial poll type (is the device ready
// 		to be read etc.) are defined in the main class. This may not
// 		be the best option as some devices use different ways, but this
// 		is the way it is at the moment
//
//      The declarations of the classes defined here are found in IEEEdev.h
//

#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include "decl-32.h"

#ifdef __cplusplus
}
#endif

#include "mydata.h"
#include "IEEEdev.h"        // Declarations of the IEEEdevice classes


//void found (unsigned int fluke);       /* Obtain readings from Fluke 45     */
void gpiberr(char *msg);               /* Error routine                     */

#define  MAVbit   0x10           /* Position of the Message Available bit.  */

/*char          buffer[101];       /* Data received from the Fluke 45         */
int           loop,              /* FOR loop counter and array index        */
              m,                 /* FOR loop counter                        */
              num_listeners,     /* Number of listeners on GPIB             */
              pad;               /* Primary address of listener on GPIB     */
short         SRQasserted,       /* Set to indicate if SRQ is asserted      */
              statusByte;        /* Serial Poll Response Byte               */
double        sum;               /* Accumulator of measurements             */
Addr4882_t    fluke,             /* Primary address of the Fluke 45         */
              instruments[32],   /* Array of primary addresses              */
              result[31];        /* Array of listen addresses               */



/****************************************
 ********  CODE TO ACCESS GPIB-32.DLL
 ***********************************************/

static void (__stdcall *PDevClear)(int boardID, Addr4882_t addr);
static void (__stdcall *PFindLstn)(int boardID, Addr4882_t * addrlist, PSHORT results, int limit);
static void (__stdcall *PEnableRemote)(int boardID, Addr4882_t * addrlist);
static int  (__stdcall *Pibonl)(int ud, int v);
static int  (__stdcall *Pibtmo)(int ud, int v);
static void (__stdcall *PReadStatusByte)(int boardID, Addr4882_t addr, PSHORT result);
static void (__stdcall *PReceive)(int boardID, Addr4882_t addr, PVOID buffer, LONG cnt, int Termination);
static void (__stdcall *PSend)(int boardID, Addr4882_t addr, PVOID databuf, LONG datacnt, int eotMode);
static void (__stdcall *PSendIFC)(int boardID);
static void (__stdcall *PWaitSRQ)(int boardID, PSHORT result);

/*
 *    This is private data for the language interface only so it is
 *    defined as 'static'.
 */
static HINSTANCE Gpib32Lib = NULL;
static int *Pibsta;
static int *Piberr;
static long *Pibcntl;

static int EotMode;
static int EosMode;


bool LoadGPIBDll(void)
{
   /*
    *  Call LoadLibrary to load the 32-bit GPIB DLL.  Save the handle
    *  into the global 'Gpib32Lib'.
    */
   Gpib32Lib = LoadLibrary ("GPIB-32.DLL");

   if (!Gpib32Lib)  {
      /*
       *    The LoadLibrary call failed, return with an error.
       */
      return FALSE;
   }


   /*
    *    OK, the GPIB library is loaded.  Let's get a pointer to the
    *    requested function.  If the GetProcAddress call fails, then
    *    return with an error.
    */
   Pibsta          = (int *) GetProcAddress(Gpib32Lib, (LPCSTR)"user_ibsta");
   Piberr          = (int *) GetProcAddress(Gpib32Lib, (LPCSTR)"user_iberr");
   Pibcntl         = (long *)GetProcAddress(Gpib32Lib, (LPCSTR)"user_ibcnt");

   PDevClear       = (void (__stdcall *)(int, Addr4882_t))GetProcAddress(Gpib32Lib, (LPCSTR)"DevClear");
   PFindLstn       = (void (__stdcall *)(int, Addr4882_t *, PSHORT, int))GetProcAddress(Gpib32Lib, (LPCSTR)"FindLstn");
   PEnableRemote   = (void (__stdcall *)(int, Addr4882_t *))GetProcAddress(Gpib32Lib, (LPCSTR)"EnableRemote");
   Pibonl          = (int  (__stdcall *)(int, int))GetProcAddress(Gpib32Lib, (LPCSTR)"ibonl");
   Pibtmo          = (int  (__stdcall *)(int, int))GetProcAddress(Gpib32Lib, (LPCSTR)"ibtmo");
   PReadStatusByte = (void (__stdcall *)(int, Addr4882_t, PSHORT))GetProcAddress(Gpib32Lib, (LPCSTR)"ReadStatusByte");
   PReceive        = (void (__stdcall *)(int, Addr4882_t, PVOID, LONG, int))GetProcAddress(Gpib32Lib, (LPCSTR)"Receive");
   PSend           = (void (__stdcall *)(int, Addr4882_t, PVOID, LONG, int))GetProcAddress(Gpib32Lib, (LPCSTR)"Send");
   PSendIFC        = (void (__stdcall *)(int))GetProcAddress(Gpib32Lib, (LPCSTR)"SendIFC");
   PWaitSRQ        = (void (__stdcall *)(int, PSHORT))GetProcAddress(Gpib32Lib, (LPCSTR)"WaitSRQ");


   if ((Pibsta         == NULL) ||
       (Piberr         == NULL) ||
       (Pibcntl        == NULL) ||
       (PDevClear      == NULL) ||
       (PFindLstn      == NULL) ||
       (PEnableRemote  == NULL) ||
       (Pibonl         == NULL) ||
       (Pibtmo         == NULL) ||
       (PReadStatusByte== NULL) ||
       (PReceive       == NULL) ||
       (PSend          == NULL) ||
       (PSendIFC       == NULL) ||
       (PWaitSRQ       == NULL))  {

      FreeLibrary (Gpib32Lib);
      Gpib32Lib = NULL;
      return FALSE;
   }
   else  {
      return TRUE;
   }

}  /* end of LoadDll */


void FreeGPIBDll(void)
{
   FreeLibrary (Gpib32Lib);
   Gpib32Lib = NULL;
   return;
}

int GPIBInterfaceClear()
{
    (*PSendIFC)(0);
    if ((*Pibsta) & ERR) return -1;
    return 0;
}

int GPIBSendCommand(short address, const char* buffer)
{
    SetRemote(address);
    void* buf = (void*)buffer;
    (*PSend) (0, address, buf, strlen(buffer), EotMode);
    if ((*Pibsta) & ERR) return *Pibsta;
    return 0;
}

int GPIBReceive(short address, char* Buffer, int MaxLength)
{
    (*PReceive) (0, address, (void*)Buffer,(long)MaxLength, EosMode);


    if ((*Pibcntl > 0) && (*Pibcntl < MaxLength)) Buffer[*Pibcntl-1]=0;
    if ((*Pibsta) & ERR) return *Pibsta;
    return 0;
}

int GPIBGetStatusByte()
{
    return *Pibsta;
}

int GPIBGetErrorByte()
{
    return *Piberr;
}

int GPIBStatusLine(int i, char* line)
{
    switch(i)
    {
        case 0: strcpy(line, "DCAS: Device Clear"); break;
        case 1: strcpy(line, "DTAS: Device Trigger"); break;
        case 2: strcpy(line, "LACS: Listener"); break;
        case 3: strcpy(line, "TACS: Talker"); break;
        case 4: strcpy(line, "ATN:  Attention asserted"); break;
        case 5: strcpy(line, "CIC:  Controlller in charge"); break;
        case 6: strcpy(line, "REM:  Remote"); break;
        case 7: strcpy(line, "LOK:  Lockout"); break;
        case 8: strcpy(line, "CMPL: I/O completed"); break;
        case 11: strcpy(line, "RQS:  Device requesting service"); break;
        case 12: strcpy(line, "SRQI: SRQ Interrupt received"); break;
        case 13: strcpy(line, "END:  END or EOS detected"); break;
        case 14: strcpy(line, "TIMO: Timeout"); break;
        case 15: strcpy(line, "ERR:  GPIB error"); break;
        default: strcpy(line, "unknown function "); break;
    }
    return 0;
}


int GPIBGetError(char* Errormessage)
{
    int error = GPIBGetErrorByte();
    switch (error)
    {
        case 0: strcpy(Errormessage,"0, System error "); break;
        case 1: strcpy(Errormessage,"1, Function requires GPIB board to be CIC "); break;
        case 2: strcpy(Errormessage,"2, Write function detected no Listeners  "); break;
        case 3: strcpy(Errormessage,"3, Interface board not addressed correctly "); break;
        case 4: strcpy(Errormessage,"4, Function requires GPIB board to be CIC "); break;
        case 5: strcpy(Errormessage,"5, Function requires GPIB board to be SAC "); break;
        case 6: strcpy(Errormessage,"6, I/O operation aborted   "); break;
        case 7: strcpy(Errormessage,"7, Non-existent interface board "); break;
        case 8: strcpy(Errormessage,"8, Error performing DMA  "); break;
        case 10: strcpy(Errormessage,"10, Previous operation uncompleted "); break;
        case 11: strcpy(Errormessage,"11, No capability for intended operation "); break;
        case 12: strcpy(Errormessage,"12, File system operation error "); break;
        case 14: strcpy(Errormessage,"14, Command error during device call "); break;
        case 15: strcpy(Errormessage,"15, Serial poll status byte lost "); break;
        case 16: strcpy(Errormessage,"16, SRQ remains asserted "); break;
        case 20: strcpy(Errormessage,"20, The return buffer is full "); break;
        case 21: strcpy(Errormessage,"21, Address or board is locked "); break;
        default: strcpy(Errormessage," No error "); break;
    }
    return error;
}

int GPIBGetLengthCount()
{
    return (int)*Pibcntl;
}

short GPIBGetDeviceStatus(int addr)
{
    short result;
    (*PReadStatusByte)(0, addr, &result);
    return result;
}

int GPIBSetTimeOut(int index)
{
    return (*Pibtmo)(0, index);
}

void GPIBSetEosMode(int n)
{
    switch (n)
    {
        case 0: EosMode = STOPend;break;
        case 1: EosMode = 0;break;
        default: EosMode = 0;break;
    }
}


void GPIBSetEotMode(int n)
{
    switch (n)
    {
        case 0: EotMode = NULLend;break;
        case 1: EotMode = NLend;break;
        case 2: EotMode = DABend;break;
        default: EotMode = 0;
    }
}

void SetRemote(short addr)
{
    short* AddrList = new short[2];
    AddrList[0] = addr;
    AddrList[1] = NOADDR;
    (*PEnableRemote)(0,AddrList);
}


int GPIBFindAllListeners(short* addr)
{
    int limit = 30;     // 30 is the highest possible primary address

    short* padlist = new short[limit+2];
    for (int i=0; i<= limit;i++)
        padlist[i] = (short)i;
    padlist[limit+1]= NOADDR;

    (*PFindLstn)(0, padlist, addr, limit+1);
    return (*Pibcntl);
}

void GPIBGetID(int address, char* deviceID, int MaxLength)
{
    char* buf = new char[20];
    strcpy(buf,"*IDN?");
    GPIBSendCommand(address, buf);
    GPIBReceive(address, deviceID, MaxLength);
}



/*****************************
 ************  BEGIN MAIN
 *************************************/

/*
void __cdecl main() {

    if (!LoadDll())  {
       printf ("Unable to correctly access the 32-bit GPIB DLL.\n");
       return;
    }
*/
/*
 *  Your board needs to be the Controller-In-Charge in order to find all
 *  listeners on the GPIB.  To accomplish this, the function SendIFC is
 *  called.
 */
/*    (*PSendIFC)(0);
    if ((*Pibsta) & ERR) gpiberr ("SendIFC Error");

/*
 *  Create an array containing all valid GPIB primary addresses.  Your GPIB
 *  interface board is at address 0 by default.  This  array (INSTRUMENTS)
 *  will be given to the function FindLstn to find all listeners.
 */

/*    for (loop = 0; loop <= 30; loop++) {
       instruments[loop] = (Addr4882_t)loop;
    }
    instruments[31] = NOADDR;
*/
/*
 *  Print message to tell user that the program is searching for all active
 *  listeners.  Find all of the listeners on the bus.   Store the listen
 *  addresses in the array RESULT.
 */
/*
    printf("Finding all listeners on the bus...\n");
    printf("\n");

    (*PFindLstn)(0, &instruments[1], (Addr4882_t *)result, 31);
    if ((*Pibsta) & ERR) gpiberr("FindLstn Error");
*/
/*
 *  Assign the value of IBCNT to the variable NUM_LISTENERS.
 *  Print the number of listeners found.
 */
/*
    num_listeners = (short)(*Pibcntl);

    printf("Number of instruments found = %d\n", num_listeners);
*/
/*
 *  Send the *IDN? command to each device that was found.
 *
 *  Establish a FOR loop to determine if the Fluke 45 is a listener on the
 *  GPIB.  The variable LOOP will serve as a counter for the FOR loop and
 *  as the index to the array RESULT.
 */

/*    for (loop = 0; loop <= num_listeners; loop++) {
*/
       /*
        *  Send the identification query to each listen address in the
        *  array RESULT.  The constant NLend instructs the function Send to
        *  append a linefeed character with EOI asserted to the end of the
        *  message.
        */

/*           (*PSend)(0, result[loop], "*IDN?", 5L, NLend);
           if ((*Pibsta) & ERR) gpiberr("Send Error");
*/
       /*
        *  Read the name identification response returned from each device.
        *  Store the response in the array BUFFER.  The constant STOPend
        *  instructs the function Receive to terminate the read when END
        *  is detected.
        */

/*           (*PReceive)(0, result[loop], buffer, 10L, STOPend);
           if ((*Pibsta) & ERR) gpiberr("Receive Error");
*/
       /*
        *  The low byte of the listen address is the primary address.
        *  Assign the variable PAD the primary address of the device.
        *  The macro GetPAD returns the low byte of the listen address.
        */

//           pad = GetPAD(result[loop]);

       /*
        *  The string returned by ibrd is a binary string whose length is
        *  specified by the byte count in IBCNTL. However, the Fluke 45
        *  sends measurements in the form of ASCII strings. Because of this,
        *  it is possible to add a NULL character to the end of the data
        *  received and use the PRINTF function to display the ASCII Fluke
        *  45 response. The following code illustrates that.
        */

//           buffer[(*Pibcntl)] = '\0';
//           printf("The instrument at address %d is a %s\n", pad, buffer);

       /*
        *  Determine if the name identification is the Fluke 45.  If it is
        *  the Fluke 45, assign PAD to FLUKE,  print message that the
        *  Fluke 45 has been found, call the function FOUND, and terminate
        *  FOR loop.
        */

//           if (strncmp(buffer, "FLUKE, 45", 9) == 0) {
//              fluke = (Addr4882_t)pad;
//              printf("**** We found the Fluke ****\n");
//              found(fluke);
//              break;
//           }

//    }      /*  End of FOR loop */

//    if (loop > num_listeners) printf("Did not find the Fluke!\n");

/*  Call the ibonl function to disable the hardware and software.           */

//    (*Pibonl) (0, 0);


//    FreeDll();
//    return;
//}


/* ===========================================================================
 *                      Function FOUND
 *  This function is called if the Fluke 45 has been identified as a listener
 *  in the array RESULT.  The variable FLUKE is the primary address of the
 *  Fluke 45.  Ten measurements are read from the fluke and the average of
 *  the sum is calculated.
 * ===========================================================================
 */

//void found(unsigned int fluke) {

/*
 *  Reset the Fluke 45 using the functions DevClear and Send.
 *  DevClear will send the GPIB Selected Device Clear (SDC) command message
 *  to the Fluke 45.
 */

//    (*PDevClear)(0, (Addr4882_t)fluke);
//    if ((*Pibsta) & ERR) gpiberr("DevClear Error");

/*
 *  Use the function Send to send the IEEE-488.2 reset command (*RST)
 *  to the Fluke 45.  The constant NLend, defined in DECL-32.H, instructs
 *  the function Send to append a linefeed character with EOI asserted
 *  to the end of the message.
 */

//    (*PSend)(0, (Addr4882_t)fluke, "*RST", 4L, NLend);
//    if ((*Pibsta) & ERR) gpiberr("Send *RST Error");

/*
 *  Use the function Send to send device configuration commands to the
 *  Fluke 45.  Instruct the Fluke 45 to measure volts alternating current
 *  (VAC) using auto-ranging (AUTO), to wait for a trigger from the GPIB
 *  interface board (TRIGGER 2), and to assert the IEEE-488 Service Request
 *  line, SRQ, when the measurement has been completed and the Fluke 45 is
 *  ready to send the result (*SRE 16).
 */

//    (*PSend)(0, (Addr4882_t)fluke, "VAC; AUTO; TRIGGER 2; *SRE 16", 29L, NLend);
//    if ((*Pibsta) & ERR) gpiberr("Send Setup Error");

/*  Initialized the accumulator of the 10 measurements to zero.             */

//    sum = 0.0;

/*
 *  Establish FOR loop to read the 10 measurements.  The variable m will
 *  serve as the counter of the FOR loop.
 */

//    for (m=0; m < 10 ; m++) {

       /*
        *  Trigger the Fluke 45 by sending the trigger command (*TRG) and
        *  request a measurement by sending the command "VAL1?".
        */

//           (*PSend)(0, (Addr4882_t)fluke, "*TRG; VAL1?", 11L, NLend);
//           if ((*Pibsta) & ERR) gpiberr("Send Trigger Error");

        /*
         *  Wait for the Fluke 45 to assert SRQ, meaning it is ready to send
         *  a measurement.  If SRQ is not asserted within the timeout period,
         *  call GPIBERR with an error message.  The timeout period by default
         *  is 10 seconds.
         */

//            (*PWaitSRQ)(0, &SRQasserted);
//            if (!SRQasserted) {
//                printf("SRQ is not asserted.  The Fluke is not ready.\n");
//                exit(1);
//            }

       /*
        *  Read the serial poll status byte of the Fluke 45.
        */

//           (*PReadStatusByte)(0, (Addr4882_t)fluke, &statusByte);
//           if ((*Pibsta) & ERR) gpiberr("ReadStatusByte Error");

       /*
        *  Check if the Message Available Bit (bit 4) of the return status
        *  byte is set.  If this bit is not set, print the status byte.
        */

//           if (!(statusByte & MAVbit)) {
//               printf(" Status Byte = 0x%x\n", statusByte);
//               gpiberr("Improper Status Byte");
//           }

       /*
        *  Read the Fluke 45 measurement.  Store the measurement in the
        *  variable BUFFER.  The constant STOPend, defined in DECL-32.H,
        *  instructs the function Receive to terminate the read when END
        *  is detected.
        */

//           (*PReceive)(0, (Addr4882_t)fluke, buffer, 10L, STOPend);
//           if ((*Pibsta) & ERR) gpiberr("Receive Error");

       /*
        *  The string returned by ibrd is a binary string whose length is
        *  specified by the byte count in IBCNTL. However, the Fluke 45
        *  sends measurements in the form of ASCII strings. Because of this,
        *  it is possible to add a NULL character to the end of the data
        *  received and use the PRINTF function to display the ASCII Fluke
        *  45 response. The following code illustrates that.
        */

//           buffer[(*Pibcntl)] = '\0';
//           printf("Reading :  %s\n", buffer);

       /*  Convert the variable BUFFER to its numeric value and add to the
        *  accumulator.
        */

//           sum = sum + atof(buffer);
                                                
//    }  /*  Continue FOR loop until 10 measurements are read.   */

/*  Print the average of the 10 readings.                                   */

//    printf("   The average of the 10 readings is : %f\n", sum/10);

//}



//////////////////////////////////////////////////////////////////////////
//
//	class IEEEdevice, the base class for all IEEE devices
//
////

//
// The default constructor:  creates a device with the specified address
//
IEEEdevice::IEEEdevice(int addr)
						: address(addr)
{
	ErrorCode = 0;                  // Set the errorcodes to "No error"
	strcpy(ErrorMessage, "No error");
}

//
//  Looks if there is an error. Reads errorcode and message
//
int IEEEdevice::ErrorTest(char *message)
{
	strcpy(message, ErrorMessage);
	return ErrorCode;
}

//
//  Protected version of errortest
//
int IEEEdevice::ErrorTest(char *message, int maxlen)
{
	int n = strlen(ErrorMessage);
	if (n < maxlen)
	{
		strcpy(message, ErrorMessage);
		message[n]=0;
	}
	else
	{
		strncpy(message, ErrorMessage, maxlen-1);
		message[maxlen-1]=0;
	}
	return ErrorCode;
}

//
//   Checks if device is ready to be read (checks bit 7 of status byte)
//
int IEEEdevice::ReadyToRead()      // Performs a serial poll and
{                                  // returns 1 if bit 7 is set
	if (address == 0) return 1;     // NB Not valid for all devices
  	poll = GPIBGetDeviceStatus(address);
	if (poll & 128 ) return 1;
	return 0;
}

//
//   Waits until bit 7 of status byte is set
//
int IEEEdevice::WaitBeforeRead()   //holds execution until
{                                   //bit 7 of status byte is set
	if (address == 0) return 0;      //For most (but not all!!)
	int const MaxPoll = 1000;
	int i=0;                         //devices this corresponds to
	poll = 0;                        //data available
	do                               //returns -1 on failure
	{
    	poll = GPIBGetDeviceStatus(address);
		i++;
		if (i>MaxPoll)
		{
			ErrorCode = 1;
			strcpy(ErrorMessage,"Read failed: Maximum number serial polls exceeded");
			return -1;
		}
	}	while (!(poll & 128 ));
	return 0;
}

//
int IEEEdevice::CommandPerformed()  // Tests if command is performed
{                                   // by looking at bit 0 of the
	if (address == 0) return 1;      // status byte (not valid for
   	poll = GPIBGetDeviceStatus(address);
	if (poll & 1) return 1;
	return 0;
}

//
int IEEEdevice::WaitUntilMessageAvailable()
{                             // Holds execution until bit 4 of
	if (address == 0) return 0;  // status byte is set. For Oxford
	int i=0;                   // devices this corresponds to
	poll = 0;                  // data available
	do                         // returns -1 on failure
	{
    	poll = GPIBGetDeviceStatus(address);
		i++;
		if (i>1000)
		{
			ErrorCode = 1;
			strcpy(ErrorMessage,"Read failed: Maximum number serial polls exceeded");
			return -1;
		}
	}	while (!(poll & 16 ));
	return 0;
}

int IEEEdevice::MessageAvailable()
{
	if (address == 0) return 0;  //
						// for Oxford devices this corresponds to
	poll = 0;                  // data available
									 // returns 1 if message there, if not 0
   	poll = GPIBGetDeviceStatus(address);
	if (poll & 16 ) return 1;
	return 0;
}
//
int IEEEdevice::Test()
{
	return 0;      // Virtual function needs to be redefined in the
						// different derived classes (different devices)
}




/*
//////////////////////////////////////////////////////////////////////////
//
//	class K175    A Keithley 175 Multimeter
//						Only a simple reading command is provided,
//						ReadVoltage(), but this may be used also for
//						current or resistance measurements
//

// The default constructor
K175::K175(int _addr)
			: IEEEdevice(_addr)
{}

double K175::ReadVoltage()
{
	// Read the value, but if the address is 0 returns a random double
	if (address != 0) enter(response, 32, &length, address, &status);
		else return 8.7 + 0.001 * double(rand())/RAND_MAX;

	if (status != 0)     // On error
	{
		ErrorCode = 1;
		sprintf(ErrorMessage,"Failure reading K175 at address %d", address);
	}
	return atof(response+4);  // The first four characters of the response
									  // are letters indicating the mode
}

//
int K175::Test()
{
	if (address == 0) return 0;  //Do nothing and leave function if address = 0

	enter(response, 32, &length, address, &status);    //read device

	if (status != 0)     										// On error
	{
		ErrorCode = status;
		sprintf(ErrorMessage,"IEEE error %d :Failure reading K175 at address %d",
											status, address);
		return ErrorCode;
	}

	return 0;														// All ok !
}





//////////////////////////////////////////////////////////////////////////
//
//	class K195A
//
//
K195A::K195A(int _addr)
			: IEEEdevice(_addr)
{}

double K195A::ReadVoltage()
{
//	send(address,"X",&status);
	if (address != 0) enter(response, 32, &length, address, &status);
		else return 0.001 * double(rand())/RAND_MAX;
	if (status != 0)
	{
		ErrorCode = 1;
		sprintf(ErrorMessage,"Failure reading K195A at address %d", address);
	}
	return atof(response+4);
}

int K195A::Test()
{
	if (address == 0) return 0;  //Do nothing and leave function if address = 0

	enter(response, 32, &length, address, &status);    //read device

	if (status != 0)     										// On error
	{
		ErrorCode = status;
		sprintf(ErrorMessage,"IEEE error %d :Failure reading K175 at address %d",
											status, address);
		return ErrorCode;
	}

	return 0;														// All ok !
}





*/
//////////////////////////////////////////////////////////////////////////
//
//	class K196
//
K196::K196(int _addr)
			: IEEEdevice(_addr)
{}

double K196::ReadVoltage()
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "X");
	if (status != 0)
	{
		ErrorCode = status;
		sprintf(ErrorMessage,"Failure reading K196 at address %d", address);
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1.0;
          else
            return atof(response+4);
      }
}

int K196::Test()
{
	if (address == 0) return 0;  //Do nothing and leave function if address = 0

    status = GPIBReceive(address, response, MaxResponse-1);

	if (status != 0)     										// On error
	{
		ErrorCode = status;
		sprintf(ErrorMessage,"IEEE error %d :Failure reading K175 at address %d",
											status, address);
		return ErrorCode;
	}

	return 0;														// All ok !
}

SRS830::SRS830(int _addr)
    :IEEEdevice(_addr)
{}

double SRS830::ReadOutput(int channel)
{
    if (address == 0) return 0.0;
    if ((channel<1) || (channel>4)) return 0.0;
    sprintf(command,"OUTP?%1d\r\n",channel);
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
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

double SRS830::GetSignalPhase()
{
    if (address == 0) return -1.0;
    sprintf(command,"OUTP?4\r\n");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
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

int SRS830::Test()
{
    if (address == 0) return -1;
    sprintf(command,"*IDN?\r\n");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1.0;
          else
          {
              return strncmp(response,"Stanford_Research_Systems,SR830",30);
          }
      }
    return -1;
}

int SRS830::SetDAC(int n, int mV)
{
    if (address == 0) return -1;
    if ((n<0) || (n>4)) return -1;
    double Volt = double(mV)/1000.0;
    sprintf(command,"AUXV%1d,%6.3f\r\n",n,Volt);
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
        return -1;
	}
    return 0;
}

int SRS830::SetRemote(int remote)
{
    if (address == 0) return -1;
    if ((remote<0) || (remote>2)) return -1;
    sprintf(command,"LOCL%1d\r\n",remote);
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
        return -1;
	}
    return 0;
}

double SRS830::GetRefFrequency()
{
    if ( address == 0 ) return -1.0;
    sprintf(command,"FREQ?\r\n");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
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

double SRS830::GetPhase()
{
    if ( address == 0 ) return -1.0;
    sprintf(command,"PHAS?\r\n");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
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

int SRS830::MeasSensitivity()
{
    if ( address == 0 ) return -1;
    sprintf(command,"SENS?\r\n");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
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

int SRS830::MeasTimeConstant()
{
    if ( address == 0 ) return -1;
    sprintf(command,"OFLT?\r\n");
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
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
    return 0;
}
        /*


//////////////////////////////////////////////////////////////////////////
//
//	class EGG5210
//
////

// Constructor,   only calls the default IEEEdevice default constructor
//						setting the address
EGG5210::EGG5210(int _addr)
			: IEEEdevice(_addr)
{}

// Test routine, returns 0 if communication with the device is OK
//					  returns ErrorCode and sets ErrorMessage on Failure
int EGG5210::Test()
{
	if (address == 0) return 0;  // return 0 if the address is set to 0

	send(address, "ID", &status);  // Send the 'identification' command
	if (status != 0)
	{
		ErrorCode = status;
		sprintf(ErrorMessage,"IEEE Error %d : Failure asking ID from EGG5210 at address %d",
								ErrorCode, address);
		return ErrorCode;
	}
	enter(response, 32, &length, address, &status); // Get the response
	if (status != 0)
	{
		ErrorCode = status;
		sprintf(ErrorMessage,"IEEE Error %d : Failure getting ID from EGG5210 at address %d",
								ErrorCode, address);
		return ErrorCode;
	}
	// Here I should check for the response
	return 0;
}

double EGG5210::ReadOutput(int channel)
{
	if (address == 0) return 0.001*double(rand())/RAND_MAX;
	int sens_code = 9;
	double signal = 0.0;
	double sensitivity = 0.001;

	switch (channel)
	{
		case 2 : send(address, "Y", &status); break;
		case 3 : send(address, "MAG", &status); break;
		default : send(address, "X", &status); break;
	}
	WaitBeforeRead();
	enter( response, 32, &length, address, &status);
	while (!CommandPerformed()) {}
	signal = atof(response) / 1.0e4 ;
	send(address, "SEN", &status);
	WaitBeforeRead();
	enter( response, 32, &length, address, &status);
	while (!CommandPerformed()) {}
	sens_code = atoi(response);

	switch (sens_code)
	{
		case 0:	sensitivity = 1.0e-7; break;
		case 1:	sensitivity = 3.0e-7; break;
		case 2:	sensitivity = 1.0e-6; break;
		case 3:	sensitivity = 3.0e-6; break;
		case 4:	sensitivity = 1.0e-5; break;
		case 5:	sensitivity = 3.0e-5; break;
		case 6:	sensitivity = 1.0e-4; break;
		case 7:	sensitivity = 3.0e-4; break;
		case 8:	sensitivity = 1.0e-3; break;
		case 9:	sensitivity = 3.0e-3; break;
		case 10:	sensitivity = 1.0e-2; break;
		case 11:	sensitivity = 3.0e-2; break;
		case 12:	sensitivity = 1.0e-1; break;
		case 13:	sensitivity = 3.0e-1; break;
		case 14:	sensitivity = 1.0; break;
		case 15:	sensitivity = 3.0; break;
	}
	signal *= sensitivity;
	return signal;
}

double EGG5210::GetSignalPhase()
{
	if (address == 0) return 0;
	if (ReadyToRead())   // If something is left in the buffer read it
	{
		enter(response, 32, &length , address, &status);
		while (!CommandPerformed()) {}
	}
	sprintf(command, "PHA");
	send(address, command, &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	while (!CommandPerformed() ) {}
	return atof(response)/1000.0;
}
// SetDAC sets the AUXILIARY DAC OUT output on the rear panel.
// It should be send an integer in the range -15000...15000 representing
// the number of mV. It provides then a voltage of -15 to 15 V with 1mV
// resolution
int EGG5210::SetDAC(int mV)
{
	if (address == 0) return 0;
	if ( mV < -15000 ) mV = -15000;
	if ( mV > 15000 ) mV = 15000;
	sprintf(command, "DAC%d", mV);
	send(address,command,&status);
	while (!CommandPerformed()) {}
	return status;
}

// Reads on of the four ADCs on the rear panel
double EGG5210::ReadADC(int channel)
{
	if (address == 0) return 10.0 * double(rand())/RAND_MAX;
	int l;
	if ((channel < 1) || (channel > 4)) channel = 1;

	sprintf(command, "ADC%d", channel);
	send(address, command, &status);
	WaitBeforeRead();
	enter(response, 32, &l , address, &status);
	while (!CommandPerformed() ) {}
	return atof(response)/1000.0;
}

char* EGG5210::GetSensitivity(char* sensitivity)
{
	if (address ==0 )
		strcpy(sensitivity,"Undfnd");
	else
	{
		send(address, "SEN", &status);
		WaitBeforeRead();
		enter( response, 32, &length, address, &status);
		while (!CommandPerformed()) {}
		int sens_code = atoi(response);
		char hlp[7];
		AsciiSensitivity(sens_code, hlp);
		strcpy(sensitivity,hlp);
	}
	return sensitivity;
}


char* EGG5210::GetTimeConstant(char *tc)
{
	if (address ==0 )
		strcpy(tc,"Undfnd");
	else
	{
		send(address, "XTC", &status);
		WaitBeforeRead();
		enter( response, 32, &length, address, &status);
		while (!CommandPerformed()) {}
		int TcCode = atoi(response);
		char hlp[7];
		AsciiTimeConstant(TcCode, hlp);
		strcpy(tc,hlp);
	}
	return tc;
}

double EGG5210::GetRefFrequency()       // returns reference frequency in Hz
{
	if (address == 0) return 0.0;
	send(address, "FRQ", &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	while (!CommandPerformed() ) {}
	return atof(response)/1000.0;
}

double EGG5210::GetPhase()       // returns phase in degrees (-180° -- +180°)
{
	if (address == 0) return 0.0;
	int quadrant;
	long degrees = 0;
	double phase;
	send(address, "DD 44", &status);  // Sets ',' as separator
	while (!CommandPerformed() ) {}
	send(address, "P", &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	sscanf(response,"%d,%ld", &quadrant, &degrees);
//	WaitBeforeRead();
//	enter(response, 32, &length , address, &status);
//	sscanf(response,"%ld", &degrees);
	while (!CommandPerformed() ) {}
	phase = 90.0*quadrant+double(degrees)/1000.0;
	if (phase >=360.0) phase -= 360.0;
	return phase;
}

void EGG5210::WriteAllParameters(char* LockInPar) const
{
	char* help = new char[80];
	char* help2 = new char[10];
	strcpy(LockInPar,"Parameters Lock-in Amplifier: \n");
	AsciiSensitivity(Sensitivity, help2);
	sprintf(help,"  Sensitivity %s, ", help2);
	strcat(LockInPar, help);
	sprintf(help,"Phase %f °\n",Phase);
	strcat(LockInPar, help);
	sprintf(help,"  Reference frequency %f Hz, ",ReferenceFrequency);
	strcat(LockInPar, help);
	switch (FilterMode)
	{
		case 0: sprintf(help,"No input filter \n");
					break;
		case 1: sprintf(help,"Notch at %f Hz \n",FilterFrequency);
					break;
		case 2: sprintf(help,"Low Pass at %f Hz \n",FilterFrequency);
					break;
		case 3: sprintf(help,"Band Pass at %f Hz\n",FilterFrequency);
					break;
		default: strcpy(help," Filter ? \n");
					break;
	}
	strcat(LockInPar, help);
	AsciiTimeConstant(TimeConstant, help2);
	sprintf(help,"  TimeConstant %s, ", help2);
	strcat(LockInPar, help);
	if (Slope==0) sprintf(help,"Output rolloff 6db/octave, ");
	  else        sprintf(help,"Output rolloff 12db/octave, ");
	strcat(LockInPar, help);
	switch (DynamicReserve)
	{
		case 0: sprintf(help,"Dynamic reserve HI STAB \n");
					break;
		case 1: sprintf(help,"Dynamic reserve NORM \n");
					break;
		case 2: sprintf(help,"Dynamic reserve HI RES \n");
					break;
		default: sprintf(help,"Dynamic reserve ? \n");
					break;
	}
	strcat(LockInPar, help);
	delete[] help;
	delete[] help2;
	return;
}

int EGG5210::UpdateParameters()
{
	Phase = GetPhase();
	Sensitivity = MeasSensitivity();
	TimeConstant = MeasTimeConstant();
	ReferenceFrequency = GetRefFrequency();
	Slope = MeasSlope();
	DynamicReserve = MeasDynRes();
	FilterFrequency = MeasFilterFrequency();
	FilterMode = MeasFilterMode();
	return 0;
}

int EGG5210::MeasSensitivity()
{
	if (address == 0) return 0;
	send(address, "SEN", &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	while (!CommandPerformed() ) {}
	return atoi(response);
}

int EGG5210::MeasTimeConstant()
{
	if (address == 0) return 0;
	send(address, "XTC", &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	while (!CommandPerformed() ) {}
	return atoi(response);
}

int EGG5210::MeasSlope()
{
	if (address == 0) return 0;
	send(address, "XDB", &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	while (!CommandPerformed() ) {}
	return atoi(response);
}

int EGG5210::MeasDynRes()
{
	if (address == 0) return 0;
	send(address, "DR", &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	while (!CommandPerformed() ) {}
	return atoi(response);
}

double EGG5210::MeasFilterFrequency()
{
	if (address == 0) return 999.9;
	int freq, band;

	send(address, "DD 44", &status);
	WaitBeforeRead();
	send(address, "FF", &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	sscanf(response, "%d,%d", &freq, &band);
//	WaitBeforeRead();
//	enter(response, 32, &length , address, &status);
//	sscanf(response, "%d", &band);
	while (!CommandPerformed() ) {}
	switch (band)
	{
		case 0 : return double(freq)/100.0;
		case 1 : return double(freq)/10.0;
		case 2 : return double(freq);
		case 3 : return double(freq)*10.0;
		case 4 : return double(freq)*100.0;
	}
	return 999.9;
}

int EGG5210::MeasFilterMode()
{
	if (address == 0) return 0;
	send(address, "FLT", &status);
	WaitBeforeRead();
	enter(response, 32, &length , address, &status);
	while (!CommandPerformed() ) {}
	return atoi(response);
}

void EGG5210::AsciiTimeConstant(int TcCode, char* tc) const
{
	switch (TcCode)
	{
		case 0:	strcpy(tc,"  1 ms"); break;
		case 1:	strcpy(tc,"  3 ms"); break;
		case 2:	strcpy(tc," 10 ms"); break;
		case 3:	strcpy(tc," 30 ms"); break;
		case 4:	strcpy(tc,"100 ms"); break;
		case 5:	strcpy(tc,"300 ms"); break;
		case 6:	strcpy(tc,"   1 s"); break;
		case 7:	strcpy(tc,"   3 s"); break;
		case 8:	strcpy(tc,"  10 s"); break;
		case 9:	strcpy(tc,"  30 s"); break;
		case 10:	strcpy(tc," 100 s"); break;
		case 11:	strcpy(tc," 300 s"); break;
		case 12:	strcpy(tc,"1000 s"); break;
		case 13:	strcpy(tc,"3000 s"); break;
	}
	return;
}

void EGG5210::AsciiSensitivity(int SensitivityCode, char* sens) const
{
	switch (SensitivityCode)
	{
		case 0:	strcpy(sens,"100 nV"); break;
		case 1:	strcpy(sens,"300 nV"); break;
		case 2:	strcpy(sens,"  1 uV"); break;
		case 3:	strcpy(sens,"  3 uV"); break;
		case 4:	strcpy(sens," 10 uV"); break;
		case 5:	strcpy(sens," 30 uV"); break;
		case 6:	strcpy(sens,"100 uV"); break;
		case 7:	strcpy(sens,"300 uV"); break;
		case 8:	strcpy(sens,"  1 mV"); break;
		case 9:	strcpy(sens,"  3 mV"); break;
		case 10:	strcpy(sens," 10 mV"); break;
		case 11:	strcpy(sens," 30 mV"); break;
		case 12:	strcpy(sens,"100 mV"); break;
		case 13:	strcpy(sens,"300 mV"); break;
		case 14:	strcpy(sens,"  1  V"); break;
		case 15:	strcpy(sens,"  3  V"); break;
		default: strcpy(sens,"nondef"); break;
	}
	return;
}







//////////////////////////////////////////////////////////////////////////
//
//	class ITC503
//
ITC503::ITC503(int addr)
		:IEEEdevice(addr), SetPoint(5), SwitchTemperature(80.0)
{}

int ITC503::Test()
{
	if (SetRemote() == -1) return -1;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'Q2' 13",address);
	transmit(command,&status);
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'V' 13",address);
	transmit(command,&status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strncmp(response,"ITC503 ",6)==0) return 0;
		else
		{
			ErrorCode = -1;
			sprintf(ErrorMessage,"No response from ITC503 at address %d", address);
			return ErrorCode;
		}
}

int ITC503::SetTemperature(double T)
{
	SetPoint = T;
	if (SetRemote() == -1) return -1;
	int errorcode = 0;
	if (address == 0) return 0;
// Set the correct sensor for the range:
// First set to manual
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'A0' 13",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"A") != 0) errorcode = -1;
				// If ITC503 doesn't echo 'H' something is wrong
// Set the right sensor
	if (SetPoint < SwitchTemperature)
		sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'H1' 13",address);
		// T < Ts use sensor 1
	 else
		sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'H2' 13",address);
		// T > Ts use sensor 2
	transmit( command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"H") != 0) errorcode = -1;
				// If ITC503 doesn't echo 'H' something is wrong
// Set in automatic mode
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'A1' 13",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"A") != 0) errorcode = -1;
				// If ITC503 doesn't echo 'H' something is wrong

	if (SetPoint >= 100.0)            //Set temp in right format
		sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'T%5.1f' 13",address, SetPoint);
	  else
		if (SetPoint >= 10.00)
		  sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'T%5.2f' 13",address, SetPoint);
		 else
			sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'T%5.3f' 13",address, SetPoint);

	transmit( command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"T") == 0) return errorcode;
		else return errorcode-1;
}

double ITC503::GetTemperature(int sensor)
{
	if (address==0) return 0.0;           //if not connected return
	if (SetRemote() == -1) return 0.0; // Set Remote & Unlocked
	if (sensor == 0) // if automatic sensor choice
	{
		sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'R2' 13",address);
		transmit(command, &status);        //read Temp channel 2
		WaitUntilMessageAvailable();
		enter(response, 32, &length , address, &status);
		double T2 = atof(response+1);
		if (T2 >= SwitchTemperature) return T2;
		sensor = 1;
	}
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'R%1d' 13",address,sensor);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response, 32, &length , address, &status);
	return atof(response+1);
}

double ITC503::GetSetTemperature()
{
	if (address==0) return 0.0;           //if not connected return
	if (SetRemote() == -1) return 0.0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'R0' 13 10",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response, 32, &length , address, &status);
	return atof(response+1);
}

int ITC503::SetRemote(int remote)
//Sets the remote mode
// 0:local & locked    (start-up value ITC503)
// 1:remote & locked
// 2:local & unlocked
// 3:remote & unlocked (default)
{
	if (address == 0) return 0;  // do nothing if not connected

	sprintf(command,"UNT UNL MTA LISTEN %2d REN DATA 'C%1d' 13",address,remote);
	transmit(command, &status);
	if (status==8) // if timeout
	{
		if (WaitUntilMessageAvailable() == -1)
			sprintf(command,"UNT UNL MTA LISTEN %2d REN DATA 'C%1d' 13",address,remote);
					transmit(command, &status);   //try again
		if (WaitUntilMessageAvailable() == -1)   // if still no answer
		{
			ErrorCode=-1;
			return -1;
		}   //This is bad luck !!
		enter(response, 79,&length, address, &status);
		if (status == 8 )
		{
			sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'Q2' 13",address);
			transmit(command, &status);
			WaitUntilMessageAvailable(); //No message is expected, just a delay.
		}
		sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'C%1d' 13",address,remote);
		transmit( command, &status);
		if (status != 0)    //if still not working !!
		{
			ErrorCode=-1;
			return -1;
		}   //This is bad luck !!
	}
	if (WaitUntilMessageAvailable() == 0)
	{
		enter(response, 79,&length, address, &status);
										// Instrument should echoe 'C'
		if (status == 8)
		{
			sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'Q2' 13",address);
			transmit(command, &status);
			WaitUntilMessageAvailable(); //No message is expected, just a delay.
			sprintf(command,"UNT UNL MTA LISTEN %2d REN DATA 'C%1d' 13",address,remote);
			transmit(command, &status);
			if (WaitUntilMessageAvailable() == 0)
						enter(response, 79,&length, address, &status);
		}
		if (strcmp(response,"C") == 0) return 0;
	}
	return -1;
}




//////////////////////////////////////////////////////////////////////////
//
//	class IPS120
//
IPS120::IPS120(int addr)
		:IEEEdevice(addr), MaxCurrent(104.0), MaxSpeed(16.0), BperA(0.1154),
				Lmagnet(23.2)
{}

int IPS120::Test()
{
	if (address == 0) return 0;
	if (SetRemote() == -1) return -1;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'Q6' 13",address);
	transmit(command,&status);
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'V' 13",address);
	transmit(command,&status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strncmp(response,"IPS120",6)==0) return 0;
		else
		{
			ErrorCode = -1;
			sprintf(ErrorMessage,"IEEE error. No correct answer from Power supply at address %d", address);
			return ErrorCode;
		}
}

int IPS120::IsSweeping()
{
	if (address == 0) return 0;
	if (SetRemote() == -1) return -1;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'X' 13",address);
	transmit(command,&status);
	WaitUntilMessageAvailable();
	enter(response,16,&length, address, &status);
	if (response[11] != '0') return 1;
		else return 0;
}

int IPS120::SetTargetField(double B)      // Sets the target field in Tesla
{
	if (SetRemote() == -1) return -1;
	ErrorCode = 0;
	if (address == 0) return 0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'J%7.4f' 13",address,B);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"J") != 0) ErrorCode = -1;
	return 0;
}

int IPS120::SetFieldSweepRate(double rate)  // Sets the sweeprate in T/min
{
	if (SetRemote() == -1) return -1;
	ErrorCode = 0;
	if (address == 0) return 0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'T%7.4f' 13",address,rate);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"T") != 0) ErrorCode = -1;
	return 0;
}

int IPS120::Zero()
{
	if (SetRemote() == -1) return -1;
	ErrorCode = 0;
	if (address == 0) return 0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'A2' 13",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"A") != 0) ErrorCode = -1;
	return 0;
}

int IPS120::Hold()
{
	if (SetRemote() == -1) return -1;
	ErrorCode = 0;
	if (address == 0) return 0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'A0' 13",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"A") != 0) ErrorCode = -1;
	return 0;
}

int IPS120::GotoSetPoint()
{
	if (SetRemote() == -1) return -1;
	ErrorCode = 0;
	if (address == 0) return 0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'A1' 13",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response,79,&length, address, &status);
	if (strcmp(response,"A") != 0) ErrorCode = -1;
	return 0;
}


double IPS120::GetTargetField()
{
	if (address==0) return 0.0;           //if not connected return
	if (SetRemote() == -1) return 0.0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'R8' 13 10",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response, 32, &length , address, &status);
	return atof(response+1);
}

double IPS120::GetDemandField()
{
	if (address==0) return 0.0;           //if not connected return
	if (SetRemote() == -1) return 0.0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'R7' 13 10",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response, 32, &length , address, &status);
	return atof(response+1);
}

double IPS120::GetMeasuredCurrent()
{
	if (address==0) return 0.0;           //if not connected return
	if (SetRemote() == -1) return 0.0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'R2' 13 10",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response, 32, &length , address, &status);
	return atof(response+1);
}

double IPS120::GetFieldSweepRate()     //response in T/min
{
	if (address==0) return 0.0;           //if not connected return
	if (SetRemote() == -1) return 0.0;
	sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'R7' 13 10",address);
	transmit(command, &status);
	WaitUntilMessageAvailable();
	enter(response, 32, &length , address, &status);
	return atof(response+1);
}

int IPS120::SetRemote(int remote)
//Sets the remote mode
// 0:local & locked    (start-up value IPS120)
// 1:remote & locked
// 2:local & unlocked
// 3:remote & unlocked (default)
{
	if (address == 0) return 0;  // do nothing if not connected

	sprintf(command,"UNT UNL MTA LISTEN %2d REN DATA 'C%1d' 13",address,remote);
	transmit(command, &status);
	if (status==8) // if timeout
	{
		if (WaitUntilMessageAvailable() == -1)
			sprintf(command,"UNT UNL MTA LISTEN %2d REN DATA 'C%1d' 13",address,remote);
					transmit(command, &status);   //try again
		if (WaitUntilMessageAvailable() == -1)   // if still no answer
		{
			ErrorCode=1;
			strcpy(ErrorMessage,"IPS120 does not respond");
			return -1;
		}   //This is bad luck !!
		enter(response, 79,&length, address, &status);
		if (status == 8 )
		{
			sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'Q6' 13",address);
			transmit(command, &status);
			WaitUntilMessageAvailable(); //No message is expected, just a delay.
		}
		sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'C%1d' 13",address,remote);
		transmit( command, &status);
		if (status != 0)    //if still not working !!
		{
			ErrorCode=1;
			strcpy(ErrorMessage,"IPS120 does not respond");
			return -1;
		}   //This is bad luck !!
	}
	if (WaitUntilMessageAvailable() == 0)
	{
		enter(response, 79,&length, address, &status);
										// Instrument should echoe 'C'
		if (status == 8)
		{
			sprintf(command,"UNT UNL MTA LISTEN %2d DATA 'Q6' 13",address);
			transmit(command, &status);
			WaitUntilMessageAvailable(); //No message is expected, just a delay.
			sprintf(command,"UNT UNL MTA LISTEN %2d REN DATA 'C%1d' 13",address,remote);
			transmit(command, &status);
			if (WaitUntilMessageAvailable() == 0)
						enter(response, 79,&length, address, &status);
		}
		if (strcmp(response,"C") == 0) return 0;
	}
	ErrorCode = 1;
	strcpy(ErrorMessage,"IPS120 does not respond, Set to REMOTE failed");

	return -1;
}


//////////////////////////////////////////////////////////////////////////
//
//	class G6062A
//

G6062A::G6062A(int addr)
		: IEEEdevice(addr)
{}

int G6062A::Test()
{
	if (address == 0) return 0;
	sprintf(command,"AP-60.0dB");
	send(address,command, &status);
	if (status != 0)
	{
		ErrorCode = status;
		sprintf(ErrorMessage,"IEEE error talking to G6062A at address %d", address);
	}
	return ErrorCode;
}

int G6062A::SetFreq(double MHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"FR%fMZ", MHz);
	send(address, command, &status);
	return status;
}

int G6062A::SetStep(double kHz)
{
	if (address == 0) return 0;
	sprintf(command,"FS%fKZ", kHz);
	send(address, command, &status);
	return status;
}

int G6062A::Step()
{
	if (address == 0) return 0;
	send(address,"SU",&status);
	return status;
}

int G6062A::SetPower(int P)
{
	if (address == 0) return 0;
	sprintf(command,"AP%d.0dB",P);
	send(address,command, &status);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting Power of G6062A");
	}
	return ErrorCode;
}

int G6062A::SetAM(int on,double Mod)
{
	if (address == 0) return 0;
	sprintf(command,"AM%fPC",Mod);
	send(address,command, &status);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting AM of G6062A");
	}
	if (on == 1) send(address,"AE1",&status);
		else send(address,"AE0",&status);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting AM of G6062A");
	}
	return ErrorCode;
}

int G6062A::SetFM(int on,double Mod)
{
	if (address == 0) return 0;
	sprintf(command,"FM%fKZ",Mod);
	send(address,command, &status);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting FM of G6062A");
	}
	if (on == 1) send(address,"FE1",&status);
		else send(address,"FE0",&status);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting FM of G6062A");
	}
	return ErrorCode;
}

int G6062A::RFon()
{
	if (address == 0) return 0;
	send(address,"RO1", &status);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error turning rf on ");
	}
	return ErrorCode;
}

int G6062A::RFoff()
{
	if (address == 0) return 0;
	send(address,"RO0", &status);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error turning rf on ");
	}
	return ErrorCode;
}
*/

//////////////////////////////////////////////////////////////////////////
//
//	class SMY02    Rohde & Schwartz 2 GHz signal generator
//
//
//  The status byte is of the following format:
//  Only the bits 4 to 6 are used (of 0..7)
//  Bit 4 MAV  Message available  (value 16 ..)
//
//  The answers from the SMY02 contain HEADERS if they are not
//  switched off by a HEADERS:OFF command. So I will test every answer
//  whether it contains a header or not before reading it

SMY02::SMY02(int addr)
		: IEEEdevice(addr)
{}

//  Test sets the device in remote and asks for the ID
//  If results OK returns 0 otherwise returns current bus status
int SMY02::Test()
{
	if (address == 0) return 0;
    SetRemote(address);

	sprintf(command,"*IDN?");
    status = GPIBSendCommand(address,command);
	ErrorCode = status;
	if (status != 0)
	{
		sprintf(ErrorMessage,"IEEE error talking to SMY02 at address %d", address);
	}

    status = GPIBReceive(address, response, MaxResponse -1);
 	if (status != 0)
	{
		sprintf(ErrorMessage,"IEEE error listening to SMY02 at address %d", address);
        ErrorCode = status;
        return ErrorCode;
	}
        // now test for correct response:  ROHDE&SCHWARZ,SMY02,826045/013,2.02

    if (strncmp(response,"ROHDE&SCHWARZ,SMY02",19) !=0)
	{
		sprintf(ErrorMessage,"Incorrect ID for SMY02 at address %d", address);
        ErrorCode = 200;
	}

	return ErrorCode;
}

int SMY02::SetFreq(double MHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"RF %fMHZ", MHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMY02::SetStartFreq(double MHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"RF:START %fMHZ", MHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMY02::SetStopFreq(double MHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"RF:STOP %fMHZ", MHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMY02::SetAFfreq(double kHz)
{
	if (address == 0 ) return 0;
	status = 0;
	sprintf(command,"AF %fKHZ", kHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMY02::SetStep(double kHz)
{
	if (address == 0) return 0;
	sprintf(command,"RF:Var_step %fKHZ", kHz);
	status = GPIBSendCommand(address, command);
	return status;
}

int SMY02::Step()
{
	if (address == 0) return 0;
	status = GPIBSendCommand(address, "INC:RF");
	return status;
}

int SMY02::SetPower(double P)
{
	if (address == 0) return 0;
	sprintf(command,"Level %fDBM",P);
	status = GPIBSendCommand(address, command);
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting Power of SMY02");
	}
	return ErrorCode;
}

int SMY02::SetAM(int on,double Mod)
{
	if (address == 0) return 0;
    switch (on)
    {
        case 1: sprintf(command,"AM:INTERNAL %5.1f", Mod);break;
        case 2: sprintf(command,"AM:EXTERNAL:AC %5.1f", Mod);break;
        case 3: sprintf(command,"AM:EXTERNAL:DC %5.1f", Mod);break;
        case 4: sprintf(command,"AM:DUAL:AC %5.1f", Mod);break;
        case 5: sprintf(command,"AM:DUAL:DC %5.1f", Mod);break;
        default: sprintf(command,"AM:OFF");break;
    }
    status = GPIBSendCommand(address, command);

	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting AM of SMY02");
	}
	return ErrorCode;
}

int SMY02::SetFM(int on,double Mod)
{
	if (address == 0) return 0;
    switch (on)
    {
        case 1: sprintf(command,"FM:INTERNAL %6.1fKHZ", Mod);break;
        case 2: sprintf(command,"FM:EXTERNAL:AC %6.1fKHZ", Mod);break;
        case 3: sprintf(command,"FM:EXTERNAL:DC %6.1fKHZ", Mod);break;
        case 4: sprintf(command,"FM:DUAL:AC %6.1fKHZ", Mod);break;
        case 5: sprintf(command,"FM:DUAL:DC %6.1fKHZ", Mod);break;
        default: sprintf(command,"FM:OFF");break;
    }
    status = GPIBSendCommand(address, command);

	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting FM of SMY02");
	}
	return ErrorCode;
}

int SMY02::SetPM(int on,double Mod)          //Set phase modulation
{
	if (address == 0) return 0;
    switch (on)
    {
        case 1: sprintf(command,"PHM:INTERNAL %6.1f RAD", Mod);break;
        case 2: sprintf(command,"PHM:EXTERNAL %6.1f RAD", Mod);break;
        case 3: sprintf(command,"PHM:EXTERNAL %6.1f RAD", Mod);break;
        case 4: sprintf(command,"PHM:DUAL %6.1f RAD", Mod);break;
        case 5: sprintf(command,"PHM:DUAL %6.1f RAD", Mod);break;
         default: sprintf(command,"PHM:OFF");break;
    }
    status = GPIBSendCommand(address, command);

	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error setting phase modul of SMY02");
	}
	return ErrorCode;
}

int SMY02::RFon()
{
	if (address == 0) return 0;
	status = GPIBSendCommand(address, "Level:ON");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error turning rf on ");
	}
	return ErrorCode;
}

int SMY02::RFoff()
{
	if (address == 0) return 0;
	status = GPIBSendCommand(address, "Level:OFF");
	if (status != 0)
	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error turning rf on ");
	}
	return ErrorCode;
}

double SMY02::GetFreq()    // returns the frequency in MHz
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "RF?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
            if (strncmp(response,"RF",2)==0)
               return atof(response+3)/1e6;
                  else return atof(response)/1e6;     // in MHz
      }
}

double SMY02::GetPower()    // returns the power level in dBm
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "Level?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
            if (strncmp(response,"LEVEL:OFF",9)==0) return -150.0;
              else if (strncmp(response,"LEVEL",5)==0)
                        return atof(response+5);
                  else return atof(response);     // in dB
      }
}

double SMY02::GetAFfreq()    // returns the frequency in MHz
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "AF?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (strncmp(response,"AF:OFF",6)==0) return 0.0;
        if (status != 0) return -1.0;
          else if (strncmp(response,"AF",2)==0)
            return atof(response+3)/1e3;
             else return atof(response)/1e3;     // in kHz
      }
}

double SMY02::GetStartFreq()    // returns the frequency in MHz
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "RF:START?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1.0;
          else if (strncmp(response,"RF:START",8)==0)
            return atof(response+8)/1e6;
             else return atof(response)/1e6;     // in MHz
      }
}

double SMY02::GetStopFreq()    // returns the frequency in MHz
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "RF:STOP?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1.0;
          else if (strncmp(response,"RF:STOP",7)==0)
            return atof(response+7)/1e6;
             else return atof(response)/1e6;     // in MHz
      }
}

double SMY02::GetStep()    // returns the frequency in MHz
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "RF:Var_Step?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1.0;
          else if (strncmp(response,"RF:VAR",6)==0)
            return atof(response+6)/1e3;
             else return atof(response)/1e3;     // in kHz
      }
}

int SMY02::GetStepTime()    // returns the frequency in MHz
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "TIME:RF_SWP?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1.0;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else if (strncmp(response,"TIME:RF_SWP",11)==0)
            return int(1000.0*atof(response+11));
             else return int(atof(response)*1000.0);     // in kHz
      }
}

int SMY02::GetSweepType()    // returns the frequency in MHz
{
    if (address == 0) return -1.0;
	status = GPIBSendCommand(address, "SWP?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else if (strncmp(response,"SWP:OFF",7)==0) return 0;
             else if (strncmp(response,"SWP:AUTO",8)==0) return 1;
                 else return 2;
      }
}

int SMY02::GetAM(double* mod)
{
    if (address == 0) return -1.0;
   	status = GPIBSendCommand(address, "HEADER:ON");
	status = GPIBSendCommand(address, "AM?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              if (strncmp(response,"AM:OFF",6)==0) return 0;
                else *mod = atof(response+6);
              if (strncmp(response,"AM:INT",6)==0) return 1;
              if (strncmp(response,"AM:E:A",6)==0) return 2;
              if (strncmp(response,"AM:E:D",6)==0) return 3;
              if (strncmp(response,"AM:D:A",6)==0) return 4;
              if (strncmp(response,"AM:D:D",6)==0) return 5;
          }

        return 0;
      }

}

int SMY02::GetFM(double* mod)
{
    if (address == 0) return -1.0;
   	status = GPIBSendCommand(address, "HEADER:ON");
	status = GPIBSendCommand(address, "FM?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              if (strncmp(response,"FM:OFF",6)==0) return 0;
                else *mod = atof(response+6)/1000.0;    // in kHz
              if (strncmp(response,"FM:INT",6)==0) return 1;
              if (strncmp(response,"FM:E:A",6)==0) return 2;
              if (strncmp(response,"FM:E:D",6)==0) return 3;
              if (strncmp(response,"FM:D:A",6)==0) return 4;
              if (strncmp(response,"FM:D:D",6)==0) return 5;
          }
        return 0;
      }

}

int SMY02::GetPM(double* mod)
{
    if (address == 0) return -1.0;
   	status = GPIBSendCommand(address, "HEADER:ON");
	status = GPIBSendCommand(address, "PHM?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1;
	}
      else
      {
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              if (strncmp(response,"PHM:OFF",7)==0) return 0;
                else *mod = atof(response+7);
              if (strncmp(response,"PHM:INT",7)==0) return 1;
              if (strncmp(response,"PHM:EXT",7)==0) return 2;
              if (strncmp(response,"PHM:DUA",7)==0) return 3;
          }
        return 0;
      }

}

int SMY02::IsError(char* message, int maxlength)
{
    if (address == 0) return -1;
	status = GPIBSendCommand(address, "ERRORS?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1;
	}
      else
        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
           else strncpy(message, response, maxlength);
        if (strncmp(response,"ERRORS 0",8) == 0) return 0;
          else return 1;
}

int SMY02::SetSweepType(int type)
{
	if (address == 0) return 0;
    switch (type)
    {
        case 1: sprintf(command,"SWP:ON");break;
        case 2: sprintf(command,"SWP:RESET");break;
        default: sprintf(command,"SWP:OFF");break;
    }
	status = GPIBSendCommand(address, command);
	return status;
}

int SMY02::SetStepTime(int ms)
{
	if (address == 0) return 0;
    sprintf(command,"TIME:RF_SWP %d ms", ms);
	status = GPIBSendCommand(address, command);
	return status;
}

AR100W1000A::AR100W1000A(int _addr)
    : IEEEdevice(_addr)
{}

int AR100W1000A::SetPowerStatus(int stat)
{
	if (address == 0) return 0;
    if ((stat < 0) || (stat >3)) return -1;
    if (stat == 0)
        sprintf(command,"POWER:OFF");
    if (stat == 1)
        sprintf(command,"POWER:ON");
    if (stat == 2)
        sprintf(command,"POWER:ON, OPERATE");
    if (stat == 3)
        sprintf(command,"RESET");
   	status = GPIBSendCommand(address, command);
	return status;
}

int AR100W1000A::SetStandBy()
{
	if (address == 0) return 0;
    sprintf(command,"STANDBY");
   	status = GPIBSendCommand(address, command);
	return status;
}

int AR100W1000A::SetOperate()
{
	if (address == 0) return 0;
    sprintf(command,"OPERATE");
  	status = GPIBSendCommand(address, command);
	return status;
}

int AR100W1000A::GetPowerStatus()
{
// returns the status of the device
// possible values: 0 (off)
//                  1 (on/standby)
//                  2 (on/operate)
//                  3 (fault)
//                  4 (undefined)
//                  -1 (Error in communication)
    if (address == 0) return -1;    // return -1
   	status = GPIBSendCommand(address, "STA?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to AR 100W1000A");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              if (strncmp(response,"POWER:OFF",9)==0) return 0;
              if (strncmp(response,"STANDBY",7)==0) return 1;
              if (strncmp(response,"OPERATE",7)==0) return 2;
              if (strncmp(response,"FAULT",7)==0) return 3;
              return 4;
          }
      }
}

double AR100W1000A::GetGain()
{
    if (address == 0) return -1.0;    // return -1
   	status = GPIBSendCommand(address, "RFG?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to AR 100W1000A");
        return -1.0;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              return atof(response+4);
          }
      }
}


int AR100W1000A::IsOperationComplete()
{
    if (address == 0) return -1;    // return -1
   	status = GPIBSendCommand(address, "*OPC?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SMY02");
        return -1;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              if (strncmp(response,"1",1)==0) return 1;
                else return 0;
          }
      }
}

int AR100W1000A::SetGain(double gain)
{
	if (address == 0) return 0;
    sprintf(command,"LEVEL:GAIN %f",gain);
  	status = GPIBSendCommand(address, command);
	return status;
}

double AR100W1000A::GetFPower()
{
    if (address == 0) return -1.0;    // return -1
   	status = GPIBSendCommand(address, "FPOW?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to AR 100W1000A");
        return -1.0;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              return atof(response+5);
          }
      }
}

double AR100W1000A::GetRPower()
{
    if (address == 0) return -1.0;    // return -1
   	status = GPIBSendCommand(address, "RPOW?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to AR 100W1000A");
        return -1.0;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              return atof(response+5);
          }
      }
}

int AR100W1000A::GetMode()
{
// returns the 4th half-byte of the State response
//  1 Manual
//  2 Pulse
//  4 ALC Internal
//  8 ALC External

   if (address == 0) return -1;    // return -1
   	status = GPIBSendCommand(address, "STATE?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to AR 100W1000A");
        return -1.0;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              return (response[10]-48);
          }
      }
}

int AR100W1000A::GetALCParameters(double* ThreeReals)
{
   if (address == 0) return -1;    // return -1
   	status = GPIBSendCommand(address, "MSB?");
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to AR 100W1000A");
        return -1.0;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              int index = 0;
              while ((response[index] != '=') && (response[index] != 0))
                 index++;
              ThreeReals[0] = atof(response+index+1);
             index++;
             while ((response[index] != '=') && (response[index] != 0))
                 index++;
             ThreeReals[1] = atof(response+index+1);
             index++;
             while ((response[index] != '=') && (response[index] != 0))
                 index++;
             ThreeReals[2] = atof(response+index+1);
             index++;
             while ((response[index] != '=') && (response[index] != 0))
                 index++;
             return atoi(response+index+1);
          }
      }

}

int AR100W1000A::SetALCMode(int mode)
{
	if (address == 0) return 0;
    switch (mode)
    {
        case 0: sprintf(command,"MODE:MANUAL");break;
        case 1: sprintf(command,"MODE:ALC INTERNAL");break;
        case 2: sprintf(command,"MODE:ALC EXTERNAL");break;
        default: sprintf(command,"MODE:MANUAL");break;
    }
  	status = GPIBSendCommand(address, command);
	return status;
}

int AR100W1000A::SetThreshold(double th)
{
	if (address == 0) return 0;
    sprintf(command,"LEVEL:THRESHOLD %f",th);
  	status = GPIBSendCommand(address, command);
	return status;
}

int AR100W1000A::SetDetectorGain(double dg)
{
	if (address == 0) return 0;
    sprintf(command,"LEVEL:DETECTOR %f",dg);
  	status = GPIBSendCommand(address, command);
	return status;
}

int AR100W1000A::SetResponse(int resp)
{
	if (address == 0) return 0;
    sprintf(command,"LEVEL:RESPONSE %d",resp);
  	status = GPIBSendCommand(address, command);
	return status;
}
SRS245::SRS245(int _addr)
    : IEEEdevice(_addr)
{}

int SRS245::Set(int i, double value)
{
    if (address == 0) return -1;
    if ((i<1) || (i>8)) return -1;
    if (value>10.237) value = 10.237;
    if (value<-10.237) value = -10.237;

    sprintf(command, "S%1d=%f\r\n",i,value);
  	status = GPIBSendCommand(address, command);
    return status;
}

double SRS245::Get(int i)
{
    if (address == 0) return -99.0;
    if ((i<1) || (i>8)) return -99.0;
    sprintf(command,"?%1d\r\n",i);
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
        return -1.0;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return -1;
          else
          {
              return atof(response);
          }
      }
}

int SRS245::SetNout(int i)
{
    if (address == 0) return -1;
    if ((i<0) || (i>8)) return -1;

    Nout = i;
    sprintf(command, "I%1d\r\n",8-i);
  	status = GPIBSendCommand(address, command);
    return status;
}

int SRS245::SetB(int i, int TTLvalue)
{
    if (address == 0) return -1;
    if (((i!=1) && (i!=2))|| ((TTLvalue!=0) && (TTLvalue!=1))) return -1;

    sprintf(command, "B%1d=%1d\r\n",i,TTLvalue);
  	status = GPIBSendCommand(address, command);
    return status;
}

bool SRS245::GetB(int i)
{
    if (address == 0) return false;
    if ((i<1) || (i>8)) return false;
    sprintf(command,"?B%1d\r\n",i);
   	status = GPIBSendCommand(address, command);
    if (status != 0)
   	{
		ErrorCode = status;
		strcpy(ErrorMessage,"IEEE error in sending to SRS245");
        return false;
	}
      else
      {
//        WaitUntilMessageAvailable();
        status = GPIBReceive(address, response, MaxResponse-1);
        if (status != 0) return false;
          else
          {
              if (strcmp(response,"1") ==0) return true;
                else return false;
          }
      }
}

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


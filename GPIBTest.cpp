//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <stdio.h>
#include "decl-32.h"

#ifdef __cplusplus
}
#endif

#include "GPIB_NatlInstrum.h"
#include "GPIBTest.h"
#include "ErrorBx.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGPIBTestForm *GPIBTestForm;
void WriteIntegerBits(int n, char* buffer);
void WriteShortBits(int n, char* buffer);

//---------------------------------------------------------------------------
__fastcall TGPIBTestForm::TGPIBTestForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TGPIBTestForm::FormCreate(TObject *Sender)
{
	//  I include this routine to load the gpib-32.dll

//    if (!LoadGPIBDll())  {
//       ErrorBox->ErrorMessage->Caption ="Unable to correctly access the 32-bit GPIB DLL.";
//       ErrorBox->Show();
//       return;
//    }

}
//---------------------------------------------------------------------------

void __fastcall TGPIBTestForm::FormActivate(TObject *Sender)
{
   char err_str[80];
   if (GPIBInterfaceClear() != 0)
	{
//       char* errorstring = new char[80];
	   GPIBGetError(err_str);
	   LabelError->Caption = err_str;
//       delete[] errorstring;
    }
    TimeOutComboBox->ItemIndex = 11;
    GPIBSetTimeOut(11);
    EotModeComboBox->ItemIndex = 1;
    GPIBSetEotMode(1);
    EosModeComboBox->ItemIndex = 0;
	GPIBSetEosMode(0);
}
//---------------------------------------------------------------------------

void __fastcall TGPIBTestForm::FormClose(TObject *Sender, TCloseAction &Action)
{
//    FreeGPIBDll();  // do not use if not main form
}
//---------------------------------------------------------------------------


void __fastcall TGPIBTestForm::ButtonSendClick(TObject *Sender)
{
	char line[80];
	short DeviceStatus = 0;
	char DevStatBits[9];
	char dummy[2];
	char dummy2[3];
	char errorstring[256];

	short addr = (short)AddressEdit->Text.ToInt();  //get the address
	SetRemote(addr);
	int Size = SendCommand->GetTextLen(); //Get length of string in Edit1
	Size+=4;                      //Add room for null character and CR LF
	char *Buffer = new char[Size];  //Creates Buffer dynamic variable
	SendCommand->GetTextBuf(Buffer,Size); //Puts Edit1->Text into Buffer
//    SendComboBox->Items->Add(Buffer);

	LabelError->Caption = "No error";

	DeviceStatus = GPIBGetDeviceStatus(addr);
	WriteShortBits(DeviceStatus, DevStatBits);
	sprintf(line, " %d or bitwise %s ", DeviceStatus, DevStatBits);
	DevStatusBeforeLabel->Caption = line;
	if (CRCheckBox->Checked)
	{
		dummy[0]=13;    // Carriage Return character
		dummy[1]=0;     // String end character
		strcat(Buffer,dummy);
	}

	if (CR_LF_CheckBox->Checked) {
		dummy2[0] = 13;
		dummy2[1] = 10;
		dummy2[2] = 0;
		strcat(Buffer, dummy2);
	}

	if (GPIBSendCommand(addr, Buffer) != 0)
	{
//       char* errorstring = new char[80];
	   GPIBGetError(errorstring);
	   LabelError->Caption = errorstring;
//       delete[] errorstring;
    }
    int stat = GPIBGetStatusByte();
    char bits[17];
    WriteIntegerBits(stat, bits);
    for (int i=0; i<16;i++)
    {
        if (bits[i] == '1')
        {
            GPIBStatusLine(i, line);  // puts message of bit i in line
            StatusMemo->Lines->Add(line);
        }
    }
    strcpy(line,"-------");
    StatusMemo->Lines->Add(line);
    LabelStatus->Caption = stat;
    LabelStatusBits->Caption = bits;
    LabelCount->Caption = GPIBGetLengthCount();

    DeviceStatus = GPIBGetDeviceStatus(addr);
    WriteShortBits(DeviceStatus, DevStatBits);
    sprintf(line, " %d or bitwise %s ", DeviceStatus, DevStatBits);
    DevStatusAfterLabel->Caption = line;

	delete[] Buffer;
	Buffer = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TGPIBTestForm::ButtonReceiveClick(TObject *Sender)
{
    int size = 254;
	char line[80];
	char errorstring[80];
    short DeviceStatus = 0;
    char DevStatBits[9];
    short addr = (short)AddressEdit->Text.ToInt();

    char *Buffer = new char[size];
    int BufLength = NBytesToReceiveEdit->Text.ToInt();

    DeviceStatus = GPIBGetDeviceStatus(addr);
    WriteShortBits(DeviceStatus, DevStatBits);
    sprintf(line, " %d or bitwise %s ", DeviceStatus, DevStatBits);
    DevStatusBeforeLabel->Caption = line;

    LabelError->Caption = "No error";
    if (GPIBReceive(addr, Buffer, BufLength) != 0)
    {
 //      char* errorstring = new char[80];
	   GPIBGetError(errorstring);
	   LabelError->Caption = errorstring;
 //      delete[] errorstring;
    }
    MemoOutput->Lines->Add(Buffer);

    int stat = GPIBGetStatusByte();
    char bits[17];
    WriteIntegerBits(stat, bits);
    for (int i=0; i<16;i++)
    {
        if (bits[i] == '1')
        {
            GPIBStatusLine(i, line);  // puts message of bit i in line
            StatusMemo->Lines->Add(line);
        }
	}
    LabelStatus->Caption = GPIBGetStatusByte();
    LabelStatusBits->Caption = bits;
    LabelCount->Caption = GPIBGetLengthCount();

    DeviceStatus = GPIBGetDeviceStatus(addr);
    WriteShortBits(DeviceStatus, DevStatBits);
    sprintf(line, " %d or bitwise %s ", DeviceStatus, DevStatBits);
    DevStatusAfterLabel->Caption = line;

    delete[] Buffer;
}
//---------------------------------------------------------------------------


void WriteIntegerBits(int n, char* buffer)
{
    for (int i=0; i<16; i++)
    {
        buffer[i] = '0';
        if ( n & (1<<i)) buffer[i] = '1';
    }
    buffer[16] = 0;
}

void WriteShortBits(int n, char* buffer)
{
    for (int i=0; i<8; i++)
    {
        buffer[i] = '0';
        if ( n & (1<<i)) buffer[i] = '1';
    }
    buffer[8] = 0;
}




void __fastcall TGPIBTestForm::TimeOutComboBoxChange(TObject *Sender)
{
    int test=0;
    test = GPIBSetTimeOut(TimeOutComboBox->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TGPIBTestForm::EotModeComboBoxChange(TObject *Sender)
{
    GPIBSetEotMode(EotModeComboBox->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TGPIBTestForm::EosModeComboBoxChange(TObject *Sender)
{
    GPIBSetEosMode(EosModeComboBox->ItemIndex);
}
//---------------------------------------------------------------------------



void __fastcall TGPIBTestForm::QuitButtonClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TGPIBTestForm::ListenersButtonClick(TObject *Sender)
{
    int n;
    short addr[32];
    n = GPIBFindAllListeners(addr);

    int MaxIDlength = 128;
    int MaxBufLength = 128;
    char* buffer =  new char[MaxBufLength];
    char* IDstring = new char[MaxIDlength];
    sprintf(buffer, "%d Listeners found: ", n);
    StatusMemo->Lines->Add(buffer);

	for (int i=0; i<n;i++)
    {
        if (addr[i] == NOADDR)
            sprintf(buffer, "One invalid address ");
          else
            if (addr[i] == 0)
                sprintf(buffer, " 0: That's me, your GPIB card ");
              else
              {
                GPIBGetID(addr[i], IDstring, MaxIDlength);
                sprintf(buffer, "%d : %s", addr[i], IDstring);
              }
        StatusMemo->Lines->Add(buffer);
    }
    delete[] buffer;
    delete[] IDstring;
}
//---------------------------------------------------------------------------


void __fastcall TGPIBTestForm::StatusButtonClick(TObject *Sender)
{
    char line[80];
    int stat = GPIBGetStatusByte();
    char bits[17];
    WriteIntegerBits(stat, bits);
    for (int i=0; i<16;i++)
    {
        if (bits[i] == '1')
        {
            GPIBStatusLine(i, line);  // puts message of bit i in line
            StatusMemo->Lines->Add(line);
        }
    }
    LabelStatus->Caption = GPIBGetStatusByte();
    LabelStatusBits->Caption = bits;
    LabelCount->Caption = GPIBGetLengthCount();
}
//---------------------------------------------------------------------------


void __fastcall TGPIBTestForm::NBytesToReceiveEditExit(TObject *Sender)
{
    // We need to check if this is a valid number
    if ((NBytesToReceiveEdit->Text.ToInt() < 1) || (NBytesToReceiveEdit->Text.ToInt() >254)) Text = "254";
}
//---------------------------------------------------------------------------



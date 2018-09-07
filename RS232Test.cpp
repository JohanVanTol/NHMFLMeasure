//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "WSC.h"
#include "RS232Test.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRS232TestForm *RS232TestForm;
void WaitMillisecs(int ms);
int GetChar(int Port, int ms);

//---------------------------------------------------------------------------
__fastcall TRS232TestForm::TRS232TestForm(TComponent* Owner)
    : TForm(Owner)
{
// Set the default values
    PortComboBox->ItemIndex = 0;
    StopBitsComboBox->ItemIndex = 0;
    ParityComboBox->ItemIndex = 0;
    WordlengthComboBox->ItemIndex = 3;
    BaudComboBox->ItemIndex = 5;
}
//---------------------------------------------------------------------------



void __fastcall TRS232TestForm::CloseButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------



void __fastcall TRS232TestForm::SendButtonClick(TObject *Sender)
{
	int error;
	unsigned int t1, t2, t3, t4;
	char command[40];
	char response[1024];
	char err[256];
	char dummy[3];
	dummy[0]=13;
	dummy[1]=10;
	dummy[2]=0;
	strcpy(command, SendEdit->Text.c_str());
	if (TermComboBox->ItemIndex == 1)
		strcat(command, dummy);
	  else
		strcat(command, dummy+1);

	int Port, Parity, Baud, StopBits, WordLen;

	switch (PortComboBox->ItemIndex)
	{
		case 0: Port = COM1;break;
		case 1: Port = COM2;break;
		case 2: Port = COM3;break;
		case 3: Port = COM4;break;
		case 4: Port = COM5;break;
		case 5: Port = COM6;break;
		case 6: Port = COM7;break;
		 default: Port = 0;break;
	}

	switch (ParityComboBox->ItemIndex)
	{
		case 1: Parity = OddParity;break;
		case 2: Parity = EvenParity;break;
		case 3: Parity = MarkParity;break;
		case 4: Parity = SpaceParity;break;
		default: Parity = NoParity;break;
	}

	switch (BaudComboBox->ItemIndex)
	{
		case 0: Baud = Baud110;break;
		case 1: Baud = Baud300;break;
		case 2: Baud = Baud1200;break;
		case 3: Baud = Baud2400;break;
		case 4: Baud = Baud4800;break;
		case 5: Baud = Baud9600;break;
		case 6: Baud = Baud19200;break;
		case 7: Baud = Baud38400;break;
		case 8: Baud = Baud57600;break;
		case 9: Baud = Baud115200;break;
		default: Baud = Baud9600;break;
	}

	switch (WordlengthComboBox->ItemIndex)
	{
		case 0: WordLen = WordLength5;break;
		case 1: WordLen = WordLength6;break;
		case 2: WordLen = WordLength7;break;
		case 3: WordLen = WordLength8;break;
		default : WordLen = WordLength8;break;
	}

	switch (StopBitsComboBox->ItemIndex)
	{
		case 0: StopBits = OneStopBit;break;
		case 1: StopBits = TwoStopBits;break;
		default: StopBits = 0;
	}

	int BufSize = BufferEdit->Text.ToInt();
	t1 = GetTickCount();
	error = SioReset(Port,BufSize, BufSize);

	if (error < 0)
	{
		error = SioWinError(err,255);
		error = SioDone(Port);
		error = SioReset(Port,BufSize, BufSize);
	}

	if (error < 0)
	{
		ErrorEdit->Text = "Cannot open port";
		return;
	}

	error = SioParms(Port, Parity, StopBits, WordLen);
	if (error < 0)
	{
		ErrorEdit->Text = "Error in Parameter setting";
		error = SioDone(Port);
		return;
	}

	error = SioBaud(Port, Baud);
	if (error < 0)
	{
		ErrorEdit->Text = "Error setting Baudrate";
		error = SioDone(Port);
		return;
	}

	error = SioTxClear(Port);
	if (error < 0)
	{
		ErrorEdit->Text = "Error clearing transfer buffer";
		error = SioDone(Port);
		return;
	}

	error = SioRxClear(Port);
	if (error < 0)
	{
		ErrorEdit->Text = "Error clearing transfer buffer";
		error = SioDone(Port);
		return;
	}
//   if (test == WSC_WIN32ERR)
//        test = SioWinError(err,256);
//    ErrorEdit->Text = error;

// Here the RTS line is asserted, to power the isobus
	error = SioRTS(Port,'S');
	if (error < 0)
	{
		ErrorEdit->Text = "Error asserting RTS line";
		error = SioDone(Port);
		return;
	}

//    error = SioFlow(Port, 'H');  // Hardware flow control
//    if (error != 0)
//    {
//        ErrorEdit->Text = "Error setting flow control";
//        error = SioDone(Port);
//        return;
//    }
	for (int i=0;i<strlen(command);i++)
	{
		error = SioPutc(Port, command[i]);
		WaitMillisecs(10);
		if (error < 0)
		{
			ErrorEdit->Text = "Error sending data";
			error = SioDone(Port);
			return;
		}
	}
	t2 = GetTickCount();

	int j=0;
	char c;
	while (( (c = GetChar(Port,500)) != WSC_NO_DATA) && ((c != 13) && (c != 10)))
	{
	   response[j++]  = c;
	}
	response[j] = 0;
	ResponseEdit->Text = response;
	t3 = GetTickCount();
	error = SioDone(Port);
	if (error < 0)
	{
		ErrorEdit->Text = "Error closing port";
		return;
	}
	ErrorEdit->Text = t3-t1;
}
//---------------------------------------------------------------------------
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





//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DAQDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDAQ_Dialog *DAQ_Dialog;
extern IOTechDaqBoard *DAQ;

//---------------------------------------------------------------------------
__fastcall TDAQ_Dialog::TDAQ_Dialog(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TDAQ_Dialog::FormActivate(TObject *Sender)
{
 //   Daq.Initialize();
}
//---------------------------------------------------------------------------

void __fastcall TDAQ_Dialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
//    DAQ->Close();
}
//---------------------------------------------------------------------------

void __fastcall TDAQ_Dialog::Button1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TDAQ_Dialog::UpdateADCButtonClick(TObject *Sender)
{
//    Set the channels Gain, whether active or not
    DaqAdcGain gain;
	if (!DAQ->IsOpen()) return;
// ch 0
    switch (GainComboBoxCh0->ItemIndex)
    {
		case 0: gain = DgainX1; break;
		case 1: gain = DgainX2; break;
		case 2: gain = DgainX4; break;
		case 3: gain = DgainX8; break;
		case 4: gain = DgainX16; break;
		case 5: gain = DgainX32; break;
		case 6: gain = DgainX64; break;
    }
    DAQ->SetGain(0,gain);

// ch 1
    switch (GainComboBoxCh1->ItemIndex)
    {
		case 0: gain = DgainX1;  break;
		case 1: gain = DgainX2;  break;
		case 2: gain = DgainX4;  break;
		case 3: gain = DgainX8;  break;
		case 4: gain = DgainX16; break;
		case 5: gain = DgainX32; break;
		case 6: gain = DgainX64; break;
    }
    DAQ->SetGain(1,gain);

// ch 2
    switch (GainComboBoxCh2->ItemIndex)
    {
		case 0: gain = DgainX1;  break;
		case 1: gain = DgainX2; break;
		case 2: gain = DgainX4; break;
		case 3: gain = DgainX8; break;
		case 4: gain = DgainX16; break;
		case 5: gain = DgainX32; break;
		case 6: gain = DgainX64; break;
    }
	DAQ->SetGain(2,gain);

// ch 3
	switch (GainComboBoxCh3->ItemIndex)
    {
		case 0: gain = DgainX1; break;
		case 1: gain = DgainX2; break;
		case 2: gain = DgainX4; break;
		case 3: gain = DgainX8; break;
		case 4: gain = DgainX16;break;
		case 5: gain = DgainX32; break;
		case 6: gain = DgainX64; break;
    }
    DAQ->SetGain(3,gain);

// ch 4
    switch (GainComboBoxCh4->ItemIndex)
    {
		case 0: gain = DgainX1; break;
		case 1: gain = DgainX2; break;
		case 2: gain = DgainX4; break;
		case 3: gain = DgainX8; break;
		case 4: gain = DgainX16;break;
		case 5: gain = DgainX32;break;
		case 6: gain = DgainX64;break;
    }
    DAQ->SetGain(4,gain);

// ch 5
    switch (GainComboBoxCh5->ItemIndex)
    {
		case 0: gain = DgainX1; break;
		case 1: gain = DgainX2; break;
		case 2: gain = DgainX4; break;
		case 3: gain = DgainX8; break;
		case 4: gain = DgainX16;break;
		case 5: gain = DgainX32;break;
		case 6: gain = DgainX64;break;
    }
    DAQ->SetGain(5,gain);

// ch 6
    switch (GainComboBoxCh6->ItemIndex)
    {
		case 0: gain = DgainX1;  break;
		case 1: gain = DgainX2;  break;
		case 2: gain = DgainX4;  break;
		case 3: gain = DgainX8;  break;
		case 4: gain = DgainX16; break;
		case 5: gain = DgainX32; break;
		case 6: gain = DgainX64; break;
    }
    DAQ->SetGain(6,gain);

// ch 7
    switch (GainComboBoxCh7->ItemIndex)
    {
		case 0: gain = DgainX1;   break;
		case 1: gain = DgainX2;   break;
		case 2: gain = DgainX4;   break;
		case 3: gain = DgainX8;   break;
		case 4: gain = DgainX16;  break;
		case 5: gain = DgainX32;  break;
        case 6: gain = DgainX64;   break;
	}
	DAQ->SetGain(7,gain);

    int nch = 0;    // the number of channels to read
    int maxchan = 8;
    int *chan = new int[maxchan];

    if (CheckBoxCh0->Checked) chan[nch++] = 0;
    if (CheckBoxCh1->Checked) chan[nch++] = 1;
    if (CheckBoxCh2->Checked) chan[nch++] = 2;
    if (CheckBoxCh3->Checked) chan[nch++] = 3;
    if (CheckBoxCh4->Checked) chan[nch++] = 4;
    if (CheckBoxCh5->Checked) chan[nch++] = 5;
    if (CheckBoxCh6->Checked) chan[nch++] = 6;
    if (CheckBoxCh7->Checked) chan[nch++] = 7;

// configure the scan
    double freq = 5000;
    int scans = 10;
    double* values = NULL;
    AnsiString ValStr;
    DAQ->ChannelInit(nch, chan);
    if (nch >0)
    {
        freq = 60000.0/(double)nch;
		scans = 1000/nch;
		DAQ->SetAcquisition(freq, scans);

//    trigger the scan
        DAQ->Trigger();

        DAQ->WaitToComplete();

        values = new double[nch];

        DAQ->GetValues(nch, values);

		for (int i = 0; i< nch; i++)
        {
            ValStr.sprintf("%8.4lf", values[i]);
          switch (chan[i])
            {
            case 0: LabelCh0->Caption = ValStr; break;
            case 1: LabelCh1->Caption = ValStr; break;
            case 2: LabelCh2->Caption = ValStr; break;
            case 3: LabelCh3->Caption = ValStr; break;
            case 4: LabelCh4->Caption = ValStr; break;
            case 5: LabelCh5->Caption = ValStr; break;
            case 6: LabelCh6->Caption = ValStr; break;
            case 7: LabelCh7->Caption = ValStr; break;
            }
        }
    }
    delete [] chan;
    if (values != NULL) delete [] values;
}

//---------------------------------------------------------------------------

void __fastcall TDAQ_Dialog::IO_ExecuteButtonClick(TObject *Sender)
{

//	daqIOGet8255Conf(handle, 0 ,0, 0, 0, &config);

			//write settings to internal register
//    daqIOWrite(handle, DiodtLocal8255, Diodp8255IR, 0, DioepP2, config);

			//update ports A, B, and C
//	daqIOWrite(handle, DiodtLocal8255, Diodp8255A, 0, DioepP2, BYTE_A);
//    daqIOWrite(handle, DiodtLocal8255, Diodp8255B, 0, DioepP2, BYTE_B);
//    daqIOWrite(handle, DiodtLocal8255, Diodp8255C, 0, DioepP2, BYTE_C);


    char A, B, C;
	int A1, A2, B1, B2, C1, C2;
    A1 = IOComboBoxChA1->ItemIndex;
    A2 = IOComboBoxChA2->ItemIndex;
    B1 = IOComboBoxChB1->ItemIndex;
    B2 = IOComboBoxChB2->ItemIndex;
    C1 = IOComboBoxChC1->ItemIndex;
    C2 = IOComboBoxChC2->ItemIndex;

    A = (char)(16*A1 + A2);
    B = (char)(16*B1 + B2);
    C = (char)(16*C1 + C2);
    if (DAQ->IsOpen()) DAQ->SetIO(A, B, C);
}
//---------------------------------------------------------------------------


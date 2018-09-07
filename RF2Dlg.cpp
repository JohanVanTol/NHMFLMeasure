//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "RF2Dlg.h"
#include "SMB100A.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

extern SMY02* RF2;

TRF2Dialog *RF2Dialog;
//---------------------------------------------------------------------------
__fastcall TRF2Dialog::TRF2Dialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TRF2Dialog::FormActivate(TObject *Sender)
{
// When the form is activated, we should update all
// information from the oscillator

    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::FMRadioButtonClick(TObject *Sender)
{
    if (FMRadioButton->Checked == true)
    {
        FMUnitLabel->Caption = "kHz";
        FMExtDCRadioButton->Enabled = true;
        FMDualDCRadioButton->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::PMRadioButtonClick(TObject *Sender)
{
   if (PMRadioButton->Checked == true)
   {
    FMUnitLabel->Caption = "rad";
    if (FMExtDCRadioButton->Checked) FMExtACRadioButton->Checked = true;
    if (FMDualDCRadioButton->Checked) FMDualACRadioButton->Checked = true;
    FMExtDCRadioButton->Enabled = false;
    FMDualDCRadioButton->Enabled = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::ErrorButtonClick(TObject *Sender)
{
    char message[60];
    int error = RF2->IsError(message, 59);
    ErrorEdit->Text = message;
}
//---------------------------------------------------------------------------

int TRF2Dialog::GetAllData()
{
// SEction Current settings

        double freq = RF2->GetFreq();
        FreqEdit->Text = FloatToStrF(freq, ffFixed, 12,4);
        double power = RF2->GetPower();
        if (power < -140)
        {
            PowerEdit->Text = "-120.0";
            PowerCheckBox->Checked = false;
            PowerEdit->Enabled = false;
        }
          else
        {
            PowerEdit->Text = FloatToStrF(power, ffFixed, 12, 1);
            PowerEdit->Enabled = true;
            PowerCheckBox->Checked = true;
        }
        double AFfreq = RF2->GetAFfreq();
        ModFreqEdit->Text = FloatToStrF(AFfreq, ffFixed, 12, 3);

// Section Sweep parameters

        double StartFreq = RF2->GetStartFreq();
        StartFreqEdit->Text = FloatToStrF(StartFreq, ffFixed, 12,3);
        double StopFreq = RF2->GetStopFreq();
        StopFreqEdit->Text = FloatToStrF(StopFreq, ffFixed, 12,3);
		double Step = RF2->GetStep();
		if (Step < 0.001) Step = 0.001;  //if Step < 1Hz Step = 1Hz
		StepEdit->Text = FloatToStrF(Step, ffFixed, 12,3);
		int NSteps = ceil(1000*(StopFreq-StartFreq)/Step);
		NStepsEdit->Text = AnsiString(NSteps);
/*		int Time = RF2->GetStepTime();
		DwellEdit->Text = AnsiString(Time);
		int Sweep = RF2->GetSweepType();
		switch (Sweep)
		{
			case 1: SweepLabel->Caption = "Sweep ON"; break;
			case 2: SweepLabel->Caption = "Sweep RESET"; break;
			default: SweepLabel->Caption = "Sweep OFF"; break;
		}

// Section Modulation
	double modul = 0.0;
	int AM = RF2->GetAM(&modul);
	AMEdit->Enabled = true;
	AMOffRadioButton->Checked = true;
	AMEdit->Text = FloatToStrF(modul, ffFixed, 5,1);
	switch (AM)
	{
		case 0: AMEdit->Enabled = false;
				AMOffRadioButton->Checked = true;
				break;
		case 1: AMIntRadioButton->Checked = true;
				AMEdit->Enabled = true;
				break;
		case 2: AMExtAcRadioButton->Checked = true;
				AMEdit->Enabled = true;
				break;
		case 3: AMExtDcRadioButton->Checked = true;
				AMEdit->Enabled = true;
				break;
		case 4: AMDualACRadioButton->Checked = true;
				AMEdit->Enabled = true;
				break;
		case 5: AMDualDCRadioButton->Checked = true;
				AMEdit->Enabled = true;
				break;
		default:AMEdit->Enabled = false;
				AMOffRadioButton->Enabled = true;
				break;
		}

	int FM = RF2->GetFM(&modul);
	FMEdit->Enabled = true;
	FMRadioButton->Checked = true;
	FMOFFRadioButton->Checked = true;
	FMEdit->Text = FloatToStrF(modul, ffFixed, 5,1);
	switch (FM)
	{
		case 0: FMEdit->Enabled = false;
				FMRadioButton->Checked = false;
				FMOFFRadioButton->Checked = true;
				break;
		case 1: FMIntRadioButton->Checked = true;
				FMEdit->Enabled = true;
				break;
		case 2: FMExtACRadioButton->Checked = true;
				FMEdit->Enabled = true;
				break;
		case 3: FMExtDCRadioButton->Checked = true;
				FMEdit->Enabled = true;
				break;
		case 4: FMDualACRadioButton->Checked = true;
				FMEdit->Enabled = true;
				break;
		case 5: FMDualDCRadioButton->Checked = true;
				FMEdit->Enabled = true;
				break;
		default:FMEdit->Enabled = false;
				FMOFFRadioButton->Enabled = true;
				FMRadioButton->Checked = false;
				break;
	}

	int PM = RF2->GetPM(&modul);
	PMRadioButton->Checked = false;
	if (PM != 0)
	{
		FMUnitLabel->Caption = "rad";
		FMEdit->Enabled = true;
		PMRadioButton->Checked = true;
		FMOFFRadioButton->Checked = true;
		FMEdit->Text = FloatToStrF(modul, ffFixed, 5,1);
		if (PM != 0) FMUnitLabel->Caption = "rad";
		switch (PM)
		{
			case 0: FMEdit->Enabled = false;
					FMRadioButton->Checked = false;
					FMOFFRadioButton->Checked = true;
					break;
			case 1: FMIntRadioButton->Checked = true;
					FMEdit->Enabled = true;
					break;
			case 2: FMExtACRadioButton->Checked = true;
					FMEdit->Enabled = true;
					break;
			case 3: FMDualACRadioButton->Checked = true;
					FMEdit->Enabled = true;
					break;
			default:FMEdit->Enabled = false;
					FMOFFRadioButton->Enabled = true;
					FMRadioButton->Checked = false;
					break;
		}
	}
	return 0;
*/
}

int TRF2Dialog::SetAllData()
{
	RF2->SetFreq(FreqEdit->Text.ToDouble());
	RF2->SetPower(PowerEdit->Text.ToDouble());
	RF2->SetAFfreq(ModFreqEdit->Text.ToDouble());
	RF2->SetStartFreq(StartFreqEdit->Text.ToDouble());
	RF2->SetStopFreq(StopFreqEdit->Text.ToDouble());
	RF2->SetStep(StepEdit->Text.ToDouble());
	RF2->SetStepTime(DwellEdit->Text.ToInt());
/*    int AM = 0;
	if (AMIntRadioButton->Checked) AM=1;
	if (AMExtAcRadioButton->Checked) AM=2;
	if (AMExtDcRadioButton->Checked) AM=3;
	if (AMDualACRadioButton->Checked) AM=4;
	if (AMDualDCRadioButton->Checked) AM=5;
	RF2->SetAM(AM,AMEdit->Text.ToDouble());
	int FM = 0;
	if (FMIntRadioButton->Checked) FM=1;
	if (FMExtACRadioButton->Checked) FM=2;
	if (FMExtDCRadioButton->Checked) FM=3;
	if (FMDualACRadioButton->Checked) FM=4;
	if (FMDualDCRadioButton->Checked) FM=5;
	if (FMRadioButton->Checked) RF2->SetFM(FM,FMEdit->Text.ToDouble());
	if (PMRadioButton->Checked) RF2->SetPM(FM,FMEdit->Text.ToDouble());
*/
}

void __fastcall TRF2Dialog::SetButtonClick(TObject *Sender)
{
    SetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::CalcelButtonClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::OKButtonClick(TObject *Sender)
{
    SetAllData();
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::FreqEditChange(TObject *Sender)
{
    try {
     double test = FreqEdit->Text.ToDouble();
     }
     catch (const EConvertError &E)
     {
        FreqEdit->Text = "999.0";
     }
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::AMOffRadioButtonClick(TObject *Sender)
{
    if ( AMOffRadioButton->Checked) AMEdit->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::AMIntRadioButtonClick(TObject *Sender)
{
    if ( AMIntRadioButton->Checked) AMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::AMExtAcRadioButtonClick(TObject *Sender)
{
    if ( AMExtAcRadioButton->Checked) AMEdit->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::AMExtDcRadioButtonClick(TObject *Sender)
{
    if ( AMExtDcRadioButton->Checked) AMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::AMDualACRadioButtonClick(TObject *Sender)
{
    if ( AMDualACRadioButton->Checked) AMEdit->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::AMDualDCRadioButtonClick(TObject *Sender)
{
    if ( AMDualDCRadioButton->Checked) AMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::FMOFFRadioButtonClick(TObject *Sender)
{
    if ( FMOFFRadioButton->Checked) FMEdit->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::FMIntRadioButtonClick(TObject *Sender)
{
    if ( FMIntRadioButton->Checked) FMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::FMExtACRadioButtonClick(TObject *Sender)
{
    if ( FMExtACRadioButton->Checked) FMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::FMExtDCRadioButtonClick(TObject *Sender)
{
    if ( FMExtDCRadioButton->Checked) FMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::FMDualACRadioButtonClick(TObject *Sender)
{
   if ( FMDualACRadioButton->Checked) FMEdit->Enabled = true;    
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::FMDualDCRadioButtonClick(TObject *Sender)
{
   if ( FMDualDCRadioButton->Checked) FMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

int TRF2Dialog::CalculateNSteps()
{
    double steps = StopFreqEdit->Text.ToDouble()-StartFreqEdit->Text.ToDouble();
    if (StepEdit->Text.ToDouble() >0.001)
        steps /= 0.001*StepEdit->Text.ToDouble();
      else steps = 1.0;
    return ceil(steps);
}

double TRF2Dialog::CalculateStep()
{
    double step = StopFreqEdit->Text.ToDouble()-StartFreqEdit->Text.ToDouble();
    if (NStepsEdit->Text.ToDouble() > 0)
        step /= 0.001*(NStepsEdit->Text.ToInt());
      else step = 1.0;
    return step;
}


void __fastcall TRF2Dialog::StartFreqEditExit(TObject *Sender)
{
    NStepsEdit->Text = CalculateNSteps();
}
//---------------------------------------------------------------------------


void __fastcall TRF2Dialog::StepEditExit(TObject *Sender)
{
    NStepsEdit->Text = CalculateNSteps();
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::StopFreqEditExit(TObject *Sender)
{
    NStepsEdit->Text = CalculateNSteps();
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::NStepsEditExit(TObject *Sender)
{
    StepEdit->Text = FloatToStrF(CalculateStep(), ffFixed, 12,3);
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::SweepStartButtonClick(TObject *Sender)
{
    SetAllData();
    RF2->SetSweepType(1);
    SweepLabel->Caption = "Sweep ON";
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::StopSweepButtonClick(TObject *Sender)
{
    RF2->SetSweepType(0);
    SweepLabel->Caption = "Sweep OFF";
}
//---------------------------------------------------------------------------

void __fastcall TRF2Dialog::SweepResetButtonClick(TObject *Sender)
{
	RF2->SetSweepType(2);
    SweepLabel->Caption = "Sweep RESET";
}


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "ENDORParameterDlg.h"
//#include "IEEEdev.h"
#include "SMB100A.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern SMY02* RF2;
extern SMB100A* RFsource;
extern AR100W1000A* RFAmplif;
TENDORParametersDialog *ENDORParametersDialog;
//---------------------------------------------------------------------------
__fastcall TENDORParametersDialog::TENDORParametersDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TENDORParametersDialog::FormActivate(TObject *Sender)
{
// When the form is activated, we should update all
// information from the oscillator

    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::FMRadioButtonClick(TObject *Sender)
{
    if (FMRadioButton->Checked == true)
    {
        FMUnitLabel->Caption = "kHz";
        FMExtDCRadioButton->Enabled = true;
        FMDualDCRadioButton->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::PMRadioButtonClick(TObject *Sender)
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

void __fastcall TENDORParametersDialog::ErrorButtonClick(TObject *Sender)
{
    char message[60];
    int error = RFsource->IsError(message, 59);
    ErrorEdit->Text = message;
}
//---------------------------------------------------------------------------

int TENDORParametersDialog::GetAllData()
{
// SEction Current settings

		double freq = RFsource->GetFreq();
		FreqEdit->Text = FloatToStrF(freq, ffFixed, 12,4);
		double power = RFsource->GetPower();
/*        if (power < -140)
		{
			PowerCheckBox->Checked = false;
			PowerEdit->Enabled = false;
		}
		  else
		{
*/			PowerEdit->Text = FloatToStrF(power, ffFixed, 12, 1);
			PowerEdit->Enabled = true;
//			PowerCheckBox->Checked = true;
//		}

		double AFfreq = RFsource->GetAFfreq();
		ModFreqEdit->Text = FloatToStrF(AFfreq, ffFixed, 12, 3);
/*		double Gain = RFAmplif->GetGain();
		AmpGainEdit->Text = FloatToStrF(Gain, ffFixed, 6,1);
		int PowerStatus = RFAmplif->GetPowerStatus();
		if (PowerStatus == 2 ) PowerCheckBox->Checked = true;
			else PowerCheckBox->Checked = false;
*/
		double RF2freq = RF2->GetFreq();
		RF2FreqEdit->Text = FloatToStrF(RF2freq, ffFixed, 12,4);
		double RF2power = RF2->GetPower();
		RF2PowerEdit->Text = FloatToStrF(RF2power, ffFixed, 12, 1);

// Section Sweep parameters

		StartFreq = RFsource->GetStartFreq();
		StartFreqEdit->Text = FloatToStrF(StartFreq, ffFixed, 12,3);
		EndFreq = RFsource->GetStopFreq();
		StopFreqEdit->Text = FloatToStrF(EndFreq, ffFixed, 12,3);
		Step = RFsource->GetStep();
		if (Step < 0.001) Step = 0.001;  //if Step < 1Hz Step = 1Hz
		StepEdit->Text = FloatToStrF(Step, ffFixed, 12,3);
		int NSteps = ceil(1000*(EndFreq-StartFreq)/Step);
		nStep = NSteps;
		NStepsEdit->Text = AnsiString(NSteps);
		int Time = RFsource->GetStepTime();
		DwellEdit->Text = AnsiString(Time);

		RF2StartFreq = RF2->GetStartFreq();
		RF2StartFreqEdit->Text = FloatToStrF(RF2StartFreq, ffFixed, 12,3);
		RF2EndFreq = RF2->GetStopFreq();
		RF2StopFreqEdit->Text = FloatToStrF(RF2EndFreq, ffFixed, 12,3);
		RF2Step = RF2->GetStep();
		if (RF2Step < 0.001) RF2Step = 0.001;  //if Step < 1Hz Step = 1Hz
		RF2StepEdit->Text = FloatToStrF(RF2Step, ffFixed, 12,3);
		int RF2NSteps = ceil(1000*(RF2EndFreq-RF2StartFreq)/RF2Step);
		RF2nStep = RF2NSteps;
		RF2NStepsEdit->Text = AnsiString(RF2NSteps);

// Section Modulation
	double modul = 0.0;
	int AM = RFsource->GetAM(&modul);
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

    int FM = RFsource->GetFM(&modul);
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

    int PM = RFsource->GetPM(&modul);
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

}

int TENDORParametersDialog::SetAllData()
{
//  Set current frequency to start of sweep
	RFsource->SetFreq(StartFreqEdit->Text.ToDouble());
	RFsource->SetPower(PowerEdit->Text.ToDouble());
//	RFsource->SetAFfreq(ModFreqEdit->Text.ToDouble());
	RFsource->SetStartFreq(StartFreqEdit->Text.ToDouble());
	RFsource->SetStopFreq(StopFreqEdit->Text.ToDouble());
	RFsource->SetStep(StepEdit->Text.ToDouble());
//	RFsource->SetStepTime(DwellEdit->Text.ToInt());

	Step = StepEdit->Text.ToDouble();
	StartFreq = StartFreqEdit->Text.ToDouble();
	EndFreq = StopFreqEdit->Text.ToDouble();
	Power = PowerEdit->Text.ToDouble();
	nStep = NStepsEdit->Text.ToInt();

	RF2->SetFreq(RF2StartFreqEdit->Text.ToDouble());
	RF2->SetPower(RF2PowerEdit->Text.ToDouble());
	RF2->SetStartFreq(RF2StartFreqEdit->Text.ToDouble());
	RF2->SetStopFreq(RF2StopFreqEdit->Text.ToDouble());
	RF2->SetStep(RF2StepEdit->Text.ToDouble());

	RF2Step = RF2StepEdit->Text.ToDouble();
	RF2StartFreq = RF2StartFreqEdit->Text.ToDouble();
	RF2EndFreq = RF2StopFreqEdit->Text.ToDouble();
	RF2Power = RF2PowerEdit->Text.ToDouble();
	RF2nStep = RF2NStepsEdit->Text.ToInt();

	//    RFAmplif->SetGain(AmpGainEdit->Text.ToDouble());

	int AM = 0;
    if (AMIntRadioButton->Checked) AM=1;
    if (AMExtAcRadioButton->Checked) AM=2;
    if (AMExtDcRadioButton->Checked) AM=3;
    if (AMDualACRadioButton->Checked) AM=4;
    if (AMDualDCRadioButton->Checked) AM=5;
    RFsource->SetAM(AM,AMEdit->Text.ToDouble());
    int FM = 0;
    if (FMIntRadioButton->Checked) FM=1;
    if (FMExtACRadioButton->Checked) FM=2;
    if (FMExtDCRadioButton->Checked) FM=3;
    if (FMDualACRadioButton->Checked) FM=4;
    if (FMDualDCRadioButton->Checked) FM=5;
    if (FMRadioButton->Checked) RFsource->SetFM(FM,FMEdit->Text.ToDouble());
    if (PMRadioButton->Checked) RFsource->SetPM(FM,FMEdit->Text.ToDouble());

}

void __fastcall TENDORParametersDialog::SetButtonClick(TObject *Sender)
{
    SetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::CalcelButtonClick(TObject *Sender)
{
	Close();
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::OKButtonClick(TObject *Sender)
{
	SetAllData();
	Close();
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::FreqEditChange(TObject *Sender)
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

void __fastcall TENDORParametersDialog::AMOffRadioButtonClick(TObject *Sender)
{
    if ( AMOffRadioButton->Checked) AMEdit->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::AMIntRadioButtonClick(TObject *Sender)
{
    if ( AMIntRadioButton->Checked) AMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::AMExtAcRadioButtonClick(TObject *Sender)
{
    if ( AMExtAcRadioButton->Checked) AMEdit->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::AMExtDcRadioButtonClick(TObject *Sender)
{
    if ( AMExtDcRadioButton->Checked) AMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::AMDualACRadioButtonClick(TObject *Sender)
{
    if ( AMDualACRadioButton->Checked) AMEdit->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::AMDualDCRadioButtonClick(TObject *Sender)
{
    if ( AMDualDCRadioButton->Checked) AMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::FMOFFRadioButtonClick(TObject *Sender)
{
    if ( FMOFFRadioButton->Checked) FMEdit->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::FMIntRadioButtonClick(TObject *Sender)
{
    if ( FMIntRadioButton->Checked) FMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::FMExtACRadioButtonClick(TObject *Sender)
{
    if ( FMExtACRadioButton->Checked) FMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::FMExtDCRadioButtonClick(TObject *Sender)
{
    if ( FMExtDCRadioButton->Checked) FMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::FMDualACRadioButtonClick(TObject *Sender)
{
   if ( FMDualACRadioButton->Checked) FMEdit->Enabled = true;    
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::FMDualDCRadioButtonClick(TObject *Sender)
{
   if ( FMDualDCRadioButton->Checked) FMEdit->Enabled = true;
}
//---------------------------------------------------------------------------

int TENDORParametersDialog::CalculateNSteps()
{
    double steps = StopFreqEdit->Text.ToDouble()-StartFreqEdit->Text.ToDouble();
    if (StepEdit->Text.ToDouble() >0.001)
		steps /= 0.001*StepEdit->Text.ToDouble();
	  else steps = 1.0;
	return ceil(steps);
}

double TENDORParametersDialog::CalculateStep()
{
	double step = StopFreqEdit->Text.ToDouble()-StartFreqEdit->Text.ToDouble();
	if (NStepsEdit->Text.ToDouble() > 0)
		step /= 0.001*(NStepsEdit->Text.ToInt());
	  else step = 1.0;
	return step;
}

int TENDORParametersDialog::CalculateRF2NSteps()
{
	double steps = RF2StopFreqEdit->Text.ToDouble() - RF2StartFreqEdit->Text.ToDouble();
	if (RF2StepEdit->Text.ToDouble() >0.001)
		steps /= 0.001*RF2StepEdit->Text.ToDouble();
	  else steps = 1.0;
	return ceil(steps);
}

double TENDORParametersDialog::CalculateRF2Step()
{
	double step = RF2StopFreqEdit->Text.ToDouble() - RF2StartFreqEdit->Text.ToDouble();
	if (RF2NStepsEdit->Text.ToDouble() > 0)
		step /= 0.001*(RF2NStepsEdit->Text.ToInt());
	  else step = 1.0;
	return step;
}


void __fastcall TENDORParametersDialog::StartFreqEditExit(TObject *Sender)
{
	NStepsEdit->Text = CalculateNSteps();
}
//---------------------------------------------------------------------------


void __fastcall TENDORParametersDialog::StepEditExit(TObject *Sender)
{
	NStepsEdit->Text = CalculateNSteps();
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::StopFreqEditExit(TObject *Sender)
{
	NStepsEdit->Text = CalculateNSteps();
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::NStepsEditExit(TObject *Sender)
{
	StepEdit->Text = FloatToStrF(CalculateStep(), ffFixed, 12,3);
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::RF2StartFreqEditExit(TObject *Sender)
{
	RF2NStepsEdit->Text = CalculateRF2NSteps();
}
//---------------------------------------------------------------------------


void __fastcall TENDORParametersDialog::RF2StepEditExit(TObject *Sender)
{
	RF2NStepsEdit->Text = CalculateRF2NSteps();
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::RF2StopFreqEditExit(TObject *Sender)
{
	RF2NStepsEdit->Text = CalculateRF2NSteps();
}
//---------------------------------------------------------------------------

void __fastcall TENDORParametersDialog::RF2NStepsEditExit(TObject *Sender)
{
	RF2StepEdit->Text = FloatToStrF(CalculateRF2Step(), ffFixed, 12,3);
}
//---------------------------------------------------------------------------




void __fastcall TENDORParametersDialog::PowerCheckBoxClick(TObject *Sender)
{
	if (PowerCheckBox->Checked == false)
		RFAmplif->SetStandBy();
	  else
		RFAmplif->SetOperate();
}
//---------------------------------------------------------------------------




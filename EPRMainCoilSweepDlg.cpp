//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include <stdio.h>
#include "GPIB_NatlInstrum.h"
#include "EPRMainCoilSweepDlg.h"
#include "Valid.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEPRMainSweepDialog *EPRMainSweepDialog;
//---------------------------------------------------------------------------
__fastcall TEPRMainSweepDialog::TEPRMainSweepDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEPRMainSweepDialog::OKButtonClick(TObject *Sender)
{
    SetAllData();
//    ModalResult = mbOK;
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::CancelButtonClick(TObject *Sender)
{
    ModalResult = mbCancel;
}
//---------------------------------------------------------------------------
int TEPRMainSweepDialog::SetAllData()
{
    SetStartField(StartFieldEdit->Text.ToDouble());
    SetEndField(EndFieldEdit->Text.ToDouble());
    SetSweepSpeed(SweepSpeedEdit->Text.ToDouble());
    int AutoAv;
    if (AverageCheckBox->Checked)
    {
        AutoAv = CalcAutoNpts();
        AutoNptsLabel->Caption = AutoAv;
        if (AutoAverageRadioButton->Checked)
            AverageNpts = AutoAv;
          else
            if (ValidInt(ManNptsEdit->Text.c_str(), &AutoAv))
                AverageNpts = AutoAv;
    }
    return 0;
}

///////////////////

int TEPRMainSweepDialog::CalcAutoNpts()
{
    // make sure we have the right numbers
    SetStartField(StartFieldEdit->Text.ToDouble());
    SetEndField(EndFieldEdit->Text.ToDouble());
    SetSweepSpeed(SweepSpeedEdit->Text.ToDouble());
	double SweepDuration = 1000.0;
	int np = 1;
	if (SweepSpeed > 0)
	   SweepDuration = 1000 * fabs(EndField-StartField)/SweepSpeed;
	if (FieldModeCheckBox->Checked)
		np = 20*(int)SweepDuration + 1;
	  else
		 np = 5 * (int)SweepDuration + 1;

	while ( np > 16000 )
	  np = ceil(np/2);

	return np;
}
/////

int TEPRMainSweepDialog::UpdateDuration()
{
	char durationString[24];
	double duration = GetSweepDuration()/60.0;
	double swSpeed = SweepSpeed;
	if (swSpeed < 2.0) swSpeed = 2.0;       // this should be the SafeSpeed
	sprintf(durationString,"Single scan %6.1f min", duration);
	DurationLabel->Caption = durationString;
	double expduration = duration;
	if (UpDownCheckBox->Checked) {
		expduration *= 2.0;
		if (AverageCheckBox->Checked)
			expduration *= AverEdit->Text.ToInt();
	}
	  else if ((AverageCheckBox->Checked) && (AverEdit->Text.ToInt() >1))
	  {
			expduration += (1000 * fabs(EndField-StartField)/swSpeed)/60.0;
			expduration *= AverEdit->Text.ToInt();
			expduration -= (1000 * fabs(EndField-StartField)/swSpeed)/60.0;
	  }
	sprintf(durationString,"Experiment %6.1f min", expduration);
	ExpDurationLabel->Caption = durationString;

	return 0;
}

void __fastcall TEPRMainSweepDialog::AverageCheckBoxClick(TObject *Sender)
{
	if (AverageCheckBox->Checked)
		AverEdit->Enabled = true;
	  else
		AverEdit->Enabled = false;
	SetAllData();
	UpdateDuration();
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::AutoAverageRadioButtonClick(
	  TObject *Sender)
{
	SetAllData();
	if (AutoAverageRadioButton->Checked)
		ManNptsEdit->Enabled = false;
	else
		ManNptsEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::ManualAverageRadioButtonClick(
	  TObject *Sender)
{
	SetAllData();
	if (AutoAverageRadioButton->Checked)
		ManNptsEdit->Enabled = false;
	else
		ManNptsEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::StartFieldEditChange(TObject *Sender)
{
	SetAllData();
	UpdateDuration();
}
//---------------------------------------------------------------------------


void __fastcall TEPRMainSweepDialog::SweepSpeedEditChange(TObject *Sender)
{
	if (SweepSpeedEdit->Text.ToDouble() < 1e-8) return;
	SetAllData();
	UpdateDuration();
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::EndFieldEditChange(TObject *Sender)
{
	SetAllData();
	UpdateDuration();
}

//---------------------------------------------------------------------------
double TEPRMainSweepDialog::GetSweepDuration() const      // in minutes
{
	if (SweepSpeed >0.00001)
		return fabs(StartField-EndField)*1000.0/SweepSpeed;
	return 999.9;
}

//--------------------
void __fastcall TEPRMainSweepDialog::FormActivate(TObject *Sender)
{
	SweepSpeedEdit->Text = FormatFloat("0.0#####",SweepSpeed);
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::UpDownCheckBoxClick(TObject *Sender)
{
	UpdateDuration();
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::AverEditChange(TObject *Sender)
{
	UpdateDuration();	
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::FormCreate(TObject *Sender)
{
	LI = NULL;	
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::FormDestroy(TObject *Sender)
{
	LI = NULL;	
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	LI = NULL;	
}
//---------------------------------------------------------------------------

void __fastcall TEPRMainSweepDialog::Button1Click(TObject *Sender)
{
	UpdateModulation();
}
//---------------------------------------------------------------------------

int TEPRMainSweepDialog::UpdateModulation()
{
	double ModAmp = 0.04;
	double ModFreq = 1.0;
	LI->SetModulationAmp(ModAmp);
	LI->SetModulationFreq(ModFreq);
}

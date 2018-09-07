//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DACSweepDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDacSweepDialog *DacSweepDialog;
//---------------------------------------------------------------------------
__fastcall TDacSweepDialog::TDacSweepDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDacSweepDialog::Button1Click(TObject *Sender)
{
//	ModalResult = mrOk;
// Range of the DAQboard DAC is from -10 to +10 V
// Corresponding to integer values of 0...65535
// Normally Npts:    0: 128 1: 256 2: 512 etc
// Normally Range:   0: 10 V  1: 5V  2: 2.5 V etc.

	int range = 65536;
	for (int i = 0; i< RangeComboBox->ItemIndex; i++) {
		range /= 2;
	}
	StartValue =  (65536-range)/2;
	if (ZeroCheckBox->Checked) {   StartValue = 32768;}

	int np = 128;
	for (int i = 0; i < NptsComboBox->ItemIndex; i++) {
		np *= 2;
	}

	while (np > range) np /= 2;
	StepSize = range/np;
	if (ZeroCheckBox->Checked) {   StepSize /=2;}

	NptsSweep = np+1;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TDacSweepDialog::Button2Click(TObject *Sender)
{
	ModalResult = -1;
	Close();
}
//---------------------------------------------------------------------------

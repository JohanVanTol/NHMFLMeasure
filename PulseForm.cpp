//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PulseForm.h"
#include <stdio.h>
#include "BNC565_GPIB.h"
#include "TekScope.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPulsedDialog *PulsedDialog;
extern BNC565 *PulseSource;
extern Tek7000 *Tek7;
//---------------------------------------------------------------------------
__fastcall TPulsedDialog::TPulsedDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPulsedDialog::SetButtonClick(TObject *Sender)
{
	int delay[8];
	int delaystep[8];
	int width[8];
	int widthstep[8];
	int IntegrationWidth;
	int AcqDelay;
	int Rate;
	int PulsAver;

	delay[0] = P1DelayEdit->Text.ToInt();
	delay[1] = P2DelayEdit->Text.ToInt();
	delay[2] = P3DelayEdit->Text.ToInt();
	delay[3] = P4DelayEdit->Text.ToInt();
	delay[4] = P5DelayEdit->Text.ToInt();
	delay[5] = P6DelayEdit->Text.ToInt();
	delay[6] = P7DelayEdit->Text.ToInt();
	delay[7] = P8DelayEdit->Text.ToInt();

	width[0] = P1WidthEdit->Text.ToInt();
	width[1] = P2WidthEdit->Text.ToInt();
	width[2] = P3WidthEdit->Text.ToInt();
	width[3] = P4WidthEdit->Text.ToInt();
	width[4] = P5WidthEdit->Text.ToInt();
	width[5] = P6WidthEdit->Text.ToInt();
	width[6] = P7WidthEdit->Text.ToInt();
	width[7] = P8WidthEdit->Text.ToInt();

	Rate = RateEdit->Text.ToInt();
	AcqDelay = AcqDelayEdit->Text.ToInt();
	IntegrationWidth = IntTimeEdit->Text.ToInt();
	PulsAver  = AverEdit->Text.ToInt();


	for (int i=1; i<=8; i++)
		PulseSource->Set(i, delay[i-1], width[i-1]);
//	PulseSource->SetRate(Rate);
	PulseSource->SetPeriod((double)Rate);

	if (QuadratureCheckBox->Checked ) {
			  Tek7->SetAverage(1);
			  Tek7->SetMathAverage(1, PulsAver);
			  Tek7->SetMathAverage(2, PulsAver);
			}
		  else
			Tek7->SetAverage(PulsAver);
	Tek7->SetDelay((double)AcqDelay * 1.0e-9);
	Tek7->Run();



}
//---------------------------------------------------------------------------






void __fastcall TPulsedDialog::OKBitButtonClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TPulsedDialog::CancelBitButtonClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------







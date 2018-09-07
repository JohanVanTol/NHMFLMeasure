//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "K6220Dialog.h"
#include "Keithley6220.h"

extern K6220 *CurrentSource;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TK6220Form *K6220Form;
//---------------------------------------------------------------------------
__fastcall TK6220Form::TK6220Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TK6220Form::FormActivate(TObject *Sender)
{

	if (CurrentSource->GetAddress() != 0)
	{
		CurrentEdit->Enabled = true;
		CompEdit->Enabled = true;
		CurrentLabel->Caption = 1.0e6 * CurrentSource->ReadCurrent();
		CompLabel->Caption = CurrentSource->ReadCompliance();
		CurrentEdit->Text = 1.0e6 * CurrentSource->GetDefaultCurr();
		CompEdit->Text = CurrentSource->GetDefaultComp();
	}
	  else
	  {
		CurrentEdit->Enabled = false;
		CompEdit->Enabled = false;
	  }
	return;

}
//---------------------------------------------------------------------------
void __fastcall TK6220Form::ApplyBitBtnClick(TObject *Sender)
{
	if (CurrentSource->GetAddress() == 0) return;

	double curr = CurrentEdit->Text.ToDouble();
	double comp = CompEdit->Text.ToDouble();
	if (curr>1000) curr = 1000.0; // 1mA max
	if (curr < -1000) curr = -1000.0;
	if (comp<0.1) comp = 0.1;
	if (comp>10) comp = 10;

	CurrentSource->SetCompliance(comp);
	CurrentSource->SetDefaultComp(comp);
	CurrentSource->SetDefaultCurr(curr*1.0e-6);
	CurrentSource->SetCurrent(curr*1.0e-6);
	return;
}
//---------------------------------------------------------------------------

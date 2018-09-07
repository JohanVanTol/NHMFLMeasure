//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "InstrumInitForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInstrumentInitForm *InstrumentInitForm;
//---------------------------------------------------------------------------
__fastcall TInstrumentInitForm::TInstrumentInitForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TInstrumentInitForm::WaitBeforeClosingTimerTimer(
      TObject *Sender)
{
    WaitBeforeClosingTimer->Enabled = false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TInstrumentInitForm::OKButtonClick(TObject *Sender)
{
	WaitBeforeClosingTimer->Enabled = false;
	Close();
}
//---------------------------------------------------------------------------

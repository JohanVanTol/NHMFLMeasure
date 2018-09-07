//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TimerFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTimerForm *TimerForm;
//---------------------------------------------------------------------------
__fastcall TTimerForm::TTimerForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTimerForm::TimerTimer(TObject *Sender)
{
    CountLabel->Caption = Count;
    if (Count < 0)
    {
        ModalResult = mbOK;
    }
    else Count--;
}
//---------------------------------------------------------------------------
void TTimerForm::SetCount(int n)
{
    Count = n;
}
void __fastcall TTimerForm::FormActivate(TObject *Sender)
{
    CountLabel->Caption = Count;
    Timer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TTimerForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Timer->Enabled = false;
}
//---------------------------------------------------------------------------

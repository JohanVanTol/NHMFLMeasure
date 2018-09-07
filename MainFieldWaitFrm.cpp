//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainFieldWaitFrm.h"
#include "IsoBusDevices.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainFieldWaitForm *MainFieldWaitForm;
extern IB_IPS120 *Magnet;
//---------------------------------------------------------------------------
__fastcall TMainFieldWaitForm::TMainFieldWaitForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainFieldWaitForm::MainCoilTimerTimer(TObject *Sender)
{
    double B = Magnet->GetField();
    FieldLabel->Caption = B;

    if (B == Target)
    {
        ModalResult = mbOK;
    }

}
//---------------------------------------------------------------------------
void __fastcall TMainFieldWaitForm::FormActivate(TObject *Sender)
{
    TargetLabel->Caption = Target;
    MainCoilTimer->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TMainFieldWaitForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    MainCoilTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainFieldWaitForm::AbortButtonClick(TObject *Sender)
{
    ModalResult = mbAbort;
}
//---------------------------------------------------------------------------

int TMainFieldWaitForm::SetTarget(double _Target)
{
    Target = _Target;
    return 0;
}
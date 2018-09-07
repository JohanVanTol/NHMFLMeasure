//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ZeroSweepCoilFrm.h"
#include "IsoBusDevices.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern IB_IPS20 *SweepCoil;
TZeroSweepCoilForm *ZeroSweepCoilForm;
//---------------------------------------------------------------------------
__fastcall TZeroSweepCoilForm::TZeroSweepCoilForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TZeroSweepCoilForm::CheckTimerTimer(TObject *Sender)
{
    double I = SweepCoil->GetCurrent();
    SweepLabel->Caption = I;

    if (I == Target)
    {
        ModalResult = mbOK;
    }
}
//---------------------------------------------------------------------------
void __fastcall TZeroSweepCoilForm::BitBtn1Click(TObject *Sender)
{
    ModalResult = mbAbort;
}
//---------------------------------------------------------------------------
void __fastcall TZeroSweepCoilForm::FormActivate(TObject *Sender)
{
    TargetLabel->Caption = Target;
    CheckTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TZeroSweepCoilForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    CheckTimer->Enabled = false;
}
//---------------------------------------------------------------------------

int TZeroSweepCoilForm::SetTarget(double _Target)
{
    Target = _Target;
    return 0;
}

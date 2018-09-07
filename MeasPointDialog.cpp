//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MeasPointDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMeasPointFrm *MeasPointFrm;
//---------------------------------------------------------------------------
__fastcall TMeasPointFrm::TMeasPointFrm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMeasPointFrm::FormActivate(TObject *Sender)
{
    XLabel->Caption = "0.0";
    PointLabel->Caption = "0";
}
//---------------------------------------------------------------------------


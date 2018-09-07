//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HeaterOnChkDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
THeaterOnCheckForm *HeaterOnCheckForm;
//---------------------------------------------------------------------------
__fastcall THeaterOnCheckForm::THeaterOnCheckForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall THeaterOnCheckForm::OKButtonClick(TObject *Sender)
{
    ModalResult = mbOK;
}
//---------------------------------------------------------------------------

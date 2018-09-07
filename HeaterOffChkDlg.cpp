//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HeaterOffChkDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
THeaterOffCheckForm *HeaterOffCheckForm;
//---------------------------------------------------------------------------
__fastcall THeaterOffCheckForm::THeaterOffCheckForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall THeaterOffCheckForm::OKButtonClick(TObject *Sender)
{
    ModalResult = mbOK;
}
//---------------------------------------------------------------------------

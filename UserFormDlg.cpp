//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UserFormDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TUserForm *UserForm;
//---------------------------------------------------------------------------
__fastcall TUserForm::TUserForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TUserForm::OKButtonClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

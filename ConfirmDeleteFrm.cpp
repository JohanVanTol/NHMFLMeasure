//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ConfirmDeleteFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDeleteConfirmDialog *DeleteConfirmDialog;
//---------------------------------------------------------------------------
__fastcall TDeleteConfirmDialog::TDeleteConfirmDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDeleteConfirmDialog::NoButtonClick(TObject *Sender)
{
    ModalResult = mbCancel;    
}
//---------------------------------------------------------------------------
void __fastcall TDeleteConfirmDialog::YesButtonClick(TObject *Sender)
{
    ModalResult = mbOK;    
}
//---------------------------------------------------------------------------



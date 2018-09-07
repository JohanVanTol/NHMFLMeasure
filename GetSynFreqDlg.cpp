//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GetSynFreqDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGetSynFreqDialog *GetSynFreqDialog;
//---------------------------------------------------------------------------
__fastcall TGetSynFreqDialog::TGetSynFreqDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TGetSynFreqDialog::CancelButtonClick(TObject *Sender)
{
    ModalResult = mbCancel;
}
//---------------------------------------------------------------------------
void __fastcall TGetSynFreqDialog::OkButtonClick(TObject *Sender)
{
//    ModalResult = mbOK;    
}
//---------------------------------------------------------------------------

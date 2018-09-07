//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LockInDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLockInDialog *LockInDialog;
//---------------------------------------------------------------------------
__fastcall TLockInDialog::TLockInDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLockInDialog::OkButtonClick(TObject *Sender)
{
    ModalResult = mbOK;    
}
//---------------------------------------------------------------------------

void __fastcall TLockInDialog::UpdateTimerTimer(TObject *Sender)
{
    XOutputLabel->Caption = LI->ReadOutput(1);
    YOutputLabel->Caption = LI->ReadOutput(2);
    ROutputLabel->Caption = LI->ReadOutput(3);
    ThetaOutputLabel->Caption = LI->ReadOutput(4);
    SensComboBox->ItemIndex = LI->MeasSensitivity();
    TCComboBox->ItemIndex = LI->MeasTimeConstant(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TLockInDialog::UpdateButtonClick(TObject *Sender)
{
    UpdateTimer->Enabled = !(UpdateTimer->Enabled);        
}
//---------------------------------------------------------------------------

void __fastcall TLockInDialog::FormDeactivate(TObject *Sender)
{
    UpdateTimer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TLockInDialog::SensComboBoxChange(TObject *Sender)
{
    LI->SetSensitivity(SensComboBox->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TLockInDialog::FormCreate(TObject *Sender)
{
    LI = NULL;    
}
//---------------------------------------------------------------------------

void __fastcall TLockInDialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    LI = NULL;    
}
//---------------------------------------------------------------------------


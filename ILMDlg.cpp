//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ILMDlg.h"
#include "IsoBusDevices.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TILMDialog *ILMDialog;
extern IB_ILM211 *ILM;
//---------------------------------------------------------------------------
__fastcall TILMDialog::TILMDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TILMDialog::CloseButtonClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TILMDialog::FormActivate(TObject *Sender)
{
//    ILM->InitPort();
    HeLabel->Caption = ILM->GetHe();
    N2Label->Caption = ILM->GetN2();
}
//---------------------------------------------------------------------------
void __fastcall TILMDialog::SampleButtonClick(TObject *Sender)
{
    HeLabel->Caption = ILM->GetHe();
    N2Label->Caption = ILM->GetN2();

}
//---------------------------------------------------------------------------
void __fastcall TILMDialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
//    ILM->FreePort();
}
//---------------------------------------------------------------------------

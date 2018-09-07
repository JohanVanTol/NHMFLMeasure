//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChannelSelectDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChannelSelectDialog *ChannelSelectDialog;
//---------------------------------------------------------------------------
__fastcall TChannelSelectDialog::TChannelSelectDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TChannelSelectDialog::CancelButtonClick(TObject *Sender)
{
    Channel = 0;    
}
//---------------------------------------------------------------------------
void __fastcall TChannelSelectDialog::OKButtonClick(TObject *Sender)
{
    if (CH1Button->Checked) Channel = 1;
    if (CH2Button->Checked) Channel = 2;
    if (CH3Button->Checked) Channel = 3;
    if (CH4Button->Checked) Channel = 4;
    if (Mem1Button->Checked) Channel = 5;
    if (Mem2Button->Checked) Channel = 6;
    if (Mem3Button->Checked) Channel = 7;
    if (Mem4Button->Checked) Channel = 8;
    if (Func1Button->Checked) Channel = 9;
    if (Func2Button->Checked) Channel = 10;
    if (Func3Button->Checked) Channel = 11;
    if (Func4Button->Checked) Channel = 12;
    if (Combine13Button->Checked) Channel = 13;
//    ModalResult = mbOK;
}
//---------------------------------------------------------------------------

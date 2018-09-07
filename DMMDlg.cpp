//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DMMDlg.h"
#include "IEEEdev.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDMM1Form *DMM1Form;
extern K196 *DMM1;
//---------------------------------------------------------------------------
__fastcall TDMM1Form::TDMM1Form(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDMM1Form::BitBtn1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TDMM1Form::FormClose(TObject *Sender, TCloseAction &Action)
{
    ReadTimer->Enabled = false;    
}
//---------------------------------------------------------------------------
void __fastcall TDMM1Form::FormActivate(TObject *Sender)
{
    ReadTimer->Enabled = true;    
}
//---------------------------------------------------------------------------
void __fastcall TDMM1Form::ReadTimerTimer(TObject *Sender)
{
    double V = DMM1->ReadVoltage();
    ReadLabel->Caption = V;
}
//---------------------------------------------------------------------------

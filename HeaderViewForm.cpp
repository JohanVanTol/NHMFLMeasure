//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HeaderViewForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TParamViewForm *ParamViewForm;
//---------------------------------------------------------------------------
__fastcall TParamViewForm::TParamViewForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TParamViewForm::BitBtn1Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

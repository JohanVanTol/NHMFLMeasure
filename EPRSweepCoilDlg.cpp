//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "EPRSweepCoilDlg.h"
#include "Valid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TEPRSweepCoilDialog *EPRSweepCoilDialog;
//---------------------------------------------------------------------------
__fastcall TEPRSweepCoilDialog::TEPRSweepCoilDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TEPRSweepCoilDialog::SetAllData()
{
    //  The values of StartField and Endfield will be in Tesla
    //  in the Dialog the values are expressed as mT !
    SetStartField((StartFieldEdit->Text.ToDouble())/1000.0);
    SetEndField((EndFieldEdit->Text.ToDouble())/1000.0);
    SetSweepSpeed(SweepSpeedEdit->Text.ToDouble());
    int AutoAv;
    if (AverageCheckBox->Checked)
    {
        AutoAv = CalcAutoNpts();
        AutoNptsLabel->Caption = AutoAv;
        if (AutoAverageRadioButton->Checked)
            AverageNpts = AutoAv;
          else
            if (ValidInt(ManNptsEdit->Text.c_str(), &AutoAv))
                AverageNpts = AutoAv;
    }
    return 0;
}

void __fastcall TEPRSweepCoilDialog::OKButtonClick(TObject *Sender)
{
    SetAllData();
//    ModalResult = mbOK;
}
//---------------------------------------------------------------------------


void __fastcall TEPRSweepCoilDialog::CancelButtonClick(TObject *Sender)
{
    ModalResult = mbCancel;    
}
//---------------------------------------------------------------------------

void __fastcall TEPRSweepCoilDialog::AverageCheckBoxClick(TObject *Sender)
{
    if (AverageCheckBox->Checked)
        AverEdit->Enabled = true;
      else
        AverEdit->Enabled = false;
    SetAllData();    
}
//---------------------------------------------------------------------------

int TEPRSweepCoilDialog::CalcAutoNpts()
{
    // make sure we have the right numbers
    SetStartField((StartFieldEdit->Text.ToDouble())/1000.0);
    SetEndField((EndFieldEdit->Text.ToDouble())/1000.0);
    SetSweepSpeed(SweepSpeedEdit->Text.ToDouble());
    double SweepDuration = 1000.0;
    int np = 1;
    if (SweepSpeed > 0)
       SweepDuration = 1000 * fabs(EndField-StartField)/SweepSpeed;
    if (FieldModeCheckBox->Checked)
        np = 10*(int)SweepDuration + 1;
      else
        np = 4*(int)SweepDuration + 1;
    while ( np > 16000 )
      np = ceil(np/2);

    return np;
}
//---------------------------------------------------------------------------

void __fastcall TEPRSweepCoilDialog::AutoAverageRadioButtonClick(
      TObject *Sender)
{
    SetAllData();
    if (AutoAverageRadioButton->Checked)
        ManNptsEdit->Enabled = false;
    else
        ManNptsEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TEPRSweepCoilDialog::ManualAverageRadioButtonClick(
      TObject *Sender)
{
    SetAllData();
    if (AutoAverageRadioButton->Checked)
        ManNptsEdit->Enabled = false;
    else
        ManNptsEdit->Enabled = true;
}
//---------------------------------------------------------------------------


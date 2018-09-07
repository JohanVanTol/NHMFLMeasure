//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HilbertDlg.h"
#include "ChildWin.h"
#include "Valid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
THilbertDialog *HilbertDialog;
extern TDataFile *AnalysisFile;
//---------------------------------------------------------------------------
__fastcall THilbertDialog::THilbertDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall THilbertDialog::CloseButtonClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall THilbertDialog::GoButtonClick(TObject *Sender)
{
    if ((ResultComboBox->ItemIndex < 0) ||
            (ImageComboBox->ItemIndex < 0) ||
                (DataComboBox->ItemIndex < 0)) return;
    double angle = AngleEdit->Text.ToDouble();
    double Integral;
//    AnalysisFile->GetData()->SubstractLine(DataComboBox->ItemIndex);
    Integral = AnalysisFile->GetData()->HilbertTransForm(DataComboBox->ItemIndex,
        ImageComboBox->ItemIndex,ResultComboBox->ItemIndex,angle);
    IntegralLabel->Caption = Integral;
    AnalysisFile->PlotData();
    AnalysisFile->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall THilbertDialog::AddColumnsButtonClick(TObject *Sender)
{
    AnalysisFile->GetData()->AddColumn();
    ColLabel->Caption = AnalysisFile->GetData()->GetNy();
    ResetComboBoxes(AnalysisFile->GetData()->GetNy());
}
//---------------------------------------------------------------------------
void __fastcall THilbertDialog::FormActivate(TObject *Sender)
{
    ColLabel->Caption = AnalysisFile->GetData()->GetNy();
    ResetComboBoxes(AnalysisFile->GetData()->GetNy());
}
//---------------------------------------------------------------------------

void __fastcall THilbertDialog::AngleEditChange(TObject *Sender)
{
    double Angle, Integral;
    if (ValidReal(AngleEdit->Text,&Angle))
    {
        Integral = AnalysisFile->GetData()->ChangeHilbertAngle(DataComboBox->ItemIndex,
        ImageComboBox->ItemIndex,ResultComboBox->ItemIndex,Angle);
        IntegralLabel->Caption = Integral;
        AnalysisFile->Invalidate();
    }
}
//---------------------------------------------------------------------------

int THilbertDialog::ResetComboBoxes(int ncol)
{
    AnsiString S;
    int idata,iimage,iresult;
    idata=DataComboBox->ItemIndex;
    iimage=ImageComboBox->ItemIndex;
    iresult=ResultComboBox->ItemIndex;
    DataComboBox->Items->Clear();
    ImageComboBox->Items->Clear();
    ResultComboBox->Items->Clear();
//    DataComboBox->Items->Clear();
    for (int i=0;i<ncol;i++)
    {
        S = i+1;
        DataComboBox->Items->Add(S);
        ImageComboBox->Items->Add(S);
        ResultComboBox->Items->Add(S);
    }
    if ((idata >=0) && (idata<ncol))
          DataComboBox->ItemIndex = idata;
        else DataComboBox->ItemIndex = 0;

    if ((iimage >=0) && (iimage<ncol) )
          ImageComboBox->ItemIndex = iimage;
        else ImageComboBox->ItemIndex = ncol-1;
    if (ImageComboBox->ItemIndex == DataComboBox->ItemIndex)
       ImageComboBox->ItemIndex = -1;

    if ((iresult >=0) && (iresult<ncol) )
          ResultComboBox->ItemIndex = iresult;
        else ResultComboBox->ItemIndex = ncol-2;
    if ((ResultComboBox->ItemIndex == DataComboBox->ItemIndex) ||
            (ResultComboBox->ItemIndex == ImageComboBox->ItemIndex))
        ResultComboBox->ItemIndex = -1;
}



void __fastcall THilbertDialog::DataComboBoxChange(TObject *Sender)
{
    ResetComboBoxes(AnalysisFile->GetData()->GetNy());
}
//---------------------------------------------------------------------------

void __fastcall THilbertDialog::ImageComboBoxChange(TObject *Sender)
{
    ResetComboBoxes(AnalysisFile->GetData()->GetNy());
}
//---------------------------------------------------------------------------

void __fastcall THilbertDialog::ResultComboBoxChange(TObject *Sender)
{
    ResetComboBoxes(AnalysisFile->GetData()->GetNy());
}
//---------------------------------------------------------------------------


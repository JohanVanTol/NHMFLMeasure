//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "ChildWin.h"
#include "TransientCorrDialog.h"
#include "Valid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTransientCorrDlg *TransientCorrDlg;
//---------------------------------------------------------------------------
__fastcall TTransientCorrDlg::TTransientCorrDlg(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTransientCorrDlg::FormShow(TObject *Sender)
{
    *OldData = *(TempFile->GetData());
    NrColumnsLabel->Caption = OldData->GetNy();
}
//---------------------------------------------------------------------------

void __fastcall TTransientCorrDlg::FormCreate(TObject *Sender)
{
    OldData = new DataArray(10);
}
//---------------------------------------------------------------------------

void __fastcall TTransientCorrDlg::FormDestroy(TObject *Sender)
{
    delete OldData;
    TempFile = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TTransientCorrDlg::ApplyButtonClick(TObject *Sender)
{
    int ncol = OldData->GetNy();

    double angle, co, si, averag;
    double NewX, NewY;
    if (ValidReal(PhaseEdit->Text,&angle))
    {
        co = cos(angle*acos(0)/90.0);
        si = sin(angle*acos(0)/90.0);
    } else return;

    int corrcol;
    if (!(ValidInt(ColumnEdit->Text, &corrcol))) return;

    DataPoint POld = OldData->Get(0);
    DataPoint PNew = OldData->Get(0);

// first let's do the substraction
// We substract the average of the prepulse columns
// from each column

    if (corrcol < ncol/2)
    for (int i=0; i<OldData->Getn(); i++)
    {
        if (!OldTypeCheckBox->Checked)
        {
            POld = OldData->Get(i);
            PNew = OldData->Get(i);
            if (corrcol >0)
            {
                if (SkipFirstCheckBox->Checked)
                {
                    averag = 0.0;
                    for (int i=2; i<= corrcol+1; i++)
                        averag += POld.Get(i);
                    averag /= corrcol;
                    for (int i=2; i<=(ncol/2)+1; i++)
                        PNew.Set(i,POld.Get(i)-averag);

                    averag = 0.0;
                    for (int i=(ncol/2)+2; i<= (ncol/2)+corrcol+1; i++)
                        averag += POld.Get(i);
                    averag /= corrcol;
                    for (int i=(ncol/2)+2; i<=ncol+1; i++)
                        PNew.Set(i,POld.Get(i)-averag);
                }
                else
                {
                    averag = 0.0;
                    for (int i=1; i<= corrcol; i++)
                        averag += POld.Get(i);
                    averag /= corrcol;
                    for (int i=1; i<=(ncol/2); i++)
                        PNew.Set(i,POld.Get(i)-averag);

                    averag = 0.0;
                    for (int i=(ncol/2)+1; i<= (ncol/2)+corrcol; i++)
                        averag += POld.Get(i);
                    averag /= corrcol;
                    for (int i=(ncol/2)+1; i<=ncol; i++)
                        PNew.Set(i,POld.Get(i)-averag);
                }
            }

            // now let's do the phase correction as well
            for (int i=1; i<=ncol/2; i++)
            {
                NewX = co*PNew.Get(i)+si*PNew.Get((ncol/2)+i);
                NewY = -si*PNew.Get(i)+co*PNew.Get((ncol/2)+i);
                PNew.Set(i, NewX);
                PNew.Set(i+(ncol/2), NewY);
            }
        }
          else
          {
            POld = OldData->Get(i);
            PNew = OldData->Get(i);
            if (corrcol >0)
            {
                averag = 0.0;
                if (EndSubstractCheckBox->Checked)
                    for (int i=ncol-1; i> ncol - 2*corrcol; i-=2)
                            averag += POld.Get(i);
                  else
                  {
                      for (int i=2; i<=2*corrcol+1; i+=2)  averag += POld.Get(i);
                  }
                averag /= corrcol;
                for (int i=1; i<= ncol; i+=2)
                        PNew.Set(i,POld.Get(i)-averag);

                averag = 0.0;
                if (EndSubstractCheckBox->Checked)
                    for (int i=ncol; i> ncol - 2*corrcol; i-=2)
                            averag += POld.Get(i);
                  else
                    for (int i=2; i<=2*corrcol; i+=2)
                            averag += POld.Get(i);
                averag /= corrcol;
                for (int i=2; i<= ncol; i+=2)
                        PNew.Set(i,POld.Get(i)-averag);
            }
            // now let's do the phase correction as well
            double factor;
            if (ErrorCheckBox->Checked) factor=0.2;
               else factor=1.0;
            for (int i=1; i<ncol; i+=2)
            {
                NewX = co*factor*PNew.Get(i)+si*PNew.Get(1+i);
                NewY = -si*factor*PNew.Get(i)+co*PNew.Get(1+i);
                PNew.Set(i, NewX);
                PNew.Set(i+1, NewY);
            }
          }

        TempFile->GetData()->Set(i, PNew);
    }
    TempFile->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TTransientCorrDlg::BitBtn2Click(TObject *Sender)
{
    TempFile->SetData(*OldData);
    TempFile = NULL;
    Close();

}
//---------------------------------------------------------------------------


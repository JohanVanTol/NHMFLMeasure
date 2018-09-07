//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChildWin.h"
#include "PlotCurvesDialog.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPlotCurvesDlg *PlotCurvesDlg;
//---------------------------------------------------------------------------
__fastcall TPlotCurvesDlg::TPlotCurvesDlg(TComponent* Owner)
    : TForm(Owner)
{
    YL = new TCheckBox*[20];
    YR = new TCheckBox*[20];
    XB = new TRadioButton*[20];
    YL[0] = YL1;
    YL[1] = YL2;
    YL[2] = YL3;
    YL[3] = YL4;
    YL[4] = YL5;
    YL[5] = YL6;
    YL[6] = YL7;
    YL[7] = YL8;
    YL[8] = YL9;
    YL[9] = YL10;
    YL[10] = YL11;
    YL[11] = YL12;
    YL[12] = YL13;
    YL[13] = YL14;
    YL[14] = YL15;
    YL[15] = YL16;
    YL[16] = YL17;
    YL[17] = YL18;
    YL[18] = YL19;
    YL[19] = YL20;
    YR[0] = YR1;
    YR[1] = YR2;
    YR[2] = YR3;
    YR[3] = YR4;
    YR[4] = YR5;
    YR[5] = YR6;
    YR[6] = YR7;
    YR[7] = YR8;
    YR[8] = YR9;
    YR[9] = YR10;
    YR[10] = YR11;
    YR[11] = YR12;
    YR[12] = YR13;
    YR[13] = YR14;
    YR[14] = YR15;
    YR[15] = YR16;
    YR[16] = YR17;
    YR[17] = YR18;
    YR[18] = YR19;
    YR[19] = YR20;
    XB[0] = XB1;
    XB[1] = XB2;
    XB[2] = XB3;
    XB[3] = XB4;
    XB[4] = XB5;
    XB[5] = XB6;
    XB[6] = XB7;
    XB[7] = XB8;
    XB[8] = XB9;
    XB[9] = XB10;
    XB[10] = XB11;
    XB[11] = XB12;
    XB[12] = XB13;
    XB[13] = XB14;
    XB[14] = XB15;
    XB[15] = XB16;
    XB[16] = XB17;
    XB[17] = XB18;
    XB[18] = XB19;
    XB[19] = XB20;

}
//---------------------------------------------------------------------------
void __fastcall TPlotCurvesDlg::OKButtonClick(TObject *Sender)
{
    SetPlotCurves();
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TPlotCurvesDlg::CancelButtonClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

int TPlotCurvesDlg::EnableButtons()
{
    int nCol = TempFile->GetData()->GetNy() + 1;
    if (nCol > 20) nCol = 20;
    for (int i=0; i< nCol; i++)
    {
        YL[i]->Enabled = true;
        YR[i]->Enabled = true;
        XB[i]->Enabled = true;
    }
    for (int i = nCol; i<20; i++)
    {
        YL[i]->Enabled = false;
        YR[i]->Enabled = false;
		XB[i]->Enabled = false;
    }
}

int TPlotCurvesDlg::SetPlotCurves()
{
    for (int j=0; j<4 ;j++)                           // Attention all 4 plots
    {
        TempFile->GetPlot(j)->ResetTraces();
        for (int i=0; i<20; i++)
        {
            if ((XB[i]->Enabled) && (XB[i]->Checked))
                     TempFile->GetPlot(j)->AddTrace(0,i);
            if ((YL[i]->Enabled) && (YL[i]->Checked))
                    TempFile->GetPlot(j)->AddTrace(1,i);
            if ((YR[i]->Enabled) && (YR[i]->Checked))
                    TempFile->GetPlot(j)->AddTrace(2,i);
        }
    }
}

int TPlotCurvesDlg::GetPlotCurves()
{
    int col;
    int nT = TempFile->GetPlot()->GetnTraces(0);

    col = TempFile->GetPlot()->GetDataColumn(0,0);
    if ( col == -1 ) XBPoint->Checked = true;
     else XB[col]->Checked = true;

    for (int i=0; i<20; i++)
    {
        YL[i]->Checked = false;
        YR[i]->Checked = false;
    }

    nT = TempFile->GetPlot()->GetnTraces(1);
    for (int i=0;i<nT;i++)
    {
        col = TempFile->GetPlot()->GetDataColumn(1,i);
        if ((col <20) && (col>=0)) YL[col]->Checked = true;
    }

    nT = TempFile->GetPlot()->GetnTraces(2);
    for (int i=0;i<nT;i++)
    {
        col = TempFile->GetPlot()->GetDataColumn(2,i);
        if ((col <20) && (col>=0)) YR[col]->Checked = true;
    }
    return 0;
}

void __fastcall TPlotCurvesDlg::FormActivate(TObject *Sender)
{
    EnableButtons();
    GetPlotCurves();
}
//---------------------------------------------------------------------------
int TPlotCurvesDlg::SetPlotCurves(TDataFile* File)
{
    SetDataFile(File);
    return SetPlotCurves();
}


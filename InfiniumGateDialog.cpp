//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "InfiniumGateDialog.h"
#include "Valid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInfiniumGatesDlg *InfiniumGatesDlg;
//---------------------------------------------------------------------------
__fastcall TInfiniumGatesDlg::TInfiniumGatesDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

int TInfiniumGatesDlg::SetGate(int i, double Start, double Stop)
{
    if ((i<1) || (i>nGates)) return -1;
    switch (i)
    {
        case 1: Gate1T1Edit->Text = Start;
                Gate1T2Edit->Text = Stop;
                break;
        case 2: Gate2T1Edit->Text = Start;
                Gate2T2Edit->Text = Stop;
                break;
        case 3: Gate3T1Edit->Text = Start;
                Gate3T2Edit->Text = Stop;
                break;
        case 4: Gate4T1Edit->Text = Start;
                Gate4T2Edit->Text = Stop;
                break;
        case 5: Gate5T1Edit->Text = Start;
                Gate5T2Edit->Text = Stop;
                break;
        case 6: Gate6T1Edit->Text = Start;
                Gate6T2Edit->Text = Stop;
                break;
        case 7: Gate7T1Edit->Text = Start;
                Gate7T2Edit->Text = Stop;
                break;
        case 8: Gate8T1Edit->Text = Start;
                Gate8T2Edit->Text = Stop;
                break;
        case 9: Gate9T1Edit->Text = Start;
                Gate9T2Edit->Text = Stop;
                break;
        default: break;  //do nothing
    }
    return 0;
}

//---------------------------------------------------------------------------

int TInfiniumGatesDlg::GetGate(int i, double *Start, double *Stop)
{
    if ((i<1) || (i>nGates)) return -1;
    double *Temp;
    Temp = new double;

    if (RegularCheckBox->Checked)
    {
        *Start = GateStart + (i-1) * GateWidth;
        *Stop  = GateStart + i * GateWidth;
        return 0;
    }

    switch (i)
    {
        case 1: if (ValidReal(Gate1T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate1T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
        case 2: if (ValidReal(Gate2T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate2T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
        case 3: if (ValidReal(Gate3T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate3T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
        case 4: if (ValidReal(Gate4T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate4T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
        case 5: if (ValidReal(Gate5T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate5T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
        case 6: if (ValidReal(Gate6T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate6T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
        case 7: if (ValidReal(Gate7T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate7T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
        case 8: if (ValidReal(Gate8T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate8T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
        case 9: if (ValidReal(Gate9T1Edit->Text, Temp))
                    *Start = (*Temp) * 1.0e-6;
                if (ValidReal(Gate9T2Edit->Text, Temp))
                    *Stop = (*Temp) * 1.0e-6;
                break;
    }
    delete Temp;
    return 0;
}



void __fastcall TInfiniumGatesDlg::NGatesEditChange(TObject *Sender)
{
    int tempint=1;
    if (ValidInt(NGatesEdit->Text, &tempint))
    {
        if (tempint < 1) tempint = 1;
        if (tempint > 100) tempint = 100;
    }
    nGates = tempint;
    ReEnableGates();
}
//---------------------------------------------------------------------------

int TInfiniumGatesDlg::ReEnableGates()
{
    if ((nGates > 9) || (RegularCheckBox->Checked))
    {
        GatesGroupBox->Enabled = false;
        StartEdit->Enabled = true;
        DurationEdit->Enabled = true;
        return nGates;
    }
    else
    {
        GatesGroupBox->Enabled = true;
        StartEdit->Enabled = false;
        DurationEdit->Enabled = false;

    if (nGates > 0)
    {
        Gate1T1Edit->Enabled = true;
        Gate1T2Edit->Enabled = true;
    }
      else
      {
        Gate1T1Edit->Enabled = false;
        Gate1T2Edit->Enabled = false;
      }
    if (nGates > 1)
    {
        Gate2T1Edit->Enabled = true;
        Gate2T2Edit->Enabled = true;
    }
      else
      {
        Gate2T1Edit->Enabled = false;
        Gate2T2Edit->Enabled = false;
      }
    if (nGates > 2)
    {
        Gate3T1Edit->Enabled = true;
        Gate3T2Edit->Enabled = true;
    }
      else
      {
        Gate3T1Edit->Enabled = false;
        Gate3T2Edit->Enabled = false;
      }
    if (nGates > 3)
    {
        Gate4T1Edit->Enabled = true;
        Gate4T2Edit->Enabled = true;
    }
      else
      {
        Gate4T1Edit->Enabled = false;
        Gate4T2Edit->Enabled = false;
      }
    if (nGates > 4)
    {
        Gate5T1Edit->Enabled = true;
        Gate5T2Edit->Enabled = true;
    }
      else
      {
        Gate5T1Edit->Enabled = false;
        Gate5T2Edit->Enabled = false;
      }
    if (nGates > 5)
    {
        Gate6T1Edit->Enabled = true;
        Gate6T2Edit->Enabled = true;
    }
      else
      {
        Gate6T1Edit->Enabled = false;
        Gate6T2Edit->Enabled = false;
      }
    if (nGates > 6)
    {
        Gate7T1Edit->Enabled = true;
        Gate7T2Edit->Enabled = true;
    }
      else
      {
        Gate7T1Edit->Enabled = false;
        Gate7T2Edit->Enabled = false;
      }
    if (nGates > 7)
    {
        Gate8T1Edit->Enabled = true;
        Gate8T2Edit->Enabled = true;
    }
      else
      {
        Gate8T1Edit->Enabled = false;
        Gate8T2Edit->Enabled = false;
      }
    if (nGates > 8)
    {
        Gate9T1Edit->Enabled = true;
        Gate9T2Edit->Enabled = true;
    }
      else
      {
        Gate9T1Edit->Enabled = false;
        Gate9T2Edit->Enabled = false;
      }
    return nGates;
    }
}

void __fastcall TInfiniumGatesDlg::FormActivate(TObject *Sender)
{
    AverageEdit->Text = Averages;
    NGatesEdit->Text = nGates;
    ReEnableGates();
}
//---------------------------------------------------------------------------

void __fastcall TInfiniumGatesDlg::AverageEditChange(TObject *Sender)
{
    int tempint;
    if (ValidInt(AverageEdit->Text, &tempint))
    {
        if (tempint < 1) tempint = 1;
        if (tempint >4096) tempint = 4096;
        Averages = tempint;
    }
}
//---------------------------------------------------------------------------

void __fastcall TInfiniumGatesDlg::OkBtnClick(TObject *Sender)
{
//    ModalResult = mbOK;    
}
//---------------------------------------------------------------------------

void __fastcall TInfiniumGatesDlg::BitBtn1Click(TObject *Sender)
{
    ModalResult = mbCancel;
}
//---------------------------------------------------------------------------

void __fastcall TInfiniumGatesDlg::RegularCheckBoxClick(TObject *Sender)
{
    ReEnableGates();    
}
//---------------------------------------------------------------------------




void __fastcall TInfiniumGatesDlg::DurationEditExit(TObject *Sender)
{
   double Temp;
    if (ValidReal(DurationEdit->Text, &Temp))
                    GateWidth = Temp * 1.0e-6;
   
}
//---------------------------------------------------------------------------

void __fastcall TInfiniumGatesDlg::StartEditExit(TObject *Sender)
{
    double Temp;
    if (ValidReal(StartEdit->Text, &Temp))
                    GateStart = Temp * 1.0e-6;

}
//---------------------------------------------------------------------------


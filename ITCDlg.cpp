//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ITCDlg.h"
#include "wsc.h"
#include "IsoBusDevices.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TITCDialog *ITCDialog;
extern IB_ITC503 *ITC;
//---------------------------------------------------------------------------
__fastcall TITCDialog::TITCDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TITCDialog::SendButtonClick(TObject *Sender)
{
    int test = SetAllData();
}
//---------------------------------------------------------------------------
void __fastcall TITCDialog::CancelButtonClick(TObject *Sender)
{
    TempTimer->Enabled = false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TITCDialog::OKButtonClick(TObject *Sender)
{
    SetAllData();
    TempTimer->Enabled = false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TITCDialog::ReadButtonClick(TObject *Sender)
{
    int test = GetAllData();
    TempTimer->Enabled = false;
}
//---------------------------------------------------------------------------

int TITCDialog::GetAllData()
{
    double value;
    char StatusString[14];
//    int test;// = ITC->InitPort();
//    if (test < 0) return -1;
    value = ITC->GetTemperature(0);
    SetTempEdit->Text = value;
    value = ITC->GetTemperature(1);
    Channel1Label->Caption = value;
    value = ITC->GetTemperature(2);
    Channel2Label->Caption = value;
    value = ITC->GetTemperature(3);
    Channel3Label->Caption = value;
    value = ITC->GetPID(0);
    PEdit->Text = value;
    IEdit->Text = ITC->GetPID(1);
    DEdit->Text = ITC->GetPID(2);
    HeaterEdit->Text = ITC->GetHeater(0); //in %
    HeaterLabel->Caption = ITC->GetHeater(1); //in Volt
    FlowEdit->Text = ITC->GetValve();
    int len = ITC->GetStatus(StatusString);
    if (len < 12) return -1;
    if (ITC->AutoHeater(StatusString))
    {
        AutoHeaterButton->Checked = true;
        HeaterEdit->Enabled = false;
    }
        else
        {
            ManualHeaterButton->Checked = true;
            HeaterEdit->Enabled = true;
        }
    if (ITC->AutoFlow(StatusString))
    {
        AutoFlowButton->Checked = true;
        FlowEdit->Enabled = false;
    }
      else
        {
            ManualFlowButton->Checked = true;
            FlowEdit->Enabled = true;
        }
    if (ITC->AutoPID(StatusString))
    {
        AutoPIDButton->Checked = true;
        PEdit->Enabled = false;
        IEdit->Enabled = false;
        DEdit->Enabled = false;
    }
      else
      {
        ManualPIDButton->Checked = true;
        PEdit->Enabled = true;
        IEdit->Enabled = true;
        DEdit->Enabled = true;
      }
    ChannelComboBox->ItemIndex = ITC->ControlChannel(StatusString)-1;
//   int mode = ITC->GetAutoMode();
//    test = ITC->FreePort();

    return 0;
}

int TITCDialog::SetAllData()
{
    char StatStr[14];
    int error;
//    int test = ITC->InitPort();
//    if (test < 0) return -1;
    if (ITC->SetTemperature(SetTempEdit->Text.ToDouble()) < 0) return -1;
    int len = ITC->GetStatus(StatStr);
    if (len < 12) return -1;

    if (ManualPIDButton->Checked)
    {
        error = ITC->SetAutoPID(0);
        error = ITC->SetP(PEdit->Text.ToDouble());
        error = ITC->SetI(IEdit->Text.ToDouble());
        error = ITC->SetD(DEdit->Text.ToDouble());
    }
        else error = ITC->SetAutoPID(1);

    int automat = 0;
    if (ManualHeaterButton->Checked)
    {
        ITC->SetControlChannel(ChannelComboBox->ItemIndex + 1);
       error = ITC->SetHeater(HeaterEdit->Text.ToDouble());
    }
       else
         automat += 1;

    if (ManualFlowButton->Checked)
        error = ITC->SetFlow(FlowEdit->Text.ToDouble());
            else automat += 2;
    error = ITC->SetAuto(automat);
//    test = ITC->FreePort();
}


void __fastcall TITCDialog::ManualHeaterButtonClick(TObject *Sender)
{
        HeaterEdit->Enabled= true;        
}
//---------------------------------------------------------------------------

void __fastcall TITCDialog::AutoHeaterButtonClick(TObject *Sender)
{
        HeaterEdit->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TITCDialog::AutoFlowButtonClick(TObject *Sender)
{
        FlowEdit->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TITCDialog::ManualFlowButtonClick(TObject *Sender)
{
        FlowEdit->Enabled = true;        
}
//---------------------------------------------------------------------------

void __fastcall TITCDialog::AutoPIDButtonClick(TObject *Sender)
{
        PEdit->Enabled = false;
        IEdit->Enabled = false;
        DEdit->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TITCDialog::ManualPIDButtonClick(TObject *Sender)
{
        PEdit->Enabled = true;
        IEdit->Enabled = true;
        DEdit->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TITCDialog::FormActivate(TObject *Sender)
{
//    ITC->InitPort();
    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TITCDialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
//    ITC->FreePort();
	TempTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TITCDialog::TempTimerTimer(TObject *Sender)
{
	bool IsTimerOn = TempTimer->Enabled;
	TempTimer->Enabled = false;
	double value;
    value = ITC->GetTemperature(1);
    Channel1Label->Caption = value;
    value = ITC->GetTemperature(2);
    Channel2Label->Caption = value;
	value = ITC->GetTemperature(3);
    Channel3Label->Caption = value;
    HeaterEdit->Text = ITC->GetHeater(0); //in %
    HeaterLabel->Caption = ITC->GetHeater(1); //in Volt
	if (IsTimerOn) TempTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TITCDialog::ContinUpdateButtonClick(TObject *Sender)
{
    TempTimer->Enabled = true;    
}
//---------------------------------------------------------------------------



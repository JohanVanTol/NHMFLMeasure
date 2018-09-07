//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RFAmplDlg.h"
#include "IEEEdev.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRFAmplifierDialog *RFAmplifierDialog;
extern AR100W1000A *RFAmplif;
//---------------------------------------------------------------------------
__fastcall TRFAmplifierDialog::TRFAmplifierDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::PowerButtonClick(TObject *Sender)
{
    int test=0;
    if (RFAmplif->GetPowerStatus() == 0)
    {
        RFAmplif->SetPowerStatus(1);  //Set on always in standby
        do test = RFAmplif->IsOperationComplete();
          while (test==0);
        for (int i=0;i<1000000;i++) {i=i+1;i=i-1;}
        GetAllData();
    }
       else
       {
        RFAmplif->SetPowerStatus(0);
        do test = RFAmplif->IsOperationComplete();
          while (test==0);
        for (int i=0;i<1000000;i++) {i=i+1;i=i-1;}
         GetAllData();
       }
}
//---------------------------------------------------------------------------


int TRFAmplifierDialog::GetAllData()
{

    int stat = RFAmplif->GetPowerStatus();
    PowerSpeedButton->Enabled = false;
    StandBySpeedButton->Enabled = false;
    OperateSpeedButton->Enabled = false;
    FaultSpeedButton->Enabled = false;
    StandByButton->Enabled = true;
    OperateButton->Enabled = true;
    ResetButton->Enabled = true;
    switch (stat)
    {
        case 0: StandByButton->Enabled = false;
                OperateButton->Enabled = false;
                ResetButton->Enabled = false;
                break;
        case 1: PowerSpeedButton->Enabled = true;
                StandBySpeedButton->Enabled = true;break;
        case 2: PowerSpeedButton->Enabled = true;
                OperateSpeedButton->Enabled = true;break;
        case 3: PowerSpeedButton->Enabled = true;
                FaultSpeedButton->Enabled = true;break;
        default : break;
    }
    double Gain = RFAmplif->GetGain();
    GainEdit->Text = FloatToStrF(Gain, ffFixed, 5,1);
    double FPower = RFAmplif->GetFPower();
    ForwardPowerLabel->Caption = FloatToStrF(FPower, ffFixed, 5,1);
    double RPower = RFAmplif->GetRPower();
    ReflectedPowerLabel->Caption = FloatToStrF(RPower, ffFixed, 5,1);
    int status = RFAmplif->GetMode();
    if (status > 2)
    {
        ALCcommandsEnable(1);
        if (status == 4) ALCintRadioButton->Checked = true;
          else ALCextRadioButton->Checked = true;
        double ThreeReals[3];
        int Resp = RFAmplif->GetALCParameters(ThreeReals);
        ThresholdEdit->Text = FloatToStrF(ThreeReals[2], ffFixed, 5,1);
        DetGainEdit->Text = FloatToStrF(ThreeReals[1], ffFixed, 5,1);
        GainEdit->Text = FloatToStrF(ThreeReals[0], ffFixed, 5,1);
        ResponseEdit->Text = Resp;
    }
       else
       {
        ALCoffRadioButton->Checked = true;
        ALCcommandsEnable(0);
       }

    if (stat == -1) return -1;
      else return 0;
}

void __fastcall TRFAmplifierDialog::FormActivate(TObject *Sender)
{
    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::StandByButtonClick(TObject *Sender)
{
    RFAmplif->SetStandBy();
    for (int i=0;i<1000000;i++) {i=i+1;i=i-1;}
    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::OperateButtonClick(TObject *Sender)
{
    RFAmplif->SetOperate();
    for (int i=0;i<1000000;i++) {i=i+1;i=i-1;}
    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::ResetButtonClick(TObject *Sender)
{
    RFAmplif->SetPowerStatus(3);
    for (int i=0;i<1000000;i++) {i=i+1;i=i-1;}
    GetAllData();
}
//---------------------------------------------------------------------------


void __fastcall TRFAmplifierDialog::GainEditExit(TObject *Sender)
{
    // Set the gain for the amplifier
    int c;
    for (int i=1; i<= (GainEdit->Text.Length()); i++)
    {
        c = GainEdit->Text[i];
        if (((c<48) || (c>57)) && (c != '.'))
        {
            GainEdit->Text = "0.0";
            return;
        }
    }
    double GainSet = GainEdit->Text.ToDouble();
    if (GainSet < 0.0) GainSet = 0.0;
    if (GainSet > 100.0) GainSet = 100.0;
    RFAmplif->SetGain(GainSet);
}
//---------------------------------------------------------------------------

void TRFAmplifierDialog::ALCcommandsEnable(int mode)
{
    switch (mode)
    {
        case 0 : DetGainLabel->Enabled = false;
                 ThresholdLabel->Enabled = false;
                 ResponseLabel->Enabled = false;
                 PercentLabel1->Enabled = false;
                 PercentLabel2->Enabled = false;
                 DetGainEdit->Enabled = false;
                 ThresholdEdit->Enabled = false;
                 ResponseEdit->Enabled = false;
                 break;
        case 1 : DetGainLabel->Enabled = true;
                 ThresholdLabel->Enabled = true;
                 ResponseLabel->Enabled = true;
                 PercentLabel1->Enabled =true;
                 PercentLabel2->Enabled =true;
                 DetGainEdit->Enabled = true;
                 ThresholdEdit->Enabled = true;
                 ResponseEdit->Enabled = true;
                 break;
        }
    return;
}

void __fastcall TRFAmplifierDialog::UpdateButtonClick(TObject *Sender)
{
    Timer1->Enabled = false;
    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::ALCoffRadioButtonClick(TObject *Sender)
{
    RFAmplif->SetALCMode(0);
    ALCcommandsEnable(0);
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::ALCintRadioButtonClick(TObject *Sender)
{
    RFAmplif->SetALCMode(1);
    ALCcommandsEnable(1);
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::ALCextRadioButtonClick(TObject *Sender)
{
    RFAmplif->SetALCMode(2);
    ALCcommandsEnable(1);
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::ThresholdEditExit(TObject *Sender)
{
    // Set the threshold for the amplifier
    int c;
    for (int i=1; i<= (ThresholdEdit->Text.Length()); i++)
    {
        c = ThresholdEdit->Text[i];
        if (((c<48) || (c>57)) && (c != '.'))
        {
            ThresholdEdit->Text = "0.0";
            return;
        }
    }
    double Set = ThresholdEdit->Text.ToDouble();
    if (Set < 0.0) Set = 0.0;
    if (Set > 100.0) Set = 100.0;
    RFAmplif->SetThreshold(Set);

}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::DetGainEditExit(TObject *Sender)
{
    // Set the detectorgain for the amplifier
    int c;
    for (int i=1; i<= (DetGainEdit->Text.Length()); i++)
    {
        c = DetGainEdit->Text[i];
        if (((c<48) || (c>57)) && (c != '.'))
        {
            DetGainEdit->Text = "0.0";
            return;
        }
    }
    double Set = DetGainEdit->Text.ToDouble();
    if (Set < 0.0) Set = 0.0;
    if (Set > 100.0) Set = 100.0;
    RFAmplif->SetDetectorGain(Set);
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::ResponseEditExit(TObject *Sender)
{
    // Set the response for the amplifier
    int c;
    for (int i=1; i<= (ResponseEdit->Text.Length()); i++)
    {
        c = ResponseEdit->Text[i];
        if ((c<48) || (c>57))
        {
            ResponseEdit->Text = "0";
            return;
        }
    }
    double Set = ResponseEdit->Text.ToInt();
    if (Set < 0) Set = 0;
    if (Set > 6) Set = 6;
    RFAmplif->SetResponse(Set);
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::ContUpdateButtonClick(TObject *Sender)
{
    Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::Timer1Timer(TObject *Sender)
{
    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::BitBtn2Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Timer1->Enabled = false;    
}
//---------------------------------------------------------------------------

void __fastcall TRFAmplifierDialog::BitBtn1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------


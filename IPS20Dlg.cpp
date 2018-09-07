//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "IPS20Dlg.h"
#include "IsoBusDevices.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIPS20Dialog *IPS20Dialog;
extern IB_IPS20 *SweepCoil;
//---------------------------------------------------------------------------
__fastcall TIPS20Dialog::TIPS20Dialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TIPS20Dialog::CancelButtonClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TIPS20Dialog::Button2Click(TObject *Sender)
{
    SetAllData();
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TIPS20Dialog::BitBtn2Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TIPS20Dialog::BitBtn1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TIPS20Dialog::Timer1Timer(TObject *Sender)
{
    GetAllData();
}
//---------------------------------------------------------------------------

int TIPS20Dialog::GetAllData()
{
    char StatStr[16];
    int error;
    error = SweepCoil->SetExtendedResolution(1);
    error = SweepCoil->GetStatus(StatStr);
    if (SweepCoil->Quenched(StatStr))
        QuenchSpeedButton->Enabled = true;
      else QuenchSpeedButton->Enabled = false;
    if (SweepCoil->SpeedLimited(StatStr))
        LimitSpeedButton->Enabled = true;
      else LimitSpeedButton->Enabled = false;
    switch (SweepCoil->Activity(StatStr))
    {
        case 0: ActionGroup->ItemIndex = 0;break;
        case 1: ActionGroup->ItemIndex = 2;break;
        case 2: ActionGroup->ItemIndex = 1;break;
        default: ActionGroup->ItemIndex = -1;break;
    }
    FieldLabel->Caption = 1000*SweepCoil->GetParameter(7);
    CurrentLabel->Caption = SweepCoil->GetCurrent();
    TargetFieldLabel->Caption = 1000*SweepCoil->GetTargetField();
    SpeedBLabel->Caption = FormatFloat("0.000",SweepCoil->GetSpeed());
    SpeedALabel->Caption = SweepCoil->GetParameter(6);
    VoltLabel->Caption = SweepCoil->GetParameter(1);
    return 0;
}

int TIPS20Dialog::SetAllData()
{
    return 0;
}
void __fastcall TIPS20Dialog::FormActivate(TObject *Sender)
{
    GetAllData();
    double target, speed;
//
    target = SweepCoil->GetParameter(5);
    FieldEdit->Text = FormatFloat("0.000",1000.0*target/SweepCoil->ApT());
    CurrentEdit->Text = FormatFloat("0.000",target);
//
    speed = SweepCoil->GetParameter(6);
    FieldSpeedEdit->Text = FormatFloat("0.000",speed*1000.0/(60.0*SweepCoil->ApT()));
    CurrentSpeedEdit->Text = FormatFloat("0.000",speed);
    Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TIPS20Dialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TIPS20Dialog::ActionGroupClick(TObject *Sender)
{
    switch (ActionGroup->ItemIndex)
    {
        case 0: SweepCoil->SetAction(0);break;
        case 1: SweepCoil->SetAction(2);break;
        case 2: SweepCoil->SetAction(1);break;
        default: break;
    }
}
//---------------------------------------------------------------------------


void __fastcall TIPS20Dialog::CurrentEditExit(TObject *Sender)
{
    double Cur = CurrentEdit->Text.ToDouble();
    SweepCoil->SetCurrent(Cur);
    FieldEdit->Text = 1000*Cur/SweepCoil->ApT();
}
//---------------------------------------------------------------------------


void __fastcall TIPS20Dialog::FieldEditExit(TObject *Sender)
{
    double BmT = FieldEdit->Text.ToDouble();
    int error = SweepCoil->SetField(BmT/1000.0);
    CurrentEdit->Text = BmT*SweepCoil->ApT()/1000.0;
}
//---------------------------------------------------------------------------


void __fastcall TIPS20Dialog::CurrentSpeedEditExit(TObject *Sender)
{
    double AperMin = CurrentSpeedEdit->Text.ToDouble();
    if (AperMin > SweepCoil->GetSafeSpeed())
    {
        AperMin = SweepCoil->GetSafeSpeed();
        CurrentSpeedEdit->Text = FormatFloat("0.000",AperMin);
    }
    int error = SweepCoil->SetAmpSweepSpeed(AperMin);
    FieldSpeedEdit->Text =
        FormatFloat("00.000", AperMin*1000.0/(60.0*SweepCoil->ApT()));

}
//---------------------------------------------------------------------------

void __fastcall TIPS20Dialog::FieldSpeedEditExit(TObject *Sender)
{
    double AperMin = 60.0*SweepCoil->ApT()*FieldSpeedEdit->Text.ToDouble()/1000.0;
    if (AperMin > SweepCoil->GetSafeSpeed())
    {
        AperMin = SweepCoil->GetSafeSpeed();
        FieldSpeedEdit->Text =
            FormatFloat("00.000", AperMin*1000.0/(60.0*SweepCoil->ApT()));
    }
    int error = SweepCoil->SetAmpSweepSpeed(AperMin);
    CurrentSpeedEdit->Text = FormatFloat("00.000", AperMin);
}
//---------------------------------------------------------------------------


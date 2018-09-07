//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "IPS120Dlg.h"
#include "IsoBusDevices.h"
#include "ErrorBx.h"
#include "HeaterOnChkDlg.h"
#include "TimerFrm.h"
#include "ZeroSweepCoilFrm.h"
#include "MainFieldWaitFrm.h"
#include "HeaterOffChkDlg.h"
#include "LogFileEdit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIPS120Dialog *IPS120Dialog;
extern IB_IPS120 *Magnet;
extern IB_IPS20 *SweepCoil;
extern int StopMeasurement;
void WaitMillisecs(int ms);

//---------------------------------------------------------------------------
__fastcall TIPS120Dialog::TIPS120Dialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TIPS120Dialog::CancelButtonClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TIPS120Dialog::Button2Click(TObject *Sender)
{
    SetAllData();
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TIPS120Dialog::BitBtn2Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TIPS120Dialog::BitBtn1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TIPS120Dialog::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled = false;
	GetAllData();
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

int TIPS120Dialog::GetAllData()
{
    char StatStr[16];
    int error;
    error = Magnet->SetExtendedResolution(1);
    error = Magnet->GetStatus(StatStr);
    if (Magnet->Quenched(StatStr))
        QuenchSpeedButton->Enabled = true;
      else QuenchSpeedButton->Enabled = false;
    if (Magnet->SpeedLimited(StatStr))
        LimitSpeedButton->Enabled = true;
      else LimitSpeedButton->Enabled = false;
    ClampedSpeedButton->Enabled=false;
    switch (Magnet->Activity(StatStr))
    {
        case 0: ActionGroup->ItemIndex = 0;break;
        case 1: ActionGroup->ItemIndex = 2;break;
        case 2: ActionGroup->ItemIndex = 1;break;
        case 4: ClampedSpeedButton->Enabled=true;
                ActionGroup->ItemIndex = -1;
                break;
        default: ActionGroup->ItemIndex = -1;break;
    }

    PersistentFieldLabel->Caption = " ";
    double Bpers;
    switch (Magnet->Heater(StatStr))
    {
        case 0: PersistentSpeedButton->Enabled= false;
                HeaterSelectSpeedButton->Enabled = false;
                HeaterConfirmSpeedButton->Enabled = false;
                break;
        case 1: PersistentSpeedButton->Enabled= false;
                HeaterSelectSpeedButton->Enabled = true;
                HeaterConfirmSpeedButton->Enabled = true;
                break;
        case 2: PersistentSpeedButton->Enabled= true;
                Bpers = Magnet->GetParameter(18);
                PersistentFieldLabel->Caption =
                     FormatFloat("00.00000", Bpers);
                HeaterSelectSpeedButton->Enabled = false;
                HeaterConfirmSpeedButton->Enabled = false;
                break;
        case 5: PersistentSpeedButton->Enabled= false;
                HeaterSelectSpeedButton->Enabled = true;
                HeaterConfirmSpeedButton->Enabled = false;
                break;
        default: PersistentSpeedButton->Enabled= false;
                HeaterSelectSpeedButton->Enabled = false;
                HeaterConfirmSpeedButton->Enabled = false;
                break;
    }
    FieldLabel->Caption = FormatFloat("0.00000",Magnet->GetField());
    CurrentLabel->Caption = Magnet->GetCurrent();
    TargetFieldLabel->Caption = Magnet->GetTargetField();
    SpeedBLabel->Caption = FormatFloat("0.000",Magnet->GetSpeed());
    VoltLabel->Caption = FormatFloat("0.000", Magnet->GetParameter(1));
    return 0;
}

int TIPS120Dialog::SetAllData()
{
    return 0;
}

void __fastcall TIPS120Dialog::FormActivate(TObject *Sender)
{
    GetAllData();
    double target, speed;
//
    target = Magnet->GetParameter(8);
    FieldEdit->Text = FormatFloat("0.00000",target);
//
    speed = Magnet->GetParameter(9);
    FieldSpeedEdit->Text = FormatFloat("0.00000",speed*1000.0/60.0);
    Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TIPS120Dialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TIPS120Dialog::ActionGroupClick(TObject *Sender)
{
	switch (ActionGroup->ItemIndex)
	{
		case 0: Magnet->SetAction(0);break;
		case 1: Magnet->SetAction(2);break;
		case 2: Magnet->SetAction(1);break;
		default: break;
	}
}
//---------------------------------------------------------------------------


void __fastcall TIPS120Dialog::FieldEditExit(TObject *Sender)
{
	double B = FieldEdit->Text.ToDouble();
	int error = Magnet->SetField(B);
}
//---------------------------------------------------------------------------



void __fastcall TIPS120Dialog::FieldSpeedEditExit(TObject *Sender)
{
	double mT_per_s = FieldSpeedEdit->Text.ToDouble();
	double safeApermin = Magnet->GetSafeSpeed();
	if ((60.0*mT_per_s/1000.0) > (safeApermin/Magnet->ApT()))
	{
		mT_per_s = safeApermin *1000.0/(60.0*Magnet->ApT());
		FieldSpeedEdit->Text = FormatFloat("00.000", mT_per_s);
	}
	int error = Magnet->SetFieldSweepSpeed(mT_per_s);
}
//---------------------------------------------------------------------------



void __fastcall TIPS120Dialog::HeaterButtonClick(TObject *Sender)
{
	int error;
	int count;
	int HeaterState = Magnet->Heater();
	double PersistentField;
	double ActualField;
	double SweepCur;
	char message[80];
	char logComment[128];
	char tempLine[80];

	time_t timer = time(NULL);  // get the time and date
	strcpy(logComment, asctime(localtime(&timer)));
    	//record the time of the heater switch action 

	int SweepZero = 0;

	if (StopMeasurement == 0) {return;}   // No measurement should be in progress


// Check HeaterState    0: Heater off field at zero

	switch (HeaterState)
	{
		case 0: PersistentField = Magnet->GetParameter(18);
				if ((PersistentField < -99) && (PersistentField > -100))
				{
					ErrorBox->ErrorMessage->Caption =
					   " Could not read Persistent Field. Action aborted ";
					ErrorBox->ShowModal();
					break;
				}
				HeaterOnCheckForm->PersistentFieldLabel->Caption = PersistentField;

				if (SweepCoil->GetAddress() == 0) SweepZero = 0;
				  else
				  {
					SweepCur = SweepCoil->GetCurrent();
					if ((SweepCur > -20.0) && (SweepCur < 20.0))
					{
						HeaterOffCheckForm->SweepLabel->Caption = SweepCur;
						SweepZero = 1;
					}
				  }

				if (HeaterOnCheckForm->ShowModal() == mbOK)
				{
					// Start procedure
					if (SweepZero == 1)
// SweepCoil to 0
					{
						SweepCoil->SetAmpSweepSpeed(SweepCoil->GetSafeSpeed());
						SweepCoil->SetAction(2);
						ZeroSweepCoilForm->SetTarget(0.0);
						if (ZeroSweepCoilForm->ShowModal() == mbAbort) break;
					}
// main coil to persistent field
					ActualField = Magnet->GetField();
					Magnet->SetField(PersistentField);
					Magnet->SetAction(1);
					MainFieldWaitForm->SetTarget(PersistentField);
					if (MainFieldWaitForm->ShowModal() == mbAbort) break;

// wait 20 s
					TimerForm->SetCount(10);
					TimerForm->Caption = "Wait for stable field...";
					TimerForm->Timer->Interval = 1000;
					TimerForm->Timer->Enabled = true;
					TimerForm->ShowModal();
// Set the heater
					ActualField = Magnet->GetField();
					if (fabs(ActualField - PersistentField) > 0.0001 )
					{
						ErrorBox->ErrorMessage->Caption =
						   " Field value has changed. Action will be aborted. ";
						ErrorBox->ShowModal();
						break;
					}
					Magnet->SetHeater(1);
// Wait 40 s
					TimerForm->SetCount(35);
					TimerForm->Caption = "Wait for switch...";
					TimerForm->Timer->Interval = 1000;
					TimerForm->Timer->Enabled = true;
					TimerForm->ShowModal();

				  // Write action to LogFile
					sprintf(tempLine, " magnet switch heater ON at %8.5f T \n\n", ActualField);
					strcat(logComment, tempLine);
					LogFileForm->AddComment(logComment);
				}

		//  if off, check persistent field
//  Check sweepcoil
//  check with user
//  Set Sweepcoil to zero
//  sweep to persistent field
//  wait 20 sec
//  switch heater on
//  wait 40 seconds
				break;
		case 1: if (Magnet->Sweeping()) break;     // do nothing if current not stable
				ActualField = Magnet->GetField();
				HeaterOffCheckForm->PersistentFieldLabel->Caption = ActualField;

				if (SweepCoil->GetAddress() == 0) SweepZero = 0;
				  else
				  {
					SweepCur = SweepCoil->GetCurrent();
					if ((SweepCur > -20.0) && (SweepCur < 20.0))
					{
						HeaterOffCheckForm->SweepLabel->Caption = SweepCur;
						SweepZero = 1;
					}
				  }
				if (HeaterOffCheckForm->ShowModal() == mbOK)
				{
					// Start procedure
					if (SweepZero == 1)
// SweepCoil to 0
					{
						SweepCoil->SetAmpSweepSpeed(SweepCoil->GetSafeSpeed());
						SweepCoil->SetAction(2);
						ZeroSweepCoilForm->SetTarget(0.0);
						if (ZeroSweepCoilForm->ShowModal() == mbAbort) break;
					}
					TimerForm->SetCount(10);
					TimerForm->Caption = "Wait for Stable Field...";
					TimerForm->Timer->Interval = 1000;
					TimerForm->Timer->Enabled = true;
					TimerForm->ShowModal();
// Set the heater
					if ((Magnet->Sweeping()) ||
						(fabs(Magnet->GetField()-ActualField) > 0.0001))
					{
						ErrorBox->ErrorMessage->Caption =
						   " Field value has changed. Action will be aborted. ";
						ErrorBox->ShowModal();
						break;
					}
					Magnet->SetHeater(0);
// Wait 40 s
					TimerForm->SetCount(35);
					TimerForm->Caption = "Wait for switch...";
					TimerForm->Timer->Interval = 1000;
					TimerForm->Timer->Enabled = true;
					TimerForm->ShowModal();

				  // Write action to LogFile
					sprintf(tempLine, " magnet switch heater OFF at %8.5f T \n\n", ActualField);
					strcat(logComment, tempLine);
					LogFileForm->AddComment(logComment);

					Magnet->SetAction(2);        // Go to zero
					// main coil to
//  if on, check if field is not sweeping
//  Check sweep coil setting
//  Check with user (procedure will take 1 min)
//  Sweep sweepcoil to 0
//  wait 20 sec
//  switch heater off
//  wait 40 sec
//  sweep power supply to 0
				}
				break;
		case 2: PersistentField = Magnet->GetParameter(18);
				if ((PersistentField < -99) && (PersistentField > -100))
				{
					ErrorBox->ErrorMessage->Caption =
					   " Could not read Persistent Field. Action aborted ";
					ErrorBox->ShowModal();
					break;
				}
				HeaterOnCheckForm->PersistentFieldLabel->Caption = PersistentField;

				if (SweepCoil->GetAddress() == 0) SweepZero = 0;
				  else
				  {
					SweepCur = SweepCoil->GetCurrent();
					if ((SweepCur > -20.0) && (SweepCur < 20.0))
					{
						HeaterOffCheckForm->SweepLabel->Caption = SweepCur;
						SweepZero = 1;
					}
				  }

				if (HeaterOnCheckForm->ShowModal() == mbOK)
				{
					// Start procedure
					if (SweepZero == 1)
// SweepCoil to 0
					{
						SweepCoil->SetAmpSweepSpeed(SweepCoil->GetSafeSpeed());
						SweepCoil->SetAction(2);
						ZeroSweepCoilForm->SetTarget(0.0);
						if (ZeroSweepCoilForm->ShowModal() == mbAbort) break;
					}
// main coil to persistent field
					ActualField = Magnet->GetField();
					Magnet->SetField(PersistentField);
					Magnet->SetAction(1);
					MainFieldWaitForm->SetTarget(PersistentField);
					if (MainFieldWaitForm->ShowModal() == mbAbort) break;

// wait 20 s
					TimerForm->SetCount(10);
					TimerForm->Caption = "Wait for stable current...";
					TimerForm->Timer->Interval = 1000;
					TimerForm->Timer->Enabled = true;
					TimerForm->ShowModal();
// Set the heater
					ActualField = Magnet->GetField();
					if (fabs(ActualField - PersistentField) > 0.0001 )
					{
						ErrorBox->ErrorMessage->Caption =
						   " Field value has changed. Action will be aborted. ";
						ErrorBox->ShowModal();
						break;
					}
					Magnet->SetHeater(1);
// Wait 40 s
					TimerForm->SetCount(35);
					TimerForm->Caption = "Wait for switch...";
					TimerForm->Timer->Interval = 1000;
					TimerForm->Timer->Enabled = true;
					TimerForm->ShowModal();

				  // Write action to LogFile
					sprintf(tempLine, " magnet switch heater ON at %8.5f T \n\n", ActualField);
					strcat(logComment, tempLine);
					LogFileForm->AddComment(logComment);

					Magnet->SetAction(0);               // Hold
                }

        //  if off, check persistent field
//  Check sweepcoil
//  check with user
//  Set Sweepcoil to zero
//  sweep to persistent field
//  wait 20 sec
//  switch heater on
//  wait 40 seconds
//  if off, check persistent field
//  Check sweepcoil
//  check with user
//  Set Sweepcoil to zero
//  sweep to persistent field
//  wait 20 sec
//  switch heater on
//  wait 40 seconds
                break;

        default:
                break;
    }
//  GetAllData()
//  Give control back to user

}
//---------------------------------------------------------------------------



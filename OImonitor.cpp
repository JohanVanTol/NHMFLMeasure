//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OImonitor.h"
#include "wsc.h"
#include "IsoBusDevices.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOImonitorForm *OImonitorForm;
extern IB_ITC503 *ITC;
extern IB_IPS120 *Magnet;
extern IB_ILM211 *ILM;
//---------------------------------------------------------------------------
__fastcall TOImonitorForm::TOImonitorForm(TComponent* Owner)
	: TForm(Owner)
{
	MonitorTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TOImonitorForm::MonitorTimerTimer(TObject *Sender)
{
//	CurrField = Magnet->GetField();
//	CurrTemp = ITC->GetTemperature(1);
//	CurrHeLevel = ILM->GetHe();
//	CurrN2Level = ILM->GetN2();


	FieldLabel->Caption = Magnet->GetField();
	TemperatureLabel->Caption = ITC->GetTemperature(1);
	HeLabel->Caption = ILM->GetHe();
	N2Label->Caption = ILM->GetN2();

	return;
}
//---------------------------------------------------------------------------

void __fastcall TOImonitorForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	MonitorTimer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TOImonitorForm::FormActivate(TObject *Sender)
{
	MonitorTimer->Enabled = true;	
}
//---------------------------------------------------------------------------

void __fastcall TOImonitorForm::FormShow(TObject *Sender)
{
	MonitorTimer->Enabled = true;	
}
//---------------------------------------------------------------------------

void __fastcall TOImonitorForm::FormDeactivate(TObject *Sender)
{
	MonitorTimer->Enabled = false;	
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "ChildWin.h"
#include "PhaseDlg.h"
#include "Valid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRePhaseDialog *RePhaseDialog;
//---------------------------------------------------------------------------
__fastcall TRePhaseDialog::TRePhaseDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TRePhaseDialog::FormShow(TObject *Sender)
{
	*OldData = *(TempFile->GetData());
	LIphase1 = 0.0;
	LIphase2 = 0.0;
	MWphase = 0.0;
	ModPhase1Edit->Text = LIphase1;
	ModPhase2Edit->Text = LIphase2;
	WavePhaseEdit->Text = MWphase;
}
//---------------------------------------------------------------------------

void __fastcall TRePhaseDialog::ModPhase1EditChange(TObject *Sender)
{
	CalcNewCurves();
}
//---------------------------------------------------------------------------

void __fastcall TRePhaseDialog::FormCreate(TObject *Sender)
{
	OldData = new DataArray(10);
	LIphase1 = 0.0;
	LIphase2 = 0.0;
	MWphase = 0.0;
}
//---------------------------------------------------------------------------

void __fastcall TRePhaseDialog::FormDestroy(TObject *Sender)
{
	delete OldData;
}
//---------------------------------------------------------------------------

void __fastcall TRePhaseDialog::OkButtonClick(TObject *Sender)
{
//  Add corrections to file:
	char line[256];
	double angle1, angle2, angle3;
	if (!(ValidReal(ModPhase1Edit->Text,&angle1)))
		angle1 = 0.0;
	if (!(ValidReal(ModPhase2Edit->Text,&angle2)))
		angle2 = 0.0;
	if (!(ValidReal(WavePhaseEdit->Text,&angle3)))
		angle3 = 0.0;
	sprintf(line,"Modulation phase corrections made: LockIn1 %6.1f degr., LockIn2 %6.1f degr. \n",
			angle1, angle2);
	TempFile->AddHeader(line);
	sprintf(line,"Millimeter phase correction (abs and disp) %6.1f degr. \n",
			angle3);
	TempFile->AddHeader(line);
	LIphase1 = angle1;
	LIphase2 = angle2;
	MWphase = angle3;
	TempFile->Saved = false;   // Otherwise we can quit without
							// being reminded to save
//    Close();
	TempFile->PlotData();
	TempFile = NULL;
//	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TRePhaseDialog::CancelButtonClick(TObject *Sender)
{
	TempFile->SetData(*OldData);
	TempFile = NULL;
//    Close();
}
//---------------------------------------------------------------------------

void __fastcall TRePhaseDialog::ModPhase2EditChange(TObject *Sender)
{
	CalcNewCurves();
}
//---------------------------------------------------------------------------

void __fastcall TRePhaseDialog::WavePhaseEditChange(TObject *Sender)
{
	CalcNewCurves();
}
//---------------------------------------------------------------------------

void TRePhaseDialog::CalcNewCurves()
{
	double co1, si1, co2, si2, co3, si3;
	double NewX, NewY, angle1, angle2, angle3;

	if (OldData->GetNy() >= 4)
	{
		DataPoint POld(OldData->GetNy());
		DataPoint PNew(OldData->GetNy());

		if (ValidReal(WavePhaseEdit->Text,&angle3))
		{
			co3 = cos(angle3*acos(0)/90.0);
			si3 = sin(angle3*acos(0)/90.0);
		}
		else return;

		if (ValidReal(ModPhase1Edit->Text,&angle1))
		{
			co1 = cos(angle1*acos(0)/90.0);
			si1 = sin(angle1*acos(0)/90.0);
		}
		else return;

		if (ValidReal(ModPhase2Edit->Text,&angle2))
		{
			co2 = cos(angle2*acos(0)/90.0);
			si2 = sin(angle2*acos(0)/90.0);
		}
		else return;

		for (int i=0; i<OldData->Getn(); i++)
		{
			POld = OldData->Get(i);
			PNew = TempFile->GetData()->Get(i);
			NewX = co3*(co1*POld.Get(1)+si1*POld.Get(3))
				 + si3*(co2*POld.Get(2) + si2*POld.Get(4));
			NewY = -(si3)*(co1*POld.Get(1)+si1*POld.Get(3))
				 + co3*(co2*POld.Get(2) + si2*POld.Get(4));
			PNew.Set(1, NewX);
			PNew.Set(2, NewY);

			NewX = co3*(co1*POld.Get(3) - si1*POld.Get(1))
				 + si3*(co2*POld.Get(4) - si2*POld.Get(2));
			NewY =  co3*(co2*POld.Get(4) - si2*POld.Get(2))
				 - si3*(co1*POld.Get(3) - si1*POld.Get(1));
			PNew.Set(3, NewX);
			PNew.Set(4, NewY);

			if (OldData->GetNy() >= 6)
			{
				NewX = co3*POld.Get(5) + si3*POld.Get(6);
				NewY = co3*POld.Get(6) - si3* POld.Get(5);
				PNew.Set(5, NewX);
				PNew.Set(6, NewY);
			}

			TempFile->GetData()->Set(i, PNew);
		}
	}
	else
	  {
		DataPoint POld(2);
		DataPoint PNew(2);

		if (ValidReal(ModPhase1Edit->Text,&angle1))
		{
			co1 = cos(angle1*acos(0)/90.0);
			si1 = sin(angle1*acos(0)/90.0);
		}
		   else return;

		for (int i=0; i<OldData->Getn(); i++)
		{
			POld = OldData->Get(i);
			PNew = TempFile->GetData()->Get(i);
			NewX = co1*POld.Get(1)+si1*POld.Get(2);
			NewY = -si1*POld.Get(1)+co1*POld.Get(2);
			PNew.Set(1, NewX);
			PNew.Set(2, NewY);

			TempFile->GetData()->Set(i, PNew);
		}
	  }
	TempFile->Invalidate();
}


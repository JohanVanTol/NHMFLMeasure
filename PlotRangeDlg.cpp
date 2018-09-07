//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#include "Valid.h"
#include "ChildWin.h"
#include "PlotRangeDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPlotRangeDialog *PlotRangeDialog;
//---------------------------------------------------------------------------
__fastcall TPlotRangeDialog::TPlotRangeDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------



int TPlotRangeDialog::SetPlotRange()
{
    int AutoRange = 0;

    if (AutoXCheckBox->Checked) AutoRange += 1;
    if (AutoY1CheckBox->Checked) AutoRange += 2;
	if (AutoY2CheckBox->Checked) AutoRange += 4;
	for (int i=0; i<TempFile->GetnPlots(); i++) TempFile->GetPlot(i)->SetAutoRange(AutoRange);
	DataPoint Min(2);
	DataPoint Max(2);
	double value;
	if (ValidReal(XminEdit->Text, &value))
		Min.Set(0,value);
	if (ValidReal(XmaxEdit->Text, &value))
		Max.Set(0,value);
	if (ValidReal(Y1minEdit->Text, &value))
		Min.Set(1,value);
	if (ValidReal(Y1maxEdit->Text, &value))
		Max.Set(1,value);
	if (ValidReal(Y2minEdit->Text, &value))
		Min.Set(2,value);
	if (ValidReal(Y2maxEdit->Text, &value))
		Max.Set(2,value);
	for (int i=0; i<TempFile->GetnPlots(); i++) TempFile->GetPlot(i)->SetFixedRanges(Min, Max);
	return 0;
}

int TPlotRangeDialog::GetPlotRange()
{
    DataPoint Pmin = TempFile->GetPlot()->GetMinRanges();
    DataPoint Pmax = TempFile->GetPlot()->GetMaxRanges();

    // Set the dialog values
    PlotRangeDialog->XminEdit->Text = Pmin.Get(0);
    PlotRangeDialog->Y1minEdit->Text = Pmin.Get(1);
    PlotRangeDialog->Y2minEdit->Text = Pmin.Get(2);

    PlotRangeDialog->XmaxEdit->Text = Pmax.Get(0);
    PlotRangeDialog->Y1maxEdit->Text = Pmax.Get(1);
    PlotRangeDialog->Y2maxEdit->Text = Pmax.Get(2);

    int mode = TempFile->GetPlot()->GetAutoRange();
    if ((mode & 0x0001) == 1) AutoXCheckBox->Checked = true;
    if ((mode & 0x0002) == 2) AutoY1CheckBox->Checked = true;
    if ((mode & 0x0004) == 4) AutoY2CheckBox->Checked = true;
    return mode;
}

void __fastcall TPlotRangeDialog::FormActivate(TObject *Sender)
{
    GetPlotRange();
}
//---------------------------------------------------------------------------

void __fastcall TPlotRangeDialog::OKButtonClick(TObject *Sender)
{
    SetPlotRange();
    TempFile->Invalidate();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TPlotRangeDialog::CancelButtonClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TPlotRangeDialog::XminEditChange(TObject *Sender)
{
    AutoXCheckBox->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TPlotRangeDialog::XmaxEditChange(TObject *Sender)
{
    AutoXCheckBox->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TPlotRangeDialog::Y1minEditChange(TObject *Sender)
{
    AutoY1CheckBox->Checked = false;

}
//---------------------------------------------------------------------------

void __fastcall TPlotRangeDialog::Y1maxEditChange(TObject *Sender)
{
    AutoY1CheckBox->Checked = false;

}
//---------------------------------------------------------------------------

void __fastcall TPlotRangeDialog::Y2minEditChange(TObject *Sender)
{
    AutoY2CheckBox->Checked = false;

}
//---------------------------------------------------------------------------

void __fastcall TPlotRangeDialog::Y2maxEditChange(TObject *Sender)
{
    AutoY2CheckBox->Checked = false;

}
//---------------------------------------------------------------------------


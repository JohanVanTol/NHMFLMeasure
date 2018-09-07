//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CommentDlg.h"
#include "Valid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCommentDialog *CommentDialog;
//---------------------------------------------------------------------------
__fastcall TCommentDialog::TCommentDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TCommentDialog::OkButtonClick(TObject *Sender)
{
//    ModalResult = mbOK;    
}
//---------------------------------------------------------------------------

void __fastcall TCommentDialog::LODialEditExit(TObject *Sender)
{
    int dialset;
    double freq, TextFreq;
    if (ValidInt(LODialEdit->Text, &dialset))
        if ((dialset>3000) && (dialset<5000))
        {
			freq = double(43000+dialset)*0.0025 +0.1;
			if (dialset>4200) {
				freq = double(43000+dialset)*0.0024 +0.1;
			}
			if (ValidReal(LOFreqEdit->Text, &TextFreq))
			{
				if ((TextFreq >200) && (TextFreq < 300)) freq *= 2.0;
				if (TextFreq >=300) freq*=3.0;
			}
            LOFreqEdit->Text = freq;
        }
	return;
}
//---------------------------------------------------------------------------


void __fastcall TCommentDialog::SourceDialEditExit(TObject *Sender)
{
	double YIGfreq;
	double freq, TextFreq;
	if (ValidReal(SourceDialEdit->Text, &YIGfreq))
	switch (MultComboBox->ItemIndex)
	{
		case 0:
			SourceFreqEdit->Text = YIGfreq*2.0;
			break;
		case 1:
			SourceFreqEdit->Text = YIGfreq*3.0;
			break;
		case 2:
			SourceFreqEdit->Text = YIGfreq*4.0;
			break;
		case 3:
			SourceFreqEdit->Text = YIGfreq*6.0;
			break;
		case 4:
			SourceFreqEdit->Text = YIGfreq*8.0;
			break;
		case 5:
			SourceFreqEdit->Text = YIGfreq*12.0;
			break;
		case 6:
			SourceFreqEdit->Text = YIGfreq*16.0;
			break;
		case 7:
			SourceFreqEdit->Text = YIGfreq*24.0;
			break;
		case 8:
			SourceFreqEdit->Text = YIGfreq*32.0;
			break;
		case 9:
			SourceFreqEdit->Text = YIGfreq*48.0;
			break;
		case 10:
			SourceFreqEdit->Text = YIGfreq*64.0;
			break;
		default:
			SourceFreqEdit->Text = YIGfreq*16.0;
    }

	return;

}
//---------------------------------------------------------------------------


void __fastcall TCommentDialog::CancelButtonClick(TObject *Sender)
{
    ModalResult = mbCancel;    
}
//---------------------------------------------------------------------------








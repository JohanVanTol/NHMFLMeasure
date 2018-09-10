//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ErrorBx.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TErrorBox *ErrorBox;
//--------------------------------------------------------------------- 
__fastcall TErrorBox::TErrorBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TErrorBox::OKButtonClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void TErrorBox::ShowMessage(const char* err_mess)
{
	ErrorMessage->Caption = err_mess;
	ShowModal();
	return;
}

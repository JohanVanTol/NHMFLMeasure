//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CommentDlg17T.h"
#include "Valid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCommentDialog2 *CommentDialog2;
//---------------------------------------------------------------------------
__fastcall TCommentDialog2::TCommentDialog2(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TCommentDialog2::OkButtonClick(TObject *Sender)
{
 //   ModalResult = mbOK;    
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall TCommentDialog2::CancelButtonClick(TObject *Sender)
{
    ModalResult = mbCancel;    
}
//---------------------------------------------------------------------------







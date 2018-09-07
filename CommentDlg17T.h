//---------------------------------------------------------------------------
#ifndef CommentDlg17TH
#define CommentDlg17TH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TCommentDialog2 : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *AddCommentGroupBox;
    TGroupBox *HeaderGroupBox;
    TMemo *HeaderMemo;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *SourceFreqEdit;
    TEdit *LOFreqEdit;
    TLabel *Label11;
    TMemo *CommentMemo;
    TBitBtn *OkButton;
    TBitBtn *CancelButton;
    TEdit *SigAttEdit;
    TLabel *Label17;
    TRadioButton *TransmissionRadioButton;
    TRadioButton *QORadioButton;
    TLabel *Label4;
    TLabel *Label5;
    TComboBox *LockComboBox;
    TLabel *Label6;
    TEdit *PowerEdit;
    TLabel *Label7;
    TLabel *Label8;
    void __fastcall OkButtonClick(TObject *Sender);
//    void __fastcall LODialEditExit(TObject *Sender);
//    void __fastcall SourceDialEditExit(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TCommentDialog2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCommentDialog2 *CommentDialog2;
//---------------------------------------------------------------------------
#endif

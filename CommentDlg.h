//---------------------------------------------------------------------------
#ifndef CommentDlgH
#define CommentDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TCommentDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *AddCommentGroupBox;
    TGroupBox *HeaderGroupBox;
    TMemo *HeaderMemo;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *SourceFreqEdit;
    TEdit *LOFreqEdit;
    TEdit *SourceDialEdit;
    TEdit *LODialEdit;
    TLabel *Label8;
    TLabel *Label9;
    TEdit *WaveLengthEdit;
    TLabel *Label10;
    TComboBox *LaserPulseComboBox;
    TLabel *Label11;
    TMemo *CommentMemo;
    TBitBtn *OkButton;
    TBitBtn *CancelButton;
    TLabel *Label12;
    TEdit *MPEdit;
    TLabel *Label13;
    TEdit *SigAttEdit;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *RefAttEdit;
    TEdit *SigDiodeEdit;
    TEdit *RefDiodeEdit;
    TLabel *Label7;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TEdit *GridAngleEdit;
    TLabel *Label19;
    TLabel *Label20;
	TLabel *Label21;
	TComboBox *MultComboBox;
    void __fastcall OkButtonClick(TObject *Sender);
    void __fastcall LODialEditExit(TObject *Sender);
    void __fastcall SourceDialEditExit(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TCommentDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCommentDialog *CommentDialog;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef ILMDlgH
#define ILMDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TILMDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TButton *CloseButton;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *HeLabel;
    TLabel *N2Label;
    TButton *SampleButton;
    void __fastcall CloseButtonClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall SampleButtonClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TILMDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TILMDialog *ILMDialog;
//---------------------------------------------------------------------------
#endif

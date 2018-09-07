//---------------------------------------------------------------------------

#ifndef GetSynFreqDlgH
#define GetSynFreqDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TGetSynFreqDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TEdit *FreqEdit;
    TLabel *Label1;
    TBitBtn *CancelButton;
    TBitBtn *OkButton;
    TUpDown *UpDown1;
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall OkButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TGetSynFreqDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGetSynFreqDialog *GetSynFreqDialog;
//---------------------------------------------------------------------------
#endif

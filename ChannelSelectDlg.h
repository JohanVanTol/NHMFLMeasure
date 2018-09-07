//---------------------------------------------------------------------------
#ifndef ChannelSelectDlgH
#define ChannelSelectDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TChannelSelectDialog : public TForm
{
__published:	// IDE-managed Components
    TRadioGroup *RadioGroup1;
    TRadioButton *CH1Button;
    TRadioButton *CH2Button;
    TRadioButton *CH3Button;
    TRadioButton *CH4Button;
    TRadioButton *Mem1Button;
    TRadioButton *Mem2Button;
    TRadioButton *Mem3Button;
    TRadioButton *Mem4Button;
    TRadioButton *Func4Button;
    TRadioButton *Func3Button;
    TRadioButton *Func2Button;
    TRadioButton *Func1Button;
    TBitBtn *CancelButton;
    TBitBtn *OKButton;
    TRadioButton *Combine13Button;
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall OKButtonClick(TObject *Sender);
private:	// User declarations
    int Channel;
public:		// User declarations
    __fastcall TChannelSelectDialog(TComponent* Owner);
    GetChannel() {return Channel;}
};
//---------------------------------------------------------------------------
extern PACKAGE TChannelSelectDialog *ChannelSelectDialog;
//---------------------------------------------------------------------------
#endif

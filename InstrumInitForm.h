//---------------------------------------------------------------------------
#ifndef InstrumInitFormH
#define InstrumInitFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TInstrumentInitForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo;
    TBitBtn *OKButton;
    TTimer *WaitBeforeClosingTimer;
    void __fastcall WaitBeforeClosingTimerTimer(TObject *Sender);
    void __fastcall OKButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TInstrumentInitForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInstrumentInitForm *InstrumentInitForm;
//---------------------------------------------------------------------------
#endif

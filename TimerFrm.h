//---------------------------------------------------------------------------
#ifndef TimerFrmH
#define TimerFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TTimerForm : public TForm
{
__published:	// IDE-managed Components
    TTimer *Timer;
    TLabel *CountLabel;
    void __fastcall TimerTimer(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    int Count;
public:		// User declarations
    void SetCount(int n);
    __fastcall TTimerForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTimerForm *TimerForm;
//---------------------------------------------------------------------------
#endif

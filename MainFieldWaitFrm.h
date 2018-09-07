//---------------------------------------------------------------------------
#ifndef MainFieldWaitFrmH
#define MainFieldWaitFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMainFieldWaitForm : public TForm
{
__published:	// IDE-managed Components
    TTimer *MainCoilTimer;
    TBitBtn *AbortButton;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *TargetLabel;
    TLabel *FieldLabel;
    void __fastcall MainCoilTimerTimer(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall AbortButtonClick(TObject *Sender);
private:	// User declarations
    double Target;
public:		// User declarations
    int SetTarget(double _Target);
    __fastcall TMainFieldWaitForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainFieldWaitForm *MainFieldWaitForm;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef ZeroSweepCoilFrmH
#define ZeroSweepCoilFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TZeroSweepCoilForm : public TForm
{
__published:	// IDE-managed Components
    TTimer *CheckTimer;
    TBitBtn *BitBtn1;
    TLabel *SweepLabel;
    TLabel *Label2;
    TLabel *Lable;
    TLabel *TargetLabel;
    TLabel *Label3;
    void __fastcall CheckTimerTimer(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    double Target;
public:		// User declarations
    int SetTarget(double _Target);
    __fastcall TZeroSweepCoilForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TZeroSweepCoilForm *ZeroSweepCoilForm;
//---------------------------------------------------------------------------
#endif

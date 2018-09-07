//---------------------------------------------------------------------------
#ifndef DMMDlgH
#define DMMDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TDMM1Form : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BitBtn1;
    TGroupBox *GroupBox1;
    TLabel *ReadLabel;
    TTimer *ReadTimer;
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ReadTimerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TDMM1Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDMM1Form *DMM1Form;
//---------------------------------------------------------------------------
#endif

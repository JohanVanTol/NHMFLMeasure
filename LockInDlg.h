//
//  This routine works for SRS830 LockIn only
//---------------------------------------------------------------------------
#ifndef LockInDlgH
#define LockInDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "SRS830.h"  // for SRS830 routines
#include "SRS860.h"  // for SRS860 routines
//---------------------------------------------------------------------------
class TLockInDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *OutputGroupBox;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *XOutputLabel;
    TLabel *YOutputLabel;
    TLabel *ROutputLabel;
    TLabel *ThetaOutputLabel;
    TTimer *UpdateTimer;
    TBitBtn *OkButton;
    TBitBtn *UpdateButton;
    TGroupBox *SettingsGroupBox;
    TComboBox *SensComboBox;
    TLabel *SensitivityLabel;
    TComboBox *TCComboBox;
    TLabel *Label5;
    void __fastcall OkButtonClick(TObject *Sender);
    void __fastcall UpdateTimerTimer(TObject *Sender);
    void __fastcall UpdateButtonClick(TObject *Sender);
    void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall SensComboBoxChange(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	SRS830 *LI;
public:		// User declarations
    __fastcall TLockInDialog(TComponent* Owner);
	void SetLockIn(SRS830* Ptr) {LI = Ptr;}
//	void SetLockIn(SRS860* Ptr) {LI = Ptr;}
};
//---------------------------------------------------------------------------
extern PACKAGE TLockInDialog *LockInDialog;
//---------------------------------------------------------------------------
#endif

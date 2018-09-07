//---------------------------------------------------------------------------
#ifndef MeasOptionsFormH
#define MeasOptionsFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMeasureOptionsDlg : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *XMeasGroupBox;
    TGroupBox *LI1GroupBox;
    TGroupBox *GroupBox3;
    TCheckBox *MagnetCheckBox;
    TCheckBox *SweepCheckBox;
    TCheckBox *FreqCheckBox;
    TCheckBox *TimeCheckBox;
    TCheckBox *LI1XCheckBox;
    TCheckBox *LI1YCheckBox;
    TCheckBox *LI1RCheckBox;
    TCheckBox *LI1ThetaCheckBox;
    TGroupBox *LI2GroupBox;
    TCheckBox *LI2XCheckBox;
    TCheckBox *LI2YCheckBox;
    TCheckBox *LI2RCheckBox;
    TCheckBox *LI2ThetaCheckBox;
private:	// User declarations
public:		// User declarations
    __fastcall TMeasureOptionsDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMeasureOptionsDlg *MeasureOptionsDlg;
//---------------------------------------------------------------------------
#endif

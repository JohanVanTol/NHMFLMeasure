//---------------------------------------------------------------------------

#ifndef ManyCurveSelectDlgH
#define ManyCurveSelectDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TManyCurveDlg : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *StartEdit;
    TEdit *SkipEdit;
    TEdit *TotalEdit;
    TUpDown *UpDown1;
    TUpDown *UpDown2;
    TUpDown *UpDown3;
    TBitBtn *BitBtn1;
private:	// User declarations
public:		// User declarations
    __fastcall TManyCurveDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TManyCurveDlg *ManyCurveDlg;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef PlotCurvesDialogH
#define PlotCurvesDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPlotCurvesDlg : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TGroupBox *GroupBox3;
    TBitBtn *OKButton;
    TBitBtn *CancelButton;
    TCheckBox *YL1;
    TCheckBox *YL2;
    TCheckBox *YL3;
    TCheckBox *YL4;
    TCheckBox *YL5;
    TCheckBox *YL6;
    TCheckBox *YL7;
    TCheckBox *YL8;
    TCheckBox *YL9;
    TCheckBox *YL10;
    TRadioButton *XB1;
    TRadioButton *XB2;
    TRadioButton *XB3;
    TRadioButton *XB4;
    TRadioButton *XB5;
    TRadioButton *XB6;
    TRadioButton *XB7;
    TCheckBox *YR1;
    TCheckBox *YR2;
    TCheckBox *YR3;
    TCheckBox *YR4;
    TCheckBox *YR5;
    TCheckBox *YR6;
    TCheckBox *YR7;
    TCheckBox *YR8;
    TCheckBox *YR9;
    TCheckBox *YR10;
    TCheckBox *YR11;
    TCheckBox *YR12;
    TCheckBox *YR13;
    TCheckBox *YR14;
    TCheckBox *YR15;
    TCheckBox *YR16;
    TCheckBox *YR17;
    TCheckBox *YR18;
    TCheckBox *YR19;
    TCheckBox *YR20;
    TRadioButton *XB8;
    TRadioButton *XB9;
    TRadioButton *XB10;
    TRadioButton *XB11;
    TRadioButton *XB12;
    TRadioButton *XB13;
    TRadioButton *XB14;
    TRadioButton *XB15;
    TRadioButton *XB16;
    TRadioButton *XB17;
    TRadioButton *XB18;
    TRadioButton *XB19;
    TRadioButton *XB20;
    TCheckBox *YL11;
    TCheckBox *YL12;
    TCheckBox *YL13;
    TCheckBox *YL14;
    TCheckBox *YL15;
    TCheckBox *YL16;
    TCheckBox *YL17;
    TCheckBox *YL18;
    TCheckBox *YL19;
    TCheckBox *YL20;
    TRadioButton *XBPoint;
    void __fastcall OKButtonClick(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
    TDataFile *TempFile;
    TCheckBox **YL;
    TCheckBox **YR;
    TRadioButton **XB;

    int SetPlotCurves();
    int GetPlotCurves();
    int EnableButtons();

public:		// User declarations
    void SetDataFile(TDataFile* File) {TempFile = File;}
    __fastcall TPlotCurvesDlg(TComponent* Owner);
    int SetPlotCurves(TDataFile* File);

};
//---------------------------------------------------------------------------
extern PACKAGE TPlotCurvesDlg *PlotCurvesDlg;
//---------------------------------------------------------------------------
#endif

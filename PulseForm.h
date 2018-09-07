//---------------------------------------------------------------------------

#ifndef PulseFormH
#define PulseFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TPulsedDialog : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *OKBitButton;
    TBitBtn *CancelBitButton;
    TGroupBox *PulseGroupBox;
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TStaticText *StaticText1;
    TMaskEdit *P3DelayEdit;
	TMaskEdit *P4DelayEdit;
    TMaskEdit *P2DelayEdit;
    TMaskEdit *P1DelayEdit;
    TStaticText *StaticText2;
    TMaskEdit *P1WidthEdit;
    TMaskEdit *P2WidthEdit;
    TMaskEdit *P3WidthEdit;
    TMaskEdit *P4WidthEdit;
    TMaskEdit *P4DelayStepEdit;
    TMaskEdit *P3DelayStepEdit;
    TMaskEdit *P2DelayStepEdit;
    TMaskEdit *P1DelayStepEdit;
    TMaskEdit *P1WidthStepEdit;
    TMaskEdit *P2WidthStepEdit;
    TMaskEdit *P3WidthStepEdit;
    TMaskEdit *P4WidthStepEdit;
    TGroupBox *AcqGroupBox;
    TLabel *Label2;
    TStaticText *StaticText3;
    TStaticText *StaticText4;
    TCheckBox *P1CheckBox;
    TCheckBox *P2CheckBox;
    TCheckBox *P3CheckBox;
    TCheckBox *P4CheckBox;
	TMaskEdit *AcqDelayEdit;
	TMaskEdit *AcqStepEdit;
    TStaticText *StaticText5;
    TStaticText *StaticText6;
	TMaskEdit *RateEdit;
	TMaskEdit *AverEdit;
    TStaticText *StaticText7;
    TButton *SetButton;
	TMaskEdit *IntTimeEdit;
	TLabel *IntergrLabel;
	TComboBox *SweepTypeListBox;
	TMaskEdit *ScanEdit;
	TLabel *Svcan;
	TMaskEdit *NptsEdit;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TCheckBox *QuadratureCheckBox;
	TCheckBox *TwoChannelCheckBox;
	TCheckBox *ChACheckBox;
	TMaskEdit *P5DelayEdit;
	TMaskEdit *P6DelayEdit;
	TMaskEdit *P7DelayEdit;
	TMaskEdit *P8DelayEdit;
	TMaskEdit *P5DelayStepEdit;
	TMaskEdit *P6DelayStepEdit;
	TMaskEdit *P7DelayStepEdit;
	TMaskEdit *P8DelayStepEdit;
	TMaskEdit *P5WidthEdit;
	TMaskEdit *P6WidthEdit;
	TMaskEdit *P7WidthEdit;
	TMaskEdit *P8WidthEdit;
	TMaskEdit *P5WidthStepEdit;
	TMaskEdit *P6WidthStepEdit;
	TMaskEdit *P7WidthStepEdit;
	TMaskEdit *P8WidthStepEdit;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TCheckBox *P5CheckBox;
	TCheckBox *P6CheckBox;
	TCheckBox *P7CheckBox;
	TCheckBox *P8CheckBox;
	TCheckBox *BaseLineCorrectionCheckBox;
    void __fastcall SetButtonClick(TObject *Sender);
	void __fastcall OKBitButtonClick(TObject *Sender);
	void __fastcall CancelBitButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TPulsedDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPulsedDialog *PulsedDialog;
//---------------------------------------------------------------------------
#endif

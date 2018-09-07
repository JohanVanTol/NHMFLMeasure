//---------------------------------------------------------------------------

#ifndef RF2DlgH
#define RF2DlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TRF2Dialog : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *RFGroupBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TEdit *FreqEdit;
	TCheckBox *CheckBox1;
	TCheckBox *PowerCheckBox;
	TEdit *PowerEdit;
	TEdit *ModFreqEdit;
	TButton *CalcelButton;
	TButton *SetButton;
	TButton *OKButton;
	TGroupBox *ModulationGroupBox;
	TLabel *Label10;
	TLabel *Label15;
	TLabel *FMUnitLabel;
	TEdit *AMEdit;
	TEdit *FMEdit;
	TPanel *AMIntPanel;
	TRadioButton *AMIntRadioButton;
	TRadioButton *AMOffRadioButton;
	TRadioButton *AMExtAcRadioButton;
	TRadioButton *AMExtDcRadioButton;
	TRadioButton *AMDualACRadioButton;
	TRadioButton *AMDualDCRadioButton;
	TPanel *AMExtPanel;
	TRadioButton *FMOFFRadioButton;
	TRadioButton *FMIntRadioButton;
	TRadioButton *FMExtACRadioButton;
	TRadioButton *FMExtDCRadioButton;
	TRadioButton *FMDualACRadioButton;
	TRadioButton *FMDualDCRadioButton;
	TRadioButton *FMRadioButton;
	TRadioButton *PMRadioButton;
	TGroupBox *PowerGroupBox;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *label;
	TLabel *Label5;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *SweepLabel;
	TEdit *StartFreqEdit;
	TEdit *StopFreqEdit;
	TEdit *NStepsEdit;
	TEdit *StepEdit;
	TEdit *DwellEdit;
	TButton *ErrorButton;
	TEdit *ErrorEdit;
	TButton *StopSweepButton;
	TButton *SweepResetButton;
	TButton *SweepStartButton;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FMRadioButtonClick(TObject *Sender);
    void __fastcall PMRadioButtonClick(TObject *Sender);
    void __fastcall ErrorButtonClick(TObject *Sender);
    void __fastcall SetButtonClick(TObject *Sender);
    void __fastcall CalcelButtonClick(TObject *Sender);
    void __fastcall OKButtonClick(TObject *Sender);
    void __fastcall FreqEditChange(TObject *Sender);
    void __fastcall AMOffRadioButtonClick(TObject *Sender);
    void __fastcall AMIntRadioButtonClick(TObject *Sender);
    void __fastcall AMExtAcRadioButtonClick(TObject *Sender);
    void __fastcall AMExtDcRadioButtonClick(TObject *Sender);
    void __fastcall AMDualACRadioButtonClick(TObject *Sender);
    void __fastcall AMDualDCRadioButtonClick(TObject *Sender);
    void __fastcall FMOFFRadioButtonClick(TObject *Sender);
    void __fastcall FMIntRadioButtonClick(TObject *Sender);
    void __fastcall FMExtACRadioButtonClick(TObject *Sender);
    void __fastcall FMExtDCRadioButtonClick(TObject *Sender);
    void __fastcall FMDualACRadioButtonClick(TObject *Sender);
    void __fastcall FMDualDCRadioButtonClick(TObject *Sender);
    void __fastcall StartFreqEditExit(TObject *Sender);
    void __fastcall StepEditExit(TObject *Sender);
    void __fastcall StopFreqEditExit(TObject *Sender);
    void __fastcall NStepsEditExit(TObject *Sender);
    void __fastcall SweepStartButtonClick(TObject *Sender);
    void __fastcall StopSweepButtonClick(TObject *Sender);
	void __fastcall SweepResetButtonClick(TObject *Sender);
private:	// User declarations
    int GetAllData();
    int SetAllData();
    int CalculateNSteps();
    double CalculateStep();
public:		// User declarations
	__fastcall TRF2Dialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRF2Dialog *RF2Dialog;
//---------------------------------------------------------------------------
#endif

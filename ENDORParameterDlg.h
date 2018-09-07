//---------------------------------------------------------------------------
#ifndef ENDORParameterDlgH
#define ENDORParameterDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TENDORParametersDialog : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *RFGroupBox;
	TButton *CalcelButton;
	TButton *OKButton;
	TGroupBox *ModulationGroupBox;
	TEdit *FreqEdit;
	TLabel *Label1;
	TLabel *Label2;
	TCheckBox *CheckBox1;
	TGroupBox *PowerGroupBox;
	TEdit *StartFreqEdit;
	TLabel *Label3;
	TEdit *StopFreqEdit;
	TLabel *Label4;
	TEdit *NStepsEdit;
	TLabel *label;
	TEdit *StepEdit;
	TLabel *Label5;
	TEdit *PowerEdit;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *ModFreqEdit;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *AMEdit;
	TEdit *FMEdit;
	TEdit *DwellEdit;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *FMUnitLabel;
	TPanel *AMIntPanel;
	TRadioButton *AMIntRadioButton;
	TButton *ErrorButton;
	TEdit *ErrorEdit;
	TPanel *AMExtPanel;
	TRadioButton *AMOffRadioButton;
	TRadioButton *AMExtAcRadioButton;
	TRadioButton *AMExtDcRadioButton;
	TRadioButton *AMDualACRadioButton;
	TRadioButton *AMDualDCRadioButton;
	TRadioButton *FMOFFRadioButton;
	TRadioButton *FMIntRadioButton;
	TRadioButton *FMExtACRadioButton;
	TRadioButton *FMExtDCRadioButton;
	TRadioButton *FMDualACRadioButton;
	TRadioButton *FMDualDCRadioButton;
	TRadioButton *FMRadioButton;
	TRadioButton *PMRadioButton;
	TLabel *AverageLabel;
	TEdit *AverageEdit;
	TGroupBox *RF2GroupBox;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label16;
	TLabel *Label17;
	TEdit *RF2FreqEdit;
	TCheckBox *CheckBox2;
	TEdit *RF2PowerEdit;
	TGroupBox *RF2SweepGroupBox;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TLabel *Label23;
	TEdit *RF2StartFreqEdit;
	TEdit *RF2StopFreqEdit;
	TEdit *RF2NStepsEdit;
	TEdit *RF2StepEdit;
	TCheckBox *PowerCheckBox;
	TCheckBox *Two_D_CheckBox;
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
	void __fastcall RF2StartFreqEditExit(TObject *Sender);
	void __fastcall RF2StepEditExit(TObject *Sender);
	void __fastcall RF2StopFreqEditExit(TObject *Sender);
	void __fastcall RF2NStepsEditExit(TObject *Sender);
	void __fastcall StartFreqEditExit(TObject *Sender);
	void __fastcall StepEditExit(TObject *Sender);
	void __fastcall StopFreqEditExit(TObject *Sender);
	void __fastcall NStepsEditExit(TObject *Sender);
	void __fastcall PowerCheckBoxClick(TObject *Sender);
private:	// User declarations
	double StartFreq;
	double EndFreq;
	double Step;
	int nStep;
	double Power;   //Power in dBm

	double RF2StartFreq;
	double RF2EndFreq;
	double RF2Step;
	int RF2nStep;
	double RF2Power;   //Power in dBm

	int GetAllData();
	int SetAllData();
	int CalculateNSteps();
	double CalculateStep();
	int CalculateRF2NSteps();
	double CalculateRF2Step();
public:		// User declarations
	__fastcall TENDORParametersDialog(TComponent* Owner);
	double GetStartFreq() {return StartFreq;}
	double GetEndFreq() {return EndFreq;}
	double GetStep() {return Step;}
	double GetPower() {return Power;}
	int GetnStep() {return nStep;}
	void SetStartFreq(double f) {StartFreq = f;}
	void SetEndFreq(double f) {EndFreq = f;}
	void SetStep(double f) {Step = f;}
	void SetnStep(int ns) {nStep = ns;}
	void SetPower(double P) {Power = P;}

	double GetRF2StartFreq() {return RF2StartFreq;}
	double GetRF2EndFreq() {return RF2EndFreq;}
	double GetRF2Step() {return RF2Step;}
	double GetRF2Power() {return RF2Power;}
	int GetRF2nStep() {return RF2nStep;}
	void SetRF2StartFreq(double f) {RF2StartFreq = f;}
	void SetRF2EndFreq(double f) {RF2EndFreq = f;}
	void SetRF2Step(double f) {RF2Step = f;}
	void SetRF2nStep(int ns) {RF2nStep = ns;}
	void SetRF2Power(double P) {RF2Power = P;}
};
//---------------------------------------------------------------------------
extern PACKAGE TENDORParametersDialog *ENDORParametersDialog;
//---------------------------------------------------------------------------
#endif

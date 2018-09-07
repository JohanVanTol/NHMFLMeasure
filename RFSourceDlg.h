//---------------------------------------------------------------------------
#ifndef RFSourceDlgH
#define RFSourceDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TRFSourceDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *RFGroupBox;
    TButton *CalcelButton;
    TButton *SetButton;
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
    TCheckBox *PowerCheckBox;
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
    TButton *StopSweepButton;
    TButton *SweepResetButton;
    TButton *SweepStartButton;
    TLabel *SweepLabel;
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
    __fastcall TRFSourceDialog(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TRFSourceDialog *RFSourceDialog;
//---------------------------------------------------------------------------
#endif

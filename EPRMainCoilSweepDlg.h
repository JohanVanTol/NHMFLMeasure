//---------------------------------------------------------------------------
#ifndef EPRMainCoilSweepDlgH
#define EPRMainCoilSweepDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <math.h>
#include "GPIB_NatlInstrum.h"
#include "SRS830.h"

//---------------------------------------------------------------------------
class TEPRMainSweepDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *SweepGroupBox;
    TLabel *Label1;
    TEdit *StartFieldEdit;
    TEdit *EndFieldEdit;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *SweepSpeedEdit;
    TLabel *Label5;
    TBitBtn *OKButton;
    TBitBtn *CancelButton;
    TGroupBox *GroupBox1;
    TEdit *AverEdit;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *ManNptsEdit;
    TCheckBox *AverageCheckBox;
    TGroupBox *GroupBox2;
    TCheckBox *BackSweepRadioButton;
    TRadioButton *AutoAverageRadioButton;
    TRadioButton *ManualAverageRadioButton;
    TLabel *AutoNptsLabel;
    TCheckBox *SaveSweepsCheckBox;
    TCheckBox *FieldModeCheckBox;
    TCheckBox *UpDownCheckBox;
        TCheckBox *HysteresisCorrectionButton;
	TGroupBox *ModulationGroupBox;
	TGroupBox *GroupBox3;
	TLabel *DurationLabel;
	TLabel *ExpDurationLabel;
	TEdit *Edit1;
	TEdit *Edit2;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TButton *Button1;
    void __fastcall OKButtonClick(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall AverageCheckBoxClick(TObject *Sender);
    void __fastcall AutoAverageRadioButtonClick(TObject *Sender);
    void __fastcall ManualAverageRadioButtonClick(TObject *Sender);
    void __fastcall StartFieldEditChange(TObject *Sender);
    void __fastcall SweepSpeedEditChange(TObject *Sender);
    void __fastcall EndFieldEditChange(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
	void __fastcall UpDownCheckBoxClick(TObject *Sender);
	void __fastcall AverEditChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
    double StartField;          // in T
    double EndField;            // in T
    double SweepSpeed;          // in mT/s
    int SetAllData();
    int CalcAutoNpts();
	int AverageNpts;
	SRS830* LI;

public:		// User declarations
    __fastcall TEPRMainSweepDialog(TComponent* Owner);
    double GetStartField() {return StartField;}
    double GetEndField() {return EndField;}
    double GetSweepSpeed() {return SweepSpeed;}
    int GetAverNpts() {return AverageNpts;}
    int SetStartField(double Start) {StartField = Start; return 0;}
    int SetEndField(double End) {EndField = End;return 0;}
    int SetSweepSpeed(double Speed) {SweepSpeed = Speed;return 0;}
    double GetSweepDuration() const;
	int UpdateDuration();
	void SetLI(SRS830 *LIptr) {LI = LIptr;}
	int UpdateModulation();
};
//---------------------------------------------------------------------------
extern PACKAGE TEPRMainSweepDialog *EPRMainSweepDialog;
//---------------------------------------------------------------------------
#endif

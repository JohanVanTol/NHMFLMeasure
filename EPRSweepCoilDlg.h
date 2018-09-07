//---------------------------------------------------------------------------
#ifndef EPRSweepCoilDlgH
#define EPRSweepCoilDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class TEPRSweepCoilDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *SweepParGroupBox;
    TEdit *StartFieldEdit;
    TEdit *EndFieldEdit;
    TEdit *SweepSpeedEdit;
    TLabel *FromLabel;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *PersistentFieldLabel;
    TLabel *Label7;
    TBitBtn *OKButton;
    TBitBtn *CancelButton;
    TGroupBox *ParametersGroup;
    TCheckBox *BackSweepRadioButton;
        TGroupBox *AverageGroupBox;
    TEdit *AverEdit;
        TCheckBox *AverageCheckBox;
        TLabel *Label6;
        TEdit *ManNptsEdit;
        TLabel *Label8;
        TRadioButton *AutoAverageRadioButton;
        TRadioButton *ManualAverageRadioButton;
        TLabel *AutoNptsLabel;
        TCheckBox *SaveSweepsCheckBox;
    TCheckBox *FieldModeCheckBox;
    TCheckBox *UpDownCheckBox;
    void __fastcall OKButtonClick(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall AverageCheckBoxClick(TObject *Sender);
    void __fastcall AutoAverageRadioButtonClick(TObject *Sender);
    void __fastcall ManualAverageRadioButtonClick(TObject *Sender);
private:	// User declarations
    double StartField;   // In Tesla !
    double EndField;     // In Tesla !
    double SweepSpeed;   // In mT/s
    int SetAllData();
    int AverageNpts;
    int CalcAutoNpts();

public:		// User declarations
    __fastcall TEPRSweepCoilDialog(TComponent* Owner);
    double GetStartField() {return StartField;}
    double GetEndField() {return EndField;}
    double GetSweepSpeed() {return SweepSpeed;}
    int GetAverNpts() {return AverageNpts;}
    int SetStartField(double Start) {StartField = Start;return 0;}
    int SetEndField(double End) {EndField = End;return 0;}
    int SetSweepSpeed(double Speed) {SweepSpeed = Speed;return 0;}
    double GetSweepDuration() const
        {return fabs(StartField-EndField)*1000.0/SweepSpeed;}

};
//---------------------------------------------------------------------------
extern PACKAGE TEPRSweepCoilDialog *EPRSweepCoilDialog;
//---------------------------------------------------------------------------
#endif

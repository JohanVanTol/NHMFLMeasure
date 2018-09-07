//---------------------------------------------------------------------------
#ifndef RFAmplDlgH
#define RFAmplDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TRFAmplifierDialog : public TForm
{
__published:	// IDE-managed Components
    TButton *PowerButton;
    TButton *StandByButton;
    TButton *OperateButton;
    TButton *ResetButton;
    TSpeedButton *PowerSpeedButton;
    TSpeedButton *OperateSpeedButton;
    TSpeedButton *StandBySpeedButton;
    TSpeedButton *FaultSpeedButton;
    TEdit *GainEdit;
    TLabel *GainLabel;
    TLabel *Label;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *ForwardPowerLabel;
    TLabel *ReflectedPowerLabel;
    TGroupBox *ALCGroupBox;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TRadioButton *ALCoffRadioButton;
    TRadioButton *ALCintRadioButton;
    TRadioButton *ALCextRadioButton;
    TEdit *ThresholdEdit;
    TEdit *DetGainEdit;
    TEdit *ResponseEdit;
    TLabel *ThresholdLabel;
    TLabel *PercentLabel2;
    TLabel *DetGainLabel;
    TLabel *PercentLabel1;
    TLabel *ResponseLabel;
    TBitBtn *UpdateButton;
    TBitBtn *ContUpdateButton;
    TTimer *Timer1;
    void __fastcall PowerButtonClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall StandByButtonClick(TObject *Sender);
    void __fastcall OperateButtonClick(TObject *Sender);
    void __fastcall ResetButtonClick(TObject *Sender);
    void __fastcall GainEditExit(TObject *Sender);
    
    void __fastcall UpdateButtonClick(TObject *Sender);
    void __fastcall ALCoffRadioButtonClick(TObject *Sender);
    void __fastcall ALCintRadioButtonClick(TObject *Sender);
    void __fastcall ALCextRadioButtonClick(TObject *Sender);
    void __fastcall ThresholdEditExit(TObject *Sender);
    void __fastcall DetGainEditExit(TObject *Sender);
    void __fastcall ResponseEditExit(TObject *Sender);
    void __fastcall ContUpdateButtonClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
    int GetAllData();
    void ALCcommandsEnable(int mode);
public:		// User declarations
    __fastcall TRFAmplifierDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRFAmplifierDialog *RFAmplifierDialog;
//---------------------------------------------------------------------------
#endif

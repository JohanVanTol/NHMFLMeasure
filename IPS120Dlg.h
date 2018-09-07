//---------------------------------------------------------------------------
#ifndef IPS120DlgH
#define IPS120DlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TIPS120Dialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *ReadGroupBox;
    TGroupBox *GroupBox2;
    TLabel *FieldLabel;
    TLabel *Label3;
    TRadioGroup *ActionGroup;
    TTimer *Timer1;
    TLabel *VoltLabel;
    TLabel *Label2;
    TGroupBox *StateGroupBox;
    TSpeedButton *QuenchSpeedButton;
    TLabel *Label4;
    TSpeedButton *LimitSpeedButton;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *FieldEdit;
    TLabel *SpeedBLabel;
    TLabel *Label7;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TLabel *Label10;
    TEdit *FieldSpeedEdit;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *TargetFieldLabel;
    TLabel *Label15;
    TLabel *Label16;
    TButton *Button1;
    TLabel *CurrentLabel;
    TLabel *Label8;
    TGroupBox *HeaterGroupBox;
    TSpeedButton *PersistentSpeedButton;
    TSpeedButton *ClampedSpeedButton;
    TLabel *Label9;
    TLabel *Label11;
    TSpeedButton *HeaterConfirmSpeedButton;
    TSpeedButton *HeaterSelectSpeedButton;
    TButton *HeaterButton;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *PersistentFieldLabel;
    TTimer *CountDownTimer;
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActionGroupClick(TObject *Sender);
    void __fastcall FieldEditExit(TObject *Sender);
    void __fastcall FieldSpeedEditExit(TObject *Sender);
    void __fastcall HeaterButtonClick(TObject *Sender);
private:	// User declarations
    int GetAllData();
    int SetAllData();
public:		// User declarations
    __fastcall TIPS120Dialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TIPS120Dialog *IPS120Dialog;
//---------------------------------------------------------------------------
#endif

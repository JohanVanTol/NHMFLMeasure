//---------------------------------------------------------------------------
#ifndef IPS20DlgH
#define IPS20DlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TIPS20Dialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *ReadGroupBox;
    TGroupBox *GroupBox2;
    TLabel *CurrentLabel;
    TLabel *Label1;
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
    TEdit *CurrentEdit;
    TEdit *FieldEdit;
    TLabel *SpeedALabel;
    TLabel *Label8;
    TLabel *SpeedBLabel;
    TLabel *Label7;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TEdit *FieldSpeedEdit;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *TargetFieldLabel;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TEdit *CurrentSpeedEdit;
    TButton *Button1;
    TStatusBar *StatusBar;
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActionGroupClick(TObject *Sender);
    void __fastcall CurrentEditExit(TObject *Sender);
    void __fastcall FieldEditExit(TObject *Sender);
    void __fastcall CurrentSpeedEditExit(TObject *Sender);
    void __fastcall FieldSpeedEditExit(TObject *Sender);
private:	// User declarations
    int GetAllData();
    int SetAllData();
public:		// User declarations
    __fastcall TIPS20Dialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TIPS20Dialog *IPS20Dialog;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef BoxcarDlgH
#define BoxcarDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TBoxcarDialog : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *CalcelButton;
    TBitBtn *OkButton;
    TGroupBox *AnalogGroupBox;
    TGroupBox *DigitalGroupBox;
    TEdit *Edit1;
    TEdit *Edit2;
    TEdit *Edit3;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit6;
    TEdit *Edit7;
    TEdit *Edit8;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TComboBox *NoutComboBox;
    TLabel *IOLabel1;
    TLabel *IOLabel2;
    TLabel *IOLabel3;
    TLabel *IOLabel4;
    TLabel *IOLabel5;
    TLabel *IOLabel6;
    TLabel *IOLabel7;
    TLabel *IOLabel8;
    TTimer *Timer1;
    TBitBtn *UpdateButton;
    TBitBtn *ContUpdateButton;
    TLabel *Label29;
    TGroupBox *B1GroupBox;
    TGroupBox *DbyteGroupBox;
    TGroupBox *B2GroupBox;
    TRadioButton *B1INRadioButton;
    TRadioButton *B1OUTRadioButton;
    TButton *B1ToggleButton;
    TLabel *B1ValueLabel;
    TLabel *B2ValueLabel;
    TButton *B2ToggleButton;
    TRadioButton *B2INRadioButton;
    TRadioButton *B2OUTRadioButton;
    TEdit *Edit9;
    TLabel *DBit0Label;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TButton *Button5;
    TButton *Button6;
    TButton *Button7;
    TButton *Button8;
    void __fastcall Edit1Exit(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall OkButtonClick(TObject *Sender);
    void __fastcall CalcelButtonClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall NoutComboBoxChange(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall ContUpdateButtonExit(TObject *Sender);
    void __fastcall UpdateButtonClick(TObject *Sender);
    void __fastcall ContUpdateButtonClick(TObject *Sender);
    void __fastcall Edit2Exit(TObject *Sender);
    void __fastcall Edit3Exit(TObject *Sender);
    void __fastcall Edit4Exit(TObject *Sender);
    void __fastcall Edit5Exit(TObject *Sender);
    void __fastcall Edit6Exit(TObject *Sender);
    void __fastcall Edit7Exit(TObject *Sender);
    void __fastcall Edit8Exit(TObject *Sender);
    
    void __fastcall B1ToggleButtonClick(TObject *Sender);
    void __fastcall B1OUTRadioButtonClick(TObject *Sender);
    void __fastcall B1INRadioButtonClick(TObject *Sender);
    void __fastcall B2OUTRadioButtonClick(TObject *Sender);
    void __fastcall B2INRadioButtonClick(TObject *Sender);
    void __fastcall B2ToggleButtonClick(TObject *Sender);
private:	// User declarations
    int GetAllData();
    int OutputActivate();

public:		// User declarations
    __fastcall TBoxcarDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBoxcarDialog *BoxcarDialog;
//---------------------------------------------------------------------------
#endif

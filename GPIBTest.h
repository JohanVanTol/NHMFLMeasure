//---------------------------------------------------------------------------
#ifndef GPIBTestH
#define GPIBTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TGPIBTestForm : public TForm
{
__published:	// IDE-managed Components
    TEdit *SendCommand;
    TButton *ButtonSend;
    TEdit *AddressEdit;
    TButton *ButtonReceive;
    TGroupBox *DeviceGroupBox;
    TLabel *Label1;
    TComboBox *EotModeComboBox;
    TLabel *Label6;
    TLabel *Label7;
    TComboBox *EosModeComboBox;
    TLabel *Label8;
    TEdit *NBytesToReceiveEdit;
    TMemo *MemoOutput;
    TGroupBox *DeviceStatusGroupBox;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *DevStatusBeforeLabel;
    TLabel *DevStatusAfterLabel;
    TGroupBox *GPIBGroupBox;
    TLabel *LabelCount;
    TLabel *LabelStatus;
    TLabel *LabelStatusBits;
    TLabel *LabelError;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TComboBox *TimeOutComboBox;
    TMemo *StatusMemo;
    TButton *QuitButton;
    TButton *ListenersButton;
    TButton *StatusButton;
    TCheckBox *CRCheckBox;
	TCheckBox *CR_LF_CheckBox;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ButtonSendClick(TObject *Sender);
    void __fastcall ButtonReceiveClick(TObject *Sender);

    void __fastcall TimeOutComboBoxChange(TObject *Sender);
    void __fastcall EotModeComboBoxChange(TObject *Sender);
    void __fastcall EosModeComboBoxChange(TObject *Sender);

    void __fastcall QuitButtonClick(TObject *Sender);
    void __fastcall ListenersButtonClick(TObject *Sender);

    void __fastcall StatusButtonClick(TObject *Sender);
    void __fastcall NBytesToReceiveEditExit(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TGPIBTestForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGPIBTestForm *GPIBTestForm;
//---------------------------------------------------------------------------
#endif

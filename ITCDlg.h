//---------------------------------------------------------------------------
#ifndef ITCDlgH
#define ITCDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TITCDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *TempGroupBox;
    TGroupBox *PIDGroupBox;
    TGroupBox *HeaterGroupBox;
    TGroupBox *FlowGroupBox;
    TButton *OKButton;
    TButton *CancelButton;
    TButton *SendButton;
    TButton *ReadButton;
    TEdit *SetTempEdit;
    TRadioButton *AutoPIDButton;
    TRadioButton *ManualPIDButton;
    TEdit *PEdit;
    TEdit *IEdit;
    TEdit *DEdit;
    TLabel *PLabel;
    TLabel *ILabel;
    TLabel *DLabel;
    TLabel *Label1;
    TLabel *Label2;
    TRadioButton *AutoHeaterButton;
    TRadioButton *ManualHeaterButton;
    TEdit *HeaterEdit;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TComboBox *ChannelComboBox;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TRadioButton *AutoFlowButton;
    TRadioButton *ManualFlowButton;
    TEdit *FlowEdit;
    TLabel *Label13;
    TLabel *Channel1Label;
    TLabel *Channel2Label;
    TLabel *Channel3Label;
    TLabel *HeaterLabel;
    TLabel *Label15;
    TTimer *TempTimer;
    TButton *ContinUpdateButton;
    void __fastcall SendButtonClick(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall OKButtonClick(TObject *Sender);
    void __fastcall ReadButtonClick(TObject *Sender);
        void __fastcall ManualHeaterButtonClick(TObject *Sender);
        void __fastcall AutoHeaterButtonClick(TObject *Sender);
        void __fastcall AutoFlowButtonClick(TObject *Sender);
        void __fastcall ManualFlowButtonClick(TObject *Sender);
        void __fastcall AutoPIDButtonClick(TObject *Sender);
        void __fastcall ManualPIDButtonClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall TempTimerTimer(TObject *Sender);
    void __fastcall ContinUpdateButtonClick(TObject *Sender);
    
    
private:	// User declarations
    int GetAllData();
    int SetAllData();
public:		// User declarations
    __fastcall TITCDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TITCDialog *ITCDialog;
//---------------------------------------------------------------------------
#endif

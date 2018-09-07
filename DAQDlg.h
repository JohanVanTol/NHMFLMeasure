//---------------------------------------------------------------------------

#ifndef DAQDlgH
#define DAQDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DaqX.h"
#include "DaqBoard.h"
//---------------------------------------------------------------------------
class TDAQ_Dialog : public TForm
{
__published:	// IDE-managed Components
    TButton *Button1;
    TGroupBox *ADCGroupBox;
    TCheckBox *CheckBoxCh0;
    TCheckBox *CheckBoxCh1;
    TCheckBox *CheckBoxCh2;
    TCheckBox *CheckBoxCh3;
    TCheckBox *CheckBoxCh4;
    TCheckBox *CheckBoxCh5;
    TCheckBox *CheckBoxCh6;
    TCheckBox *CheckBoxCh7;
    TComboBox *GainComboBoxCh0;
    TComboBox *GainComboBoxCh1;
    TComboBox *GainComboBoxCh2;
    TComboBox *GainComboBoxCh3;
    TComboBox *GainComboBoxCh4;
    TComboBox *GainComboBoxCh5;
    TComboBox *GainComboBoxCh6;
    TComboBox *GainComboBoxCh7;
    TLabel *LabelCh0;
    TLabel *LabelCh1;
    TLabel *LabelCh2;
    TLabel *LabelCh3;
    TLabel *LabelCh4;
    TLabel *LabelCh5;
    TLabel *LabelCh6;
    TLabel *LabelCh7;
    TGroupBox *IOGroupBox;
    TButton *IO_ExecuteButton;
    TLabel *Label9;
    TComboBox *IOComboBoxChA1;
    TComboBox *IOComboBoxChA2;
    TLabel *Label10;
    TLabel *Label11;
    TComboBox *IOComboBoxChB1;
    TComboBox *IOComboBoxChB2;
    TComboBox *IOComboBoxChC2;
    TComboBox *IOComboBoxChC1;
    TButton *UpdateADCButton;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall UpdateADCButtonClick(TObject *Sender);
    void __fastcall IO_ExecuteButtonClick(TObject *Sender);
private:	// User declarations
        DaqHandleT  handle;
        IOTechDaqBoard Daq;
public:		// User declarations
        __fastcall TDAQ_Dialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDAQ_Dialog *DAQ_Dialog;
//---------------------------------------------------------------------------
#endif

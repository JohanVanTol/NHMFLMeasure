//---------------------------------------------------------------------------

#ifndef DACSweepDlgH
#define DACSweepDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TDacSweepDialog : public TForm
{
__published:	// IDE-managed Components
	TComboBox *NptsComboBox;
	TComboBox *RangeComboBox;
	TButton *Button1;
	TButton *Button2;
	TComboBox *DelayComboBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TCheckBox *ZeroCheckBox;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TDacSweepDialog(TComponent* Owner);
	int StepSize;
	int StartValue;
	int NptsSweep;
};
//---------------------------------------------------------------------------
extern PACKAGE TDacSweepDialog *DacSweepDialog;
//---------------------------------------------------------------------------
#endif

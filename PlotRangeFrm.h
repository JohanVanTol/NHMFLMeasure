//---------------------------------------------------------------------------
#ifndef PlotRangeFrmH
#define PlotRangeFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TPlotRangeDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *Edit1;
    TEdit *Edit2;
    TEdit *Edit3;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit6;
    TCheckBox *CheckBox1;
    TCheckBox *CheckBox2;
    TCheckBox *CheckBox3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
private:	// User declarations
public:		// User declarations
    __fastcall TPlotRangeDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlotRangeDialog *PlotRangeDialog;
//---------------------------------------------------------------------------
#endif

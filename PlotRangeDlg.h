//---------------------------------------------------------------------------
#ifndef PlotRangeDlgH
#define PlotRangeDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "mydata.h"
#include "myplot20.h"
//---------------------------------------------------------------------------
class TPlotRangeDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *AxesRangeGroupBox;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *XminEdit;
    TEdit *Y1minEdit;
    TEdit *Y2minEdit;
    TEdit *XmaxEdit;
    TEdit *Y1maxEdit;
    TEdit *Y2maxEdit;
    TCheckBox *AutoXCheckBox;
    TCheckBox *AutoY1CheckBox;
    TCheckBox *AutoY2CheckBox;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TBitBtn *OKButton;
    TBitBtn *CancelButton;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall OKButtonClick(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall XminEditChange(TObject *Sender);
    void __fastcall XmaxEditChange(TObject *Sender);
    void __fastcall Y1minEditChange(TObject *Sender);
    void __fastcall Y1maxEditChange(TObject *Sender);
    void __fastcall Y2minEditChange(TObject *Sender);
    void __fastcall Y2maxEditChange(TObject *Sender);
private:	// User declarations
    int SetPlotRange();
    int GetPlotRange();
    TDataFile *TempFile;
public:		// User declarations
    void SetDataFile(TDataFile* File) {TempFile = File;}
    __fastcall TPlotRangeDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlotRangeDialog *PlotRangeDialog;
//---------------------------------------------------------------------------
#endif

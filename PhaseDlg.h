//---------------------------------------------------------------------------

#ifndef PhaseDlgH
#define PhaseDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include "mydata.h"
//---------------------------------------------------------------------------
class TRePhaseDialog : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TEdit *ModPhase1Edit;
    TLabel *Label2;
    TEdit *WavePhaseEdit;
	TBitBtn *CancelButton;
    TBitBtn *OkButton;
    TUpDown *UpDown1;
    TUpDown *UpDown2;
    TLabel *Label3;
    TEdit *ModPhase2Edit;
    TUpDown *UpDown3;
	TCheckBox *AutoPhaseCheckBox;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ModPhase1EditChange(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall OkButtonClick(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall ModPhase2EditChange(TObject *Sender);
    void __fastcall WavePhaseEditChange(TObject *Sender);
private:	// User declarations
    TDataFile *TempFile;
    DataArray *OldData;
	void CalcNewCurves();
	double LIphase1;
	double LIphase2;
	double MWphase;
public:		// User declarations
	__fastcall TRePhaseDialog(TComponent* Owner);
	void SetDataFile(TDataFile* File) {TempFile = File;}
	double GetPh1() {return LIphase1;}
	double GetPh2() {return LIphase2;}
	double GetPh3() {return MWphase;}

};
//---------------------------------------------------------------------------
extern PACKAGE TRePhaseDialog *RePhaseDialog;
//---------------------------------------------------------------------------
#endif

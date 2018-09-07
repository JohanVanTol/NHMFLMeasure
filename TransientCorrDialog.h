//---------------------------------------------------------------------------

#ifndef TransientCorrDialogH
#define TransientCorrDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "mydata.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TTransientCorrDlg : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TLabel *Label1;
    TGroupBox *GroupBox2;
    TEdit *ColumnEdit;
    TUpDown *UpDown1;
    TBitBtn *ApplyButton;
    TLabel *Label2;
    TEdit *PhaseEdit;
    TCheckBox *OldTypeCheckBox;
    TUpDown *UpDown2;
    TLabel *Label3;
    TLabel *NrColumnsLabel;
    TCheckBox *ErrorCheckBox;
    TCheckBox *EndSubstractCheckBox;
    TCheckBox *SkipFirstCheckBox;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ApplyButtonClick(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
    TDataFile *TempFile;
    DataArray *OldData;
    void CalcNewCurves();
public:		// User declarations
      void SetDataFile(TDataFile* File) {TempFile = File;}
    __fastcall TTransientCorrDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTransientCorrDlg *TransientCorrDlg;
//---------------------------------------------------------------------------
#endif

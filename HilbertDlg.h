//---------------------------------------------------------------------------
#ifndef HilbertDlgH
#define HilbertDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class THilbertDialog : public TForm
{
__published:	// IDE-managed Components
    TButton *AddColumnsButton;
    TLabel *ColLabel;
    TButton *GoButton;
    TButton *CloseButton;
    TLabel *DatLabel;
    TComboBox *DataComboBox;
    TLabel *Label1;
    TComboBox *ImageComboBox;
    TLabel *Label2;
    TComboBox *ResultComboBox;
    TLabel *Label3;
    TEdit *AngleEdit;
    TUpDown *UpDown1;
    TLabel *Label4;
    TLabel *IntegralLabel;
    TLabel *Lable5;
    void __fastcall CloseButtonClick(TObject *Sender);
    void __fastcall GoButtonClick(TObject *Sender);
    void __fastcall AddColumnsButtonClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall AngleEditChange(TObject *Sender);
    void __fastcall DataComboBoxChange(TObject *Sender);
    void __fastcall ImageComboBoxChange(TObject *Sender);
    void __fastcall ResultComboBoxChange(TObject *Sender);
private:	// User declarations
    int ResetComboBoxes(int ncol);
public:		// User declarations
    __fastcall THilbertDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE THilbertDialog *HilbertDialog;
//---------------------------------------------------------------------------
#endif

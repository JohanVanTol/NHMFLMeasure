//---------------------------------------------------------------------------
#ifndef HeaterOffChkDlgH
#define HeaterOffChkDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class THeaterOffCheckForm : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *AbortButton;
    TBitBtn *OKButton;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *PersistentFieldLabel;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *SweepLabel;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    void __fastcall OKButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall THeaterOffCheckForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE THeaterOffCheckForm *HeaterOffCheckForm;
//---------------------------------------------------------------------------
#endif

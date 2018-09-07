//---------------------------------------------------------------------------
#ifndef IB_IPS20DlgH
#define IB_IPS20DlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *ReadGroupBox;
    TGroupBox *GroupBox2;
    TLabel *CurrentLabel;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TGroupBox *StateGroupBox;
    TRadioGroup *ActionGroup;
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

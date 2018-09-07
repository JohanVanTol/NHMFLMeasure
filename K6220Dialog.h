//---------------------------------------------------------------------------

#ifndef K6220DialogH
#define K6220DialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>

//---------------------------------------------------------------------------
class TK6220Form : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TGroupBox *CurrentGroupBox;
	TGroupBox *ComplianceGroupBox;
	TBitBtn *ApplyBitBtn;
	TBitBtn *BitBtn2;
	TLabel *CurrentLabel;
	TLabel *CompLabel;
	TEdit *CurrentEdit;
	TEdit *CompEdit;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ApplyBitBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TK6220Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TK6220Form *K6220Form;
//---------------------------------------------------------------------------
#endif

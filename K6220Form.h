//---------------------------------------------------------------------------

#ifndef K6220FormH
#define K6220FormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TGroupBox *CurrentGroupBox;
	TGroupBox *ComplianceGroupBox;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *CurrLabel;
	TLabel *CompLabel;
	TEdit *CurrentEdit;
	TEdit *CompEdit;
	TLabel *Label1;
	TLabel *Label2;
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

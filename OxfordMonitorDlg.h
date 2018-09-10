//---------------------------------------------------------------------------

#ifndef OxfordMonitorDlgH
#define OxfordMonitorDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *FieldLabel;
	TLabel *TemperatureLabel;
	TLabel *HeLevelLabel;
	TLabel *N2LevelLabel;
	TBitBtn *CloseButton;
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef SaveOptionsDlgH
#define SaveOptionsDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSaveOptionsDialog : public TForm
{
__published:	// IDE-managed Components
        TRadioGroup *SaveOptionsRadioGroup;
        TBitBtn *BitBtn1;
private:	// User declarations
public:		// User declarations
        __fastcall TSaveOptionsDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSaveOptionsDialog *SaveOptionsDialog;
//---------------------------------------------------------------------------
#endif

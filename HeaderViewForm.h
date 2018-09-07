//---------------------------------------------------------------------------

#ifndef HeaderViewFormH
#define HeaderViewFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TParamViewForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *HeaderMemo;
    TBitBtn *BitBtn1;
    void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TParamViewForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TParamViewForm *ParamViewForm;
//---------------------------------------------------------------------------
#endif

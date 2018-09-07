//---------------------------------------------------------------------------

#ifndef UserFormDlgH
#define UserFormDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TUserForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *UserInfoGroupBox;
    TEdit *UserNameEdit;
    TMaskEdit *ExperimentMaskEdit;
    TBitBtn *OKButton;
    TLabel *Label1;
    TLabel *Label2;
	TLabel *Label3;
    void __fastcall OKButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TUserForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TUserForm *UserForm;
//---------------------------------------------------------------------------
#endif

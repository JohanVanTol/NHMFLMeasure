//---------------------------------------------------------------------------
#ifndef ConfirmDeleteFrmH
#define ConfirmDeleteFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TDeleteConfirmDialog : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *YesButton;
    TBitBtn *NoButton;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    void __fastcall NoButtonClick(TObject *Sender);
    void __fastcall YesButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TDeleteConfirmDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDeleteConfirmDialog *DeleteConfirmDialog;
//---------------------------------------------------------------------------
#endif

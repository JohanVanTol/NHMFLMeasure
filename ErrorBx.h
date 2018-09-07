//----------------------------------------------------------------------------
#ifndef ErrorBxH
#define ErrorBxH
//----------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TErrorBox : public TForm
{
__published:
	TPanel *Panel1;
	TImage *ProgramIcon;
    TLabel *ErrorMessage;
	TButton *OKButton;
    void __fastcall OKButtonClick(TObject *Sender);
private:
public:
	virtual __fastcall TErrorBox(TComponent* AOwner);
	void ShowMessage(const char*);
};
//----------------------------------------------------------------------------
extern PACKAGE TErrorBox *ErrorBox;
//----------------------------------------------------------------------------
#endif

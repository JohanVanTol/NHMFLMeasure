//----------------------------------------------------------------------------
#ifndef AboutBoxH
#define AboutBoxH
//----------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <StdCtrls.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
//----------------------------------------------------------------------------
class TEPRAboutBox : public TForm
{
__published:
	TPanel *Panel1;
	TButton *OKButton;
	TImage *ProgramIcon;
	TLabel *ProductName;
	TLabel *Version;
	TLabel *Copyright;
private:
public:
	virtual __fastcall TEPRAboutBox(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TEPRAboutBox *EPRAboutBox;
//----------------------------------------------------------------------------
#endif

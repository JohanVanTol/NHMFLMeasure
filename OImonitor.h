//---------------------------------------------------------------------------

#ifndef OImonitorH
#define OImonitorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------


class TOImonitorForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *FieldLabel;
	TLabel *TemperatureLabel;
	TLabel *HeLabel;
	TLabel *N2Label;
	TTimer *MonitorTimer;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	void __fastcall MonitorTimerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDeactivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TOImonitorForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOImonitorForm *OImonitorForm;
//---------------------------------------------------------------------------
#endif

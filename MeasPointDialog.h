//---------------------------------------------------------------------------
#ifndef MeasPointDialogH
#define MeasPointDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMeasPointFrm : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *AverageLabel;
	TLabel *XaxisLabel;
    TLabel *XLabel;
    TLabel *Label2;
    TLabel *PointLabel;
    TLabel *StatusLabel;
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TMeasPointFrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMeasPointFrm *MeasPointFrm;
//---------------------------------------------------------------------------
#endif

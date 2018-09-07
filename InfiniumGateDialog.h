//---------------------------------------------------------------------------
#ifndef InfiniumGateDialogH
#define InfiniumGateDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TInfiniumGatesDlg : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
	TBitBtn *OkBtn;
    TGroupBox *GatesGroupBox;
        TGroupBox *GroupBox2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label10;
        TLabel *Label11;
    TEdit *Gate1T1Edit;
    TEdit *Gate2T1Edit;
    TEdit *Gate3T1Edit;
    TEdit *Gate2T2Edit;
    TEdit *Gate1T2Edit;
    TEdit *Gate4T1Edit;
    TEdit *Gate3T2Edit;
    TEdit *Gate4T2Edit;
    TEdit *Gate5T1Edit;
    TEdit *Gate6T1Edit;
    TEdit *Gate9T2Edit;
    TEdit *Gate8T2Edit;
    TEdit *Gate7T2Edit;
    TEdit *Gate6T2Edit;
    TEdit *Gate5T2Edit;
    TEdit *Gate7T1Edit;
    TEdit *Gate8T1Edit;
    TEdit *Gate9T1Edit;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
    TEdit *NptsEdit;
        TLabel *Label29;
    TEdit *AverageEdit;
        TLabel *Label30;
    TCheckBox *LowPassCheckBox;
        TLabel *Label31;
    TEdit *NGatesEdit;
    TEdit *TriggerEdit;
        TLabel *Label32;
        TLabel *Label33;
        TComboBox *TimePerPointComboBox;
    TGroupBox *RegularGroupBox;
    TLabel *Label35;
    TEdit *DurationEdit;
    TLabel *Label36;
    TLabel *Label37;
    TEdit *StartEdit;
    TLabel *Label38;
    TLabel *Label34;
    TLabel *Label39;
    TCheckBox *RegularCheckBox;
    TGroupBox *GroupBox1;
    TCheckBox *CalCheckBox;
    void __fastcall NGatesEditChange(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall AverageEditChange(TObject *Sender);
    void __fastcall OkBtnClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall RegularCheckBoxClick(TObject *Sender);
    void __fastcall DurationEditExit(TObject *Sender);
    void __fastcall StartEditExit(TObject *Sender);
private:	// User declarations
        int Averages;
        int nGates;
        double GateStart;
        double GateWidth;
public:		// User declarations
        int GetAverages() const {return Averages;}
        int GetnGates() const {return nGates;}
        int SetAverages(int nAver) {Averages = nAver; return 0;}
        int SetnGates(int nGat) {nGates = nGat; return 0;}
        int SetGate(int i, double Start, double Stop);
        int GetGate(int i, double *Start, double *Stop);
        int ReEnableGates();
        int SetStart();
        int SetWidth();
        double GetStart() const {return GateStart;}
        double GetWidth() const {return GateWidth;}
        __fastcall TInfiniumGatesDlg(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TInfiniumGatesDlg *InfiniumGatesDlg;
//---------------------------------------------------------------------------
#endif

//----------------------------------------------------------------------------
#ifndef MeasmainH
#define MeasmainH
//----------------------------------------------------------------------------
#include "ChildWin.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Messages.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <System.hpp>

//----------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:
    TMainMenu *MainMenu;
	TMenuItem *File1;
	TMenuItem *FileNewItem;
	TMenuItem *FileOpenItem;
	TMenuItem *FileCloseItem;
	TMenuItem *Window1;
	TMenuItem *Help1;
	TMenuItem *N1;
	TMenuItem *FileExitItem;
	TMenuItem *WindowCascadeItem;
	TMenuItem *WindowTileItem;
	TMenuItem *WindowArrangeItem;
	TMenuItem *HelpAboutItem;
	TOpenDialog *OpenDialog;
	TMenuItem *FileSaveItem;
	TMenuItem *FileSaveAsItem;
	TMenuItem *WindowMinimizeItem;
	TPanel *SpeedPanel;
	TSpeedButton *OpenBtn;
	TSpeedButton *SaveBtn;
	TStatusBar *StatusBar;
    TSpeedButton *NewBtn;
    TMenuItem *MeasureMenu;
    TMenuItem *CWEPRMenuItem;
    TMenuItem *CWENDOR;
    TMenuItem *TransientEPRMenuItem;
    TSaveDialog *SaveDialog;
    TBevel *Bevel1;
    TMenuItem *PlotMenu;
    TMenuItem *ControlMenu;
    TMenuItem *MagnetMenuItem;
    TMenuItem *GPIBtestMenuItem;
    TMenuItem *N2;
    TMenuItem *RFSourceMenuItem;
    TMenuItem *RFampMenuItem;
    TMenuItem *BoxcarMenuItem;
    TMenuItem *Rs232TestMenuItem;
    TMenuItem *ITCMenuItem;
    TMenuItem *ILMMenuItem;
    TMenuItem *IPS20MenuItem;
    TMenuItem *DMM1MenuItem;
    TMenuItem *LockIn1MenuItem;
    TMenuItem *N3;
    TMenuItem *InfiniumMenuItem;
    TSpeedButton *StartMeasureButton;
    TSpeedButton *PauzeMeasureButton;
    TSpeedButton *StopMeasureButton;
    TTimer *MeasureENDORTimer;
    TMenuItem *AutoX1;
    TMenuItem *AutoY11;
    TMenuItem *AutoY21;
    TSpeedButton *AutoXButton;
    TSpeedButton *AutoY1Button;
    TSpeedButton *AutoY2Button;
    TMenuItem *RangePlotMenuItem;
    TMenuItem *N4;
    TMenuItem *RePlotMenuItem;
    TTimer *GoToStartFieldTimer;
    TTimer *EPRMeasureTimer;
    TTimer *GoToSweepStartTimer;
    TTimer *EPRSweepMeasureTimer;
        TMenuItem *TransientEPR2MenuItem;
    TMenuItem *AnalysisMenu;
    TMenuItem *HilbertMenuItem;
    TMenuItem *HelpContentsMenuItem;
    TMenuItem *N5;
    TTimer *MeasPointTimer;
    TMenuItem *LockIn2MenuItem;
    TMenuItem *N6;
    TMenuItem *HeteroEPRMenuItem;
    TMenuItem *SetCustomMenuItem;
    TMenuItem *HetTransientMenuItem;
    TMenuItem *AnalyserReadMenuItem;
    TMenuItem *N7;
    TMenuItem *PrintMenuItem;
    TPrintDialog *PrintDialog;
    TMenuItem *N8;
    TMenuItem *DataCurves1;
    TMenuItem *RePhase1;
    TMenuItem *N9;
    TMenuItem *PlotHeaderViewMenuItem;
    TMenuItem *TransientCorrectItem;
    TMenuItem *N10;
    TMenuItem *ViewParamItem;
    TMenuItem *N6GHzSynthesizer1;
        TMenuItem *N11;
        TMenuItem *ViewEditLogFile1;
    TMenuItem *MeasureMenuFullItem;
        TMenuItem *DaQtest1;
    TMenuItem *FreqSetMenuItem;
    TMenuItem *Pulsed1;
	TTimer *MeasurePulsTimer;
	TMenuItem *SweepMenuItem;
	TTimer *DacSweepTimer;
	TTimer *PulsedEndorTimer;
	TMenuItem *ek7104Read1;
	TMenuItem *TimeSweep;
	TMenuItem *RFSource21;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TMenuItem *ReconnectAll1;
	TMenuItem *CurrentSourceMenuItem;
	TMenuItem *OIMonitor;
	TSpeedButton *RepeatLastSpeedButton;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FileNewItemClick(TObject *Sender);
	void __fastcall WindowCascadeItemClick(TObject *Sender);
	void __fastcall UpdateMenuItems(TObject *Sender);
	void __fastcall WindowTileItemClick(TObject *Sender);
	void __fastcall WindowArrangeItemClick(TObject *Sender);
	void __fastcall FileCloseItemClick(TObject *Sender);
	void __fastcall FileOpenItemClick(TObject *Sender);
	void __fastcall FileExitItemClick(TObject *Sender);
	void __fastcall FileSaveItemClick(TObject *Sender);
	void __fastcall FileSaveAsItemClick(TObject *Sender);
	void __fastcall CutItemClick(TObject *Sender);
	void __fastcall CopyItemClick(TObject *Sender);
	void __fastcall PasteItemClick(TObject *Sender);
	void __fastcall WindowMinimizeItemClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);

	void __fastcall HelpAboutItemClick(TObject *Sender);

    void __fastcall NewBtnClick(TObject *Sender);
    void __fastcall CWEPRMenuItemClick(TObject *Sender);
    void __fastcall GPIBtestMenuItemClick(TObject *Sender);
    void __fastcall RFSourceMenuItemClick(TObject *Sender);

    void __fastcall RFampMenuItemClick(TObject *Sender);
    void __fastcall BoxcarMenuItemClick(TObject *Sender);
    void __fastcall Rs232TestMenuItemClick(TObject *Sender);
    void __fastcall ITCMenuItemClick(TObject *Sender);
    void __fastcall ILMMenuItemClick(TObject *Sender);
    void __fastcall IPS20MenuItemClick(TObject *Sender);
    void __fastcall MagnetMenuItemClick(TObject *Sender);
    void __fastcall DMM1MenuItemClick(TObject *Sender);
    void __fastcall LockIn1MenuItemClick(TObject *Sender);
    void __fastcall InfiniumMenuItemClick(TObject *Sender);
    void __fastcall CWENDORClick(TObject *Sender);
    void __fastcall MeasureENDORTimerTimer(TObject *Sender);
    void __fastcall AutoX1Click(TObject *Sender);
    void __fastcall AutoY11Click(TObject *Sender);
    void __fastcall AutoY21Click(TObject *Sender);
    void __fastcall StartMeasureButtonClick(TObject *Sender);
    void __fastcall PauzeMeasureButtonClick(TObject *Sender);
    void __fastcall StopMeasureButtonClick(TObject *Sender);
    void __fastcall RePlotMenuItemClick(TObject *Sender);
    void __fastcall GoToStartFieldTimerTimer(TObject *Sender);
    void __fastcall EPRMeasureTimerTimer(TObject *Sender);
    void __fastcall GoToSweepStartTimerTimer(TObject *Sender);
    void __fastcall EPRSweepMeasureTimerTimer(TObject *Sender);
    void __fastcall RangePlotMenuItemClick(TObject *Sender);
    void __fastcall TransientEPRMenuItemClick(TObject *Sender);
        void __fastcall TransientEPR2MenuItemClick(TObject *Sender);
    void __fastcall HilbertMenuItemClick(TObject *Sender);
    void __fastcall HelpContentsMenuItemClick(TObject *Sender);
    void __fastcall LockIn2MenuItemClick(TObject *Sender);
    void __fastcall HeteroEPRMenuItemClick(TObject *Sender);
    void __fastcall HetTransientMenuItemClick(TObject *Sender);
    void __fastcall AnalyserReadMenuItemClick(TObject *Sender);
    void __fastcall PrintMenuItemClick(TObject *Sender);
    void __fastcall DataCurves1Click(TObject *Sender);
    void __fastcall RePhase1Click(TObject *Sender);
    void __fastcall TransientCorrectItemClick(TObject *Sender);
    void __fastcall ViewParamItemClick(TObject *Sender);
    void __fastcall N6GHzSynthesizer1Click(TObject *Sender);
        void __fastcall ViewEditLogFile1Click(TObject *Sender);
    void __fastcall MeasureMenuFullItemClick(TObject *Sender);
        void __fastcall DaQtest1Click(TObject *Sender);
	void __fastcall Pulsed1Click(TObject *Sender);
	void __fastcall MeasurePulsTimerTimer(TObject *Sender);
	void __fastcall SweepMenuItemClick(TObject *Sender);
	void __fastcall DacSweepTimerTimer(TObject *Sender);
	void __fastcall PulsedEndorTimerTimer(TObject *Sender);
	void __fastcall ek7104Read1Click(TObject *Sender);
	void __fastcall TimeSweepClick(TObject *Sender);
	void __fastcall MeasPointTimerTimer(TObject *Sender);
	void __fastcall RFSource21Click(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall ReconnectAll1Click(TObject *Sender);
	void __fastcall CurrentSourceMenuItemClick(TObject *Sender);
	void __fastcall OIMonitorClick(TObject *Sender);

private:
	void __fastcall CreateDataFile(const String Name);
	void __fastcall ShowHint(TObject *Sender);
public:
	virtual __fastcall TMainForm(TComponent *Owner);
//  My functions
    int InitializeRemoteDevices();
    int AddInstrumentHeader();
    int SetSynthesizer(double Freq);
	void PrepareEPRMeasurement();
	void PulsedPrepare();
	void PulsedEndorPrepare();
	void PulsedSweepPrepare(); 
};
//----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TDataFile *__fastcall MDIChildCreate(void);
//----------------------------------------------------------------------------
#endif

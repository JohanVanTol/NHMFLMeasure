//----------------------------------------------------------------------------
#ifndef MainH
#define MainH
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
	TMenuItem *Edit1;
	TMenuItem *CutItem;
	TMenuItem *CopyItem;
	TMenuItem *PasteItem;
	TMenuItem *WindowMinimizeItem;
	TPanel *SpeedPanel;
	TSpeedButton *OpenBtn;
	TSpeedButton *SaveBtn;
	TSpeedButton *CutBtn;
	TSpeedButton *CopyBtn;
	TSpeedButton *PasteBtn;
	TStatusBar *StatusBar;
    TSpeedButton *NewBtn;
    TMenuItem *MeasureMenu;
    TMenuItem *CWEPR;
    TMenuItem *CWENDOR;
    TMenuItem *TransientEPR;
    TSaveDialog *SaveDialog;
    TBevel *Bevel1;
    TMenuItem *Plot;
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
    void __fastcall CWEPRClick(TObject *Sender);
private:
	void __fastcall CreateMDIChild(const String Name);
	void __fastcall ShowHint(TObject *Sender);
public:
	virtual __fastcall TMainForm(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);
//----------------------------------------------------------------------------
#endif

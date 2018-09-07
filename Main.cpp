//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "About.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TMainForm *MainForm;
//--------------------------------------------------------------------- 
__fastcall TMainForm::TMainForm(TComponent *Owner)
	: TForm(Owner)
{
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	Application->OnHint = ShowHint;
	Screen->OnActiveFormChange = UpdateMenuItems;
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::ShowHint(TObject *Sender)
{
	StatusBar->SimpleText = Application->Hint;
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::CreateMDIChild(String Name)
{
	TMDIChild *Child;

	//--- create a new MDI child window ----
	Child = new TMDIChild(Application);
	Child->Caption = Name;
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::FileNewItemClick(TObject *Sender)
{
	CreateMDIChild("NONAME" + IntToStr(MDIChildCount + 1));
    MeasureMenu->Enabled = TRUE;
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::FileOpenItemClick(TObject *Sender)
{
	if (OpenDialog->Execute())
		CreateMDIChild(OpenDialog->FileName);
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::FileCloseItemClick(TObject *Sender)
{
	if (ActiveMDIChild)
		ActiveMDIChild->Close();
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::FileSaveItemClick(TObject *Sender)
{
	//---- save current file (ActiveMDIChild points to the window) ----
    if (SaveDialog->Execute())
    {};

    //---- save current file (ActiveMDIChild points to the window) ----
}
//---------------------------------------------------------------------
void __fastcall TMainForm::FileSaveAsItemClick(TObject *Sender)
{
	//---- save current file under new name ----
    SaveDialog->Execute();
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::FileExitItemClick(TObject *Sender)
{
	Close();
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::CutItemClick(TObject *Sender)
{
	//---- cut selection to clipboard ----
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::CopyItemClick(TObject *Sender)
{
	//---- copy selection to clipboard ----
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::PasteItemClick(TObject *Sender)
{
	//---- paste from clipboard ----
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::WindowCascadeItemClick(TObject *Sender)
{
	Cascade();
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::WindowTileItemClick(TObject *Sender)
{
	Tile();
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::WindowArrangeItemClick(TObject *Sender)
{
	ArrangeIcons();
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::WindowMinimizeItemClick(TObject *Sender)
{
	int i;

	//---- Must be done backwards through the MDIChildren array ----
	for (i=MDIChildCount-1; i >= 0; i--)
		MDIChildren[i]->WindowState = wsMinimized;
}
//--------------------------------------------------------------------- 
void __fastcall TMainForm::UpdateMenuItems(TObject *Sender)
{
	FileCloseItem->Enabled = MDIChildCount > 0;
	FileSaveItem->Enabled = MDIChildCount > 0;
	FileSaveAsItem->Enabled = MDIChildCount > 0;
	CutItem->Enabled = MDIChildCount > 0;
	CopyItem->Enabled = MDIChildCount > 0;
	PasteItem->Enabled = MDIChildCount > 0;
	SaveBtn->Enabled = MDIChildCount > 0;
	CutBtn->Enabled = MDIChildCount > 0;
	CopyBtn->Enabled = MDIChildCount > 0;
	PasteBtn->Enabled = MDIChildCount > 0;
	WindowCascadeItem->Enabled = MDIChildCount > 0;
	WindowTileItem->Enabled = MDIChildCount > 0;
	WindowArrangeItem->Enabled = MDIChildCount > 0;
	WindowMinimizeItem->Enabled = MDIChildCount > 0;
}
//---------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	Screen->OnActiveFormChange = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::HelpAboutItemClick(TObject *Sender)
{
    AboutBox->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::NewBtnClick(TObject *Sender)
{
    FileNewItemClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CWEPRClick(TObject *Sender)
{
    CurrMeasType = 1;
    CWEPRloop(MeasType);

}
//---------------------------------------------------------------------------


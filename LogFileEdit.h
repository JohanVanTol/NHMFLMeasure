//---------------------------------------------------------------------------

#ifndef LogFileEditH
#define LogFileEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <StdActns.hpp>
#include <Dialogs.hpp>
#include <ToolWin.hpp>
#include <SysUtils.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class TLogFileForm : public TForm
{
__published:	// IDE-managed Components
        TRichEdit *RichEdit1;
        TStatusBar *StatusBar1;
        TImageList *ImageList1;
        TActionList *ActionList1;
        TAction *HelpContents;
        TAction *HelpIndex;
        TEditCopy *EditCopy1;
        TEditCut *EditCut1;
        TEditPaste *EditPaste1;
    TMainMenu *LogFormMainMenu;
        TMenuItem *File1;
    TMenuItem *SaveCopyMenuItem;
    TMenuItem *CloseMenuItem;
        TMenuItem *Edit1;
        TMenuItem *Cut1;
        TMenuItem *Copy1;
        TMenuItem *Paste1;
        TMenuItem *Help1;
        TMenuItem *Contents1;
        TMenuItem *Index1;
        TSaveDialog *SaveDialog1;
    TFontDialog *FontDialog1;
    TColorDialog *ColorDialog1;
    TToolBar *ToolBar1;
    TWindowClose *WindowClose1;
    TWindowClose *WindowClose2;
    TAction *CloseWindow;
    TFileSaveAs *SaveCopyAs;
    TSpeedButton *SpeedButton1;
        void __fastcall FileNewExecute(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
    void __fastcall CloseMenuItemClick(TObject *Sender);
    void __fastcall SaveCopyMenuItemClick(TObject *Sender);
    void __fastcall CloseWindowExecute(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        AnsiString LogName;
        AnsiString LogDirectory;
        void WriteInitData();
		void AddFileHeader(char* Header);
		void AddComment(char* Comment);

        __fastcall TLogFileForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLogFileForm *LogFileForm;
//---------------------------------------------------------------------------
#endif

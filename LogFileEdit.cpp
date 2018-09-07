//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LogFileEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern AnsiString UserName;
extern AnsiString ExperimentID;
TLogFileForm *LogFileForm;
//---------------------------------------------------------------------------
__fastcall TLogFileForm::TLogFileForm(TComponent* Owner)
        : TForm(Owner)
{
    LogDirectory = "C:\\LogFiles\\";
}
//---------------------------------------------------------------------------
void __fastcall TLogFileForm::FileNewExecute(TObject *Sender)
{
        if (LogName.c_str() != "No")
        {
                RichEdit1->Lines->SaveToFile(LogDirectory + LogName);
        }
        if (SaveDialog1->Execute())
        {
                LogName = SaveDialog1->FileName;
                WriteInitData();
                RichEdit1->Lines->SaveToFile(LogDirectory + LogName);
        }
        return;
}
//---------------------------------------------------------------------------
void TLogFileForm::WriteInitData()
{
    AnsiString Line;
    Line = "User(s): " + UserName + ", ExperimentID: " + ExperimentID +
        "  date: " + DateToStr(Date());

    RichEdit1->Lines->Add(LogName);
    RichEdit1->Lines->Add(Line);

    RichEdit1->Lines->SaveToFile(LogDirectory + LogName);
}

void TLogFileForm::AddFileHeader(char* Header)
{
		RichEdit1->Lines->Add(Header);
		RichEdit1->Lines->SaveToFile(LogDirectory + LogName);
}

void TLogFileForm::AddComment(char* Comment)
{
		AddFileHeader(Comment);
}

void __fastcall TLogFileForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        RichEdit1->Lines->SaveToFile(LogDirectory + LogName);
}
//---------------------------------------------------------------------------

void __fastcall TLogFileForm::FormDestroy(TObject *Sender)
{
        RichEdit1->Lines->SaveToFile(LogDirectory + LogName);
}
//---------------------------------------------------------------------------



void __fastcall TLogFileForm::FormCreate(TObject *Sender)
{
  unsigned short Year, Month, Day, Hour, Minute, Second, Millis;
  TDateTime dtPresent = Now();
  DecodeDate(dtPresent, Year, Month, Day);
  DecodeTime(dtPresent, Hour, Minute, Second, Millis);
  LogName = "NHMFL" + AnsiString("_") + IntToStr(Year) +
        AnsiString("-") + IntToStr(Month) +
        AnsiString("-") + IntToStr(Day) +
        AnsiString("_") + IntToStr(Hour) + IntToStr(Minute) +
        AnsiString(".log");
  WriteInitData();
}
//---------------------------------------------------------------------------



void __fastcall TLogFileForm::CloseMenuItemClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TLogFileForm::SaveCopyMenuItemClick(TObject *Sender)
{
        if (LogName.c_str() != "No")
        {
                RichEdit1->Lines->SaveToFile(LogDirectory + LogName);
        }
        if (SaveDialog1->Execute())
        {
                RichEdit1->Lines->SaveToFile(SaveDialog1->FileName);
        }
        return;
}
//---------------------------------------------------------------------------

void __fastcall TLogFileForm::CloseWindowExecute(TObject *Sender)
{
     Close();
}
//---------------------------------------------------------------------------

void __fastcall TLogFileForm::SpeedButton1Click(TObject *Sender)
{
    if (FontDialog1->Execute())
        RichEdit1->SelAttributes->Assign(FontDialog1->Font);
}
//---------------------------------------------------------------------------


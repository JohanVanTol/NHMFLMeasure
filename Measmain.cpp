//
//  Measmain is the main program. It's accompanying form
//     "Mainform" is the main shell of the program
//
//
//---------------------------------------------------------------------
#include <vcl.h>
#include <iostream.h>
#include <fstream.h>
#include <sys\timeb.h>
#include <printers.hpp> // for printing
#include <time.h>
#include <stdio.h>
#include <dos.h>
#include "HTMLHelpViewer.hpp"

#pragma hdrstop
#include <Filectrl.hpp>
#include "Measmain.h"
#include "AboutBox.h"
#include "GPIBTest.h"
#include "RFSourceDlg.h"
#include "RF2Dlg.h"
#include "ErrorBx.h"
#include "RFAmplDlg.h"
#include "BoxcarDlg.h"
#include "RS232Test.h"
#include "IsoBusDevices.h"
#include "wsc.h"
#include "ITCDlg.h"
#include "ILMDlg.h"
#include "IPS20Dlg.h"
#include "IPS120Dlg.h"
#include "DMMDlg.h"
#include "LockInDlg.h"
#include "mydata.h"
#include "ENDORParameterDlg.h"
#include "InstrumInitForm.h"
#include "myplot20.h"
#include "MeasPointDialog.h"
#include "EPRMainCoilSweepDlg.h"
#include "EPRSweepCoilDlg.h"
#include "ChannelSelectDlg.h"
#include "CommentDlg.h"
#include "PlotRangeDlg.h"
#include "HP54845A.h"
#include "TekScope.h"
#include "HilbertDlg.h"
#include "InfiniumGateDialog.h"
#include "HP34401A.h"
#include "Keithley6220.h"
#include "Valid.h"
#include "HP8590.h"
#include "PlotCurvesDialog.h"
#include "PhaseDlg.h"
#include "TransientCorrDialog.h"
#include "HeaderViewForm.h"
#include "GetSynFreqDlg.h"
#include "LogFileEdit.h"
#include "CommentDlg17T.h"
#include "UserFormDlg.h"
#include "DAQDlg.h"
#include "BN555.h"
#include "BNC565_GPIB.h"
#include "SMB100A.h"
#include "PulseForm.h"
#include "DacSweepDlg.h"
#include "K6220Dialog.h"
#include "SRS830.h"
#include "SRS860.h"
#include "ThermometerClass.h"
#include "OImonitor.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
#pragma link "HTMLHelpViewer"

// The GPIB (IEEE488)-connected devices.
//          NB These are pointers and need to be
//          Initialized with through
//                pointer = new RFsource(address)

SMY02* RF2;
SMB100A* RFsource;
AR100W1000A* RFAmplif;
SRS245* Boxcar;
K196* DMM1;
HP34401A* DMM2;
HP34401A* DMM3;
SRS830* LockIn1;
// SRS860* LockIn1;
SRS830* LockIn2;
HP54845A* Infinium;
HP8590* Analyser;
Tek7000* Tek7;
BNC565* PulseSource;
K6220* CurrentSource;
//BN555* PulseSource;

// The Oxford instruments devices connected through
//  ISOBUS. These need a number of the serial port (0=COM1, 1=COM2)
//  and the device address (1-9) during initialization.

IB_ITC503* ITC;
IB_ILM211* ILM;
IB_IPS20* SweepCoil;
IB_IPS120* Magnet;

IOTechDaqBoard* DAQ;

Thermometer* Cernox10mA;
Thermometer* Cernox1mA;

int MagnetSystem;    // This value is returned by the routine for the magnet power supply test
					 // routine IB_IPS120::Test()
					 // = 0  for 12.5 T magnet (IPS120)
					 // = 1  for 17 T magnet (IPS180)

//  Global measure parameters. These define the status
//  of the program. (measuring, storing, idle)

double T1[100];   //Used for InfiniumGates
double T2[100];   //Used for InfiniumGates
int NumberOfGates = 2;
int NumberOfMeas = 2;
int TakingData = 0;
int StopMeasurement = 1;
int MeasurementType = 0;
bool Interrupted = false;
bool ReturnSweep = false;
//  Global counters
int Averages = 1;
int CurrentSweep = 0;
int TotalPoints = 0;
int CurrentPoint = 0;

bool _testmode = true;  // If running a not connected system
                         // it is useful to debug the program

int time0, time1;     // Used for system time in milliseconds

int const nCustomMeas = 10;
int CustomMeas[10];
// these 32-bit integers will define the measurements made per point

//  The application is of the multiple-child interface type
//  To keep track of the datafile involved in the measurement
//  the CurrFile pointer is defined

TMainForm *MainForm;
TDataFile *CurrFile = NULL;
TDataFile *AnalysisFile = NULL;
MyPlot *CurrPlot=NULL;

AnsiString UserName;
AnsiString ExperimentID;

bool ChannelA_On;
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// Mainform constructor
__fastcall TMainForm::TMainForm(TComponent *Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------
// EVENT FORMCREATE
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	Application->OnHint = ShowHint;
	Screen->OnActiveFormChange = UpdateMenuItems;

//  Initialize the instruments
	int nDevices = InitializeRemoteDevices();
	if (nDevices < 0)
		ErrorBox->ShowMessage("Error Initializing Instruments");

//  Set default gate values to zero
	for (int i = 0; i<10; i++)
	{
		T1[i] = 0.0;
		T2[i] = 0.0;
	}

	UserForm = new TUserForm(MainForm);
	UserForm->ShowModal();
	UserName = UserForm->UserNameEdit->Text;
	ExperimentID = UserForm->ExperimentMaskEdit->Text;
	MeasureMenuFullItemClick(this);

}

//---------------------------------------------------------------------
void __fastcall TMainForm::ShowHint(TObject *Sender)
{
	StatusBar->SimpleText = Application->Hint;
}
//---------------------------------------------------------------------
void __fastcall TMainForm::CreateDataFile(String Name)
{
	TDataFile *Child;

	//--- create a new MDI child window ----
	Child = new TDataFile(Application);
	Child->Caption = Name;
}
//---------------------------------------------------------------------
void __fastcall TMainForm::FileNewItemClick(TObject *Sender)
{
	CreateDataFile("NONAME" + IntToStr(MDIChildCount + 1));
    MeasureMenu->Enabled = TRUE;
    PlotCurvesDlg->SetPlotCurves(dynamic_cast<TDataFile*> (ActiveMDIChild));
}
//---------------------------------------------------------------------
void __fastcall TMainForm::FileOpenItemClick(TObject *Sender)
{
	if (OpenDialog->Execute())
	{
    	CreateDataFile(OpenDialog->FileName);
        TDataFile *NewFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
        int npts = NewFile->ReadData(OpenDialog->FileName);
        if (npts < 1)
        {
            ErrorBox->ErrorMessage->Caption = "Not a valid Data File";
            ErrorBox->ShowModal();
            ActiveMDIChild->Close();
            return;
        }

        NewFile->GetPlot(0)->ResetTraces();
        int nYTraces = NewFile->GetData()->GetNy();

        NewFile->GetPlot(0)->AddTrace(0,0); // First column is X-axis
		int skip =0;
        if (nYTraces > 20) skip = floor(nYTraces/20);
        for (int i=0; i<nYTraces; i+=skip+1)
                NewFile->GetPlot(0)->AddTrace(1,i+1);

    }
}

//---------------------------------------------------------------------
void __fastcall TMainForm::FileCloseItemClick(TObject *Sender)
{
	if (ActiveMDIChild)
     if (ActiveMDIChild->CloseQuery())
		ActiveMDIChild->Close();

}
//---------------------------------------------------------------------
void __fastcall TMainForm::FileSaveItemClick(TObject *Sender)
{
	//---- save current file (ActiveMDIChild points to the window) ----
	TDataFile *File = dynamic_cast<TDataFile*> (ActiveMDIChild);
	char *line;
	line = new char[80];
	AnsiString FirstEight = File->Caption.SubString(1,6);
	AnsiString NameWOext;

	char* FileHeader;
	AnsiString UpFileName;
	AnsiString DownFileName;
	AnsiString FileNameBase;

	int FileHeaderSize = File->GetHeaderSize();
	FileHeader = new char[FileHeaderSize];
	File->GetHeader(FileHeader,FileHeaderSize-1);
	if (FirstEight.AnsiCompare(AnsiString("NONAME")) == 0)    //File Unsaved
	{
		if (MagnetSystem == 0 )                         // 9 T Magnet
		{
			CommentDialog->HeaderMemo->Lines->SetText(FileHeader);
			CommentDialog->ShowModal();
			if (CommentDialog->ModalResult == mrOk)
			{
//  Add the frequencies to the Header
				sprintf(line,"Source Freq %s GHz (dial %s), LO Freq %s GHz (dial %s) \n",
					CommentDialog->SourceFreqEdit->Text.c_str(), CommentDialog->SourceDialEdit->Text.c_str(),
					CommentDialog->LOFreqEdit->Text.c_str(), CommentDialog->LODialEdit->Text.c_str());
				File->AddHeader(line);

//Power/QO settings:
//      Laser excitation
				sprintf(line,"Laser excitation %s , at %s nm\n",
				CommentDialog->LaserPulseComboBox->Text.c_str(), CommentDialog->WaveLengthEdit->Text.c_str());
				if (strcmp(CommentDialog->LaserPulseComboBox->Text.c_str(),"none") != 0) File->AddHeader(line);
//      Martin-Pupplet and Grid 1
				sprintf(line,"Martin Pupplet setting %s mm, Attenuator Grid %s dB\n",
				CommentDialog->MPEdit->Text.c_str(), CommentDialog->GridAngleEdit->Text.c_str());
				File->AddHeader(line);

//      Attenuators and diodes
				sprintf(line,"Source attn %s dB, Diode reading (-20dB) %s mV\n",
				CommentDialog->SigAttEdit->Text.c_str(), CommentDialog->SigDiodeEdit->Text.c_str());
				File->AddHeader(line);
				sprintf(line,"Reference attn %s dB, Diode reading (-30dB) %s mV\n",
				CommentDialog->RefAttEdit->Text.c_str(), CommentDialog->RefDiodeEdit->Text.c_str());
				File->AddHeader(line);

//  Add the comment to the header
				File->AddHeader(CommentDialog->CommentMemo->Lines->GetText());
				File->AddHeader("\n");  // new line
			}
			  else                    // ModalResult != OK (Cancel)
				{
					delete[] line;
					delete[] FileHeader;
					File = NULL;
					return;
				}
		 }
		 else                         // MagnetSystem != 0 ( 17 T)
		 {
			  CommentDialog2->HeaderMemo->Lines->SetText(FileHeader);
			  CommentDialog2->ShowModal();
			  if (CommentDialog2->ModalResult == mrOk)
			  {
//  Add the frequencies to the Header
					sprintf(line,"Source Freq %s GHz, %s \n",
					CommentDialog2->SourceFreqEdit->Text.c_str(),					CommentDialog2->LockComboBox->Items->Strings[CommentDialog2->LockComboBox->ItemIndex].c_str());					File->AddHeader(line);

					if (CommentDialog2->TransmissionRadioButton->Checked)
					sprintf(line,"Transmission mode, attenuation %s dB , (chopped) power %s mV \n",
					  CommentDialog2->SigAttEdit->Text.c_str(),
					  CommentDialog2->PowerEdit->Text.c_str());
					  else
					sprintf(line,"Quasi-Optical mode, attenuation %s dB , (chopped) power %s mV \n",
					  CommentDialog2->SigAttEdit->Text.c_str(),
					  CommentDialog2->PowerEdit->Text.c_str());

//  Add the comment to the header
					File->AddHeader(CommentDialog2->CommentMemo->Lines->GetText());
					File->AddHeader("\n");  // new line

				}
				else
				  {
					delete[] line;
					delete[] FileHeader;
					File = NULL;
					return;
				  }
		 }
	}       //   end of (if NONAME)
	if (SaveDialog->Execute())
	{
//       if (SaveDialog->FileName->SubString
	   FileNameBase = SaveDialog->FileName.SubString(1,SaveDialog->FileName.Length()-4);
	   if (File->GetUpDown())
	   {
			UpFileName = FileNameBase + "_u.dat";
			DownFileName = FileNameBase + "_d.dat";

			if (File->GetAverage())
			{
				File->WriteData(1, UpFileName.c_str());   // Save Averaged
				File->WriteData(3, DownFileName.c_str());
			}
			  else
				{
					File->WriteData(0, UpFileName.c_str());   // Save Averaged
					File->WriteData(1, DownFileName.c_str());
				}
			File->WriteData(10, SaveDialog->FileName.c_str()); // This saves only the header
			// But is included to avoid overwriting of files ...
	   }
		 else
		   if (File->GetAverage())
			  File->WriteData(1, SaveDialog->FileName.c_str());   // Save Averaged
				else   File->WriteData(0,SaveDialog->FileName.c_str());  // Save File
	   if ((File->GetAverage()) &&
		   ((EPRMainSweepDialog->SaveSweepsCheckBox->Checked)
			 || (EPRSweepCoilDialog->SaveSweepsCheckBox->Checked)) )
	   {
	   // create directory Filename-".dat"
			if (!DirectoryExists(FileNameBase))
					CreateDir(FileNameBase);
			TSearchRec sr;
			int iAttributes = 0;
			int filecount = 0;
			AnsiString newName;
			if (FindFirst("C:\\TempData\\up*.dat", iAttributes, sr) == 0)
			{
				do
				{
					newName = FileNameBase+"\\"+sr.Name;
					RenameFile("C:\\TempData\\"+sr.Name, newName);
					filecount++;
				} while ((FindNext(sr) == 0) && (filecount <Averages));
			}
			FindClose(sr);
			filecount = 0;
			if (FindFirst("C:\\TempData\\down*.dat", iAttributes, sr) == 0)
			{
				do
				{
					RenameFile("C:\\TempData\\"+sr.Name, FileNameBase+"\\"+ sr.Name);
					filecount++;
				} while ((FindNext(sr) == 0) && (filecount <Averages));
			}
			FindClose(sr);
			filecount = 0;
			if (FindFirst("C:\\TempData\\ENDOR*.dat", iAttributes, sr) == 0)
			{
				do
				{
					newName = FileNameBase+"\\"+sr.Name;
					RenameFile("C:\\TempData\\"+sr.Name, newName);
					filecount++;
				} while ((FindNext(sr) == 0) && (filecount <Averages));
			}
			FindClose(sr);
			filecount = 0;
			if (FindFirst("C:\\TempData\\TimeSweep*.dat", iAttributes, sr) == 0)
			{
				do
				{
					newName = FileNameBase+"\\"+sr.Name;
					RenameFile("C:\\TempData\\"+sr.Name, newName);
					filecount++;
				} while ((FindNext(sr) == 0) && (filecount <Averages));
			}
			FindClose(sr);
			filecount = 0;

	   }

	   FileHeaderSize = File->GetHeaderSize();
	   delete[] FileHeader;
	   FileHeader = new char[FileHeaderSize];
	   File->GetHeader(FileHeader,FileHeaderSize-1);
	   LogFileForm->RichEdit1->Font->Style = TFontStyles()<< fsBold << fsItalic;
	   LogFileForm->RichEdit1->Lines->Add(SaveDialog->FileName);
       LogFileForm->RichEdit1->Font->Style = TFontStyles();
       LogFileForm->AddFileHeader(FileHeader);
       ActiveMDIChild->Caption = SaveDialog->FileName;
       File->Saved = true;
    }

    delete[] line;
    delete[] FileHeader;
    File = NULL;
}
//---------------------------------------------------------------------
void __fastcall TMainForm::FileSaveAsItemClick(TObject *Sender)
{
	FileSaveItemClick(Sender);
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
	SaveBtn->Enabled = MDIChildCount > 0;
    MeasureMenu->Enabled = MDIChildCount > 0;
	AnalysisMenu->Enabled = MDIChildCount > 0;
	PlotMenu->Enabled = MDIChildCount > 0;
	WindowCascadeItem->Enabled = MDIChildCount > 0;
	WindowTileItem->Enabled = MDIChildCount > 0;
	WindowArrangeItem->Enabled = MDIChildCount > 0;
	WindowMinimizeItem->Enabled = MDIChildCount > 0;
	StartMeasureButton->Enabled = ((TakingData == 0) && (StopMeasurement == 0));
	PauzeMeasureButton->Enabled = ((TakingData == 1) && (StopMeasurement == 0));
	StopMeasureButton->Enabled = (StopMeasurement == 0);
}
//---------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	Screen->OnActiveFormChange = NULL;
	ITC->SetRemote(0);
	ILM->SetRemote(0);
	Magnet->SetRemote(0);
	SweepCoil->SetRemote(0);
	LockIn1->SetRemote(0);

	ITC->FreePort();
	if (DAQ->IsOpen()) DAQ->Close();

	FreeGPIBDll();   //unloads the GPIB.dll from memory
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::HelpAboutItemClick(TObject *Sender)
{
	EPRAboutBox->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::NewBtnClick(TObject *Sender)
{
    FileNewItemClick(Sender);
}


//---------------------------------------------------------------------------

void __fastcall TMainForm::CWEPRMenuItemClick(TObject *Sender)
{
	MeasurementType = 2;
	NumberOfMeas = 2;
	EPRMeasureTimer->Interval = 50;//ceil(PulsedDialog->AverEdit->Text.ToInt() * millisec_pp);
	EPRSweepMeasureTimer->Interval = 50;//ceil(PulsedDialog->AverEdit->Text.ToInt() *

	PrepareEPRMeasurement();
}

//---------------------------------------------------------------------------

void TMainForm::PrepareEPRMeasurement()
{
	CurrFile = dynamic_cast<TDataFile*> (ActiveMDIChild); // GetDocumentPointer

//  Test if the file is empty, if not display message
//  and return
	if (!CurrFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		CurrFile = NULL;  // Reset CurrFile  to NULL on end of measurement
		return;
	}

	int status;        // for Remote commands checking
	CurrentSweep = 0;  // for averaging purposes
	CurrentPoint = 0;  // npoints in sweep

	Averages = 1;      // Default averages
	int av;

	double Xmin, Xmax;
// Get the sweep parameters
	char line[80];

	bool Persistent = false;
// Possibility 1: Main magnet non-persistent.
	if ((Magnet->Heater() == 1) || (Magnet->GetAddress() == 0))//  Heater ON. We'll sweep main coil or simulate
	{
		EPRMainSweepDialog->ShowModal();
			//Get the Main magnet sweep parameters
		if (EPRMainSweepDialog->ModalResult != mrOk)
		{
			CurrFile = NULL;
			return;
		}

//Check for Averaging
		if (EPRMainSweepDialog->AverageCheckBox->Checked)
		{
			if (ValidInt(EPRMainSweepDialog->AverEdit->Text, &av))
			{
				if ((av>1) && (av<1000))
				{
					CurrFile->SetAverage(1); // Set averagemode on
					Averages = av;
				}
			}
		} else CurrFile->SetAverage(0);

//Check for Up and Down Sweep
		if (EPRMainSweepDialog->UpDownCheckBox->Checked)
			CurrFile->SetUpDown(1); // Set UpDownmode on
		   else CurrFile->SetUpDown(0);

// If MainSweepDialog succesfull set the Xplotlimits
		if (EPRMainSweepDialog->GetEndField()
				< EPRMainSweepDialog->GetStartField())
		{
			Xmax = EPRMainSweepDialog->GetStartField();
			Xmin = EPRMainSweepDialog->GetEndField();
		}
		  else
		  {
			Xmin = EPRMainSweepDialog->GetStartField();
			Xmax = EPRMainSweepDialog->GetEndField();
		  }

		  // Initiate sweep to startfield
		double Sweeprate = EPRMainSweepDialog->GetSweepSpeed();
		if (Sweeprate <0) Sweeprate = 0.0;         // Do not allow negative rates
		double Max_mTps = Magnet->GetSafeSpeed()*1000/(Magnet->ApT()*60.0);
		if (Sweeprate > Max_mTps) {Sweeprate = Max_mTps;}   // Do not go faster than SafeSpeed
				  // Check SweepSpeed rounding accuracy
		Magnet->SetFieldSweepSpeed(Sweeprate);
		if (Magnet->GetAddress() != 0)
		{
			Sweeprate = Magnet->GetAccurateSpeed();
			EPRMainSweepDialog->SetSweepSpeed(Sweeprate);
		}

		if (Sweeprate < 2.0) Sweeprate = 2.0;             //default rate
		Magnet->SetFieldSweepSpeed(Sweeprate);
		Magnet->SetField(EPRMainSweepDialog->GetStartField());
		Magnet->SetAction(1);

	}
	else
	 {
		if (Magnet->Heater() != 2)   // If field clamped or anything return
		{
			ErrorBox->ErrorMessage->Caption = "Check State of Magnet";
			ErrorBox->ShowModal();
			CurrFile = NULL;
			return;
		}
		Persistent = true;           // Or else sweep sweepcoil
	//  We want to read the persistent field
	//  from the main power supply
		Magnet->GetParameter(18);
	// Get Field sweepParameters
		EPRSweepCoilDialog->PersistentFieldLabel->Caption = Magnet->GetParameter(18);
		EPRSweepCoilDialog->ShowModal();
		if (EPRSweepCoilDialog->ModalResult != mrOk)
		{
			CurrFile = NULL;
			return;           // If Dialog was canceled return
		}

// Check for averaging mode

		if (EPRSweepCoilDialog->AverageCheckBox->Checked)
		{
			if (ValidInt(EPRSweepCoilDialog->AverEdit->Text, &av))
			{
				if ((av>1) && (av<1000))
				{
					CurrFile->SetAverage(1);
					Averages = av;
				}
			}
		}

		if (EPRSweepCoilDialog->UpDownCheckBox->Checked)
			CurrFile->SetUpDown(1); // Set averagemode on
		   else CurrFile->SetUpDown(0);

 // Set X-Plot limits
		if (EPRSweepCoilDialog->GetEndField()
				< EPRSweepCoilDialog->GetStartField())
		{
			Xmax = EPRSweepCoilDialog->GetStartField()*1000.0;
			Xmin = EPRSweepCoilDialog->GetEndField()*1000.0;
		}
		  else
		  {
			Xmin = EPRSweepCoilDialog->GetStartField()*1000.0;
			Xmax = EPRSweepCoilDialog->GetEndField()*1000.0;
		  }
		CurrFile->GetPlot()->SetAxisTitle("Sweepcoil Field (mT)",0);
// Initiate sweep
		status = SweepCoil->SetAction(0);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting field to HOLD";
			ErrorBox->ShowModal();
			return;
		}

		status = SweepCoil->SetFieldSweepSpeed(99);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting SweepSpeed";
			ErrorBox->ShowModal();
			return;
		}

		status = SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting field";
			ErrorBox->ShowModal();
			return;
		}
		status = SweepCoil->SetAction(1);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting SweepCoil to GOTO SETPOINT";
			ErrorBox->ShowModal();
			return;
		}
	}

	CurrFile->AllDataReset(NumberOfMeas);  //Reset Data-arrays for correct number of y-values
	DataPoint Pmin(NumberOfMeas);      // DataPoints for PlotLimits
	DataPoint Pmax(NumberOfMeas);

	Pmin.Set(0,Xmin);
	Pmax.Set(0,Xmax);

// Set the y-plot limits.
	double range = LockIn1->RealSensitivity();
	double tc1, tc2;
	if (LockIn1->MeasTimeConstant(&tc1) <0) tc1 = 0.1;
	if (LockIn2->MeasTimeConstant(&tc2) <0) tc2 = 0.1;
	if (range > 0)
	{
		range *= 1000.0;                         // for millivolts
		Pmin.Set(1, -range);
		Pmax.Set(1,range);
		Pmin.Set(2, -range);
		Pmax.Set(2,range);
	}
	time_t timer = time(NULL);  // get the time and date
	CurrFile->SetHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));
	if (Persistent)
	{
		sprintf(line,"Main magnet persistent at %8.5f T \n",
				Magnet->GetParameter(18));
		CurrFile->AddHeader(line);

		sprintf(line,"SweepCoil Sweep form %8.3f to %8.3f mT at %6.4f mT/s \n",
		   EPRSweepCoilDialog->GetStartField(),
			 EPRSweepCoilDialog->GetEndField(),
			   EPRSweepCoilDialog->GetSweepSpeed());
		CurrFile->AddHeader(line);
	}
	  else
	  {
		sprintf(line,"Main magnet sweep %8.5f to %8.5f Tesla at %6.3f mT/s \n",
		   EPRMainSweepDialog->GetStartField(),
		   EPRMainSweepDialog->GetEndField(),
		   EPRMainSweepDialog->GetSweepSpeed());
		CurrFile->AddHeader(line);
		if (SweepCoil->GetAddress() != 0)
		{
			sprintf(line,"SweepCoil at %8.3f mT \n",SweepCoil->GetField());
			CurrFile->AddHeader(line);
		}
	  }


	AddInstrumentHeader();

	for (int i = 0; i< CurrFile->GetnPlots(); i++)
	{
		CurrFile->GetPlot(i)->SetAutoRange(0);
		CurrFile->GetPlot(i)->SetFixedRanges(Pmin, Pmax);
		CurrFile->GetPlot(i)->SetAxisTitle("X-signal (mV)",1);
		CurrFile->GetPlot(i)->SetAxisTitle("Y-signal (mV)",2);
		CurrFile->GetPlot(i)->SetAxisTitle("Field (T)",0); // Set X-axis title
	}
	CurrFile->PlotData();
		//    CurrFile->DataReset(2,TotalPoints);

	CurrFile->Invalidate(); //replot after changing axes
	TakingData = 0;
	StopMeasurement = 0;
	Interrupted = 0;
	UpdateMenuItems(this);
	MeasPointFrm->Show();
//	if (tc1 < tc2) EPRMeasureTimer->Interval = (int)(1000*tc1/3);
	EPRSweepMeasureTimer->Interval = EPRMeasureTimer->Interval;
	ReturnSweep = false;
	if (Persistent)
	{
		GoToSweepStartTimer->Interval = 1000;
		GoToSweepStartTimer->Enabled = true;
	}
	  else
	  {
		GoToStartFieldTimer->Interval = 1000;
		GoToStartFieldTimer->Enabled = true;
	  }
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::GPIBtestMenuItemClick(TObject *Sender)
{
	GPIBTestForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RFSourceMenuItemClick(TObject *Sender)
{
	if (RFsource->GetAddress() == 0)
	{
		ErrorBox->ErrorMessage->Caption = " RF Source not connected or initialized ";
		ErrorBox->ShowModal();
		return;
	}
	RFSourceDialog->Show();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::RFampMenuItemClick(TObject *Sender)
{
	if (RFAmplif->GetAddress() == 0)
    {
        ErrorBox->ErrorMessage->Caption = " Amplifier not connected or initialized ";
        ErrorBox->ShowModal();
        return;
    }
    RFAmplifierDialog->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxcarMenuItemClick(TObject *Sender)
{
    if (Boxcar->GetAddress() == 0)
    {
        ErrorBox->ErrorMessage->Caption = " Boxcar not connected or initialized ";
        ErrorBox->ShowModal();
        return;
    }
    BoxcarDialog->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Rs232TestMenuItemClick(TObject *Sender)
{

    ITC->FreePort();
    RS232TestForm->ShowModal();
    ITC->InitPort();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ITCMenuItemClick(TObject *Sender)
{
	if (ITC->GetAddress() == 0)
    {
        ErrorBox->ErrorMessage->Caption = " ITC not connected or initialized ";
        ErrorBox->ShowModal();
		return;
    }
    ITCDialog->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ILMMenuItemClick(TObject *Sender)
{
    if (ILM->GetAddress() == 0)
    {
        ErrorBox->ErrorMessage->Caption = " ILM not connected or initialized ";
        ErrorBox->ShowModal();
        return;
    }
    ILMDialog->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::IPS20MenuItemClick(TObject *Sender)
{
    if (SweepCoil->GetAddress() == 0)
    {
        ErrorBox->ErrorMessage->Caption = " SweepCoil not connected or initialized ";
        ErrorBox->ShowModal();
		return;
    }
    IPS20Dialog->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MagnetMenuItemClick(TObject *Sender)
{
	if (Magnet->GetAddress() == 0)
    {
        ErrorBox->ErrorMessage->Caption = " Magnet not connected or initialized ";
        ErrorBox->ShowModal();
        return;
	}
    IPS120Dialog->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DMM1MenuItemClick(TObject *Sender)
{
	if (DMM1->GetAddress() == 0)
	{
		ErrorBox->ErrorMessage->Caption = " DMM1 not connected or initialized ";
		ErrorBox->ShowModal();
		return;
	}
	DMM1Form->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LockIn1MenuItemClick(TObject *Sender)
{
    if (LockIn1->GetAddress() == 0)
    {
        ErrorBox->ErrorMessage->Caption = " LockIn1 not connected or initialized ";
        ErrorBox->ShowModal();
        return;
    }
	LockInDialog->SetLockIn(LockIn1);
	LockInDialog->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::InfiniumMenuItemClick(TObject *Sender)
{
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	if (!MyFile->IsEmpty())

	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		MyFile = NULL;
		return;
	}
	CurrFile = MyFile;

	time_t timer = time(NULL);  // get the time and date
	CurrFile->SetHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));

	int chan = 1;
	int npts = 0;
	DataArray NewData;
	ChannelSelectDialog->ShowModal();
	if (ChannelSelectDialog->ModalResult == mrOk)
	{
		chan = ChannelSelectDialog->GetChannel();
		if ((chan > 0) && (chan <= 12)) npts = Infinium->GetData(chan, &NewData);
		if (chan == 13)
		{
			npts = Infinium->GetData(1, 3, &NewData);
		}
	}
	char line[80];
	sprintf(line, "Channel %2d \n",chan);
	if (npts > 0)
	{
		MyFile->SetData(NewData);
		AddInstrumentHeader();
		CurrFile->AddHeader(line);
	}
	MyFile = NULL;
	CurrFile = NULL;
	return;
}
//---------------------------------------------------------------------------

int TMainForm::InitializeRemoteDevices()
{
	int nDev =0;
// first we'll initialize the devices to their default
//  values
	TInstrumentInitForm* InitForm = new TInstrumentInitForm(MainForm);
	InitForm->Memo->Lines->Add("Setting default values for the instruments");
	InitForm->Show();
	InitForm->Update();

//  IEEE connected instruments
	RF2 = new SMY02(28);
	RFsource = new SMB100A(23);
	RFAmplif = new AR100W1000A(1);
	Boxcar = new SRS245(6);
	DMM1 = new K196(2);
	DMM2 = new HP34401A(18);
	LockIn1 = new SRS830(4);
//	LockIn1 = new SRS860(6);
	LockIn2 = new SRS830(5);
	Infinium = new HP54845A(11);
	Analyser = new HP8590(18);
	Tek7 = new Tek7000(19);
	PulseSource = new BNC565(7);
	CurrentSource = new K6220(12);

	int BN555SerialPort = COM4;
	int IsoBusSerialPort = COM1;
//	int LaserSerialPort = COM2;
// Isobus instruments
	ITC = new IB_ITC503(IsoBusSerialPort, 1);
	ILM = new IB_ILM211(IsoBusSerialPort, 6);
	SweepCoil = new IB_IPS20(IsoBusSerialPort, 3);
	Magnet = new IB_IPS120(IsoBusSerialPort, 2);

//	PulseSource = new BN555(COM4);

	DAQ = new IOTechDaqBoard;
	DWORD nDAQ = 0;
	char DAQName[20];
	DWORD compmask = daq2000AI|daq1000AI|daqboard|daqbook|daq216|daqPCcard|daq1000|daq500;
// Open the initialisation file
//  this file contains the addresses of the devices

	ifstream InitFile("measure.ini");
	if ((InitFile.fail()) || (!InitFile))
	{
		InitForm->Memo->Lines->Add("Failed to open initialization file MEASURE.INI");
		InitForm->Memo->Lines->Add("Default values are retained");
	}
	  else
	  {
		InitForm->Memo->Font->Color = clGreen;
		InitForm->Memo->Lines->Add("Initialization file MEASURE.INI opened");

		// if InitFile opened correctly

		int addr =0;
		char line[255];
		char introtext[255];

		// Read first line...
		// Which is for testing purposes especially
		InitFile.getline(line, 254);
		if (sscanf(line,"%s %ld",introtext, &nDev) < 2)
		{
			InitForm->Memo->Lines->Add("Could not read file correctly");
			InitForm->Memo->Lines->Add("Assuming testmode");
			nDev = 0;
		}
		  else
		  {
			// The file can be read. Set all adresses to 0 first
			RFsource->SetAddress(0);
			RF2->SetAddress(0);
			RFAmplif->SetAddress(0);
			Boxcar->SetAddress(0);
			DMM1->SetAddress(0);
			DMM2->SetAddress(0);
			LockIn1->SetAddress(0);
			LockIn2->SetAddress(0);
			Infinium->SetAddress(0);
			Analyser->SetAddress(0);
			Tek7->SetAddress(0);
			PulseSource->SetAddress(0);

			ITC->Reset(IsoBusSerialPort, 0);
			ILM->Reset(IsoBusSerialPort, 0);
			SweepCoil->Reset(IsoBusSerialPort, 0);
			Magnet->Reset(IsoBusSerialPort, 0);


			InitForm->Memo->Lines->Add(line);
			if (nDev > 0)
				while (!InitFile.eof())
				{
					InitFile.getline(line,254);
					InitForm->Memo->Lines->Add(line);
					if (sscanf(line,"%s %ld",introtext, &addr) > 1)
					{
						if (strcmp(introtext,"RFsource")==0)
							RFsource->SetAddress(addr);
						if (strcmp(introtext,"RFsource2")==0)
							RF2->SetAddress(addr);
						if (strcmp(introtext,"RFAmplif")==0)
							RFAmplif->SetAddress(addr);
						if (strcmp(introtext,"Infinium")==0)
							Infinium->SetAddress(addr);
						if (strcmp(introtext,"TekScope")==0)
							Tek7->SetAddress(addr);
						if (strcmp(introtext,"Analyser")==0)
							Analyser->SetAddress(addr);
						if (strcmp(introtext,"DMM1")==0)
							DMM1->SetAddress(addr);
						if (strcmp(introtext,"DMM2")==0)
							DMM2->SetAddress(addr);
						if (strcmp(introtext,"LockIn1")==0)
							LockIn1->SetAddress(addr);
						if (strcmp(introtext,"LockIn2")==0)
							LockIn2->SetAddress(addr);
						if (strcmp(introtext,"Boxcar")==0)
							Boxcar->SetAddress(addr);
						if (strcmp(introtext,"PulseBox")==0)
							PulseSource->SetAddress(addr);
						if (strcmp(introtext,"CurrentSource")==0)
							PulseSource->SetAddress(addr);


						if (strcmp(introtext,"Isobus")==0)
						{
							switch (addr) {
								case 1: IsoBusSerialPort = COM1; break;
								case 2: IsoBusSerialPort = COM2; break;
								case 3: IsoBusSerialPort = COM3; break;
								case 4: IsoBusSerialPort = COM4; break;
							default: IsoBusSerialPort = COM1; break;
							}
						}
						if (strcmp(introtext,"ILM")==0)
							ILM->Reset(IsoBusSerialPort,addr);
						if (strcmp(introtext,"ITC")==0)
							ITC->Reset(IsoBusSerialPort,addr);
						if (strcmp(introtext,"Magnet")==0)
							Magnet->Reset(IsoBusSerialPort,addr);
						if (strcmp(introtext,"SweepCoil")==0)
							SweepCoil->Reset(IsoBusSerialPort,addr);

						if (strcmp(introtext,"BN555")==0)
						{
							switch (addr) {
								case 1: BN555SerialPort = COM1; break;
								case 2: BN555SerialPort = COM2; break;
								case 3: BN555SerialPort = COM3; break;
								case 4: BN555SerialPort = COM4; break;
							default: BN555SerialPort = COM4; break;
							}
						}

						if (strcmp(introtext,"DAQ")==0)
							nDAQ = (DWORD)addr;


					}
				}
				InitFile.close();
		}
	}
//LoadTheGPIB. dll (we use the dynamic link library of the
//        national instruments GPIB IEEE488 software. This means we'll
//          have to access all functions via their pointers)

	if (nDev>0)
	{   // if only testing on other PC do not load dll
		if (!LoadGPIBDll())
			InitForm->Memo->Lines->Add("Unable to correctly access the 32-bit GPIB DLL.");
		  else
		  {
			InitForm->Memo->Lines->Add(" 32-bit GPIB DLL loaded.");
			if (GPIBInterfaceClear() != 0)
				InitForm->Memo->Lines->Add(" IFC (Interface Clear) failed.");
			  else
			  {
				InitForm->Memo->Lines->Add(" IFC succesful.");
				GPIBSetTimeOut(11);      // Set to 1 s
				GPIBSetEotMode(1);       // SetEndOfTransmission mode
				GPIBSetEosMode(0);       // SetEndOfSend mode
			  }
		  }
		ITC->InitPort();  //Initialize the ISOBus Port on one of the devices

		InitForm->Memo->Lines->Add(" Testing GPIB devices:");

		// RF generator communication test

// TO DO:  Test for the presence of GPIB controllers

		if (RFsource->GetAddress() != 0)
		{
			if (RFsource->Test() == 0)
			   InitForm->Memo->Lines->Add("RFsource OK");
			 else
			 {
			   InitForm->Memo->Font->Color = clRed;
			   InitForm->Memo->Lines->Add("RFsource communication failed. Address set to 0");
			   InitForm->Memo->Font->Color = clWindowText;
			   RFsource->SetAddress(0);
			 }
		}

		if (RF2->GetAddress() != 0)
		{
			if (RF2->Test() == 0)
			   InitForm->Memo->Lines->Add("RFsource 2 OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("RFsource 2 communication failed. Address set to 0");
			   RF2->SetAddress(0);
			 }
		}

		if (LockIn1->GetAddress() != 0)
		{
			if (LockIn1->Test() == 0)
			   InitForm->Memo->Lines->Add("LockIn1 OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("LockIn1 communication failed. Address set to 0");
			   LockIn1->SetAddress(0);
			 }
		}

		if (LockIn2->GetAddress() != 0)
		{
			if (LockIn2->Test() == 0)
			   InitForm->Memo->Lines->Add("LockIn2 OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("LockIn2 communication failed. Address set to 0");
			   LockIn2->SetAddress(0);
			 }
		}

		if (Infinium->GetAddress() != 0)
		{
			if (Infinium->Test() == 0)
			   InitForm->Memo->Lines->Add("Infinium OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("Infinium communication failed. Address set to 0");
			   Infinium->SetAddress(0);
			 }
		}

		if (Tek7->GetAddress() != 0)
		{
			if (Tek7->Test() == 0)
			   InitForm->Memo->Lines->Add("Tektronix scope OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("TekScope communication failed. Address set to 0");
			   Tek7->SetAddress(0);
			 }
		}

		if (Analyser->GetAddress() != 0)
		{
			if (Analyser->Test() == 0)
			   InitForm->Memo->Lines->Add("Analyser OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("Analyser communication failed. Address set to 0");
			   Analyser->SetAddress(0);
			 }
		}

		if (RFAmplif->GetAddress() != 0)
		{
			if (RFAmplif->Test() == 0)
			   InitForm->Memo->Lines->Add("RFAmplif OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("RFAmplif communication failed. Address set to 0");
			   RFAmplif->SetAddress(0);
			 }
		}

		if (Boxcar->GetAddress() != 0)
		{
			if (Boxcar->Test() == 0)
			   InitForm->Memo->Lines->Add("Boxcar OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("Boxcar communication failed. Address set to 0");
			   Boxcar->SetAddress(0);
			 }
		}

		if (DMM1->GetAddress() != 0)
		{
			if (DMM1->Test() == 0)
			   InitForm->Memo->Lines->Add("DMM1 OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("DMM1 communication failed. Address set to 0");
			   DMM1->SetAddress(0);
			 }
		}

		if (DMM2->GetAddress() != 0)
		{
			if (DMM2->Test() == 0)
			   InitForm->Memo->Lines->Add("DMM2 OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("DMM2 communication failed. Address set to 0");
			   DMM2->SetAddress(0);
			 }
		}

		if (PulseSource->GetAddress() != 0)
		{
			if (PulseSource->Test() == 0)
			   InitForm->Memo->Lines->Add("PulseSource OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("Pulse source communication failed. Address set to 0");
			   PulseSource->SetAddress(0);
			 }
		}

		if (CurrentSource->GetAddress() != 0)
		{
			if (CurrentSource->Test() == 0)
			   InitForm->Memo->Lines->Add("CurrentSource OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("CurrentSource communication failed. Address set to 0");
			   CurrentSource->SetAddress(0);
			 }
		}

		InitForm->Memo->Lines->Add(" Testing IsoBus devices:");
		if (ITC->GetAddress() != 0)
		{
			if (ITC->Test() == 0)
			   InitForm->Memo->Lines->Add("ITC OK");
			 else
			 {
			   InitForm->Memo->Lines->Add("ITC communication failed. Address set to 0");
			   ITC->Reset(IsoBusSerialPort,0);
			 }
		}

        if (ILM->GetAddress() != 0)
        {
            if (ILM->Test() == 0)
               InitForm->Memo->Lines->Add("ILM OK");
			 else
			 {
               InitForm->Memo->Lines->Add("ILM communication failed. Address set to 0");
               ILM->Reset(IsoBusSerialPort,0);
             }
        }

        if (SweepCoil->GetAddress() != 0)
        {
            if (SweepCoil->Test() == 0)
            {
               InitForm->Memo->Lines->Add("SweepCoil OK");
			   SweepCoil->SetExtendedResolution(1);
			}
			 else
			 {
               InitForm->Memo->Lines->Add("SweepCoil communication failed. Address set to 0");
			   SweepCoil->Reset(IsoBusSerialPort,0);
             }
        }

        if (Magnet->GetAddress() != 0)
        {
			MagnetSystem = Magnet->Test();
            if (MagnetSystem >= 0)
            {
               InitForm->Memo->Lines->Add("Magnet OK");
               Magnet->SetExtendedResolution(1);
            }
             else
             {
               InitForm->Memo->Lines->Add("Magnet communication failed. Address set to 0");
               Magnet->Reset(IsoBusSerialPort,0);
               MagnetSystem = 0;
             }
        }


		if (nDAQ > 0) daqGetDeviceCount(&nDAQ);
		if (nDAQ == 0) InitForm->Memo->Lines->Add(" No Acquisition Board found ");
			 else if (nDAQ==1) InitForm->Memo->Lines->Add("One Acquisition Board found ");
		if (nDAQ > 0)
		{
			InitForm->Memo->Lines->Add("IOTech Acquisition Board found: ");
			strcpy(DAQName,GetDeviceName(compmask));
			   InitForm->Memo->Lines->Add(DAQName);
				DAQ->SetName(DAQName);
			   InitForm->Memo->Lines->Add("Daq Name Set");
			   if (	DAQ->Connect() >= 0) InitForm->Memo->Lines->Add("Daq Connected");
				  else InitForm->Memo->Lines->Add("Daq connection failed");
		}

	}
	   else //   In testmode all addresses are set to 0
	   {
			InitForm->Memo->Lines->Add("TestMode: all adresses set to 0");
			RFsource->SetAddress(0);
			RF2->SetAddress(0);
			RFAmplif->SetAddress(0);
			Boxcar->SetAddress(0);
			DMM1->SetAddress(0);
			DMM2->SetAddress(0);
			LockIn1->SetAddress(0);
			LockIn1->SetAddress(0);
			Infinium->SetAddress(0);
			Tek7->SetAddress(0);
			PulseSource->SetAddress(0);
			CurrentSource->SetAddress(0);

			ITC->Reset(IsoBusSerialPort, 0);
			ILM->Reset(IsoBusSerialPort, 0);
			SweepCoil->Reset(IsoBusSerialPort, 0);
			Magnet->Reset(IsoBusSerialPort, 0);

			if (nDAQ > 0) daqGetDeviceCount(&nDAQ);
			if (nDAQ == 0) InitForm->Memo->Lines->Add(" No Acquisition Board found ");

			if (nDAQ > 0)
			{
				InitForm->Memo->Lines->Add("IOTech Acquisition Board found: ");
				strcpy(DAQName,GetDeviceName(compmask));
				InitForm->Memo->Lines->Add(DAQName);
				DAQ->SetName(DAQName);
				if (DAQ->Connect() < 0)
					InitForm->Memo->Lines->Add("Connection Failed ");
				  else InitForm->Memo->Lines->Add("IOTech DAQ Open success ");

			}
		}

	InitForm->WaitBeforeClosingTimer->Enabled = true;

	Cernox1mA = new Thermometer(1.27, 10.0, 24.63963, -14.57297, 2.10294);
	Cernox10mA = new Thermometer(4.0, 300.0, 210.35108, -357.47221, 230.71778, -65.96581, 6.9983);
//  InitForm->Close();
//	InitForm->ShowModal();
	return nDev;
}

void __fastcall TMainForm::CWENDORClick(TObject *Sender)
{
	CurrFile = dynamic_cast<TDataFile*> (ActiveMDIChild); // GetDocumentPointer

//  Test if the file is empty, if not display message
//  and return
	if (!CurrFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		CurrFile = NULL;  // Reset CurrFile  to NULL on end of measurement
		return;
	}

	CurrentSweep = 0;  // for averaging purposes
	CurrentPoint = 0;  // npoints in sweep

	Averages = 1;      // Default averages
	int av;

	DataPoint Pmin(4);      // DataPoints for PlotLimits
	DataPoint Pmax(4);

// Get the parameters
	ENDORParametersDialog->ShowModal();
	if (ENDORParametersDialog->ModalResult != mrOk) return;

// Set StartFrequency
	RFsource->SetFreq(ENDORParametersDialog->GetStartFreq());
// Set StepSize
	RFsource->SetStep(ENDORParametersDialog->GetStep());
// Set Power ON
	RFsource->SetPower(ENDORParametersDialog->GetPower());
// Set Amplifier ON
	MeasurementType = 1;

	Averages = ENDORParametersDialog->AverageEdit->Text.ToInt();
	TotalPoints = ENDORParametersDialog->NStepsEdit->Text.ToInt() + 1;
	CurrentSweep = 1;
	CurrentPoint = 0;

	Pmin.Set(0,ENDORParametersDialog->GetStartFreq());
	Pmax.Set(0,ENDORParametersDialog->GetEndFreq());
	double range = LockIn1->RealSensitivity();
	if (range > 0)
	{
		Pmin.Set(1, -range);
		Pmax.Set(1,range);
		Pmin.Set(2, -range);
		Pmax.Set(2,range);
	}

	CurrFile->GetPlot()->SetAutoRange(0);
	CurrFile->GetPlot()->SetFixedRanges(Pmin, Pmax);
	CurrFile->GetPlot()->SetAutoY1();
	CurrFile->GetPlot()->SetAutoY2();
	CurrFile->GetPlot()->SetAxisTitle("Frequency (MHz)",0);
	CurrFile->GetPlot()->SetAxisTitle("X-signal (mV)",1);
	CurrFile->GetPlot()->SetAxisTitle("Y-signal (mV)",2);

	CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
	CurrFile->DataReset(4,TotalPoints);

	time_t timer = time(NULL);  // get the time and date

	CurrFile->SetHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));
	char line[128];
	sprintf(line,"ENDOR measurement %8.3f to %8.3f MHz in %d steps of %8.3f kHz \n",
		ENDORParametersDialog->GetStartFreq(),ENDORParametersDialog->GetEndFreq(),
		  TotalPoints, ENDORParametersDialog->GetStep());
	CurrFile->AddHeader(line);
	sprintf(line,"RF power %8.3f dB, dwelltime %d ms, %d averages \n",
		ENDORParametersDialog->GetPower(), ENDORParametersDialog->DwellEdit->Text.ToInt(),
		Averages);
	CurrFile->AddHeader(line);
	sprintf(line,"Modulation FM %8.3f kHz, modulation AM %8.3f percent \n",
		ENDORParametersDialog->FMEdit->Text.ToDouble(),
		ENDORParametersDialog->AMEdit->Text.ToDouble());
	CurrFile->AddHeader(line);

	AddInstrumentHeader();

	TakingData = 1;
	CurrFile->Measuring = true;
	StopMeasurement = 0;
	UpdateMenuItems(this);
	MeasPointFrm->Show();

//   Magnet->SetField(EPRMainSweepDialog->GetStartField());
//   Magnet->SetAction(1);
	MeasureENDORTimer->Interval = ENDORParametersDialog->DwellEdit->Text.ToInt();
	MeasureENDORTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void TMainForm::PulsedPrepare()
{
	CurrFile = dynamic_cast<TDataFile*> (ActiveMDIChild); // GetDocumentPointer

//  Test if the file is empty, if not display message
//  and return
	if (!CurrFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		CurrFile = NULL;  // Reset CurrFile  to NULL on end of measurement
		return;
	}

	CurrentSweep = 0;  // for averaging purposes
	CurrentPoint = 0;  // npoints in sweep

	Averages = 1;      // Default averages
	int av = PulsedDialog->ScanEdit->Text.ToInt();
	if (av>1) {
		CurrFile->SetAverage(1);
		Averages = av;

	}

	CurrFile->AllDataReset(4);  //Reset Data-arrays for correct number of y-values
	DataPoint Pmin(4);      // DataPoints for PlotLimits
	DataPoint Pmax(4);

	T1[0] = PulsedDialog->AcqDelayEdit->Text.ToInt()*1e-9;
	T2[0] = T1[0] + PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;
	NumberOfGates = 2;
	T1[1] = T1[0] + 2 * PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;
	T2[1] = T1[1] + PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;

// Get the parameters

// Set StartFrequency
	bool active[8];
	if (PulsedDialog->P1CheckBox->Checked) {active[0]=true;}
	if (PulsedDialog->P2CheckBox->Checked) {active[1]=true;}
	if (PulsedDialog->P3CheckBox->Checked) {active[2]=true;}
	if (PulsedDialog->P4CheckBox->Checked) {active[3]=true;}
	if (PulsedDialog->P5CheckBox->Checked) {active[4]=true;}
	if (PulsedDialog->P6CheckBox->Checked) {active[5]=true;}
	if (PulsedDialog->P7CheckBox->Checked) {active[6]=true;}
	if (PulsedDialog->P8CheckBox->Checked) {active[7]=true;}

	int delay[8];
	int delaystep[8];
	int width[8];
	int widthstep[8];
	int IntegrationWidth;
	int AcqDelay;
	int Rate;
	int PulsAver;

	delay[0] = PulsedDialog->P1DelayEdit->Text.ToInt();
	delay[1] = PulsedDialog->P2DelayEdit->Text.ToInt();
	delay[2] = PulsedDialog->P3DelayEdit->Text.ToInt();
	delay[3] = PulsedDialog->P4DelayEdit->Text.ToInt();
	delay[4] = PulsedDialog->P5DelayEdit->Text.ToInt();
	delay[5] = PulsedDialog->P6DelayEdit->Text.ToInt();
	delay[6] = PulsedDialog->P7DelayEdit->Text.ToInt();
	delay[7] = PulsedDialog->P8DelayEdit->Text.ToInt();

	width[0] = PulsedDialog->P1WidthEdit->Text.ToInt();
	width[1] = PulsedDialog->P2WidthEdit->Text.ToInt();
	width[2] = PulsedDialog->P3WidthEdit->Text.ToInt();
	width[3] = PulsedDialog->P4WidthEdit->Text.ToInt();
	width[4] = PulsedDialog->P5WidthEdit->Text.ToInt();
	width[5] = PulsedDialog->P6WidthEdit->Text.ToInt();
	width[6] = PulsedDialog->P7WidthEdit->Text.ToInt();
	width[7] = PulsedDialog->P8WidthEdit->Text.ToInt();

	for (int i=0; i < 8; i++) {
		if (active[i])
		{
			PulseSource->Set(i+1, delay[i], width[i]);

		}
	}

	MeasurementType = 1;

	Averages = PulsedDialog->ScanEdit->Text.ToInt();
	TotalPoints = PulsedDialog->NptsEdit->Text.ToInt() + 1;
	CurrentSweep = 1;
	CurrentPoint = 0;

	Pmin.Set(0,0.0);

	int step, wstep;
	int TimeStep = 0;
	if (PulsedDialog->AcqStepEdit->Text.ToInt() == 0)
	{
		if (((step = PulsedDialog->P1DelayStepEdit->Text.ToInt()) != 0) && (active[0] == true))
			TimeStep = step;
		  else
			if (((step = PulsedDialog->P2DelayStepEdit->Text.ToInt()) != 0) && (active[1] == true))
				TimeStep = step;
			  else
				if (((step = PulsedDialog->P3DelayStepEdit->Text.ToInt()) != 0) && (active[2] == true))
					TimeStep = step;
				  else
					if (((step = PulsedDialog->P4DelayStepEdit->Text.ToInt()) != 0) && (active[3] == true))
						TimeStep = step;
		else
		if (((wstep = PulsedDialog->P1WidthStepEdit->Text.ToInt()) != 0) && (active[0] == true))
			TimeStep = step;
		  else
			if (((wstep = PulsedDialog->P2WidthStepEdit->Text.ToInt()) != 0) && (active[1] == true))
				TimeStep = step;
			  else
				if (((wstep = PulsedDialog->P3WidthStepEdit->Text.ToInt()) != 0) && (active[2] == true))
					TimeStep = step;
				  else
					if (((wstep = PulsedDialog->P4WidthStepEdit->Text.ToInt()) != 0) && (active[3] == true))
						TimeStep = step;
					 else TimeStep = 1;

	}
	else TimeStep = PulsedDialog->AcqStepEdit->Text.ToInt();

	Pmax.Set(0,(double)TotalPoints*(double)TimeStep/1000.0);

	double range = 0.1;
	if (range > 0)
	{
		Pmin.Set(1, -range);
		Pmax.Set(1,range);
		Pmin.Set(2, -range);
		Pmax.Set(2,range);
	}

	for (int i = 0; i< CurrFile->GetnPlots(); i++)
	{
		CurrFile->GetPlot(i)->SetAutoRange(0);
		CurrFile->GetPlot(i)->SetFixedRanges(Pmin, Pmax);
		CurrFile->GetPlot(i)->SetAxisTitle("Echo Intensity (mV)",1);
		CurrFile->GetPlot(i)->SetAxisTitle("Echo Intensity (mV)",2);
		CurrFile->GetPlot(i)->SetAxisTitle("Acquisition Delay (microsec)",0); // Set X-axis title
	}

	CurrFile->PlotData();
/*	CurrFile->GetPlot()->SetAutoRange(0);
	CurrFile->GetPlot()->SetFixedRanges(Pmin, Pmax);
	CurrFile->GetPlot()->SetAutoY1();
	CurrFile->GetPlot()->SetAutoY2();
	CurrFile->GetPlot()->SetAxisTitle("Time (microsec)",0);
	CurrFile->GetPlot()->SetAxisTitle("X-signal (mV)",1);
	CurrFile->GetPlot()->SetAxisTitle("Y-signal (mV)",2);

	CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
*/	CurrFile->AllDataReset(4,TotalPoints);

	time_t timer = time(NULL);  // get the time and date

	CurrFile->SetHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));
	char line[128];
	sprintf(line,"Pulsed measurement \n");
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse A: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P1DelayEdit->Text.c_str(),
		PulsedDialog->P1DelayStepEdit->Text.c_str(),
		PulsedDialog->P1WidthEdit->Text.c_str(),
		PulsedDialog->P1WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse B: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P2DelayEdit->Text.c_str(),
		PulsedDialog->P2DelayStepEdit->Text.c_str(),
		PulsedDialog->P2WidthEdit->Text.c_str(),
		PulsedDialog->P2WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse C: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P3DelayEdit->Text.c_str(),
		PulsedDialog->P3DelayStepEdit->Text.c_str(),
		PulsedDialog->P3WidthEdit->Text.c_str(),
		PulsedDialog->P3WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse D: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns (amplifier protection)\n",
		PulsedDialog->P4DelayEdit->Text.c_str(),
		PulsedDialog->P4DelayStepEdit->Text.c_str(),
		PulsedDialog->P4WidthEdit->Text.c_str(),
		PulsedDialog->P4WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse E is driving the microwave switch, the sum of A, B, C \n");
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse F (Scope Trig): delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P6DelayEdit->Text.c_str(),
		PulsedDialog->P6DelayStepEdit->Text.c_str(),
		PulsedDialog->P6WidthEdit->Text.c_str(),
		PulsedDialog->P6WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse G (RF1): delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P7DelayEdit->Text.c_str(),
		PulsedDialog->P7DelayStepEdit->Text.c_str(),
		PulsedDialog->P7WidthEdit->Text.c_str(),
		PulsedDialog->P7WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse H (RF2): delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P8DelayEdit->Text.c_str(),
		PulsedDialog->P8DelayStepEdit->Text.c_str(),
		PulsedDialog->P8WidthEdit->Text.c_str(),
		PulsedDialog->P8WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"RepTime: %s microsec, Acq.delay step %s ns, Acq Gate %s ns, %s Shots/point \n",
		PulsedDialog->RateEdit->Text.c_str(),
		PulsedDialog->AcqStepEdit->Text.c_str(),
		PulsedDialog->IntTimeEdit->Text.c_str(),
		PulsedDialog->AverEdit->Text.c_str());
	CurrFile->AddHeader(line);

	AddInstrumentHeader();

	int mmode = 0;

	if (PulsedDialog->QuadratureCheckBox->Checked) mmode =1;

	if (Tek7->GetAddress() > 0)
	{
		Tek7->RetrieveScaleSettings(mmode);
		Tek7->SetDelay((double)PulsedDialog->AcqDelayEdit->Text.ToInt()*1e-9);
		Tek7->Run();
	} else
	  {
		Infinium->RetrieveScaleSettings();
		Infinium->SetDelay((double)PulsedDialog->AcqDelayEdit->Text.ToInt()*1e-9);
		Infinium->Run();
	  }
	TakingData = 1;
	CurrFile->Measuring = true;
	StopMeasurement = 0;
	UpdateMenuItems(this);
	MeasPointFrm->Show();

//   Magnet->SetField(EPRMainSweepDialog->GetStartField());
//   Magnet->SetAction(1);
	int millisec_pp = 4;  //scope does not average faster

	if (mmode ==1)
	{
		millisec_pp = 16;  //scope does not average faster in uqadrature mode
		if (PulsedDialog->RateEdit->Text.ToInt() > 16000)
			millisec_pp = ceil(PulsedDialog->RateEdit->Text.ToInt()/1000);
	}
	  else if (PulsedDialog->RateEdit->Text.ToInt() > 4000)
	  {
		millisec_pp = ceil(PulsedDialog->RateEdit->Text.ToInt()/1000);
	  }
	MeasurePulsTimer->Interval = ceil(PulsedDialog->AverEdit->Text.ToInt() * millisec_pp);
	MeasurePulsTimer->Enabled = true;

}

void TMainForm::PulsedEndorPrepare()
{
	CurrFile = dynamic_cast<TDataFile*> (ActiveMDIChild); // GetDocumentPointer

//  Test if the file is empty, if not display message
//  and return
	if (!CurrFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		CurrFile = NULL;  // Reset CurrFile  to NULL on end of measurement
		return;
	}

	CurrentSweep = 0;  // for averaging purposes
	CurrentPoint = 0;  // npoints in sweep

	Averages = 1;      // Default averages
	int av;


	T1[0] = PulsedDialog->AcqDelayEdit->Text.ToInt()*1e-9;
	T2[0] = T1[0] + PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;
	T1[1] = T1[0] + 2*PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;
	T2[1] = T1[1] + 2*PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;

	NumberOfGates = 2;

// Get the parameters
	ENDORParametersDialog->ShowModal();
	if (ENDORParametersDialog->ModalResult != mrOk) return;

// Set StartFrequency
	RFsource->SetFreq(ENDORParametersDialog->GetStartFreq());
// Set StepSize
	RFsource->SetStep(ENDORParametersDialog->GetStep());
// Set Power ON
	RFsource->SetPower(ENDORParametersDialog->GetPower());

	RF2->SetFreq(ENDORParametersDialog->GetRF2StartFreq());
	RF2->SetStep(ENDORParametersDialog->GetRF2Step());
	RF2->SetPower(ENDORParametersDialog->GetRF2Power());
	MeasurementType = 1;

	Averages = ENDORParametersDialog->AverageEdit->Text.ToInt();
	if (ENDORParametersDialog->Two_D_CheckBox->Checked) {
		   Averages = ENDORParametersDialog->RF2NStepsEdit->Text.ToInt();
	}
	TotalPoints = ENDORParametersDialog->NStepsEdit->Text.ToInt() + 1;
	if ((Averages>1) && (Averages <1001)) {
		CurrFile->SetAverage(1);
	}
	CurrFile->AllDataReset(4, TotalPoints);  //Reset Data-arrays for correct number of y-values
	DataPoint Pmin(4);      // DataPoints for PlotLimits
	DataPoint Pmax(4);

	CurrentSweep = 1;
	CurrentPoint = 0;

	Pmin.Set(0,ENDORParametersDialog->GetStartFreq());
	Pmax.Set(0,ENDORParametersDialog->GetEndFreq());
	double range = LockIn1->RealSensitivity();
	if (range > 0)
	{
		Pmin.Set(1, -range);
		Pmax.Set(1,range);
		Pmin.Set(2, -range);
		Pmax.Set(2,range);
	}

	for (int i = 0; i< CurrFile->GetnPlots(); i++)
	{
		CurrFile->GetPlot(i)->SetAutoRange(0);
		CurrFile->GetPlot(i)->SetFixedRanges(Pmin, Pmax);
		CurrFile->GetPlot()->SetAxisTitle("Frequency (MHz)",0);
		CurrFile->GetPlot()->SetAxisTitle("X-signal (mV)",1);
		CurrFile->GetPlot()->SetAxisTitle("Y-signal (mV)",2);
	}
	CurrFile->PlotData();
		//    CurrFile->DataReset(2,TotalPoints);

	CurrFile->Invalidate(); //replot after changing axes

//	CurrFile->DataReset(4,TotalPoints);

	time_t timer = time(NULL);  // get the time and date

	CurrFile->SetHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));
	char line[128];
	sprintf(line,"ENDOR measurement %8.3f to %8.3f MHz in %d steps of %8.3f kHz \n",
		ENDORParametersDialog->GetStartFreq(),ENDORParametersDialog->GetEndFreq(),
		  TotalPoints, ENDORParametersDialog->GetStep());
	CurrFile->AddHeader(line);
	sprintf(line,"RF power %8.3f dB, dwelltime %d ms, %d averages \n",
		ENDORParametersDialog->GetPower(), ENDORParametersDialog->DwellEdit->Text.ToInt(),
		Averages);
	CurrFile->AddHeader(line);
	sprintf(line,"RF2 %8.3f to %8.3f MHz in steps of %8.3f kHz \n",
		ENDORParametersDialog->GetRF2StartFreq(),ENDORParametersDialog->GetRF2EndFreq(),
		  ENDORParametersDialog->GetRF2Step());
	CurrFile->AddHeader(line);
	if (ENDORParametersDialog->Two_D_CheckBox->Checked) {
		sprintf(line,"2D Experiment with RF2 power %8.3f dB \n", ENDORParametersDialog->GetRF2Power());
		}  else sprintf(line,"RF2 power %8.3f dB \n", ENDORParametersDialog->GetRF2Power());
	CurrFile->AddHeader(line);

	sprintf(line,"Modulation FM %8.3f kHz, modulation AM %8.3f percent \n",
		ENDORParametersDialog->FMEdit->Text.ToDouble(),
		ENDORParametersDialog->AMEdit->Text.ToDouble());
	CurrFile->AddHeader(line);

	sprintf(line,"Pulsed measurement \n");
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse A: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P1DelayEdit->Text.c_str(),
		PulsedDialog->P1DelayStepEdit->Text.c_str(),
		PulsedDialog->P1WidthEdit->Text.c_str(),
		PulsedDialog->P1WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse B: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P2DelayEdit->Text.c_str(),
		PulsedDialog->P2DelayStepEdit->Text.c_str(),
		PulsedDialog->P2WidthEdit->Text.c_str(),
		PulsedDialog->P2WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse C: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P3DelayEdit->Text.c_str(),
		PulsedDialog->P3DelayStepEdit->Text.c_str(),
		PulsedDialog->P3WidthEdit->Text.c_str(),
		PulsedDialog->P3WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse D: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P4DelayEdit->Text.c_str(),
		PulsedDialog->P4DelayStepEdit->Text.c_str(),
		PulsedDialog->P4WidthEdit->Text.c_str(),
		PulsedDialog->P4WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse E is driving the microwave switch, the sum of A, B, C, and D");
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse F (Scope Trig): delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P6DelayEdit->Text.c_str(),
		PulsedDialog->P6DelayStepEdit->Text.c_str(),
		PulsedDialog->P6WidthEdit->Text.c_str(),
		PulsedDialog->P6WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse G: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P7DelayEdit->Text.c_str(),
		PulsedDialog->P7DelayStepEdit->Text.c_str(),
		PulsedDialog->P7WidthEdit->Text.c_str(),
		PulsedDialog->P7WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse H (Prot): delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P8DelayEdit->Text.c_str(),
		PulsedDialog->P8DelayStepEdit->Text.c_str(),
		PulsedDialog->P8WidthEdit->Text.c_str(),
		PulsedDialog->P8WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"RepTime: %s microsec, Acq.delay step %s ns, Acq Gate %s ns, %s Shots/point \n",
		PulsedDialog->RateEdit->Text.c_str(),
		PulsedDialog->AcqStepEdit->Text.c_str(),
		PulsedDialog->IntTimeEdit->Text.c_str(),
		PulsedDialog->AverEdit->Text.c_str());
	CurrFile->AddHeader(line);
//	sprintf(line,"Modulation FM %8.3f kHz, modulation AM %8.3f percent \n",
//		ENDORParametersDialog->FMEdit->Text.ToDouble(),
//		ENDORParametersDialog->AMEdit->Text.ToDouble());
//	CurrFile->AddHeader(line);

	AddInstrumentHeader();

	TakingData = 1;
	CurrFile->Measuring = true;
	StopMeasurement = 0;
	UpdateMenuItems(this);
	MeasPointFrm->Show();

//   Magnet->SetField(EPRMainSweepDialog->GetStartField());
//   Magnet->SetAction(1);
//	MeasureENDORTimer->Interval = ENDORParametersDialog->DwellEdit->Text.ToInt();
//	MeasureENDORTimer->Enabled = true;

// Set StartFrequency

	int delay[8];
	int delaystep[8];
	int width[8];
	int widthstep[8];
	int IntegrationWidth;
	int AcqDelay;
	int Rate;
	int PulsAver;

	delay[0] = PulsedDialog->P1DelayEdit->Text.ToInt();
	delay[1] = PulsedDialog->P2DelayEdit->Text.ToInt();
	delay[2] = PulsedDialog->P3DelayEdit->Text.ToInt();
	delay[3] = PulsedDialog->P4DelayEdit->Text.ToInt();
	delay[4] = PulsedDialog->P5DelayEdit->Text.ToInt();
	delay[5] = PulsedDialog->P6DelayEdit->Text.ToInt();
	delay[6] = PulsedDialog->P7DelayEdit->Text.ToInt();
	delay[7] = PulsedDialog->P8DelayEdit->Text.ToInt();

	width[0] = PulsedDialog->P1WidthEdit->Text.ToInt();
	width[1] = PulsedDialog->P2WidthEdit->Text.ToInt();
	width[2] = PulsedDialog->P3WidthEdit->Text.ToInt();
	width[3] = PulsedDialog->P4WidthEdit->Text.ToInt();
	width[4] = PulsedDialog->P5WidthEdit->Text.ToInt();
	width[5] = PulsedDialog->P6WidthEdit->Text.ToInt();
	width[6] = PulsedDialog->P7WidthEdit->Text.ToInt();
	width[7] = PulsedDialog->P8WidthEdit->Text.ToInt();

	for (int i=0; i < 8; i++) {
			PulseSource->Set(i+1, delay[i], width[i]);
	}

	MeasurementType = 1;

//	Averages = PulsedDialog->ScanEdit->Text.ToInt();
//	TotalPoints = PulsedDialog->NptsEdit->Text.ToInt() + 1;
//	CurrentSweep = 1;
//	CurrentPoint = 0;



//	Pmin.Set(0,0.0);
//	Pmax.Set(0,(double)TotalPoints*(double)PulsedDialog->AcqStepEdit->Text.ToInt()/1000.0);
//	double range = 0.1;
//	if (range > 0)
//	{
//		Pmin.Set(1, -range);
//		Pmax.Set(1,range);
//		Pmin.Set(2, -range);
//		Pmax.Set(2,range);
//	}

//	CurrFile->GetPlot()->SetAutoRange(0);
//	CurrFile->GetPlot()->SetFixedRanges(Pmin, Pmax);
//	CurrFile->GetPlot()->SetAutoY1();
//	CurrFile->GetPlot()->SetAutoY2();
//	CurrFile->GetPlot()->SetAxisTitle("Freq (MHz)",0);
//	CurrFile->GetPlot()->SetAxisTitle("X-signal (mV)",1);
//	CurrFile->GetPlot()->SetAxisTitle("Y-signal (mV)",2);

//	CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
//	CurrFile->DataReset(4,TotalPoints);

//	time_t timer = time(NULL);  // get the time and date

//	CurrFile->SetHeader("Measurement made ");
//	CurrFile->AddHeader(asctime(localtime(&timer)));
//	char line[128];

//	AddInstrumentHeader();
	int mathmode = 0;
	if (PulsedDialog->QuadratureCheckBox->Checked) mathmode = 1;
	if (Tek7->GetAddress() > 0)
	{
		Tek7->RetrieveScaleSettings(mathmode);
		Tek7->SetDelay((double)PulsedDialog->AcqDelayEdit->Text.ToInt()*1e-9);
		Tek7->Run();
	} else
	  {
		Infinium->RetrieveScaleSettings();
		Infinium->SetDelay((double)PulsedDialog->AcqDelayEdit->Text.ToInt()*1e-9);
		Infinium->Run();
	  }
	TakingData = 1;
	CurrFile->Measuring = true;
	StopMeasurement = 0;
	UpdateMenuItems(this);
	MeasPointFrm->Show();

//   Magnet->SetField(EPRMainSweepDialog->GetStartField());
//   Magnet->SetAction(1);
	int millisec_pp = 4;
	if (mathmode ==1)
	{
		millisec_pp = 16;  //scope does not average faster in uqadrature mode
		if (PulsedDialog->RateEdit->Text.ToInt() > 16000)
			millisec_pp = ceil(PulsedDialog->RateEdit->Text.ToInt()/1000);
	}
	  else if (PulsedDialog->RateEdit->Text.ToInt() > 4000)
	  {
		millisec_pp = ceil(PulsedDialog->RateEdit->Text.ToInt()/1000);
	  }
	PulsedEndorTimer->Interval = ceil(PulsedDialog->AverEdit->Text.ToInt() * millisec_pp);
	PulsedEndorTimer->Enabled = true;

}


void __fastcall TMainForm::MeasureENDORTimerTimer(TObject *Sender)
{
//  parameters
	double Freq, x1,x2,y1,y2;
	double EndF = ENDORParametersDialog->GetEndFreq();

	MeasureENDORTimer->Enabled = false;

	DataPoint P(4);

	LockIn1->ReadXY(&x1,&y1);
	LockIn2->ReadXY(&x2,&y2);
	P.Set(1,x1);
	P.Set(2,x2);
	P.Set(3,y1);
	P.Set(4,y2);
	Freq = RFsource->GetFreq();
	P.Set(0, Freq);

	MeasPointFrm->XLabel->Caption = Freq;
//    MeasPointFrm->Y1Label->Caption = x1;
//    MeasPointFrm->Y2Label->Caption = y1;
	MeasPointFrm->PointLabel->Caption = CurrentPoint;


	char averLabel[20];
	sprintf(averLabel,"%d/%d",CurrentSweep, Averages);
	MeasPointFrm->AverageLabel->Caption = averLabel;

	DataArray* D = CurrFile->GetData();
	if (TakingData)
	{
		D->AddToAverage(CurrentPoint, CurrentSweep,P);
		CurrentPoint += 1;

		if (CurrentPoint > 1)
		CurrFile->GetPlot()->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
	}

	if ((CurrentPoint >= TotalPoints) && (CurrentSweep < Averages))
	{
		CurrentPoint = 0;
		RFsource->SetFreq(ENDORParametersDialog->GetStartFreq());
		CurrentSweep += 1;
		MeasureENDORTimer->Enabled = true;
		return;
	}

	if ((CurrentPoint >= TotalPoints) && (CurrentSweep >= Averages))
		StopMeasurement = 1;

   if (StopMeasurement == 1)
   {
		TakingData = 0;
		CurrFile->Measuring = false;
		StopMeasurement =1;
		UpdateMenuItems(this);
		MeasPointFrm->Close();
		return;
   }
	else
   {
	  if (TakingData) RFsource->Step();
	  MeasureENDORTimer->Enabled = true;
   }

   UpdateMenuItems(this);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AutoX1Click(TObject *Sender)
{
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	for (int i=0; i<MyFile->GetnPlots(); i++)
		MyFile->GetPlot(i)->SetAutoX();
	MyFile->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AutoY11Click(TObject *Sender)
{
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	for (int i=0; i<MyFile->GetnPlots(); i++)
		MyFile->GetPlot(i)->SetAutoY1();
	MyFile->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AutoY21Click(TObject *Sender)
{
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	for (int i=0; i<MyFile->GetnPlots(); i++)
		MyFile->GetPlot(i)->SetAutoY2();
	MyFile->Invalidate();

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::StartMeasureButtonClick(TObject *Sender)
{
	TakingData = 1;
	UpdateMenuItems(this);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PauzeMeasureButtonClick(TObject *Sender)
{
	TakingData = 0;
	UpdateMenuItems(this);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StopMeasureButtonClick(TObject *Sender)
{
    StopMeasurement = 1;
	Interrupted = true;
	UpdateMenuItems(this);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RePlotMenuItemClick(TObject *Sender)
{
	ActiveMDIChild->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GoToStartFieldTimerTimer(TObject *Sender)
{
   if (StopMeasurement == 1)   // if interrupted
   {
		Magnet->SetAction(0);
		TakingData = 0;
		CurrFile->Measuring = false;
		StopMeasurement = 1;
		UpdateMenuItems(this);
		MeasPointFrm->Close();
		CurrFile = NULL;
		GoToStartFieldTimer->Enabled = false;
		return;
   }
   if ((Magnet->Sweeping()) && (Magnet->GetAddress() != 0))
   {
		char line[40];
		sprintf(line, "Sweeping Magnet... %7.4f T",Magnet->GetField());
		MeasPointFrm->StatusLabel->Caption = line;
		return;
   }
   // If not sweeping any more
//   GoToStartFieldTimer->Enabled = false;
   if (((!ReturnSweep) && (fabs(Magnet->GetField() - EPRMainSweepDialog->GetStartField()))<=3.0e-4)
	|| ((ReturnSweep) && (fabs(Magnet->GetField() - EPRMainSweepDialog->GetEndField()))<3.0e-4)
	 || (Magnet->GetAddress() == 0))
   {
	   GoToStartFieldTimer->Enabled = false;
	   Magnet->SetFieldSweepSpeed(EPRMainSweepDialog->GetSweepSpeed());
	   if (ReturnSweep) Magnet->SetField(EPRMainSweepDialog->GetStartField());
		 else Magnet->SetField(EPRMainSweepDialog->GetEndField());
	   Magnet->SetAction(1);
	   TakingData = 1;
	   CurrFile->Measuring = true;
	   CurrentPoint = 0;
	   if (!ReturnSweep) CurrentSweep++;
	   time0 = GetTickCount();
 	   EPRMeasureTimer->Enabled = true;

	   MeasPointFrm->StatusLabel->Caption = "Taking data...";
//	   EPRMeasureTimerTimer(this);
   }
	else return;
   time0 = GetTickCount();

   int* chan = NULL;
   int nch;
   if ( (nch = DAQ->GetnActiveChannels()) > 0)
   {
		DAQ->Arm();
		DAQ->Trigger();
   }


}
//---------------------------------------------------------------------------
//
//      EPRMeasureTimerTimer
//
//    This is the main routine for a main field-sweep measurement of any type
//
//    Important parameters
//      MeasurementType
//                2: Reads LockIn 1, both X and Y channels    (CW-EPR)
//                3: Reads Boxcar                             (TR EPR 1)
//                4: Reads Infinium Gates (1 channel)         (TR EPR 2)
//                5: Reads Infinium Gates (channel 1 and 3)   (Het TR-EPR)
//                6: Like 5, but adds a lockin channel (ref)  (Het TR-EPR)
//               10: Reads X and Y channels of two lock-ins   (Het EPR)
//
void __fastcall TMainForm::EPRMeasureTimerTimer(TObject *Sender)
{
	EPRMeasureTimer->Enabled = false;                   // Set timer off
	int ok;
	int millis, delay1, delay2;
	double field;
	double baselineCorrected;
	double fastspeed = 2.0;                             // To be revisited

	char errorr[80];
	DataPoint *P1;
	char TempFileName[255];
	if (MeasurementType == 4) NumberOfMeas = NumberOfGates;
	if ((MeasurementType == 5) || (MeasurementType == 7))
	{
		if (PulsedDialog->TwoChannelCheckBox->Checked)
			NumberOfMeas = 2 * NumberOfGates;
		  else
			NumberOfMeas = NumberOfGates;
		P1 = new DataPoint(NumberOfGates);
	}
	if (MeasurementType == 6)
	{
		NumberOfMeas = 2 * NumberOfGates+1;
		P1 = new DataPoint(NumberOfGates);
	}
	DataPoint P(NumberOfMeas);
	int nDaqMeas = 0;
	if (DAQ->IsOpen())
	{
	   nDaqMeas = DAQ->GetnActiveChannels();
	}
	double* DAQresult = NULL;
	if (nDaqMeas > 0) DAQresult = new double[nDaqMeas];

	double x1,y1,x2,y2;

//  Do the measurement

	switch (MeasurementType)
	{
		case 2:
			LockIn1->ReadXY(&x1,&y1);
			P.Set(1,x1);
			P.Set(2,y1);
			break;
		case 3:
			P.Set(2,Boxcar->Get(1));
			P.Set(1,Boxcar->Get(2)-P.Get(2));
			break;
		case 4:
			if (Tek7->GetAddress() > 0)
				ok = Tek7->GatedValues(NumberOfGates,&P,T1,T2);
			  else Infinium->GatedValues(NumberOfGates,&P,T1,T2);
			if (ok == -2) //if aquisition unfinished
			{
				EPRMeasureTimer->Enabled = true;
				return;
			}
			break;
		case 5:
			if (Tek7->GetAddress() > 0)
			{
				Tek7->Stop();
//            millis = GetTickCount();
				Tek7->RetrieveScaleSettings(0);
				ok = Tek7->RetrieveBuffer(1);
//            delay1 = GetTickCount()-millis;
				if (PulsedDialog->TwoChannelCheckBox->Checked) {
					Tek7->RetrieveScaleSettings(0);
					ok = Tek7->RetrieveBuffer(3);
				}
//            delay2 = GetTickCount()-millis;
				Tek7->Run();
				if (Tek7->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
				if (ok==-2)
				{
					EPRMeasureTimer->Enabled = true;
					return; // if aquisition unfinished return
				}
		 //   Infinium->Stop();
//				Tek7->RetrieveScaleSettings(0);
				ok = Tek7->GetGateValues(NumberOfGates,P1,T1,T2,1);
				if ((NumberOfGates ==2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
				{
					baselineCorrected = P1->Get(1) - P1->Get(2);
					P1->Set(1, baselineCorrected);
				}

				for (int i=1;i<=NumberOfGates;i++)
					P.Set(i, P1->Get(i));
				if (PulsedDialog->TwoChannelCheckBox->Checked)
					ok = Tek7->GetGateValues(NumberOfGates,P1,T1,T2,3);
				if (Tek7->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
			  }
			  else
			  {
				Infinium->Stop();
//            millis = GetTickCount();
				ok = Infinium->RetrieveBuffer(1);
//            delay1 = GetTickCount()-millis;
				ok = Infinium->RetrieveBuffer(3);
//            delay2 = GetTickCount()-millis;
				Infinium->Run();
				if (Infinium->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
				if (ok==-2)
				{
					EPRMeasureTimer->Enabled = true;
					return; // if aquisition unfinished return
				}
			 //   Infinium->Stop();

				ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,1);
//				if ((NumberOfGates ==2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
//					P1->Set(1, P1->Get(1) - P1->Get(2));
				for (int i=1;i<=NumberOfGates;i++)
					P.Set(i, P1->Get(i));
				ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,3);
				if (Infinium->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
			}
			if (PulsedDialog->TwoChannelCheckBox->Checked)
			{
				if ((NumberOfGates == 2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
				{
					baselineCorrected = P1->Get(1) - P1->Get(2);
					P1->Set(1, baselineCorrected);
				}
				for (int i=1;i<=NumberOfGates;i++)
					P.Set(NumberOfGates+i, P1->Get(i));
            }
			delete P1;
//            Infinium->Clear();
		 //   Infinium->Run();
			break;
		case 6:
			if (Tek7->GetAddress() > 0)
			{
				Tek7->Stop();
//            millis = GetTickCount();
				ok = Tek7->RetrieveBuffer(1);
//            delay1 = GetTickCount()-millis;
				ok = Tek7->RetrieveBuffer(3);
//            delay2 = GetTickCount()-millis;
				Tek7->Run();
				if (Tek7->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
				if (ok==-2)
				{
					EPRMeasureTimer->Enabled = true;
					return; // if aquisition unfinished return
				}
		 //   Infinium->Stop();
				P.Set(1,LockIn1->ReadOutput(1));

				ok = Tek7->GetGateValues(NumberOfGates,P1,T1,T2,1);
				for (int i=1;i<=NumberOfGates;i++)
					P.Set(i+1, P1->Get(i));
				ok = Tek7->GetGateValues(NumberOfGates,P1,T1,T2,3);
				if (Tek7->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
			  }
			  else
			  {
				Infinium->Stop();
//            millis = GetTickCount();
				ok = Infinium->RetrieveBuffer(1);
//            delay1 = GetTickCount()-millis;
				ok = Infinium->RetrieveBuffer(3);
//            delay2 = GetTickCount()-millis;
				Infinium->Run();
				if (Infinium->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
				if (ok==-2)
				{
					EPRMeasureTimer->Enabled = true;
					return; // if aquisition unfinished return
				}
			 //   Infinium->Stop();
				P.Set(1,LockIn1->ReadOutput(1));

				ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,1);
//				if ((NumberOfGates ==2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
//					P1->Set(1, P1->Get(1) - P1->Get(2));
				for (int i=1;i<=NumberOfGates;i++)
					P.Set(i+1, P1->Get(i));
				ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,3);
				if (Infinium->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
			}
//			if ((NumberOfGates ==2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
//				P1->Set(1, P1->Get(1) - P1->Get(2));
			for (int i=1;i<=NumberOfGates;i++)
				P.Set(NumberOfGates+i+1, P1->Get(i));
			delete P1;
//            Infinium->Clear();
		 //   Infinium->Run();
			break;

		case 7:
			if (Tek7->GetAddress() > 0)
			{
				Tek7->Stop();
//            millis = GetTickCount();
				ok = Tek7->RetrieveBuffer(9);
//            delay1 = GetTickCount()-millis;
				if (PulsedDialog->TwoChannelCheckBox->Checked)
					 ok = Tek7->RetrieveBuffer(10);
//            delay2 = GetTickCount()-millis;
//				Tek7->Run();
				if (Tek7->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
				if (ok==-2)
				{
					EPRMeasureTimer->Enabled = true;
					return; // if aquisition unfinished return
				}
		 //   Infinium->Stop();
				Tek7->RetrieveScaleSettings(1);
				Tek7->Run();

				ok = Tek7->GetGateValues(NumberOfGates,P1,T1,T2,9);
				if ((NumberOfGates == 2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
				{
					baselineCorrected = P1->Get(1) - P1->Get(2);
					P1->Set(1, baselineCorrected);
				}

				for (int i=1;i<=NumberOfGates;i++)
					P.Set(i, P1->Get(i));
				if (PulsedDialog->TwoChannelCheckBox->Checked)
					ok = Tek7->GetGateValues(NumberOfGates,P1,T1,T2,10);
				if ((NumberOfGates == 2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
				{
					baselineCorrected = P1->Get(1) - P1->Get(2);
					P1->Set(1, baselineCorrected);
				}
				if (Tek7->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
			  }
			  else
			  {
				Infinium->Stop();
//            millis = GetTickCount();
				ok = Infinium->RetrieveBuffer(9);
//            delay1 = GetTickCount()-millis;
				if (PulsedDialog->TwoChannelCheckBox->Checked)
					ok = Infinium->RetrieveBuffer(10);
//            delay2 = GetTickCount()-millis;
				Infinium->Run();
				if (Infinium->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
				if (ok==-2)
				{
					EPRMeasureTimer->Enabled = true;
					return; // if aquisition unfinished return
				}
			 //   Infinium->Stop();

				ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,9);
				for (int i=1;i<=NumberOfGates;i++)
					P.Set(i, P1->Get(i));
					if (PulsedDialog->TwoChannelCheckBox->Checked)
						ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,10);
				if (Infinium->ErrorTest(errorr) != 0)
				{
					ErrorBox->ErrorMessage->Caption = errorr;
					ErrorBox->ShowModal();
				}
			}
			if (PulsedDialog->TwoChannelCheckBox->Checked)
			{
				for (int i=1;i<=NumberOfGates;i++)
					P.Set(NumberOfGates+i, P1->Get(i));
			}
			delete P1;
//            Infinium->Clear();
		 //   Infinium->Run();
			break;

		case 10:
			 LockIn1->ReadXY(&x1,&y1);
			 LockIn2->ReadXY(&x2,&y2);
			 if (nDaqMeas >0)
			 {
				DAQ->WaitToComplete(2);
				DAQ->GetValues(nDaqMeas, DAQresult);
				DAQ->Arm();

				for (int i = 0; i<nDaqMeas; i++)
					P.Set(5+i,DAQresult[i]);
				DAQ->Trigger();
			 }

			 P.Set(1,x1);
			 P.Set(2,x2);
			 P.Set(3,y1);
			 P.Set(4,y2);
//            P.Set(1,LockIn1->ReadOutput(1));
//            P.Set(2,LockIn2->ReadOutput(1));
//            P.Set(3,LockIn1->ReadOutput(2));
//            P.Set(4,LockIn2->ReadOutput(2));
			break;
		default:
			LockIn1->ReadXY(&x1,&y1);
			P.Set(1,x1);
			P.Set(2,y1);
		break;
	}

	bool up;
	up = ((EPRMainSweepDialog->GetEndField()-EPRMainSweepDialog->GetStartField()) > 0);
	if (ReturnSweep) up = !(up);

	double HysteresisCorrection = 0.0;
	double Hyst = Magnet->GetHysteresis();
	double seconds = 0.001*(double)(GetTickCount() - time0);
	if (EPRMainSweepDialog->HysteresisCorrectionButton->Checked)
	{
		HysteresisCorrection = Hyst *0.001*EPRMainSweepDialog->GetSweepSpeed();
		if (seconds < 20*Hyst) HysteresisCorrection *= (1-exp(-seconds/Hyst));

		if (up) HysteresisCorrection *= -1;
	}

//  Now read the field or calculate it from the time
	if (EPRMainSweepDialog->FieldModeCheckBox->Checked)
	{      // Calc field from time to speed up measurement
		if (!(ReturnSweep)) field = EPRMainSweepDialog->GetStartField();
		   else field = EPRMainSweepDialog->GetEndField();

		if (up)
			field += ((GetTickCount()-time0)*1.0e-6)*EPRMainSweepDialog->GetSweepSpeed();
		else
			field -= ((GetTickCount()-time0)*1.0e-6)*EPRMainSweepDialog->GetSweepSpeed();
		P.Set(0, field + HysteresisCorrection);
	}
	  else P.Set(0,Magnet->GetField() + HysteresisCorrection);
		   // Read field directly

//  Now DataPoint P contains the measurement
    AnsiString FormStr = "000.0000";
    MeasPointFrm->XLabel->Caption = FloatToStrF(P.Get(0), ffGeneral, 8, 0);
//    MeasPointFrm->Y1Label->Caption.FormatFloat(FormStr,P.Get(1)*1000.0);
//    MeasPointFrm->Y2Label->Caption = P.Get(2);
	MeasPointFrm->PointLabel->Caption = CurrentPoint;

    char averLabel[20];
    sprintf(averLabel,"%d/%d",CurrentSweep, Averages);
    MeasPointFrm->AverageLabel->Caption = averLabel;

//  Add the point to the DataArray and plot it
    DataArray* D = NULL;
    if ((ReturnSweep) && (Averages > 1))
        D = CurrFile->GetData(2);
	  else
        if (ReturnSweep) D = CurrFile->GetData(1);
          else D = CurrFile->GetData(0);

	if (TakingData)
	{
        D->Add(P);
        CurrentPoint++;

		if (CurrentPoint > 1)
		  if ((ReturnSweep) && (Averages > 1))
		CurrFile->GetPlot(2)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
			 else
				if (ReturnSweep)
			CurrFile->GetPlot(1)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
				 else
					 CurrFile->GetPlot(0)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
	}




//  See if we are at the end of the sweep
	if (EPRMainSweepDialog->FieldModeCheckBox->Checked)
	{
		if (((GetTickCount()-time0)*1.0e-3) > EPRMainSweepDialog->GetSweepDuration())
			StopMeasurement = 1;
	}
	   else  if (!(Magnet->Sweeping()))  StopMeasurement = 1;

//  On end of sweep
   if (StopMeasurement == 1)
   {
		Magnet->SetAction(0);                      // Set Magnet to HOLD
		TakingData = 0;
		CurrFile->Measuring = false;
		if ((EPRMainSweepDialog->BackSweepRadioButton->Checked == true) && (!Interrupted))
		{
			Magnet->SetFieldSweepSpeed(fastspeed);
			Magnet->SetField(EPRMainSweepDialog->GetStartField());
			Magnet->SetAction(1); //Goto SetPoint
		}
		if ((nDaqMeas > 0) && (MeasurementType ==10)) DAQ->WaitToComplete();
		UpdateMenuItems(this);
		if (Averages > 1)
		{
			DataArray* AD;
			if (!ReturnSweep) AD = CurrFile->GetData(1);
			   else AD = CurrFile->GetData(3);
			if (CurrentSweep == 1)
			{
				*AD = D->ReSample(EPRMainSweepDialog->GetStartField(),
					EPRMainSweepDialog->GetEndField(), EPRMainSweepDialog->GetAverNpts());
			}
			  else
			  {
				DataArray TempDat = D->ReSample(EPRMainSweepDialog->GetStartField(),
					EPRMainSweepDialog->GetEndField(), EPRMainSweepDialog->GetAverNpts());
				AD->AddToAverage(CurrentSweep, &TempDat);
			}
			CurrFile->PlotData();
			if (up)
			{
				sprintf(TempFileName,"C:\\TempData\\up%03d.dat",CurrentSweep);
				if (EPRMainSweepDialog->SaveSweepsCheckBox->Checked)
										CurrFile->WriteData(0,TempFileName);
			}
			  else
				{
					sprintf(TempFileName,"C:\\TempData\\down%03d.dat",CurrentSweep);
					if (EPRMainSweepDialog->SaveSweepsCheckBox->Checked)
										CurrFile->WriteData(2,TempFileName);
				}

			if (Interrupted)
			{
				MeasPointFrm->Close();
				CurrFile = NULL;
				Interrupted = false;
				return;
			}

			if ((CurrentSweep < Averages) && (!CurrFile->GetUpDown()))
			{
				CurrFile->GetData(0)->flush();
				Magnet->SetFieldSweepSpeed(fastspeed);
				Magnet->SetField(EPRMainSweepDialog->GetStartField());
				Magnet->SetAction(1); //Goto SetPoint
				GoToStartFieldTimer->Enabled = true;
				StopMeasurement = 0;
				UpdateMenuItems(this);
				return;
			}

			if ((CurrentSweep < Averages) && (CurrFile->GetUpDown()))
				if (ReturnSweep)     // If we were going back
				{
					CurrFile->GetData(0)->flush();
					Magnet->SetFieldSweepSpeed(fastspeed);
					Magnet->SetField(EPRMainSweepDialog->GetStartField());
					Magnet->SetAction(1); //Goto SetPoint
					ReturnSweep = false;
					GoToStartFieldTimer->Enabled = true;
					StopMeasurement = 0;
					UpdateMenuItems(this);
					return;
				}
				  else
				  {
					CurrFile->GetData(2)->flush();
					Magnet->SetFieldSweepSpeed(fastspeed);
					Magnet->SetField(EPRMainSweepDialog->GetEndField());
					Magnet->SetAction(1); //Goto SetPoint
					ReturnSweep = true;
					GoToStartFieldTimer->Enabled = true;
					StopMeasurement = 0;
					UpdateMenuItems(this);
					return;
				  }

		}
// if we were not averaging or at the end of the averaging cycle
		if (Interrupted)
		{
			MeasPointFrm->Close();
			CurrFile = NULL;
			Interrupted = false;
			return;
		}

		if ((CurrFile->GetUpDown()) && (!ReturnSweep))
		{
			if (Averages>1) CurrFile->GetData(2)->flush();
			  else CurrFile->GetData(1)->flush();
			Magnet->SetFieldSweepSpeed(fastspeed);
			Magnet->SetField(EPRMainSweepDialog->GetEndField());
			Magnet->SetAction(1); //Goto SetPoint
			ReturnSweep = true;
			GoToStartFieldTimer->Enabled = true;
			StopMeasurement = 0;
			UpdateMenuItems(this);
			return;
		}
		  else
		  {
			MeasPointFrm->Close();
			CurrFile = NULL;
			return;
		  }
   }
	else
   {
	  EPRMeasureTimer->Enabled = true;
   }

   UpdateMenuItems(this);
   delete [] DAQresult;
   return;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GoToSweepStartTimerTimer(TObject *Sender)
{
   if (StopMeasurement == 1)                   // if interrupted
   {
		SweepCoil->SetAction(0);
		TakingData = 0;
		CurrFile->Measuring = false;
		UpdateMenuItems(this);
		if (EPRSweepCoilDialog->BackSweepRadioButton->Checked == true)
		{
			SweepCoil->SetFieldSweepSpeed(99);
			SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
			SweepCoil->SetAction(1); //Goto SetPoint
		}
		GoToSweepStartTimer->Enabled = false; // if field is not sweeping set timer off
		MeasPointFrm->Close();
		CurrFile = NULL;
		return;
   }
   if (SweepCoil->Sweeping())
   {
		MeasPointFrm->StatusLabel->Caption = "Sweep to Start";
		return;
   }
   GoToSweepStartTimer->Enabled = false; // if field is not sweeping set timer off
	if (((!ReturnSweep) && (fabs(SweepCoil->GetField()-EPRSweepCoilDialog->GetStartField()) < 0.00002))
	   || ((ReturnSweep) && (fabs(SweepCoil->GetField()-EPRSweepCoilDialog->GetEndField()) < 0.00002)))
	{
		SweepCoil->SetFieldSweepSpeed(EPRSweepCoilDialog->GetSweepSpeed());
		if (ReturnSweep) SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
		  else SweepCoil->SetField(EPRSweepCoilDialog->GetEndField());
		TakingData = 1;
		CurrFile->Measuring = true;
		CurrentPoint = 0;
		EPRSweepMeasureTimer->Enabled = true;
		if (!ReturnSweep) CurrentSweep++;
		MeasPointFrm->StatusLabel->Caption = "Taking data";

	}
	time0 = GetTickCount();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::EPRSweepMeasureTimerTimer(TObject *Sender)
{
	EPRSweepMeasureTimer->Enabled = false;
    int ok;
    double field;
    char errorr[80];
    DataPoint *P1;
    char TempFileName[255];
    if (MeasurementType == 4) NumberOfMeas = NumberOfGates;
    if (MeasurementType == 5)
    {
        NumberOfMeas = 2 * NumberOfGates;
        P1 = new DataPoint(NumberOfGates);
    }
    if (MeasurementType == 6)
    {
        NumberOfMeas = 2 * NumberOfGates+1;
        P1 = new DataPoint(NumberOfGates);
    }
    DataPoint P(NumberOfMeas);
    double x1,y1,x2,y2;
    switch (MeasurementType)
    {
        case 2:
            LockIn1->ReadXY(&x1,&y1);
            P.Set(1,x1);
            P.Set(2,y1);
            break;
        case 3:
            P.Set(2,Boxcar->Get(1));
            P.Set(1,Boxcar->Get(2)-P.Get(2));
            break;
        case 4:
			ok = Infinium->GatedValues(NumberOfGates,&P,T1,T2,3);
			if (ok==-2)
            {
                EPRSweepMeasureTimer->Enabled = true;
                return; // if aquisition unfinished return
            }
//            Infinium->Clear();
//            Infinium->Run();
            break;
        case 5:
            Infinium->Stop();
//            millis = GetTickCount();
            ok = Infinium->RetrieveBuffer(1);
//            delay1 = GetTickCount()-millis;
            ok = Infinium->RetrieveBuffer(3);
//            delay2 = GetTickCount()-millis;
            Infinium->Run();
            if (Infinium->ErrorTest(errorr) != 0)
            {
                ErrorBox->ErrorMessage->Caption = errorr;
                ErrorBox->ShowModal();
            }
            if (ok==-2)
            {
                EPRSweepMeasureTimer->Enabled = true;
                return; // if aquisition unfinished return
            }
         //   Infinium->Stop();

            ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,1);
            for (int i=1;i<=NumberOfGates;i++)
                P.Set(i, P1->Get(i));
            ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,3);
            if (Infinium->ErrorTest(errorr) != 0)
            {
                ErrorBox->ErrorMessage->Caption = errorr;
				ErrorBox->ShowModal();
			}
            for (int i=1;i<=NumberOfGates;i++)
                P.Set(NumberOfGates+i, P1->Get(i));
            delete P1;
//            Infinium->Clear();
         //   Infinium->Run();
            break;
        case 6:
            Infinium->Stop();
//            millis = GetTickCount();
            ok = Infinium->RetrieveBuffer(1);
//            delay1 = GetTickCount()-millis;
            ok = Infinium->RetrieveBuffer(3);
//            delay2 = GetTickCount()-millis;
            Infinium->Run();
            if (Infinium->ErrorTest(errorr) != 0)
            {
                ErrorBox->ErrorMessage->Caption = errorr;
                ErrorBox->ShowModal();
            }
            if (ok==-2)
            {
                EPRSweepMeasureTimer->Enabled = true;
                return; // if aquisition unfinished return
            }
         //   Infinium->Stop();
            P.Set(1,LockIn1->ReadOutput(1));
            ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,1);
            for (int i=1;i<=NumberOfGates;i++)
                P.Set(i+1, P1->Get(i));
            ok = Infinium->GetGateValues(NumberOfGates,P1,T1,T2,3);
            if (Infinium->ErrorTest(errorr) != 0)
            {
                ErrorBox->ErrorMessage->Caption = errorr;
				ErrorBox->ShowModal();
            }
            for (int i=1;i<=NumberOfGates;i++)
				P.Set(NumberOfGates+i+1, P1->Get(i));
            delete P1;
//            Infinium->Clear();
         //   Infinium->Run();
            break;
        case 10:
             LockIn1->ReadXY(&x1,&y1);
             LockIn2->ReadXY(&x2,&y2);
             P.Set(1,x1);
             P.Set(2,x2);
             P.Set(3,y1);
             P.Set(4,y2);
//            P.Set(1,LockIn1->ReadOutput(1));
//            P.Set(2,LockIn2->ReadOutput(1));
//            P.Set(3,LockIn1->ReadOutput(2));
//            P.Set(4,LockIn2->ReadOutput(2));
             break;
        default:
            LockIn1->ReadXY(&x1,&y1);
            P.Set(1,x1);
            P.Set(2,y1);
        break;
    }

    bool up;
    up = (EPRSweepCoilDialog->GetEndField()-EPRSweepCoilDialog->GetStartField() > 0);    if (EPRSweepCoilDialog->FieldModeCheckBox->Checked)
    if (ReturnSweep) up = !(up);

    if (EPRSweepCoilDialog->FieldModeCheckBox->Checked)
    {      // Calc field from time to speed up measurement
        if (!ReturnSweep) field = EPRSweepCoilDialog->GetStartField()*1000.0;
          else field = EPRSweepCoilDialog->GetEndField()*1000.0;
		if (up)
            field += ((GetTickCount()-time0)*0.001*EPRSweepCoilDialog->GetSweepSpeed());
        else
            field -= ((GetTickCount()-time0)*0.001*EPRSweepCoilDialog->GetSweepSpeed());
        P.Set(0, field);
	}
      else P.Set(0,1000.0*SweepCoil->GetField());
           // Read field directly

//
    MeasPointFrm->XLabel->Caption = P.Get(0);
//    MeasPointFrm->Y1Label->Caption = P.Get(1);
//    MeasPointFrm->Y2Label->Caption = P.Get(2);
    MeasPointFrm->PointLabel->Caption = CurrentPoint;

    char averLabel[20];
    sprintf(averLabel,"%d/%d",CurrentSweep, Averages);
    MeasPointFrm->AverageLabel->Caption = averLabel;

    DataArray* D = NULL;

    if ((ReturnSweep) && (Averages > 1))
        D = CurrFile->GetData(2);
      else
        if (ReturnSweep) D = CurrFile->GetData(1);
          else D = CurrFile->GetData(0);

    if (TakingData)
    {
        D->Add(P);
        CurrentPoint+=1;
//        D->AddToAverage(CurrentPoint, CurrentSweep,P);

        if (CurrentPoint > 1)
          if ((ReturnSweep) && (Averages > 1))
		CurrFile->GetPlot(2)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
             else
                if (ReturnSweep)
            CurrFile->GetPlot(1)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
                 else
                     CurrFile->GetPlot(0)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
    }

//  See if we are at the end of the sweep
    if (EPRSweepCoilDialog->FieldModeCheckBox->Checked)
    {
        if (((GetTickCount()-time0)*1.0e-3) > EPRSweepCoilDialog->GetSweepDuration())
            StopMeasurement = 1;
    }
       else  if (!(SweepCoil->Sweeping()))  StopMeasurement = 1;

//  On end of sweep:
   if (StopMeasurement == 1)
   {
        SweepCoil->SetAction(0);
		TakingData = 0;
		CurrFile->Measuring = false;
        UpdateMenuItems(this);
        if (EPRSweepCoilDialog->BackSweepRadioButton->Checked == true)
        {
            SweepCoil->SetFieldSweepSpeed(99);
            SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
            SweepCoil->SetAction(1); //Goto SetPoint
        }

        if (Averages > 1)
        {
            DataArray* AD;
            if (!ReturnSweep) AD = CurrFile->GetData(1);
               else AD = CurrFile->GetData(3);
            if (CurrentSweep == 1)
			{
                *AD = D->ReSample(1000.0*EPRSweepCoilDialog->GetStartField(),
                    1000.0*EPRSweepCoilDialog->GetEndField(), EPRSweepCoilDialog->GetAverNpts());
            }
              else
              {
                DataArray TempDat = D->ReSample(1000.0*EPRSweepCoilDialog->GetStartField(),
                    1000.0*EPRSweepCoilDialog->GetEndField(), EPRSweepCoilDialog->GetAverNpts());
                AD->AddToAverage(CurrentSweep, &TempDat);
            }
            CurrFile->PlotData();

            if (up)
            {
                sprintf(TempFileName,"C:\\TempData\\up%03d.dat",CurrentSweep);
                if (EPRSweepCoilDialog->SaveSweepsCheckBox->Checked)
                                        CurrFile->WriteData(0,TempFileName);
            }
              else
                {
                    sprintf(TempFileName,"C:\\TempData\\down%03d.dat",CurrentSweep);
                    if (EPRSweepCoilDialog->SaveSweepsCheckBox->Checked)
                                        CurrFile->WriteData(2,TempFileName);
                }
            if (Interrupted)
            {
                MeasPointFrm->Close();
                CurrFile = NULL;
                return;
            }

            if ((CurrentSweep < Averages) && (!CurrFile->GetUpDown()))
            {
                CurrFile->GetData(0)->flush();
                SweepCoil->SetFieldSweepSpeed(99);
				SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
                SweepCoil->SetAction(1); //Goto SetPoint
                GoToSweepStartTimer->Enabled = true;
                StopMeasurement = 0;
                UpdateMenuItems(this);
                return;
            }

            if ((CurrentSweep < Averages) && (CurrFile->GetUpDown()))
                if (ReturnSweep)     // If we were going back
                {
                    CurrFile->GetData(0)->flush();
                    SweepCoil->SetFieldSweepSpeed(99);
                    SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
                    SweepCoil->SetAction(1); //Goto SetPoint
                    ReturnSweep = false;
                    GoToSweepStartTimer->Enabled = true;
                    StopMeasurement = 0;
                    UpdateMenuItems(this);
                    return;
                }
                  else
                  {
                    CurrFile->GetData(2)->flush();
                    SweepCoil->SetFieldSweepSpeed(99);
                    SweepCoil->SetField(EPRSweepCoilDialog->GetEndField());
                    SweepCoil->SetAction(1); //Goto SetPoint
                    ReturnSweep = true;
                    GoToSweepStartTimer->Enabled = true;
                    StopMeasurement = 0;
                    UpdateMenuItems(this);
                    return;
                  }

        }
		   // if we were not averaging or at the end of the averaging cycle
        if ((CurrFile->GetUpDown()) && (!ReturnSweep))
        {
            if (Averages>1) CurrFile->GetData(2)->flush();
              else CurrFile->GetData(1)->flush();
            SweepCoil->SetFieldSweepSpeed(99);
            SweepCoil->SetField(EPRSweepCoilDialog->GetEndField());
            SweepCoil->SetAction(1); //Goto SetPoint
            ReturnSweep = true;
            GoToSweepStartTimer->Enabled = true;
            StopMeasurement = 0;
            UpdateMenuItems(this);
            return;
        }
          else
          {
            MeasPointFrm->Close();
            CurrFile = NULL;
            return;
          }
   }
    else
   {
      EPRSweepMeasureTimer->Enabled = true;
   }

   UpdateMenuItems(this);
   return;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RangePlotMenuItemClick(TObject *Sender)
{
    // first get current plotranges
    TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
    PlotRangeDialog->SetDataFile(MyFile);
    PlotRangeDialog->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TransientEPRMenuItemClick(TObject *Sender)
{
// Remark: Split in 2 seperate functions is better
    TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
// Test if the file is empty, if not display message
//  and return
    if (!MyFile->IsEmpty())
    {
        ErrorBox->ErrorMessage->Caption =
        "This file contains data \n Please open a new file first";
        ErrorBox->ShowModal();
        return;
    }
// Get the parameters
    char line[80];
// Possibility 1: Main magnet non-presistent.
    if (Magnet->Heater() == 1) //  Heater ON. Conclude sweep of main coil
    {
        EPRMainSweepDialog->ShowModal();
        if (EPRMainSweepDialog->ModalResult != mrOk) return;

        MeasurementType = 3;  // read boxcar instead of Lock-in

        CurrentSweep = 1;  // for averaging purposes
        CurrentPoint = 0;

        Averages = 1;
        MyFile->DataReset(2);  //Reset Data-arrays for 2 y-values
        DataPoint Pmin(2);
        DataPoint Pmax(2);

// Set the plotlimits
        if (EPRMainSweepDialog->GetEndField()
                < EPRMainSweepDialog->GetStartField())
        {
            Pmax.Set(0,EPRMainSweepDialog->GetStartField());
            Pmin.Set(0,EPRMainSweepDialog->GetEndField());
        }
          else
          {
            Pmin.Set(0,EPRMainSweepDialog->GetStartField());
            Pmax.Set(0,EPRMainSweepDialog->GetEndField());
          }
// Set the y-plot limits. Should replace later with the
//  range of the LockIn
        Pmin.Set(1,-0.001);
        Pmax.Set(1,+0.001);
        Pmin.Set(2,-0.001);
        Pmax.Set(2,+0.001);

        CurrFile = MyFile;

        time_t timer = time(NULL);  // get the time and date
        CurrFile->SetHeader("Measurement made ");
        CurrFile->AddHeader(asctime(localtime(&timer)));
        sprintf(line,"Main magnet sweep %8.5f to %8.5f Tesla at %6.3f mT/s \n",
           EPRMainSweepDialog->GetStartField(),
             EPRMainSweepDialog->GetEndField(),
               EPRMainSweepDialog->GetSweepSpeed());
        CurrFile->AddHeader(line);
        if (SweepCoil->GetAddress() != 0)
        {
            sprintf(line,"SweepCoil at %8.3f mT \n",SweepCoil->GetField());
            CurrFile->AddHeader(line);
        }


        CurrFile->GetPlot()->SetAutoRange(0);
        CurrFile->GetPlot()->SetFixedRanges(Pmin, Pmax);
        CurrFile->GetPlot()->SetAutoY1();
        CurrFile->GetPlot()->SetAutoY2();
        CurrFile->GetPlot()->SetAxisTitle("Field (T)",0);
        CurrFile->GetPlot()->SetAxisTitle("Gate 1 (mV)",1);
        CurrFile->GetPlot()->SetAxisTitle("Gate 2 - Gate 1",2);

        CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
    //    CurrFile->DataReset(2,TotalPoints);
        CurrFile->Invalidate(); //replot after changing axes
		TakingData = 0;
		CurrFile->Measuring = false;
        StopMeasurement = 0;
        UpdateMenuItems(this);
        MeasPointFrm->Show();

        Magnet->SetFieldSweepSpeed(EPRMainSweepDialog->GetSweepSpeed());
        Magnet->SetField(EPRMainSweepDialog->GetStartField());
        Magnet->SetAction(1);
        GoToStartFieldTimer->Interval = 1000;
        GoToStartFieldTimer->Enabled = true;
    }
    else
     if (Magnet->Heater() == 2)   // If fieldPersistent

    {
    //  We want to read the persistent field
    //  from the main power supply
        Magnet->GetParameter(18);
        EPRSweepCoilDialog->PersistentFieldLabel->Caption = Magnet->GetParameter(18);
        EPRSweepCoilDialog->ShowModal();
        if (EPRSweepCoilDialog->ModalResult != mrOk) return;

        MeasurementType = 3; // Transient mode: read Boxcar

        CurrentSweep = 1;
        CurrentPoint = 0;
        Averages = 1;
        MyFile->DataReset(2);
        DataPoint Pmin(2);
        DataPoint Pmax(2);
 // Set Plot limits
        if (EPRSweepCoilDialog->GetEndField()
                < EPRSweepCoilDialog->GetStartField())
        {
            Pmax.Set(0,EPRSweepCoilDialog->GetStartField()*1000.0);
            Pmin.Set(0,EPRSweepCoilDialog->GetEndField()*1000.0);
        }
          else
          {
            Pmin.Set(0,EPRSweepCoilDialog->GetStartField()*1000.0);
            Pmax.Set(0,EPRSweepCoilDialog->GetEndField()*1000.0);
          }
        Pmin.Set(1,-0.001);
        Pmax.Set(1,+0.001);
        Pmin.Set(2,-0.001);
        Pmax.Set(2,+0.001);

        CurrFile = MyFile;

        CurrFile->GetPlot()->SetAutoRange(0);
        CurrFile->GetPlot()->SetFixedRanges(Pmin, Pmax);
        CurrFile->GetPlot()->SetAutoY1();
        CurrFile->GetPlot()->SetAutoY2();
        CurrFile->GetPlot()->SetAxisTitle("Sweepcoil Field (mT)",0);
        CurrFile->GetPlot()->SetAxisTitle("Gate 1 (mV)",1);
        CurrFile->GetPlot()->SetAxisTitle("Gate 2-1 (mV)",2);

        CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
        CurrFile->Invalidate();

		TakingData = 0;
		CurrFile->Measuring = false;
        StopMeasurement = 0;
        UpdateMenuItems(this);
        MeasPointFrm->Show();

        int status;

//  Set the header add the date
        time_t timer = time(NULL);
        CurrFile->SetHeader("Measurement made ");
        CurrFile->AddHeader(asctime(localtime(&timer)));
        sprintf(line,"Main magnet persistent at %8.5f T \n",
                Magnet->GetParameter(18));
        CurrFile->AddHeader(line);

        sprintf(line,"SweepCoil Sweep from %8.3f to %8.3f mT at %6.4f mT/s \n",
           EPRSweepCoilDialog->GetStartField(),
             EPRSweepCoilDialog->GetEndField(),
               EPRSweepCoilDialog->GetSweepSpeed());
        CurrFile->AddHeader(line);

        status = SweepCoil->SetAction(0);
        if (status !=0)
        {
            ErrorBox->ErrorMessage->Caption =
                "Error setting field to HOLD";
            ErrorBox->ShowModal();
            return;
        }

        status = SweepCoil->SetFieldSweepSpeed(99);
        if (status !=0)
        {
            ErrorBox->ErrorMessage->Caption =
                "Error setting SweepSpeed";
            ErrorBox->ShowModal();
            return;
        }

        status = SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
        if (status !=0)
        {
            ErrorBox->ErrorMessage->Caption =
                "Error setting field";
            ErrorBox->ShowModal();
            return;
        }
		status = SweepCoil->SetAction(1);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting SweepCoil to GOTO SETPOINT";
			ErrorBox->ShowModal();
			return;
		}
		GoToSweepStartTimer->Interval = 1000;
		GoToSweepStartTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TransientEPR2MenuItemClick(TObject *Sender)
{
// Test if the file is empty, if not display message
//  and return
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	if (!MyFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		MyFile = NULL;
		return;
	}
	char line[80];
	DataPoint Pmin(2); // for the plot limits
	DataPoint Pmax(2); // for the plot limits

	int Persistent=0;
// Possibility 1: Main magnet non-presistent.
	if (Magnet->Heater() == 1) //  Heater ON. Conclude sweep of main coil
	{
		Persistent = 0;
		EPRMainSweepDialog->ShowModal();
		if (EPRMainSweepDialog->ModalResult != mrOk) return;
 // Set the plotlimits
		if (EPRMainSweepDialog->GetEndField()
				< EPRMainSweepDialog->GetStartField())
		{
			Pmax.Set(0,EPRMainSweepDialog->GetStartField());
			Pmin.Set(0,EPRMainSweepDialog->GetEndField());
		}
		  else
		  {
			Pmin.Set(0,EPRMainSweepDialog->GetStartField());
			Pmax.Set(0,EPRMainSweepDialog->GetEndField());
		  }
	}
//----
	  else
	  {
		if (Magnet->Heater() == 2)
		{
		  // Get Persistent field
			Persistent = 1;
			EPRSweepCoilDialog->PersistentFieldLabel->Caption = Magnet->GetParameter(18);
			EPRSweepCoilDialog->ShowModal();
			if (EPRSweepCoilDialog->ModalResult != mrOk) return;
 // Set Plot limits
			if (EPRSweepCoilDialog->GetEndField()
				  < EPRSweepCoilDialog->GetStartField())
			{
			  Pmax.Set(0,EPRSweepCoilDialog->GetStartField()*1000.0);
			  Pmin.Set(0,EPRSweepCoilDialog->GetEndField()*1000.0);
			}
			   else
			{
			  Pmin.Set(0,EPRSweepCoilDialog->GetStartField()*1000.0);
			  Pmax.Set(0,EPRSweepCoilDialog->GetEndField()*1000.0);
			}


		} else return;
	  }
	MeasurementType = 4;  // Get data from Infimium

//        for (int i=0;i<Gate;i++)
//            InfiniumGatesDlg->SetGate(i+1,T1[i], T2[i]);
	InfiniumGatesDlg->ShowModal();
	int Gates = 0;
	if (InfiniumGatesDlg->ModalResult == mrOk)
	{
		if (Tek7->GetAddress() > 0)
			Tek7->SetAverage(InfiniumGatesDlg->GetAverages());
		  else
			Infinium->SetAverage(InfiniumGatesDlg->GetAverages());
		Gates = InfiniumGatesDlg->GetnGates();
		for (int i=0; i<Gates; i++)
		  {
			  InfiniumGatesDlg->GetGate(i+1,T1+i, T2+i);
		  }
	} else return;

	CurrentSweep = 1;  // for averaging purposes
	CurrentPoint = 0;

	Averages = 1;
	MyFile->DataReset(Gates);  //Reset Data-arrays for 2 y-values

// Set the y-plot limits. Should replace later with the
//  range of the LockIn
//-----

    double range = 0.005;

    Pmin.Set(1, -range);
    Pmax.Set(1,range);
    Pmin.Set(2, -range);
    Pmax.Set(2,range);

    CurrFile = MyFile;

    time_t timer = time(NULL);  // get the time and date
    CurrFile->SetHeader("Measurement made ");
    CurrFile->AddHeader(asctime(localtime(&timer)));
//-----
    if (Persistent == 0)
    {
          sprintf(line,"Main magnet sweep %8.5f to %8.5f Tesla at %6.3f mT/s \n",
            EPRMainSweepDialog->GetStartField(),
             EPRMainSweepDialog->GetEndField(),
               EPRMainSweepDialog->GetSweepSpeed());
            CurrFile->AddHeader(line);

            if (SweepCoil->GetAddress() != 0)
            {
                sprintf(line,"SweepCoil at %8.3f mT\n",SweepCoil->GetField());
                CurrFile->AddHeader(line);
            }
    }
      else
      {
        sprintf(line,"Main magnet persistent at %8.5f T \n",
        Magnet->GetParameter(18));
        CurrFile->AddHeader(line);

        sprintf(line,"SweepCoil Sweep from %8.3f to %8.3f mT at %6.4f mT/s",
              EPRSweepCoilDialog->GetStartField(),
                 EPRSweepCoilDialog->GetEndField(),
                 EPRSweepCoilDialog->GetSweepSpeed());
        CurrFile->AddHeader(line);
      }
//------
        /// Transient Experiment
    sprintf(line,"Transient with %d gates \n",Gates);
    CurrFile->AddHeader(line);
    for (int i=0; i<Gates;i++)
    {
        sprintf(line,"Gate %d : From %f to %f microseconds \n",i+1,T1[i]*1.0e6,T2[i]*1.0e6);
        CurrFile->AddHeader(line);
    }
    AddInstrumentHeader();

    NumberOfGates = Gates;
//----
//-----

    CurrFile->GetPlot()->SetAutoRange(0);
    CurrFile->GetPlot()->SetFixedRanges(Pmin, Pmax);
    CurrFile->GetPlot()->SetAutoY1();
    CurrFile->GetPlot()->SetAutoY2();
    if (Persistent == 0) CurrFile->GetPlot()->SetAxisTitle("Field (T)",0);
          else CurrFile->GetPlot()->SetAxisTitle("Sweepcoil Field (mT)",0);
    CurrFile->GetPlot()->SetAxisTitle("Gate 1 (mV)",1);
    CurrFile->GetPlot()->SetAxisTitle("Gate 2 - Gate 1",2);
    CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
    //    CurrFile->DataReset(2,TotalPoints);
    CurrFile->Invalidate(); //replot after changing axes
	TakingData = 0;
	CurrFile->Measuring = false;
    StopMeasurement = 0;
    UpdateMenuItems(this);
    MeasPointFrm->Show();
//-----
    if (Persistent==0)
    {
        Magnet->SetFieldSweepSpeed(EPRMainSweepDialog->GetSweepSpeed());
        Magnet->SetField(EPRMainSweepDialog->GetStartField());
        Magnet->SetAction(1);
        GoToStartFieldTimer->Interval = 1000;
        GoToStartFieldTimer->Enabled = true;
    }
      else
      {
        int status = SweepCoil->SetAction(0);
        if (status !=0)
        {
            ErrorBox->ErrorMessage->Caption =
                "Error setting field to HOLD";
            ErrorBox->ShowModal();
            return;
        }

        status = SweepCoil->SetFieldSweepSpeed(99);
        if (status !=0)
        {
            ErrorBox->ErrorMessage->Caption =
                "Error setting SweepSpeed";
            ErrorBox->ShowModal();
            return;
        }

        status = SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
        if (status !=0)
        {
            ErrorBox->ErrorMessage->Caption =
                "Error setting field";
            ErrorBox->ShowModal();
            return;
        }
        status = SweepCoil->SetAction(1);
        if (status !=0)
        {
            ErrorBox->ErrorMessage->Caption =
                "Error setting SweepCoil to GOTO SETPOINT";
            ErrorBox->ShowModal();
            return;
        }
        GoToSweepStartTimer->Interval = 1000;
        GoToSweepStartTimer->Enabled = true;
      }
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::HilbertMenuItemClick(TObject *Sender)
{
    TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
    AnalysisFile = MyFile;
    HilbertDialog->Show();
}
//---------------------------------------------------------------------------

int TMainForm::AddInstrumentHeader()
{
    char line[80];
    char helpstr[25];
    double tcc;
	double modu;
	double Vpos,Vneg,Rohm, Tuncorr, Tcorr, Mfield;
	if (Magnet->GetAddress() != 0)
	{
		if (Magnet->Heater() == 1) // if not persistent
			sprintf(line,"Main Magnet at %8.5f T \n",Mfield = Magnet->GetField());
		  else
			sprintf(line,"Main Magnet persistent at %8.5f \n", Mfield = Magnet->GetParameter(18));
		CurrFile->AddHeader(line);
	}
	if (SweepCoil->GetAddress() != 0)
	{
		sprintf(line, "Sweep Coil at %8.6f T \n", SweepCoil->GetField());
		CurrFile->AddHeader(line);
	}
	if (ITC->GetAddress() != 0)
	{
		sprintf(line,"ITC set at %8.3f K, T1: %7.3f K, T2: %7.3f K, T3: %7.3f K \n",
		ITC->GetTemperature(0), ITC->GetTemperature(1),
				  ITC->GetTemperature(2), ITC->GetTemperature(3) );
		CurrFile->AddHeader(line);
	}

	if ((CurrentSource->GetAddress() != 0) && (DMM2->GetAddress() != 0))
	{
		if (CurrentSource->GetDefaultCurr() > 1e-9)      // at least  1nA
		{
			CurrentSource->SetCurrent(CurrentSource->GetDefaultCurr());
			Vpos = DMM2->Read();
			CurrentSource->SetCurrent(-CurrentSource->GetDefaultCurr());
			Vneg = DMM2->Read();
			Rohm = fabs((Vpos-Vneg)/(2*CurrentSource->GetDefaultCurr()));
			if (Rohm < 400)
			{
				Tuncorr = Cernox10mA->GetTemperature(Rohm);
				Tcorr = Tuncorr;
			}
			else
			{
				Tuncorr = Cernox1mA->GetTemperature(Rohm);
				Tcorr = Cernox1mA->GetTemperature(Rohm, Mfield);
			}
			sprintf(line,"Sample Temperature at beginning of sweep:  R= %f Ohm \n", Rohm);
			sprintf(line,"For Cernox1 %f Ohm corresponds to T = %f K at 0 T or %f K at %f T. \n", Rohm, Tuncorr, Tcorr, Mfield);
		} else sprintf(line,"Current too low to read temperature \n");

	}
		else if (DMM2->GetAddress() != 0)
		{
			// this assumes a direct read from the Voltmeter with 10mA or 1mA applied
			Vpos = DMM2->Read();
			if (ITC->GetTemperature(1) > 10.0)
			{
				Rohm = Vpos*1.0e5;   // assuming 10 mA
				Tuncorr = Cernox10mA->GetTemperature(Rohm);
			}
			  else
			  {
				Rohm = Vpos*1.0e6;   // assuming 1 mA
				Tuncorr = Cernox1mA->GetTemperature(Rohm);
			  }
			sprintf(line,"For Cernox1 %f Ohm corresponds to T = %f K at 0 T \n", Rohm, Tuncorr);
		 }
			else sprintf(line,"Sample Temperature could not be read directly");
	CurrFile->AddHeader(line);


    if (LockIn1->GetAddress() != 0)
    {
        strcpy(line,"LockIn1: ");
        LockIn1->GetSensitivity(helpstr);
        strcat(line,helpstr);
        sprintf(helpstr,", Mod Freq %f Hz",LockIn1->GetRefFrequency());
        strcat(line,helpstr);
        sprintf(helpstr,", Phase %f degr",LockIn1->GetPhase());
        strcat(line,helpstr);
        modu =  LockIn1->GetRefAmp();
        if (modu >0.0)
        {
            sprintf(helpstr, ", Ampl. %f V", modu);
            strcat(line, helpstr);
        }
        strcat(line,"\n  ");

        //filters
        //tc
        CurrFile->AddHeader(line);
        LockIn1->GetFilters(line,78);
        CurrFile->AddHeader(line);
    }
    if (LockIn2->GetAddress() != 0)
    {
        strcpy(line,"LockIn2: ");
        LockIn2->GetSensitivity(helpstr);
        strcat(line,helpstr);
        sprintf(helpstr,", Mod Freq %f Hz",LockIn2->GetRefFrequency());
        strcat(line,helpstr);
        sprintf(helpstr,", Phase %f degr",LockIn2->GetPhase());
        strcat(line,helpstr);
        modu =  LockIn2->GetRefAmp();
        if (modu >0.0)
        {
            sprintf(helpstr, ", Ampl. %f V", modu);
            strcat(line, helpstr);
        }
        strcat(line,"\n  ");
        //filters
        //tc
		CurrFile->AddHeader(line);
        LockIn2->GetFilters(line,78);
        CurrFile->AddHeader(line);
    }
    if (DMM1->GetAddress() != 0)         // DMM1 measures the modulation
    {
        strcpy(line,"Modulation: ");
        sprintf(helpstr,"%f mA \n",DMM1->ReadVoltage()*1000);
        strcat(line,helpstr);
        CurrFile->AddHeader(line);
    }
       else if (DMM2->GetAddress() != 0)
       {
            strcpy(line,"Modulation: ");
            sprintf(helpstr,"%f mA \n",DMM2->ReadValue()*1000.0);
            strcat(line,helpstr);
            CurrFile->AddHeader(line);
       }
    return 0;
}

void __fastcall TMainForm::HelpContentsMenuItemClick(TObject *Sender)
{
    Application->HelpCommand(HELP_CONTENTS, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LockIn2MenuItemClick(TObject *Sender)
{
    if (LockIn2->GetAddress() == 0)
    {
        ErrorBox->ErrorMessage->Caption = " LockIn2 not connected or initialized ";
		ErrorBox->ShowModal();
        return;
    }
    LockInDialog->SetLockIn(LockIn2);
    LockInDialog->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HeteroEPRMenuItemClick(TObject *Sender)
{
	MeasurementType = 10;
	int nDaqMeas = 0;
	if (DAQ->IsOpen())
	{
		nDaqMeas = DAQ->GetnActiveChannels();
	}
	NumberOfMeas = 4 + nDaqMeas;               // Number of Y values per datapoint

	EPRMeasureTimer->Interval = 50;//ceil(PulsedDialog->AverEdit->Text.ToInt() * millisec_pp);
	EPRSweepMeasureTimer->Interval = 50;//ceil(PulsedDialog->AverEdit->Text.ToInt() *
//			millisec_pp);

	PrepareEPRMeasurement();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::HetTransientMenuItemClick(TObject *Sender)
{
// Test if the file is empty, if not display message
//  and return
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	if (!MyFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		MyFile = NULL;
		return;
	}
	CurrFile = MyFile;  // CurrFile will point to file measured

	char line[80];
	DataPoint Pmin(2); // for the plot limits
	DataPoint Pmax(2); // for the plot limits

	CurrentSweep = 0;
	CurrentPoint = 0;

	Averages = 1;
	int av;

	int Persistent=0;
// Possibility 1: Main magnet non-presistent.
	if ((Magnet->Heater() == 1) || (_testmode)) //  Heater ON. Conclude sweep of main coil
	{
		Persistent = 0;
		EPRMainSweepDialog->ShowModal();
		if (EPRMainSweepDialog->ModalResult != mrOk)
		{
			CurrFile = NULL;
			return;
		}
 // Set the plotlimits
		if (EPRMainSweepDialog->GetEndField()
				< EPRMainSweepDialog->GetStartField())
		{
			Pmax.Set(0,EPRMainSweepDialog->GetStartField());
			Pmin.Set(0,EPRMainSweepDialog->GetEndField());
		}
		  else
		  {
			Pmin.Set(0,EPRMainSweepDialog->GetStartField());
			Pmax.Set(0,EPRMainSweepDialog->GetEndField());
		  }
		if (EPRMainSweepDialog->AverageCheckBox->Checked)
		{
			if (ValidInt(EPRMainSweepDialog->AverEdit->Text, &av))
			{
				if ((av>1) && (av<1000))
				{
					CurrFile->SetAverage(1); // Set averagemode on
					Averages = av;
				}
			}
		}  else CurrFile->SetAverage(0);

		if (EPRMainSweepDialog->UpDownCheckBox->Checked)
				CurrFile->SetUpDown(1); // Set UpDownMode On
		   else CurrFile->SetUpDown(0);

 // Initiate sweep to startfield

		double Sweeprate = EPRMainSweepDialog->GetSweepSpeed();
		if (Sweeprate < 2.0) Sweeprate = 2.0;             //default rate
		Magnet->SetFieldSweepSpeed(Sweeprate);
		Magnet->SetField(EPRMainSweepDialog->GetStartField());
		Magnet->SetAction(1);

	}
//----
	  else
	  {
		if (Magnet->Heater() == 2)
		{
		  // Get Persistent field
			Persistent = 1;
			EPRSweepCoilDialog->PersistentFieldLabel->Caption = Magnet->GetParameter(18);
			EPRSweepCoilDialog->ShowModal();
			if (EPRSweepCoilDialog->ModalResult != mrOk) return;
 // Set Plot limits
			if (EPRSweepCoilDialog->GetEndField()
				  < EPRSweepCoilDialog->GetStartField())
			{
			  Pmax.Set(0,EPRSweepCoilDialog->GetStartField()*1000.0);
			  Pmin.Set(0,EPRSweepCoilDialog->GetEndField()*1000.0);
			}
			   else
			{
			  Pmin.Set(0,EPRSweepCoilDialog->GetStartField()*1000.0);
			  Pmax.Set(0,EPRSweepCoilDialog->GetEndField()*1000.0);
			}


		} else return;
	  }


	MeasurementType = 5;  // Get data from TekScope
	if (PulsedDialog->QuadratureCheckBox->Checked) MeasurementType = 7;

//        for (int i=0;i<Gate;i++)
//            InfiniumGatesDlg->SetGate(i+1,T1[i], T2[i]);
	InfiniumGatesDlg->ShowModal();
	int Gates = 0;
	if (InfiniumGatesDlg->ModalResult == mrOk)
	{
		if (Tek7->GetAddress() > 0)
		{
			if (MeasurementType == 7 ) {
			  Tek7->SetAverage(1);
			  Tek7->SetMathAverage(1, InfiniumGatesDlg->GetAverages());
			}
		  else
			Tek7->SetAverage(InfiniumGatesDlg->GetAverages());
		}
		  else
			Infinium->SetAverage(InfiniumGatesDlg->GetAverages());
		Gates = InfiniumGatesDlg->GetnGates();
		for (int i=0; i<Gates; i++)
          {
              InfiniumGatesDlg->GetGate(i+1,T1+i, T2+i);
          }
    } else return;

	int millisec_pp = 10;  //scope does not average faster
	if (PulsedDialog->RateEdit->Text.ToInt() > 10000) {
	   millisec_pp = ceil(PulsedDialog->RateEdit->Text.ToInt()/1000);
	}
	if (PulsedDialog->SweepTypeListBox->ItemIndex >= 0)
	{
		EPRMeasureTimer->Interval = ceil(PulsedDialog->AverEdit->Text.ToInt() * millisec_pp);
		EPRSweepMeasureTimer->Interval = ceil(PulsedDialog->AverEdit->Text.ToInt() *
			millisec_pp);
	  }
	  else
	  {
		EPRMeasureTimer->Interval = InfiniumGatesDlg->GetAverages() * 80;
		EPRSweepMeasureTimer->Interval = InfiniumGatesDlg->GetAverages() * 80;
	}

	// for the interval we keep in mind that we have a 10 Hz laser,
	// so to complete the number of averages we need nAverages*100 ms
	// I (kind of) arbitrarily take two samples per full
	// average so 50 ms times the number of averages of the scope

	// This also gives us a estimate for the number of points
	// in one sweep, which should be the sweep duration divided by
	// the measurement interval.
	int EstimatedNumberOfPoints;
	if (Persistent==0)
		EstimatedNumberOfPoints = ceil(EPRMainSweepDialog->GetSweepDuration()*1000.0/EPRMeasureTimer->Interval);
	  else
		EstimatedNumberOfPoints = ceil(EPRSweepCoilDialog->GetSweepDuration()*1000.0/EPRMeasureTimer->Interval);

	CurrentSweep = 0;  // for averaging purposes
	CurrentPoint = 0;

//    Averages = 1;
	if (InfiniumGatesDlg->CalCheckBox->Checked )
	{
		MyFile->AllDataReset(2*Gates+1, EstimatedNumberOfPoints);  //Reset Data-arrays for 2 times number of Gates
        MeasurementType = 6;   // if not MeasurementType remains 5
    }
     else
        MyFile->AllDataReset(2*Gates, EstimatedNumberOfPoints);  //Reset Data-arrays for 2 times number of Gates

// Set the y-plot limits. 

    double range = 0.005;

    Pmin.Set(1, -range);
    Pmax.Set(1,range);
    Pmin.Set(2, -range);
    Pmax.Set(2,range);


    time_t timer = time(NULL);  // get the time and date
    CurrFile->SetHeader("DataFile HansFormat \n");
	CurrFile->AddHeader("Measurement made ");
    CurrFile->AddHeader(asctime(localtime(&timer)));
//-----
    if (Persistent == 0)
	{
		  sprintf(line,"Main magnet sweep %8.5f to %8.5f Tesla at %6.3f mT/s \n",
            EPRMainSweepDialog->GetStartField(),
             EPRMainSweepDialog->GetEndField(),
               EPRMainSweepDialog->GetSweepSpeed());
            CurrFile->AddHeader(line);

            if (SweepCoil->GetAddress() != 0)
            {
                sprintf(line,"SweepCoil at %8.3f mT\n",SweepCoil->GetField());
                CurrFile->AddHeader(line);
            }
    }
      else
      {
        sprintf(line,"Main magnet persistent at %8.5f T \n",
		Magnet->GetParameter(18));
		CurrFile->AddHeader(line);

		sprintf(line,"SweepCoil Sweep from %8.3f to %8.3f mT at %6.4f mT/s \n",
			  EPRSweepCoilDialog->GetStartField(),
				 EPRSweepCoilDialog->GetEndField(),
				 EPRSweepCoilDialog->GetSweepSpeed());
		CurrFile->AddHeader(line);
	  }
//------
		/// Transient Experiment
	sprintf(line,"Transient with %d gates \n",Gates);
	CurrFile->AddHeader(line);
	if (InfiniumGatesDlg->RegularCheckBox->Checked)
	{
		sprintf(line," Gatewidth %f microsec, Starting at %f microsec. \n",
			InfiniumGatesDlg->GetWidth()*1e6, InfiniumGatesDlg->GetStart()*1e6);
		CurrFile->AddHeader(line);
	}
	else
		for (int i=0; i<Gates;i++)
		{
			sprintf(line,"Gate %d : From %f to %f microseconds \n",i+1,T1[i]*1.0e6,T2[i]*1.0e6);
			CurrFile->AddHeader(line);
		}
	sprintf(line,"Pulsed measurement \n");
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse A: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P1DelayEdit->Text.c_str(),
		PulsedDialog->P1DelayStepEdit->Text.c_str(),
		PulsedDialog->P1WidthEdit->Text.c_str(),
		PulsedDialog->P1WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse B: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P2DelayEdit->Text.c_str(),
		PulsedDialog->P2DelayStepEdit->Text.c_str(),
		PulsedDialog->P2WidthEdit->Text.c_str(),
		PulsedDialog->P2WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse C: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P3DelayEdit->Text.c_str(),
		PulsedDialog->P3DelayStepEdit->Text.c_str(),
		PulsedDialog->P3WidthEdit->Text.c_str(),
		PulsedDialog->P3WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse D: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P4DelayEdit->Text.c_str(),
		PulsedDialog->P4DelayStepEdit->Text.c_str(),
		PulsedDialog->P4WidthEdit->Text.c_str(),
		PulsedDialog->P4WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"RepTime: %s microsec, Acq.delay step %s ns, Acq Gate %s ns, %s Shots/point \n",
		PulsedDialog->RateEdit->Text.c_str(),
		PulsedDialog->AcqStepEdit->Text.c_str(),
		PulsedDialog->IntTimeEdit->Text.c_str(),
		PulsedDialog->AverEdit->Text.c_str());
	CurrFile->AddHeader(line);
//	sprintf(line,"Modulation FM %8.3f kHz, modulation AM %8.3f percent \n",
//		ENDORParametersDialog->FMEdit->Text.ToDouble(),
//		ENDORParametersDialog->AMEdit->Text.ToDouble());
//	CurrFile->AddHeader(line);

	AddInstrumentHeader();

    NumberOfGates = Gates;
//----
//-----

    for (int i = 0; i< CurrFile->GetnPlots(); i++)
    {
        CurrFile->GetPlot(i)->SetAutoRange(0);
        CurrFile->GetPlot(i)->SetFixedRanges(Pmin, Pmax);
        CurrFile->GetPlot(i)->SetAutoY1();
        CurrFile->GetPlot(i)->SetAutoY2();
        if (Persistent == 0) CurrFile->GetPlot(i)->SetAxisTitle("Field (T)",0);
          else CurrFile->GetPlot(i)->SetAxisTitle("Sweepcoil Field (mT)",0);
        CurrFile->GetPlot(i)->SetAxisTitle("Gate 1 Ch 1 (mV)",1);
        CurrFile->GetPlot(i)->SetAxisTitle("Gate 1 Ch 2 (mV)",2);
    }
    CurrFile->PlotData();
    //    CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
    //    CurrFile->DataReset(2,TotalPoints);
    CurrFile->Invalidate(); //replot after changing axes
	TakingData = 0;
	CurrFile->Measuring = false;
    StopMeasurement = 0;
    Interrupted = 0;
    UpdateMenuItems(this);
	MeasPointFrm->Show();
	ReturnSweep = false;
//    Infinium->RetrieveOffsets();
//    Infinium->RetrieveIncrements();
	if (Tek7->GetAddress() > 0)
	{
		if (MeasurementType == 7 ) Tek7->RetrieveScaleSettings(1);
		  else Tek7->RetrieveScaleSettings(0);
//		Tek7->SetDefaultFormat();
	}
	  else
	  {
		Infinium->RetrieveScaleSettings();
		Infinium->SetDefaultFormat();
	  }

//-----
	if (Persistent==0)
	{
		Magnet->SetFieldSweepSpeed(EPRMainSweepDialog->GetSweepSpeed());
		Magnet->SetField(EPRMainSweepDialog->GetStartField());
		Magnet->SetAction(1);
		GoToStartFieldTimer->Interval = 1000;
		GoToStartFieldTimer->Enabled = true;
	}
	  else
	  {
		int status = SweepCoil->SetAction(0);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting field to HOLD";
			ErrorBox->ShowModal();
			return;
		}

		status = SweepCoil->SetFieldSweepSpeed(99);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting SweepSpeed";
			ErrorBox->ShowModal();
			return;
		}

		status = SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting field";
			ErrorBox->ShowModal();
			return;
		}
		status = SweepCoil->SetAction(1);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting SweepCoil to GOTO SETPOINT";
			ErrorBox->ShowModal();
			return;
		}
		GoToSweepStartTimer->Interval = 1000;
		GoToSweepStartTimer->Enabled = true;
	  }

}
//---------------------------------------------------------------------------

void TMainForm::PulsedSweepPrepare()
{
// Test if the file is empty, if not display message
//  and return
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	if (!MyFile->IsEmpty())
	{
		ErrorBox->ShowMessage("This file contains data \n Please open a new file first");
		MyFile = NULL;
		return;
	}
	CurrFile = MyFile;  // CurrFile will point to file measured

	char line[80];
	DataPoint Pmin(4); // for the plot limits
	DataPoint Pmax(4); // for the plot limits

	CurrentSweep = 0;
	CurrentPoint = 0;

	Averages = 1;
	int av;

	int Persistent=0;
// Possibility 1: Main magnet non-presistent.
	if ((Magnet->Heater() == 1) || (_testmode)) //  Heater ON. Conclude sweep of main coil
	{
		Persistent = 0;
		EPRMainSweepDialog->ShowModal();
		if (EPRMainSweepDialog->ModalResult != mrOk)
		{
			CurrFile = NULL;
			return;
		}
 // Set the plotlimits
		if (EPRMainSweepDialog->GetEndField()
				< EPRMainSweepDialog->GetStartField())
		{
			Pmax.Set(0,EPRMainSweepDialog->GetStartField());
			Pmin.Set(0,EPRMainSweepDialog->GetEndField());
		}
		  else
		  {
			Pmin.Set(0,EPRMainSweepDialog->GetStartField());
			Pmax.Set(0,EPRMainSweepDialog->GetEndField());
		  }
		if (EPRMainSweepDialog->AverageCheckBox->Checked)
		{
			if (ValidInt(EPRMainSweepDialog->AverEdit->Text, &av))
			{
				if ((av>1) && (av<1000))
				{
					CurrFile->SetAverage(1); // Set averagemode on
					Averages = av;
				}
			}
		}  else CurrFile->SetAverage(0);

		if (EPRMainSweepDialog->UpDownCheckBox->Checked)
				CurrFile->SetUpDown(1); // Set UpDownMode On
		   else CurrFile->SetUpDown(0);

 // Initiate sweep to startfield

		double Sweeprate = EPRMainSweepDialog->GetSweepSpeed();
		if (Sweeprate < 4.0) Sweeprate = 4.0;             //default rate
		Magnet->SetFieldSweepSpeed(Sweeprate);
		Magnet->SetField(EPRMainSweepDialog->GetStartField());
		Magnet->SetAction(1);

	}
//----
	  else
	  {
		if (Magnet->Heater() == 2)             //heater off, magnet persistent
		{
		  // Get Persistent field
			Persistent = 1;
			EPRSweepCoilDialog->PersistentFieldLabel->Caption = Magnet->GetParameter(18);
			EPRSweepCoilDialog->ShowModal();
			if (EPRSweepCoilDialog->ModalResult != mrOk) return;
 // Set Plot limits
			if (EPRSweepCoilDialog->GetEndField()
				  < EPRSweepCoilDialog->GetStartField())
			{
			  Pmax.Set(0,EPRSweepCoilDialog->GetStartField()*1000.0);
			  Pmin.Set(0,EPRSweepCoilDialog->GetEndField()*1000.0);
			}
			   else
			{
			  Pmin.Set(0,EPRSweepCoilDialog->GetStartField()*1000.0);
			  Pmax.Set(0,EPRSweepCoilDialog->GetEndField()*1000.0);
			}


		} else return;
	  }


	MeasurementType = 5;  // Get data from TekScope
	if (PulsedDialog->QuadratureCheckBox->Checked) MeasurementType = 7;
		//read math channels

//        for (int i=0;i<Gate;i++)
//            InfiniumGatesDlg->SetGate(i+1,T1[i], T2[i]);
//	InfiniumGatesDlg->ShowModal();                  //We should eliminate this
//	int Gates = 2;
	NumberOfGates = 2;

	if (Tek7->GetAddress() > 0)
	{
		if (MeasurementType == 7 ) {
		  Tek7->SetAverage(1);
		  Tek7->SetMathAverage(1, PulsedDialog->AverEdit->Text.ToInt());
		  Tek7->SetMathAverage(2, PulsedDialog->AverEdit->Text.ToInt());
		}
	  else
		Tek7->SetAverage(PulsedDialog->AverEdit->Text.ToInt());
	}
	  else
		Infinium->SetAverage(PulsedDialog->AverEdit->Text.ToInt());

	T1[0] = PulsedDialog->AcqDelayEdit->Text.ToInt()*1e-9;
	T2[0] = T1[0] + PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;
	T1[1] = T1[0] + 2 * PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;
	T2[1] = T1[1] + PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;

	int millisec_pp;
	if (MeasurementType == 7 ) {
		millisec_pp = 16;  //scope does not average faster
		if (PulsedDialog->RateEdit->Text.ToInt() > 16000)
			millisec_pp = ceil(PulsedDialog->RateEdit->Text.ToInt()/1000);
		}
	  else  {
		millisec_pp = 1;  //scope does not average faster
		if (PulsedDialog->RateEdit->Text.ToInt() > 1000)
			millisec_pp = ceil(PulsedDialog->RateEdit->Text.ToInt()/1000);
		}


	EPRMeasureTimer->Interval = ceil(PulsedDialog->AverEdit->Text.ToInt() * millisec_pp);
	EPRSweepMeasureTimer->Interval = ceil(PulsedDialog->AverEdit->Text.ToInt() *
			millisec_pp);

	// for the interval we keep in mind that we have a 10 Hz laser,
	// so to complete the number of averages we need nAverages*100 ms
	// I (kind of) arbitrarily take two samples per full
	// average so 50 ms times the number of averages of the scope

	// This also gives us a estimate for the number of points
	// in one sweep, which should be the sweep duration divided by
	// the measurement interval.

	int EstimatedNumberOfPoints;
	if (Persistent==0)
		EstimatedNumberOfPoints = ceil(EPRMainSweepDialog->GetSweepDuration()*1000.0/EPRMeasureTimer->Interval);
	  else
		EstimatedNumberOfPoints = ceil(EPRSweepCoilDialog->GetSweepDuration()*1000.0/EPRMeasureTimer->Interval);

	CurrentSweep = 0;  // for averaging purposes
	CurrentPoint = 0;

//    Averages = 1;
	if (PulsedDialog->TwoChannelCheckBox->Checked) {
		CurrFile->AllDataReset(2*NumberOfGates, EstimatedNumberOfPoints);  //Reset Data-arrays for 2 times number of Gates
	}
	   else CurrFile->AllDataReset(NumberOfGates, EstimatedNumberOfPoints);  //Reset Data-arrays for 2 times number of Gates

// Set the y-plot limits.

	double range = 0.1;     //Could perhaps read the scale of the scope before setting the standard scale

	Pmin.Set(1, -range);
	Pmax.Set(1,range);
	Pmin.Set(2, -range);
	Pmax.Set(2,range);


	time_t timer = time(NULL);  // get the time and date
	CurrFile->SetHeader("DataFile HansFormat \n");
	CurrFile->AddHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));
//-----
	if (Persistent == 0)
	{
		  sprintf(line,"Main magnet sweep %8.5f to %8.5f Tesla at %6.3f mT/s \n",
			EPRMainSweepDialog->GetStartField(),
			 EPRMainSweepDialog->GetEndField(),
			   EPRMainSweepDialog->GetSweepSpeed());
			CurrFile->AddHeader(line);

			if (SweepCoil->GetAddress() != 0)
			{
				sprintf(line,"SweepCoil at %8.3f mT\n",SweepCoil->GetField());
				CurrFile->AddHeader(line);
			}
	}
	  else
	  {
		sprintf(line,"Main magnet persistent at %8.5f T \n",
		Magnet->GetParameter(18));
		CurrFile->AddHeader(line);

		sprintf(line,"SweepCoil Sweep from %8.3f to %8.3f mT at %6.4f mT/s \n",
			  EPRSweepCoilDialog->GetStartField(),
				 EPRSweepCoilDialog->GetEndField(),
				 EPRSweepCoilDialog->GetSweepSpeed());
		CurrFile->AddHeader(line);
	  }
//------
	for (int i=0; i<NumberOfGates;i++)
	{
		sprintf(line,"Gate %d : From %f to %f microseconds \n",i+1,T1[i]*1.0e6,T2[i]*1.0e6);
		CurrFile->AddHeader(line);
	}
	sprintf(line,"Pulsed measurement \n");
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse A: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P1DelayEdit->Text.c_str(),
		PulsedDialog->P1DelayStepEdit->Text.c_str(),
		PulsedDialog->P1WidthEdit->Text.c_str(),
		PulsedDialog->P1WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse B: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P2DelayEdit->Text.c_str(),
		PulsedDialog->P2DelayStepEdit->Text.c_str(),
		PulsedDialog->P2WidthEdit->Text.c_str(),
		PulsedDialog->P2WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse C: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P3DelayEdit->Text.c_str(),
		PulsedDialog->P3DelayStepEdit->Text.c_str(),
		PulsedDialog->P3WidthEdit->Text.c_str(),
		PulsedDialog->P3WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse D: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P4DelayEdit->Text.c_str(),
		PulsedDialog->P4DelayStepEdit->Text.c_str(),
		PulsedDialog->P4WidthEdit->Text.c_str(),
		PulsedDialog->P4WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse E is driving the microwave switch, the sum of A, B, C, and D");
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse F (Scope Trig): delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P6DelayEdit->Text.c_str(),
		PulsedDialog->P6DelayStepEdit->Text.c_str(),
		PulsedDialog->P6WidthEdit->Text.c_str(),
		PulsedDialog->P6WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse G: delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P7DelayEdit->Text.c_str(),
		PulsedDialog->P7DelayStepEdit->Text.c_str(),
		PulsedDialog->P7WidthEdit->Text.c_str(),
		PulsedDialog->P7WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);
	sprintf(line,"Pulse H (Prot): delay %s ns, delaystep %s ns, width %s ns, widthstep %s ns \n",
		PulsedDialog->P8DelayEdit->Text.c_str(),
		PulsedDialog->P8DelayStepEdit->Text.c_str(),
		PulsedDialog->P8WidthEdit->Text.c_str(),
		PulsedDialog->P8WidthStepEdit->Text.c_str());
	CurrFile->AddHeader(line);	sprintf(line,"RepTime: %s microsec, Acq.delay step %s ns, Acq Gate %s ns, %s Shots/point \n",
		PulsedDialog->RateEdit->Text.c_str(),
		PulsedDialog->AcqStepEdit->Text.c_str(),
		PulsedDialog->IntTimeEdit->Text.c_str(),
		PulsedDialog->AverEdit->Text.c_str());
	CurrFile->AddHeader(line);
//	sprintf(line,"Modulation FM %8.3f kHz, modulation AM %8.3f percent \n",
//		ENDORParametersDialog->FMEdit->Text.ToDouble(),
//		ENDORParametersDialog->AMEdit->Text.ToDouble());
//	CurrFile->AddHeader(line);

	AddInstrumentHeader();

//----
//-----

	for (int i = 0; i< CurrFile->GetnPlots(); i++)
	{
		CurrFile->GetPlot(i)->SetAutoRange(0);
		CurrFile->GetPlot(i)->SetFixedRanges(Pmin, Pmax);
		CurrFile->GetPlot(i)->SetAutoY1();
		CurrFile->GetPlot(i)->SetAutoY2();
		if (Persistent == 0) CurrFile->GetPlot(i)->SetAxisTitle("Field (T)",0);
		  else CurrFile->GetPlot(i)->SetAxisTitle("Sweepcoil Field (mT)",0);
		CurrFile->GetPlot(i)->SetAxisTitle("Gate 1 Ch 1 (mV)",1);
		CurrFile->GetPlot(i)->SetAxisTitle("Gate 1 Ch 2 (mV)",2);
	}
	CurrFile->PlotData();
	//    CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
	//    CurrFile->DataReset(2,TotalPoints);
	CurrFile->Invalidate(); //replot after changing axes
	TakingData = 0;
	CurrFile->Measuring = false;
	StopMeasurement = 0;
	Interrupted = 0;
	UpdateMenuItems(this);
	MeasPointFrm->Show();
	ReturnSweep = false;
//    Infinium->RetrieveOffsets();
//    Infinium->RetrieveIncrements();
	if (Tek7->GetAddress() > 0)
	{
		if (MeasurementType == 7 ) Tek7->RetrieveScaleSettings(1);
		  else Tek7->RetrieveScaleSettings(0);
//		Tek7->SetDefaultFormat();
	}
	  else
	  {
		Infinium->RetrieveScaleSettings();
		Infinium->SetDefaultFormat();
	  }

//-----
	if (Persistent==0)
	{
		double tempspeed = EPRMainSweepDialog->GetSweepSpeed();
		if (tempspeed < 4.0) tempspeed = 4.0;
		Magnet->SetFieldSweepSpeed(tempspeed);
		Magnet->SetField(EPRMainSweepDialog->GetStartField());
		Magnet->SetAction(1);
		GoToStartFieldTimer->Interval = 1000;
		GoToStartFieldTimer->Enabled = true;
	}
	  else
	  {
		int status = SweepCoil->SetAction(0);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting field to HOLD";
			ErrorBox->ShowModal();
			return;
		}

		status = SweepCoil->SetFieldSweepSpeed(99);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting SweepSpeed";
			ErrorBox->ShowModal();
			return;
		}

		status = SweepCoil->SetField(EPRSweepCoilDialog->GetStartField());
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting field";
			ErrorBox->ShowModal();
			return;
		}
		status = SweepCoil->SetAction(1);
		if (status !=0)
		{
			ErrorBox->ErrorMessage->Caption =
				"Error setting SweepCoil to GOTO SETPOINT";
			ErrorBox->ShowModal();
			return;
		}
		GoToSweepStartTimer->Interval = 1000;
		GoToSweepStartTimer->Enabled = true;
	  }

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::AnalyserReadMenuItemClick(TObject *Sender)
{
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	if (!MyFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
        "This file contains data \n Please open a new file first";
        ErrorBox->ShowModal();
        MyFile = NULL;
        return;
    }
    CurrFile = MyFile;

    time_t timer = time(NULL);  // get the time and date
    CurrFile->SetHeader("Measurement made ");
    CurrFile->AddHeader(asctime(localtime(&timer)));

    char line[255];
    sprintf(line,"Resolution BW: %f kHz \n",Analyser->ResBW());
    CurrFile->AddHeader(line);

    sprintf(line,"     Video BW: %f kHz \n",Analyser->VideoBW());
    CurrFile->AddHeader(line);

    int chan = 1;
    int npts = 0;
    DataArray NewData;
    npts = Analyser->GetData(0, &NewData);
    if (npts > 0)
       MyFile->SetData(NewData);

    CurrFile->GetPlot()->SetAxisTitle("Frequency (MHz)",0);
    CurrFile->GetPlot()->SetAxisTitle("Power(dB)",1);
    CurrFile->GetPlot()->SetAxisTitle(" ",2);
    CurrFile->GetPlot()->SetDataSize(2);
    return;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::PrintMenuItemClick(TObject *Sender)
{
    TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
    char* Heading;
    int HeadSize = MyFile->GetHeaderSize() + 1;
    Heading = new char[HeadSize];
    MyFile->GetHeader(Heading, HeadSize);
// We'll find the linebreaks in the header

    int lb[100];
    int lbi = 1;
    lb[0] = -1;
    int j=0;
    while ((Heading[j] != 0) && (j<HeadSize) && (lbi<100)) // while not end of heading
    {
         if (Heading[j] == '\n') lb[lbi++] = j;
         j++;
    }

    int nlines = lbi-1;
	int CharacterHeight = Printer()->Canvas->TextHeight("6789");

    AnsiString HeaderText = Heading;
    AnsiString HeaderLine;
    if (PrintDialog->Execute())
    {
        Printer()->Orientation = poPortrait;
        Printer()->BeginDoc();
        MyFile->PrintData();
        for (int i=0; i<nlines; i++)
        {
            HeaderLine = HeaderText.SubString(lb[i]+2, lb[i+1]-(lb[i]+1));
        Printer()->Canvas->TextOut(200,CharacterHeight*i+Printer()->PageHeight/2,
                        HeaderLine);
        }
        Printer()->Canvas->Font->Style = TFontStyles()<< fsBold;
        Printer()->Canvas->TextOut(200, Printer()->PageHeight/2  - CharacterHeight,
            MyFile->Caption);
        Printer()->Canvas->Font->Style = TFontStyles();

		Printer()->EndDoc();
	}

	delete[] Heading;             // clean up

//    MyFile->Print();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::DataCurves1Click(TObject *Sender)
{
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	PlotCurvesDlg->SetDataFile(MyFile);

	PlotCurvesDlg->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RePhase1Click(TObject *Sender)
{
	// will work only for a valid 2 y-axes data file
	// or a (more than) 4 column data file
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	if ((MyFile==CurrFile) && (StopMeasurement != 1))
	{
		ErrorBox->ErrorMessage->Caption =
		"You're still measuring !!";
		ErrorBox->ShowModal();
		MyFile = NULL;
		return;
	}

	if ((MyFile->GetData()->Getn() <= 0) || ((MyFile->GetData()->GetNy() < 4)
		&& (MyFile->GetData()->GetNy() != 2)))
		return;

	if (MyFile->GetData()->GetNy() == 2)
	{
		RePhaseDialog->ModPhase2Edit->Visible = false;
		RePhaseDialog->Label3->Visible = false;
		RePhaseDialog->WavePhaseEdit->Visible = false;
		RePhaseDialog->Label2->Visible = false;
		RePhaseDialog->UpDown3->Visible = false;
		RePhaseDialog->UpDown2->Visible = false;
	}
	  else
	  {
		RePhaseDialog->ModPhase2Edit->Visible = true;
		RePhaseDialog->Label3->Visible = true;
		RePhaseDialog->WavePhaseEdit->Visible = true;
		RePhaseDialog->Label2->Visible = true;
		RePhaseDialog->UpDown3->Visible = true;
		RePhaseDialog->UpDown2->Visible = true;
	  }


	double LockInPhase = 0.0;

	RePhaseDialog->SetDataFile(MyFile);
	if (RePhaseDialog->ShowModal() == mrOk)
	{
		if (RePhaseDialog->AutoPhaseCheckBox->Checked == true)
		{
			LockInPhase = LockIn1->GetPhase();
			LockInPhase += RePhaseDialog->GetPh1();
			if (LockInPhase > 180.0) LockInPhase -= 360.0;
			if (LockInPhase < -180.0) LockInPhase -= 360.0;
			LockIn1->SetModulationPhase(LockInPhase);

			LockInPhase = LockIn2->GetPhase();
			LockInPhase += RePhaseDialog->GetPh2();
			if (LockInPhase > 180.0) LockInPhase -= 360.0;
			if (LockInPhase < -180.0) LockInPhase -= 360.0;
			LockIn2->SetModulationPhase(LockInPhase);
		}

	}


	MyFile = NULL;

}
//---------------------------------------------------------------------------



void __fastcall TMainForm::TransientCorrectItemClick(TObject *Sender)
{
    TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
    if ((MyFile==CurrFile) && (StopMeasurement != 1))
    {
        ErrorBox->ErrorMessage->Caption =
        "You're still measuring !!";
        ErrorBox->ShowModal();
        MyFile = NULL;
        return;
    }
	TransientCorrDlg->SetDataFile(MyFile);
    TransientCorrDlg->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewParamItemClick(TObject *Sender)
{
    TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);

    int FileHeaderSize = MyFile->GetHeaderSize();
    char *FileHeader = new char[FileHeaderSize];
    MyFile->GetHeader(FileHeader,FileHeaderSize-1);
    ParamViewForm->HeaderMemo->Lines->SetText(FileHeader);
    ParamViewForm->Show();
    delete[] FileHeader;
    MyFile = NULL;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N6GHzSynthesizer1Click(TObject *Sender)
{
    //  the synthesizer settings all in MHz
    double Freq;

    if (GetSynFreqDialog->ShowModal() == mrOk )
    {
        if (!ValidReal(GetSynFreqDialog->FreqEdit->Text, &Freq)) return;

        SetSynthesizer(Freq);

    }
    else return;
}
//---------------------------------------------------------------------------

int TMainForm::SetSynthesizer(double Freq)
{
    double MinFreq = 5200.0;
    double MaxFreq = 6200.0;
    double Interval = 0.5;

    int high = 4000;
    int low = 0;

    if (Freq > MaxFreq) return -1;
    if (Freq < MinFreq) return -1;

    unsigned short int IntFreq = (short)( Freq /Interval);

    LockIn2->SetDAC(3,low); // enable line down (D9 pin 3)
    LockIn2->SetDAC(1,low);

    // We're going to send 24 bits
    // first send 'C'
    char c = 'C';

    int bits[24];
    for (int i=0; i<24; i++) bits[i] = low;

    if (c & 0x01) bits[7] = high;
    if (c & 0x02) bits[6] = high;
    if (c & 0x04) bits[5] = high;
    if (c & 0x08) bits[4] = high;
    if (c & 0x10) bits[3] = high;
    if (c & 0x20) bits[2] = high;
    if (c & 0x40) bits[1] = high;
    if (c & 0x80) bits[0] = high;

    if (IntFreq & 0x0001) bits[23] = high;
    if (IntFreq & 0x0002) bits[22] = high;
    if (IntFreq & 0x0004) bits[21] = high;
    if (IntFreq & 0x0008) bits[20] = high;
    if (IntFreq & 0x0010) bits[19] = high;
    if (IntFreq & 0x0020) bits[18] = high;
    if (IntFreq & 0x0040) bits[17] = high;
    if (IntFreq & 0x0080) bits[16] = high;
    if (IntFreq & 0x0100) bits[15] = high;
    if (IntFreq & 0x0200) bits[14] = high;
    if (IntFreq & 0x0400) bits[13] = high;
    if (IntFreq & 0x0800) bits[12] = high;
    if (IntFreq & 0x1000) bits[11] = high;
    if (IntFreq & 0x2000) bits[10] = high;
    if (IntFreq & 0x4000) bits[9] = high;
    if (IntFreq & 0x8000) bits[8] = high;


    for (int i=0; i<24; i++)
    {
        LockIn2->SetDAC(1,low);            //clock
        LockIn2->SetDAC(2,bits[i]);        // data
        LockIn2->SetDAC(1,high);           //clock
    }

    LockIn2->SetDAC(1,low);
    LockIn2->SetDAC(3,high);
    LockIn2->SetDAC(2,low);

    return IntFreq;
};


void __fastcall TMainForm::ViewEditLogFile1Click(TObject *Sender)
{
        LogFileForm->Show();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::MeasureMenuFullItemClick(TObject *Sender)
{
    if (MeasureMenuFullItem->Checked == true)
    {
        if ( MagnetSystem == 0 )
        {
            CWEPRMenuItem->Visible = false;
            CWEPRMenuItem->Enabled = false;
            HeteroEPRMenuItem->Visible = true;
            HeteroEPRMenuItem->Enabled = true;
            HetTransientMenuItem->Visible = true;
            HetTransientMenuItem->Enabled = true;
            InfiniumMenuItem->Visible = true;
            InfiniumMenuItem->Enabled = true;
        }
          else
          {
            CWEPRMenuItem->Visible = true;
            CWEPRMenuItem->Enabled = true;
            HeteroEPRMenuItem->Visible = false;
            HeteroEPRMenuItem->Enabled = false;
            HetTransientMenuItem->Visible = false;
            HetTransientMenuItem->Enabled = false;
            InfiniumMenuItem->Visible = false;
            InfiniumMenuItem->Enabled = false;
          }

        TransientEPRMenuItem->Visible = false;
        TransientEPRMenuItem->Enabled = false;
        TransientEPR2MenuItem->Visible = false;
        TransientEPR2MenuItem->Enabled = false;
        MeasureMenuFullItem->Checked = false;

    }
       else
      {
        CWEPRMenuItem->Visible = true;
        CWEPRMenuItem->Enabled = true;
        HeteroEPRMenuItem->Visible = true;
        HeteroEPRMenuItem->Enabled = true;
        HetTransientMenuItem->Visible = true;
        HetTransientMenuItem->Enabled = true;
		InfiniumMenuItem->Visible = true;
        InfiniumMenuItem->Enabled = true;
        TransientEPRMenuItem->Visible = true;
        TransientEPRMenuItem->Enabled = true;
        TransientEPR2MenuItem->Visible = true;
        TransientEPR2MenuItem->Enabled = true;
        MeasureMenuFullItem->Checked = true;
      }
    return;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::DaQtest1Click(TObject *Sender)
{
        DAQ_Dialog->ShowModal();
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::Pulsed1Click(TObject *Sender)
{
	PulsedDialog->ShowModal();
	if (PulsedDialog->ModalResult == mrOk)
	{
		if (PulsedDialog->SweepTypeListBox->ItemIndex == 0)
		{
			PulsedPrepare();
		} else if (PulsedDialog->SweepTypeListBox->ItemIndex == 1)
			{
				PulsedSweepPrepare();
			} else if (PulsedDialog->SweepTypeListBox->ItemIndex == 2)
				{		PulsedEndorPrepare();}
	}
	else return;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::MeasurePulsTimerTimer(TObject *Sender)
{
//  parameters
	double delay, x1,x2,y1,y2;
	double baselineCorrected;
//	double EndF = ENDORParametersDialog->GetEndFreq();
	int ok;
	char errorr[80];
	MeasurePulsTimer->Enabled = false;
	bool TwoChannel = true;
	if (PulsedDialog->TwoChannelCheckBox->Checked == false) TwoChannel = false;

	char TempFileName[255];
	DataPoint P(2*NumberOfGates);
	int gates = NumberOfGates;
	DataPoint *P1;
	P1 = new DataPoint(NumberOfGates);
	int mathmode = 0;

	if (Tek7->GetAddress() > 0) {
		Tek7->Stop();
//            millis = GetTickCount();
		if (PulsedDialog->QuadratureCheckBox->Checked == true)
		{
			mathmode = 1;
			Tek7->RetrieveScaleSettings(mathmode);
			ok = Tek7->RetrieveBuffer(9);
			if (TwoChannel) {
				Tek7->RetrieveScaleSettings(mathmode);
				ok = Tek7->RetrieveBuffer(10);
			}
		}
		  else
		  {

			Tek7->RetrieveScaleSettings(mathmode);
			ok = Tek7->RetrieveBuffer(1);
			if (TwoChannel) {
				Tek7->RetrieveScaleSettings(mathmode);
				ok = Tek7->RetrieveBuffer(3);
			}
		  }
//            delay1 = GetTickCount()-millis;
//            delay2 = GetTickCount()-millis;
		if (Tek7->ErrorTest(errorr) != 0)
		{
			ErrorBox->ErrorMessage->Caption = errorr;
			ErrorBox->ShowModal();
		}
			if (ok==-2)
			{
				MeasurePulsTimer->Enabled = true;
				Tek7->Run();
				return; // if aquisition unfinished return
			}
		 //   Infinium->Stop();
//	Tek7->Clear();
//		Tek7->RetrieveScaleSettings(mathmode);
		if (Tek7->ErrorTest(errorr) != 0)
		{
			ErrorBox->ErrorMessage->Caption = errorr;
			ErrorBox->ShowModal();
		}

		if (mathmode == 1)
			ok = Tek7->GetGateValues(gates,P1,T1,T2,9);
		   else
			ok = Tek7->GetGateValues(gates,P1,T1,T2,1);
		if ((NumberOfGates == 2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
		{
			baselineCorrected = P1->Get(1) - P1->Get(2);
			P1->Set(1, baselineCorrected);
		}
		for (int i=1;i<=NumberOfGates;i++)
					P.Set(i, P1->Get(i));
		if (mathmode == 1)
			ok = Tek7->GetGateValues(gates,P1,T1,T2,10);
		   else
			ok = Tek7->GetGateValues(gates,P1,T1,T2,3);
		if ((NumberOfGates == 2) && (PulsedDialog->BaseLineCorrectionCheckBox->Checked))
		{
			baselineCorrected = P1->Get(1) - P1->Get(2);
			P1->Set(1, baselineCorrected);
		}


	  }
	  else
	  {
		Infinium->Stop();
//            millis = GetTickCount();
		ok = Infinium->RetrieveBuffer(1);
//            delay1 = GetTickCount()-millis;
		if (PulsedDialog->QuadratureCheckBox->Checked == true)
			ok = Infinium->RetrieveBuffer(11);
		  else 	ok = Infinium->RetrieveBuffer(3);

//            delay2 = GetTickCount()-millis;
		if (Infinium->ErrorTest(errorr) != 0)
		{
			ErrorBox->ErrorMessage->Caption = errorr;
			ErrorBox->ShowModal();
		}
			if (ok==-2)
			{
				MeasurePulsTimer->Enabled = true;
				Infinium->Run();
				return; // if aquisition unfinished return
			}
		 //   Infinium->Stop();
		Infinium->Clear();
		Infinium->RetrieveScaleSettings();

		ok = Infinium->GetGateValues(gates,P1,T1,T2,1);
		for (int i=1;i<=NumberOfGates;i++)
				P.Set(i, P1->Get(i));
		ok = Infinium->GetGateValues(gates,P1,T1,T2,3);
		if (Infinium->ErrorTest(errorr) != 0)
		{
			ErrorBox->ErrorMessage->Caption = errorr;
			ErrorBox->ShowModal();
		}
	}
	for (int i=1;i<=NumberOfGates;i++)
			P.Set(NumberOfGates+i, P1->Get(i));
//	if (TwoChannel==false) P.Set(NumberOfGates+1, DMM2->ReadValue());
	delete P1;
//            Infinium->Clear();
		 //   Infinium->Run();


	int step = 0;
	int wstep = 0;
	if (PulsedDialog->AcqStepEdit->Text.ToInt() == 0)
	{
		if ((step = PulsedDialog->P1DelayStepEdit->Text.ToInt()) != 0)
			delay = ((double)step)*((double)CurrentPoint)*0.001;
		  else
			if ((step = PulsedDialog->P2DelayStepEdit->Text.ToInt()) != 0)
				delay = ((double)step)*((double)CurrentPoint)*0.001;
			  else
				if ((step = PulsedDialog->P3DelayStepEdit->Text.ToInt()) != 0)
					delay = ((double)step)*((double)CurrentPoint)*0.001;
				  else
					if ((step = PulsedDialog->P4DelayStepEdit->Text.ToInt()) != 0)
						delay = ((double)step)*((double)CurrentPoint)*0.001;
		else
		if ((wstep = PulsedDialog->P1WidthStepEdit->Text.ToInt()) != 0)
			delay = ((double)wstep)*((double)CurrentPoint)*0.001;
		  else
			if ((wstep = PulsedDialog->P2WidthStepEdit->Text.ToInt()) != 0)
				delay = ((double)wstep)*((double)CurrentPoint)*0.001;
			  else
				if ((wstep = PulsedDialog->P3WidthStepEdit->Text.ToInt()) != 0)
					delay = ((double)wstep)*((double)CurrentPoint)*0.001;
				  else
					if ((wstep = PulsedDialog->P4WidthStepEdit->Text.ToInt()) != 0)
						delay = ((double)wstep)*((double)CurrentPoint)*0.001;
							else delay = CurrentPoint;

	}
	  else delay = PulsedDialog->AcqStepEdit->Text.ToInt()*CurrentPoint*0.001;

	P.Set(0, delay);


	MeasPointFrm->XLabel->Caption = delay;
//    MeasPointFrm->Y1Label->Caption = x1;
//    MeasPointFrm->Y2Label->Caption = y1;
	MeasPointFrm->PointLabel->Caption = CurrentPoint;


	char averLabel[20];
	sprintf(averLabel,"%d/%d",CurrentSweep, Averages);
	MeasPointFrm->AverageLabel->Caption = averLabel;
//
	DataArray* D = CurrFile->GetData();
	DataArray* AverD = CurrFile->GetData(1);

	DataPoint LastP(2*NumberOfGates);
	if (PulsedDialog->ChACheckBox->Checked)
	{
		if (CurrentPoint>1) {
			LastP = D->Get(CurrentPoint-1);
			if (ChannelA_On)
				P.Set(4, LastP.Get(1)-P.Get(1));
			  else
				P.Set(4, P.Get(1)-LastP.Get(1));
		}
	}

	if (TakingData)
	{
		if (Averages>1) AverD->AddToAverage(CurrentPoint, CurrentSweep,P);
		D->Add(P);
		CurrentPoint += 1;

		if (CurrentPoint > 1)
		{
			CurrFile->GetPlot()->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
			if (Averages >1) CurrFile->GetPlot(1)->LineToLast(CurrFile->Canvas,AverD->Get(CurrentPoint-2), AverD->Get(CurrentPoint-1));
		}
	}
//
/*	DataArray* D = CurrFile->GetData();

	DataPoint LastP(2*NumberOfGates);

	if (PulsedDialog->ChACheckBox->Checked)
	{
		if (CurrentPoint>1) {
			LastP = D->Get(CurrentPoint-1);
			if (ChannelA_On)
				P.Set(4, LastP.Get(1)-P.Get(1));
			  else
				P.Set(4, P.Get(1)-LastP.Get(1));
		}
	}




	if (TakingData)
	{
		D->AddToAverage(CurrentPoint, CurrentSweep,P);
		CurrentPoint += 1;

		if (CurrentPoint > 1)
		CurrFile->GetPlot()->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
	}
*/
/*	if ((CurrentPoint >= TotalPoints) && (CurrentSweep < Averages))
	{
		CurrentPoint = 0;
		sprintf(TempFileName,"C:\\TempData\\ENDOR%03d.dat",CurrentSweep);
		CurrFile->WriteData(0,TempFileName);
		CurrFile->GetData(0)->flush();
		CurrentSweep += 1;
		if (ENDORParametersDialog->Two_D_CheckBox->Checked) {
			RF2->Step(ENDORParametersDialog->GetRF2EndFreq());
		}
		RFsource->SetFreq(ENDORParametersDialog->GetStartFreq());
		PulsedEndorTimer->Enabled = true;
		return;
	}

	if ((CurrentPoint >= TotalPoints) && (CurrentSweep >= Averages))
		StopMeasurement = 1;

	double w,d;
   if (StopMeasurement == 1)
   {
		TakingData = 0;
		StopMeasurement =1;
		sprintf(TempFileName,"C:\\TempData\\ENDOR%03d.dat",CurrentSweep);
		CurrFile->WriteData(0,TempFileName);
		UpdateMenuItems(this);
		MeasPointFrm->Close();
		return;
*/	if ((CurrentPoint >= TotalPoints) && (CurrentSweep < Averages))
	{
		CurrentPoint = 0;
		sprintf(TempFileName,"C:\\TempData\\TimeSweep%03d.dat",CurrentSweep);
		CurrFile->WriteData(0,TempFileName);
		CurrFile->GetData(0)->flush();
		CurrentSweep += 1;
		PulsedDialog->SetButtonClick(NULL);
		T1[0] = PulsedDialog->AcqDelayEdit->Text.ToInt()*1e-9;
		T2[0] = T1[0] + PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;
//		NumberOfGates = 2;
		T1[1] = T1[0] + 2 * PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;
		T2[1] = T1[1] + PulsedDialog->IntTimeEdit->Text.ToInt()*1e-9;

		MeasurePulsTimer->Enabled = true;
		return;
	}

	if ((CurrentPoint >= TotalPoints) && (CurrentSweep >= Averages))
		StopMeasurement = 1;

	long long w;
	long long d;
	long long tempdelay;
	AnsiString ShortStr;

	bool large = false;
   if (StopMeasurement == 1)
   {
		TakingData = 0;
		CurrFile->Measuring = false;
		StopMeasurement =1;
		sprintf(TempFileName,"C:\\TempData\\TimeSweep%03d.dat",CurrentSweep);
		CurrFile->WriteData(0,TempFileName);

		UpdateMenuItems(this);
		MeasPointFrm->Close();
		return;
   }

	else
   {
	  if (TakingData)
	  {
		  if ((PulsedDialog->P1DelayStepEdit->Text.ToInt()>0) && (PulsedDialog->P1CheckBox->Checked))
		  {
				d = (long long)PulsedDialog->P1DelayEdit->Text.ToInt() + CurrentPoint *
						(long long)PulsedDialog->P1DelayStepEdit->Text.ToInt();
				PulseSource->SetDelay(1, d);
		  }
		  if ((PulsedDialog->P1WidthStepEdit->Text.ToInt()>0) && (PulsedDialog->P1CheckBox->Checked))
		  {
				w = (long long)PulsedDialog->P1WidthEdit->Text.ToInt() + CurrentPoint *
						PulsedDialog->P1WidthStepEdit->Text.ToInt();
				PulseSource->SetWidth(1, w);
		  }
		  if ((PulsedDialog->P2DelayStepEdit->Text.TrimLeft()).Length()>9)
		  {
			large = true;
			ShortStr = PulsedDialog->P2DelayStepEdit->Text.SubString(1, PulsedDialog->P2DelayStepEdit->Text.Length()-3);
			tempdelay = ShortStr.ToInt();
		  }
		  if (PulsedDialog->P2CheckBox->Checked)
		  {
				if (large)
					d = (long long)PulsedDialog->P2DelayEdit->Text.ToInt() + CurrentPoint * tempdelay * 1000;
				  else
					d = (long long)PulsedDialog->P2DelayEdit->Text.ToInt() + CurrentPoint *
						(long long)PulsedDialog->P2DelayStepEdit->Text.ToInt();
				PulseSource->SetDelay(2, d);
				large = false;
		  }
		  if ((PulsedDialog->P2WidthStepEdit->Text.ToInt()>0) && (PulsedDialog->P2CheckBox->Checked))
		  {
				w = (long long)PulsedDialog->P2WidthEdit->Text.ToInt() + CurrentPoint *
						PulsedDialog->P2WidthStepEdit->Text.ToInt();
				PulseSource->SetWidth(2, w);
		  }
		  if ((PulsedDialog->P3DelayStepEdit->Text.TrimLeft()).Length()>9)
		  {
			large = true;
			ShortStr = PulsedDialog->P3DelayStepEdit->Text.SubString(1, PulsedDialog->P3DelayStepEdit->Text.Length()-3);
			tempdelay = ShortStr.ToInt();
		  }
		  if (PulsedDialog->P3CheckBox->Checked)
		  {
				if (large)
					d = (long long)PulsedDialog->P3DelayEdit->Text.ToInt() + CurrentPoint * tempdelay * 1000;
				  else
					d = (long long)PulsedDialog->P3DelayEdit->Text.ToInt() + CurrentPoint *
						(long long)PulsedDialog->P3DelayStepEdit->Text.ToInt();
				PulseSource->SetDelay(3, d);
		  }
		  if ((PulsedDialog->P3WidthStepEdit->Text.ToInt()>0) && (PulsedDialog->P3CheckBox->Checked))
		  {
				w = (long long)PulsedDialog->P3WidthEdit->Text.ToInt() + CurrentPoint *
						PulsedDialog->P3WidthStepEdit->Text.ToInt();
				PulseSource->SetWidth(3, w);
		  }
		  if ((PulsedDialog->P4DelayStepEdit->Text.ToInt()>0) && (PulsedDialog->P4CheckBox->Checked))
		  {
				d = (long long)PulsedDialog->P4DelayEdit->Text.ToInt() + CurrentPoint *
						(long long)PulsedDialog->P4DelayStepEdit->Text.ToInt();
				PulseSource->SetDelay(4, d);
		  }
		  if ((PulsedDialog->P4WidthStepEdit->Text.ToInt()>0) && (PulsedDialog->P4CheckBox->Checked))
		  {
				w = (long long)PulsedDialog->P4WidthEdit->Text.ToInt() + CurrentPoint *
						PulsedDialog->P4WidthStepEdit->Text.ToInt();
				PulseSource->SetWidth(4, w);
		  }
		  if ((PulsedDialog->P6DelayStepEdit->Text.ToInt()>0) && (PulsedDialog->P6CheckBox->Checked))
		  {
				d = (long long)PulsedDialog->P6DelayEdit->Text.ToInt() + CurrentPoint *
						(long long)PulsedDialog->P6DelayStepEdit->Text.ToInt();
				PulseSource->SetDelay(6, d);
		  }
		  if ((PulsedDialog->P6WidthStepEdit->Text.ToInt()>0) && (PulsedDialog->P6CheckBox->Checked))
		  {
				w = (long long)PulsedDialog->P6WidthEdit->Text.ToInt() + CurrentPoint *
						PulsedDialog->P6WidthStepEdit->Text.ToInt();
				PulseSource->SetWidth(6, w);
		  }
		  for (int i=0; i < NumberOfGates; i++)
		  {
			T1[i]+= (double)PulsedDialog->AcqStepEdit->Text.ToInt()*1e-9;
			T2[i]+= (double)PulsedDialog->AcqStepEdit->Text.ToInt()*1e-9;
		  }

//		  if (PulsedDialog->AcqStepEdit->Text.ToInt() > 0) {
			double TekDelay = (double)PulsedDialog->AcqDelayEdit->Text.ToInt() + CurrentPoint*
				(double)PulsedDialog->AcqStepEdit->Text.ToInt();


		   if (Tek7->GetAddress() >0)

				Tek7->SetDelay(TekDelay*1e-9);
				else Infinium->SetDelay(TekDelay*1e-9);

 //		  }
 //		Infinium->RetrieveScaleSettings();
			if (PulsedDialog->ChACheckBox->Checked)
			{
				if (ChannelA_On) {
					PulseSource->Disable(1);
					ChannelA_On = false;
					Tek7->SetDelay((TekDelay+1)*1e-9);
				}
				 else {
					PulseSource->Enable(1);
					ChannelA_On = true;
					Tek7->SetDelay(TekDelay*1e-9);
				}
			}

// We need a delay of a single trigger pulse to make sure
// that the pulses are changed
	int ddel = PulsedDialog->RateEdit->Text.ToInt()/1000;
	DWORD start=GetTickCount();
	do{;} while((GetTickCount()-start)<ddel);

		if (Tek7->GetAddress() >0) Tek7->Run();
		   else Infinium->Run();

	  }
	  MeasurePulsTimer->Enabled = true;
   }

   UpdateMenuItems(this);
}


//---------------------------------------------------------------------------

void __fastcall TMainForm::SweepMenuItemClick(TObject *Sender)
{
	CurrFile = dynamic_cast<TDataFile*> (ActiveMDIChild); // GetDocumentPointer

//  Test if the file is empty, if not display message
//  and return
	if (!CurrFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		CurrFile = NULL;  // Reset CurrFile  to NULL on end of measurement
		return;
	}

	CurrentSweep = 0;  // for averaging purposes
	CurrentPoint = 0;  // npoints in sweep

	Averages = 1;      // Default averages
	int av;

	DataPoint Pmin(4);      // DataPoints for PlotLimits
	DataPoint Pmax(4);
							// Change this for ADC readings

// Get the parameters
	DacSweepDialog->ShowModal();
	if (DacSweepDialog->ModalResult != mbOK) return;

// Set StartFrequency

    DAQ->InitDAC();
	DAQ->SetDAC(0,DacSweepDialog->StartValue);
// Set StepSize
//	RFsource->SetStep(ENDORParametersDialog->GetStep());
// Set Power ON
//	RFsource->SetPower(ENDORParametersDialog->GetPower());
// Set Amplifier ON
	MeasurementType = 1;

//	Averages = DacSweepDialog->AverageEdit->Text.ToInt();
	TotalPoints = DacSweepDialog->NptsSweep;
	CurrentSweep = 1;
	CurrentPoint = 0;

	Pmin.Set(0,-DacSweepDialog->StartValue);
	Pmax.Set(0,DacSweepDialog->StartValue);
	double range = LockIn1->RealSensitivity();
	if (range > 0)
	{
		Pmin.Set(1, -range);
		Pmax.Set(1,range);
		Pmin.Set(2, -range);
		Pmax.Set(2,range);
	}

	CurrFile->GetPlot()->SetAutoRange(0);
	CurrFile->GetPlot()->SetFixedRanges(Pmin, Pmax);
	CurrFile->GetPlot()->SetAutoY1();
	CurrFile->GetPlot()->SetAutoY2();
	CurrFile->GetPlot()->SetAxisTitle("DAC setting",0);
	CurrFile->GetPlot()->SetAxisTitle("X-signal (mV)",1);
	CurrFile->GetPlot()->SetAxisTitle("Y-signal (mV)",2);

	CurrFile->GetPlot()->PlotTheAxes(CurrFile->Canvas);
	CurrFile->DataReset(4,TotalPoints);

	time_t timer = time(NULL);  // get the time and date

	CurrFile->SetHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));
	char line[128];
/*	sprintf(line,"ENDOR measurement %8.3f to %8.3f MHz in %d steps of %8.3f kHz \n",
		ENDORParametersDialog->GetStartFreq(),ENDORParametersDialog->GetEndFreq(),
		  TotalPoints, ENDORParametersDialog->GetStep());
	CurrFile->AddHeader(line);
	sprintf(line,"RF power %8.3f dB, dwelltime %d ms, %d averages \n",
		ENDORParametersDialog->GetPower(), ENDORParametersDialog->DwellEdit->Text.ToInt(),
		Averages);
	CurrFile->AddHeader(line);
	sprintf(line,"Modulation FM %8.3f kHz, modulation AM %8.3f percent \n",
		ENDORParametersDialog->FMEdit->Text.ToDouble(),
		ENDORParametersDialog->AMEdit->Text.ToDouble());
	CurrFile->AddHeader(line);
*/
	AddInstrumentHeader();

	TakingData = 1;
	CurrFile->Measuring = true;
	StopMeasurement = 0;
	UpdateMenuItems(this);
	MeasPointFrm->Show();

//   Magnet->SetField(EPRMainSweepDialog->GetStartField());
//   Magnet->SetAction(1);
	switch (DacSweepDialog->DelayComboBox->ItemIndex)
	{
		case 0: DacSweepTimer->Interval = 10; break;
		case 1: DacSweepTimer->Interval = 30; break;
		case 2: DacSweepTimer->Interval = 100; break;
		case 3: DacSweepTimer->Interval = 300; break;
		case 4: DacSweepTimer->Interval = 1000; break;
		default: DacSweepTimer->Interval = 100; break;
	}
	DacSweepTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DacSweepTimerTimer(TObject *Sender)
{
//  parameters
	double Freq, x1,x2,y1,y2;
//	double EndF = DacSweepDialog->StartValue +
//				DacSweepDialog->NptsSweep*DacSweepDialog->StepSize;

	DacSweepTimer->Enabled = false;

	DataPoint P(4);

	LockIn1->ReadXY(&x1,&y1);
	LockIn2->ReadXY(&x2,&y2);
	P.Set(1,x1);
	P.Set(2,x2);
	P.Set(3,y1);
	P.Set(4,y2);

	Freq = DacSweepDialog->StartValue +
				CurrentPoint*DacSweepDialog->StepSize;
	if (Freq > 65535)
	{
		Freq = 32568;
	}
	P.Set(0, Freq);

	MeasPointFrm->XLabel->Caption = Freq;
//    MeasPointFrm->Y1Label->Caption = x1;
//    MeasPointFrm->Y2Label->Caption = y1;
	MeasPointFrm->PointLabel->Caption = CurrentPoint;


	char averLabel[20];
	sprintf(averLabel,"%d/%d",CurrentSweep, Averages);
	MeasPointFrm->AverageLabel->Caption = averLabel;

	DataArray* D = CurrFile->GetData();
	if (TakingData)
	{
		D->AddToAverage(CurrentPoint, CurrentSweep,P);
		CurrentPoint += 1;

		if (CurrentPoint > 1)
		CurrFile->GetPlot()->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
	}

	if ((CurrentPoint >= TotalPoints) && (CurrentSweep < Averages))
	{
		CurrentPoint = 0;
		DAQ->SetDAC(0,DacSweepDialog->StartValue);
		CurrentSweep += 1;
		DacSweepTimer->Enabled = true;
		return;
	}

	if ((CurrentPoint >= TotalPoints) && (CurrentSweep >= Averages))
		StopMeasurement = 1;

   if (StopMeasurement == 1)
   {
		TakingData = 0;
		CurrFile->Measuring = false;
		StopMeasurement =1;
		UpdateMenuItems(this);
		DAQ->SetDAC(0,32768);
		MeasPointFrm->Close();
		return;
   }
	else
   {
	  if (TakingData)
		if (DAQ->SetDAC(0,Freq+DacSweepDialog->StepSize) < 0)
			ErrorBox->ShowModal();
	  DacSweepTimer->Enabled = true;
   }

   UpdateMenuItems(this);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PulsedEndorTimerTimer(TObject *Sender)
{
//  parameters
	double Fstep, Freq, x1,x2,y1,y2;
	double EndF = ENDORParametersDialog->GetEndFreq();
	int ok;
	char errorr[80];
	PulsedEndorTimer->Enabled = false;
	bool TwoChannel = true;
	if (PulsedDialog->TwoChannelCheckBox->Checked == false) TwoChannel = false;

	DataPoint P(2*NumberOfGates);
	int gates = NumberOfGates;
	DataPoint *P1;
	P1 = new DataPoint(gates);
	int mathmode = 0;
	char TempFileName[255];
//

	if (Tek7->GetAddress() > 0)
	{
		Tek7->Stop();
//            millis = GetTickCount();
		if (PulsedDialog->QuadratureCheckBox->Checked == true)
		{
			mathmode = 1;
			Tek7->RetrieveScaleSettings(mathmode);
			ok = Tek7->RetrieveBuffer(9);
			if (TwoChannel) {
				Tek7->RetrieveScaleSettings(mathmode);
				ok = Tek7->RetrieveBuffer(10);
			}
		}
		  else
		  {

			Tek7->RetrieveScaleSettings(mathmode);
			ok = Tek7->RetrieveBuffer(1);
			if (TwoChannel) {
				Tek7->RetrieveScaleSettings(mathmode);
				ok = Tek7->RetrieveBuffer(3);
			}
		  }
		if (Tek7->ErrorTest(errorr) != 0)
		{
			ErrorBox->ErrorMessage->Caption = errorr;
			ErrorBox->ShowModal();
		}
		if (ok==-2)
		{
			PulsedEndorTimer->Enabled = true;
			Tek7->Run();
				return; // if aquisition unfinished return
		}
//		Infinium->Clear();
		if (Tek7->ErrorTest(errorr) != 0)
		{
			ErrorBox->ErrorMessage->Caption = errorr;
			ErrorBox->ShowModal();
		}

		if (mathmode == 1) ok = Tek7->GetGateValues(gates,P1,T1,T2,9);
		   else ok = Tek7->GetGateValues(gates,P1,T1,T2,1);
		for (int i=1;i<=NumberOfGates;i++)
				P.Set(i, P1->Get(i));
		if (mathmode == 1) ok = Tek7->GetGateValues(gates,P1,T1,T2,10);
		   else ok = Tek7->GetGateValues(gates,P1,T1,T2,3);
	  }
	  else
	  {
		Infinium->Stop();
//            millis = GetTickCount();
		ok = Infinium->RetrieveBuffer(1);
//            delay1 = GetTickCount()-millis;
		ok = Infinium->RetrieveBuffer(3);
//            delay2 = GetTickCount()-millis;
		if (Infinium->ErrorTest(errorr) != 0)
		{
			ErrorBox->ErrorMessage->Caption = errorr;
			ErrorBox->ShowModal();
		}
		if (ok==-2)
		{
			PulsedEndorTimer->Enabled = true;
			Infinium->Run();
			return; // if aquisition unfinished return
		}
		Infinium->Clear();
		Infinium->RetrieveScaleSettings();

		ok = Infinium->GetGateValues(gates,P1,T1,T2,1);
		for (int i=1;i<=NumberOfGates;i++)
				P.Set(i, P1->Get(i));
		ok = Infinium->GetGateValues(gates,P1,T1,T2,3);
		if (Infinium->ErrorTest(errorr) != 0)
		{
			ErrorBox->ErrorMessage->Caption = errorr;
			ErrorBox->ShowModal();
		}
	}
	for (int i=1;i<=NumberOfGates;i++)
			P.Set(NumberOfGates+i, P1->Get(i));
	delete P1;
//            Infinium->Clear();
		 //   Infinium->Run();

	Freq = RFsource->GetFreq();
	P.Set(0, Freq);
	Fstep = RFsource->GetStep();

	MeasPointFrm->XLabel->Caption = Freq;
	MeasPointFrm->PointLabel->Caption = CurrentPoint;

	char averLabel[20];
	sprintf(averLabel,"%d/%d",CurrentSweep, Averages);
	MeasPointFrm->AverageLabel->Caption = averLabel;

	DataArray* D = CurrFile->GetData();
	DataArray* AverD = CurrFile->GetData(1);
	if (TakingData)
	{
		if (Averages>1) AverD->AddToAverage(CurrentPoint, CurrentSweep,P);
		D->Add(P);
		CurrentPoint += 1;

		if (CurrentPoint > 1)
		{
			CurrFile->GetPlot()->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
			if (Averages >1) CurrFile->GetPlot(1)->LineToLast(CurrFile->Canvas,AverD->Get(CurrentPoint-2), AverD->Get(CurrentPoint-1));
		}
	}

	if ((CurrentPoint >= TotalPoints) && (CurrentSweep < Averages))
	{
		CurrentPoint = 0;
		sprintf(TempFileName,"C:\\TempData\\ENDOR%03d.dat",CurrentSweep);
		CurrFile->WriteData(0,TempFileName);
		CurrFile->GetData(0)->flush();
		CurrentSweep += 1;
		if (ENDORParametersDialog->Two_D_CheckBox->Checked) {
			RF2->SetFreq(ENDORParametersDialog->GetRF2EndFreq());
		}
		RFsource->SetFreq(ENDORParametersDialog->GetStartFreq());
		PulsedEndorTimer->Enabled = true;
		return;
	}

	if ((CurrentPoint >= TotalPoints) && (CurrentSweep >= Averages))
		StopMeasurement = 1;

	double w,d;
   if (StopMeasurement == 1)
   {
		TakingData = 0;
		CurrFile->Measuring = false;
		StopMeasurement =1;
		sprintf(TempFileName,"C:\\TempData\\ENDOR%03d.dat",CurrentSweep);
		CurrFile->WriteData(0,TempFileName);
		UpdateMenuItems(this);
		MeasPointFrm->Close();
		return;
   }
	else
   {
	  if (TakingData)
	  {
		RFsource->SetFreq(Freq+Fstep/1000.0);
		if (Tek7->GetAddress() > 0) Tek7->Run(); else Infinium->Run();
	  }
	  PulsedEndorTimer->Enabled = true;
   }

   UpdateMenuItems(this);

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ek7104Read1Click(TObject *Sender)
{
	TDataFile *MyFile = dynamic_cast<TDataFile*> (ActiveMDIChild);
	if (!MyFile->IsEmpty())

	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		MyFile = NULL;
		return;
	}
	CurrFile = MyFile;

	time_t timer = time(NULL);  // get the time and date
	CurrFile->SetHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));

	int chan = 1;
	int npts = 0;
	DataArray NewData;
	ChannelSelectDialog->ShowModal();
	if (ChannelSelectDialog->ModalResult == mrOk)
	{
		chan = ChannelSelectDialog->GetChannel();
		if ((chan > 0) && (chan <= 12)) npts = Tek7->GetData(chan, &NewData);
		if (chan == 13)
		{
			npts = Tek7->GetData(1, 3, &NewData);
		}
	}
	char line[80];
	sprintf(line, "Channel %2d \n",chan);
	if (npts > 0)
	{
		MyFile->SetData(NewData);
		AddInstrumentHeader();
		CurrFile->AddHeader(line);
	}
	MyFile = NULL;
	CurrFile = NULL;
	return;

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::TimeSweepClick(TObject *Sender)
{
	int nDaqMeas = 0;

	// Here we should open a dialog box
	// For now I'll just measure the temperature and the DMMs



	if (DAQ->IsOpen())
	{
		nDaqMeas = DAQ->GetnActiveChannels();
	}

	CurrFile = dynamic_cast<TDataFile*> (ActiveMDIChild); // GetDocumentPointer

//  Test if the file is empty, if not display message
//  and return
	if (!CurrFile->IsEmpty())
	{
		ErrorBox->ErrorMessage->Caption =
		"This file contains data \n Please open a new file first";
		ErrorBox->ShowModal();
		CurrFile = NULL;  // Reset CurrFile  to NULL on end of measurement
		return;
	}

	int status;        // for Remote commands checking
	CurrentSweep = 0;  // for averaging purposes
	CurrentPoint = 0;  // npoints in sweep
	NumberOfMeas = 10;
	Averages = 1;      // Default averages
	int av;

	double Xmin, Xmax;
// Get the sweep parameters
	char line[80];

	bool Persistent = false;
// Possibility 1: Main magnet non-persistent.
	CurrFile->SetAverage(0);

	Xmax = 3600;                      // one hour
	Xmin = 0;

	CurrFile->AllDataReset(NumberOfMeas);  //Reset Data-arrays for correct number of y-values
	DataPoint Pmin(NumberOfMeas);      // DataPoints for PlotLimits
	DataPoint Pmax(NumberOfMeas);

	Pmin.Set(0,Xmin);
	Pmax.Set(0,Xmax);

// Set the y-plot limits.
	double range = LockIn1->RealSensitivity();
	double tc1, tc2;
	if (LockIn1->MeasTimeConstant(&tc1) <0) tc1 = 0.1;
	if (LockIn2->MeasTimeConstant(&tc2) <0) tc2 = 0.1;
	if (range > 0)
	{
		range *= 1000.0;                         // for millivolts
		Pmin.Set(1, -range);
		Pmax.Set(1,range);
		Pmin.Set(2, -range);
		Pmax.Set(2,range);
	}
	time_t timer = time(NULL);  // get the time and date
	CurrFile->SetHeader("Measurement made ");
	CurrFile->AddHeader(asctime(localtime(&timer)));
	if (Persistent)
	{
		sprintf(line,"Main magnet persistent at %8.5f T \n",
				Magnet->GetParameter(18));
		CurrFile->AddHeader(line);

		sprintf(line,"SweepCoil Sweep form %8.3f to %8.3f mT at %6.4f mT/s \n",
		   EPRSweepCoilDialog->GetStartField(),
			 EPRSweepCoilDialog->GetEndField(),
			   EPRSweepCoilDialog->GetSweepSpeed());
		CurrFile->AddHeader(line);
	}
	  else
	  {
		sprintf(line,"Main magnet sweep %8.5f to %8.5f Tesla at %6.3f mT/s \n",
		   EPRMainSweepDialog->GetStartField(),
		   EPRMainSweepDialog->GetEndField(),
		   EPRMainSweepDialog->GetSweepSpeed());
		CurrFile->AddHeader(line);
		if (SweepCoil->GetAddress() != 0)
		{
			sprintf(line,"SweepCoil at %8.3f mT \n",SweepCoil->GetField());
			CurrFile->AddHeader(line);
		}
	  }


	AddInstrumentHeader();

	for (int i = 0; i< CurrFile->GetnPlots(); i++)
	{
		CurrFile->GetPlot(i)->SetAutoRange(0);
		CurrFile->GetPlot(i)->SetFixedRanges(Pmin, Pmax);
		CurrFile->GetPlot(i)->SetAxisTitle("X-signal (mV)",1);
		CurrFile->GetPlot(i)->SetAxisTitle("Y-signal (mV)",2);
		CurrFile->GetPlot(i)->SetAxisTitle("X-value",0); // Set X-axis title
	}
	CurrFile->PlotData();
		//    CurrFile->DataReset(2,TotalPoints);

	CurrFile->Invalidate(); //replot after changing axes
	TakingData = 0;
	CurrFile->Measuring = false;
	StopMeasurement = 0;
	Interrupted = 0;
	UpdateMenuItems(this);
	MeasPointFrm->Show();

   TakingData = 1;
	CurrFile->Measuring = true;
   CurrentPoint = 0;
   MeasPointFrm->StatusLabel->Caption = "Taking data...";

   time0 = GetTickCount();


	MeasPointTimer->Interval = 2000;
	MeasPointTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MeasPointTimerTimer(TObject *Sender)
{
	MeasPointTimer->Enabled = false;                   // Set timer off
	int ok;
	int millis, delay1, delay2;
	double field;

	double fastspeed = 2.0;                             // To be revisited

	char errorr[80];
	DataPoint *P1;
	char TempFileName[255];

	DataPoint P(NumberOfMeas);
	int nDaqMeas = 0;
	if (DAQ->IsOpen())
	{
	   nDaqMeas = DAQ->GetnActiveChannels();
	}
	double* DAQresult = NULL;
	if (nDaqMeas > 0) DAQresult = new double[nDaqMeas];

	double Meas[10];

//  Do the measurement

	Meas[0] = DMM1->ReadVoltage();
	Meas[1] = DMM2->ReadValue();
	Meas[2] = ITC->GetTemperature(1);
	Meas[3] = ITC->GetTemperature(2);
	Meas[4] = Magnet->GetField();
	Meas[5] = 0.0;
	Meas[6] = 0.0;
	Meas[7] = 0.0;
	Meas[8] = 0.0;
	Meas[9] = 0.0;

	for (int i=0; i< NumberOfMeas; i++)
		P.Set(i+1,Meas[i]);

	double seconds = 0.001*(double)(GetTickCount() - time0);
	P.Set(0,seconds);
//  Now read the field or calculate it from the time
//  Now DataPoint P contains the measurement
	AnsiString FormStr = "000.0000";
	MeasPointFrm->XLabel->Caption = FloatToStrF(P.Get(0), ffGeneral, 8, 0);
//    MeasPointFrm->Y1Label->Caption.FormatFloat(FormStr,P.Get(1)*1000.0);
//    MeasPointFrm->Y2Label->Caption = P.Get(2);
	MeasPointFrm->PointLabel->Caption = CurrentPoint;

	char averLabel[20];
	sprintf(averLabel,"%d/%d",CurrentSweep, Averages);
	MeasPointFrm->AverageLabel->Caption = averLabel;

//  Add the point to the DataArray and plot it
	DataArray* D = NULL;
	if ((ReturnSweep) && (Averages > 1))
		D = CurrFile->GetData(2);
	  else
		if (ReturnSweep) D = CurrFile->GetData(1);
		  else D = CurrFile->GetData(0);

	if (TakingData)
	{
		D->Add(P);
		CurrentPoint++;

		if (CurrentPoint > 1)
		  if ((ReturnSweep) && (Averages > 1))
		CurrFile->GetPlot(2)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
			 else
				if (ReturnSweep)
			CurrFile->GetPlot(1)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
				 else
					 CurrFile->GetPlot(0)->LineToLast(CurrFile->Canvas,D->Get(CurrentPoint-2), D->Get(CurrentPoint-1));
	}




//  See if we are at the end of the sweep

//  On end of sweep
   if (StopMeasurement == 1)
   {
		Magnet->SetAction(0);                      // Set Magnet to HOLD
		TakingData = 0;
		CurrFile->Measuring = false;
		if ((nDaqMeas > 0) && (MeasurementType ==10)) DAQ->WaitToComplete();
		UpdateMenuItems(this);
// if we were not averaging or at the end of the averaging cycle
		if (Interrupted)
		{
			MeasPointFrm->Close();
			CurrFile = NULL;
			Interrupted = false;
			return;
		}

		MeasPointFrm->Close();
		CurrFile = NULL;
		return;
   }
	else
   {
	  MeasPointTimer->Enabled = true;
   }

   UpdateMenuItems(this);
   delete [] DAQresult;
   return;

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::RFSource21Click(TObject *Sender)
{
	if (RF2->GetAddress() == 0)
	{
		ErrorBox->ErrorMessage->Caption = " RF Source 2 not connected or initialized ";
		ErrorBox->ShowModal();
		return;
	}
	RF2Dialog->Show();

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SpeedButton1Click(TObject *Sender)
{
	MagnetMenuItemClick(Sender);	
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SpeedButton2Click(TObject *Sender)
{
	ITCMenuItemClick(Sender);	
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ReconnectAll1Click(TObject *Sender)
{
	//   This routine needs some work
		//clear the GPIB interface
		//	GPIBInterfaceClear();
		//reinitialize all devices
	FreeGPIBDll();
	
	int nDevices = InitializeRemoteDevices();
	if (nDevices < 0)
	{
		ErrorBox->ErrorMessage->Caption = "Error Initializing Instruments";
		ErrorBox->ShowModal();
	}
	return;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::CurrentSourceMenuItemClick(TObject *Sender)
{
	if (CurrentSource->GetAddress() == 0)
	{
		ErrorBox->ErrorMessage->Caption = "CurrentSource not connected or initialized ";
		ErrorBox->ShowModal();
		return;
	}
	 else
		K6220Form->ShowModal();
	 return;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OIMonitorClick(TObject *Sender)
{
	OImonitorForm->Show();
	OImonitorForm->MonitorTimer->Enabled = true;
}
//---------------------------------------------------------------------------



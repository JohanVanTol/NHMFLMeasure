//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("ChildWin.cpp", DataFile);
USERES("Measure.res");
USEFORM("about.cpp", AboutBox);
USEUNIT("Myplot20.cpp");
USEUNIT("Mydata.cpp");
USEUNIT("GPIB_NatlInstrum.cpp");
USEFORM("Measmain.cpp", MainForm);
USEFORM("ErrorBx.cpp", ErrorBox);
USEFORM("GPIBTest.cpp", GPIBTestForm);
USEFORM("RFSourceDlg.cpp", RFSourceDialog);
USEFORM("RFAmplDlg.cpp", RFAmplifierDialog);
USEFORM("BoxcarDlg.cpp", BoxcarDialog);
USEUNIT("Valid.cpp");
USEFORM("RS232Test.cpp", RS232TestForm);
USELIB("..\wsc\Wsc32.lib");
USEUNIT("IsoBusDevices.cpp");
USEFORM("ITCDlg.cpp", ITCDialog);
USEFORM("ILMDlg.cpp", ILMDialog);
USEFORM("IPS20Dlg.cpp", IPS20Dialog);
USEFORM("IPS120Dlg.cpp", IPS120Dialog);
USEFORM("HeaterOnChkDlg.cpp", HeaterOnCheckForm);
USEFORM("TimerFrm.cpp", TimerForm);
USEFORM("ZeroSweepCoilFrm.cpp", ZeroSweepCoilForm);
USEFORM("MainFieldWaitFrm.cpp", MainFieldWaitForm);
USEFORM("HeaterOffChkDlg.cpp", HeaterOffCheckForm);
USEFORM("DMMDlg.cpp", DMM1Form);
USEFORM("LockInDlg.cpp", LockInDialog);
USEFORM("ENDORParameterDlg.cpp", ENDORParametersDialog);
USEFORM("splash.cpp", SplashForm);
USEFORM("InstrumInitForm.cpp", InstrumentInitForm);
USEFORM("ConfirmDeleteFrm.cpp", DeleteConfirmDialog);
USEFORM("MeasPointDialog.cpp", MeasPointFrm);
USEUNIT("Keithley197.cpp");
USEFORM("EPRMainCoilSweepDlg.cpp", EPRMainSweepDialog);
USEFORM("EPRSweepCoilDlg.cpp", EPRSweepCoilDialog);
USEFORM("ChannelSelectDlg.cpp", ChannelSelectDialog);
USEFORM("CommentDlg.cpp", CommentDialog);
USEFORM("PlotRangeDlg.cpp", PlotRangeDialog);
USEUNIT("HP54845A.cpp");
USEUNIT("HP54845A.cpp");
USEFORM("InfiniumGateDialog.cpp", InfiniumGatesDlg);
USEFORM("HilbertDlg.cpp", HilbertDialog);
USEUNIT("HP34401A.cpp");
USEFORM("MeasOptionsForm.cpp", MeasureOptionsDlg);
USEUNIT("HP8590.cpp");
USE("Measure.todo", ToDo);
USEFORM("PlotCurvesDialog.cpp", PlotCurvesDlg);
USEFORM("PhaseDlg.cpp", RePhaseDialog);
USEFORM("TransientCorrDialog.cpp", TransientCorrDlg);
USEFORM("ManyCurveSelectDlg.cpp", ManyCurveDlg);
USEFORM("HeaderViewForm.cpp", ParamViewForm);
USEFORM("GetSynFreqDlg.cpp", GetSynFreqDialog);
USEFORM("LogFileEdit.cpp", LogFileForm);
//---------------------------------------------------------------------------
#include "splash.h"
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
       SplashForm = new TSplashForm(Application);
       SplashForm->Show();
       SplashForm->Update();
    	Application->Initialize();
     	Application->Title = "EPR-ENDOR Measurement program";
         Application->HelpFile = "C:\\hans\\Programs\\Testhelp.hlp";
         Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TErrorBox), &ErrorBox);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->CreateForm(__classid(TGPIBTestForm), &GPIBTestForm);
                 Application->CreateForm(__classid(TRFSourceDialog), &RFSourceDialog);
                 Application->CreateForm(__classid(TRFAmplifierDialog), &RFAmplifierDialog);
                 Application->CreateForm(__classid(TBoxcarDialog), &BoxcarDialog);
                 Application->CreateForm(__classid(TRS232TestForm), &RS232TestForm);
                 Application->CreateForm(__classid(TITCDialog), &ITCDialog);
                 Application->CreateForm(__classid(TILMDialog), &ILMDialog);
                 Application->CreateForm(__classid(TIPS20Dialog), &IPS20Dialog);
                 Application->CreateForm(__classid(TIPS120Dialog), &IPS120Dialog);
                 Application->CreateForm(__classid(THeaterOnCheckForm), &HeaterOnCheckForm);
                 Application->CreateForm(__classid(TTimerForm), &TimerForm);
                 Application->CreateForm(__classid(TZeroSweepCoilForm), &ZeroSweepCoilForm);
                 Application->CreateForm(__classid(TMainFieldWaitForm), &MainFieldWaitForm);
                 Application->CreateForm(__classid(THeaterOffCheckForm), &HeaterOffCheckForm);
                 Application->CreateForm(__classid(TDMM1Form), &DMM1Form);
                 Application->CreateForm(__classid(TLockInDialog), &LockInDialog);
                 Application->CreateForm(__classid(TENDORParametersDialog), &ENDORParametersDialog);
                 Application->CreateForm(__classid(TInstrumentInitForm), &InstrumentInitForm);
                 Application->CreateForm(__classid(TDeleteConfirmDialog), &DeleteConfirmDialog);
                 Application->CreateForm(__classid(TMeasPointFrm), &MeasPointFrm);
                 Application->CreateForm(__classid(TEPRMainSweepDialog), &EPRMainSweepDialog);
                 Application->CreateForm(__classid(TEPRSweepCoilDialog), &EPRSweepCoilDialog);
                 Application->CreateForm(__classid(TChannelSelectDialog), &ChannelSelectDialog);
                 Application->CreateForm(__classid(TCommentDialog), &CommentDialog);
                 Application->CreateForm(__classid(TPlotRangeDialog), &PlotRangeDialog);
                 Application->CreateForm(__classid(TInfiniumGatesDlg), &InfiniumGatesDlg);
                 Application->CreateForm(__classid(THilbertDialog), &HilbertDialog);
                 Application->CreateForm(__classid(TMeasureOptionsDlg), &MeasureOptionsDlg);
                 Application->CreateForm(__classid(TPlotCurvesDlg), &PlotCurvesDlg);
                 Application->CreateForm(__classid(TRePhaseDialog), &RePhaseDialog);
                 Application->CreateForm(__classid(TTransientCorrDlg), &TransientCorrDlg);
                 Application->CreateForm(__classid(TManyCurveDlg), &ManyCurveDlg);
                 Application->CreateForm(__classid(TParamViewForm), &ParamViewForm);
                 Application->CreateForm(__classid(TGetSynFreqDialog), &GetSynFreqDialog);
                 Application->CreateForm(__classid(TLogFileForm), &LogFileForm);
                 SplashForm->Hide();
       SplashForm->Close();
       Application->Run();

	return 0;
}
//---------------------------------------------------------------------

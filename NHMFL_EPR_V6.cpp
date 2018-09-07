//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("ChildWin.cpp", DataFile);
USEFORM("AboutBox.cpp", EPRAboutBox);
USEFORM("Measmain.cpp", MainForm);
USEFORM("ErrorBx.cpp", ErrorBox);
USEFORM("GPIBTest.cpp", GPIBTestForm);
USEFORM("RFSourceDlg.cpp", RFSourceDialog);
USEFORM("RFAmplDlg.cpp", RFAmplifierDialog);
USEFORM("BoxcarDlg.cpp", BoxcarDialog);
USEFORM("RS232Test.cpp", RS232TestForm);
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
USEFORM("EPRMainCoilSweepDlg.cpp", EPRMainSweepDialog);
USEFORM("EPRSweepCoilDlg.cpp", EPRSweepCoilDialog);
USEFORM("ChannelSelectDlg.cpp", ChannelSelectDialog);
USEFORM("CommentDlg.cpp", CommentDialog);
USEFORM("PlotRangeDlg.cpp", PlotRangeDialog);
USEFORM("InfiniumGateDialog.cpp", InfiniumGatesDlg);
USEFORM("HilbertDlg.cpp", HilbertDialog);
USEFORM("MeasOptionsForm.cpp", MeasureOptionsDlg);
USEFORM("PlotCurvesDialog.cpp", PlotCurvesDlg);
USEFORM("PhaseDlg.cpp", RePhaseDialog);
USEFORM("TransientCorrDialog.cpp", TransientCorrDlg);
USEFORM("ManyCurveSelectDlg.cpp", ManyCurveDlg);
USEFORM("HeaderViewForm.cpp", ParamViewForm);
USEFORM("GetSynFreqDlg.cpp", GetSynFreqDialog);
USEFORM("LogFileEdit.cpp", LogFileForm);
USEFORM("CommentDlg17T.cpp", CommentDialog2);
USEFORM("UserFormDlg.cpp", UserForm);
USEFORM("DAQDlg.cpp", DAQ_Dialog);
USEFORM("PulseForm.cpp", PulsedDialog);
USEFORM("DACSweepDlg.cpp", DacSweepDialog);
USEFORM("RF2Dlg.cpp", RF2Dialog);
USEFORM("K6220Dialog.cpp", K6220Form);
USEFORM("OImonitor.cpp", OImonitorForm);
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
		 Application->HelpFile = "C:\\Users\\vantol\\My Documents\\Programs\\NHMFL_EPR_2015_V5p0\\NHMFL_EPR.chm";
		 Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TErrorBox), &ErrorBox);
		Application->CreateForm(__classid(TEPRAboutBox), &EPRAboutBox);
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
		Application->CreateForm(__classid(TCommentDialog2), &CommentDialog2);
		Application->CreateForm(__classid(TUserForm), &UserForm);
		Application->CreateForm(__classid(TDAQ_Dialog), &DAQ_Dialog);
		Application->CreateForm(__classid(TPulsedDialog), &PulsedDialog);
		Application->CreateForm(__classid(TDacSweepDialog), &DacSweepDialog);
		Application->CreateForm(__classid(TRF2Dialog), &RF2Dialog);
		Application->CreateForm(__classid(TK6220Form), &K6220Form);
		Application->CreateForm(__classid(TOImonitorForm), &OImonitorForm);
		SplashForm->Hide();
       SplashForm->Close();
       Application->Run();

	return 0;
}
//---------------------------------------------------------------------


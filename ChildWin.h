//----------------------------------------------------------------------------
#ifndef ChildWinH
#define ChildWinH
//----------------------------------------------------------------------------
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include "mydata.h"
#include "myplot20.h"
//----------------------------------------------------------------------------
class TDataFile : public TForm
{
__published:
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:
    DataArray *Dat;
    DataArray *AverDat;
    DataArray **Datas;
    MyPlot *Plot;
    MyPlot *AverPlot;
    MyPlot **Plots;
    char *Header;
    int HeaderSize;
    TRect *FocusRect;
    bool AverageOn;
    bool UpDown;
    int nDatas;
    int nPlots;
    DataPoint PlotCenter;

public:
	virtual __fastcall TDataFile(TComponent *Owner);
    void __fastcall PlotAxes();
    void __fastcall PlotData();
    void __fastcall PrintData();

    int ReadData(AnsiString Filename);
    int WriteData(AnsiString Filename);
    int WriteData(int mode, AnsiString Filename);
    int SetHeader(const char* text);
    int AddHeader(const char* text);
    int GetHeaderSize() {return HeaderSize;}
    int GetHeader(char* H, int nchar);
    int SetData(DataArray NewData);
    bool IsEmpty();
	bool Saved;
	bool Measuring;
    bool GetAverage() {return AverageOn;}
    bool GetUpDown() {return UpDown;}
    void SetAverage(int Av);
    void SetUpDown(int UpDo);
    DataArray* GetData() {return Datas[0];}
    DataArray* GetData(int i);
    DataArray* GetAverData() {return AverDat;}
    MyPlot* GetPlot() {return Plots[0];}
    MyPlot* GetPlot(int i);
    MyPlot* GetAverPlot() {return AverPlot;}
    void DataReset(int dim, int Npoints = MAXPNT); // dim is number of y-values
    void AllDataReset(int dim, int Npoints = MAXPNT); // dim is number of y-values
    void DataReset(int dataset, int dim, int Npoints);
    int GetCursorPlot(int X, int Y) const;
    int GetnDatas() const {return nDatas;}
    int GetnPlots() const {return nPlots;}
    void SetnDatas(int n) {nDatas = n;}
    void SetnPlots(int n) {nPlots = n;}
};
//----------------------------------------------------------------------------
#endif

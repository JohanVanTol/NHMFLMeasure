#include <windows.h>
#include <vcl\graphics.hpp>
#ifndef __MYPLOT.H
#define __MYPLOT.H

class PlotAxis
{
	public:
		PlotAxis(double _min=0.0, double _max=1.0);
        PlotAxis(const PlotAxis& Ax);
		~PlotAxis() {delete [] title;}
		void operator=(const PlotAxis& Ax);
		void AutoRange(double l, double h);
		void Range(double _min, double _max);
		double Min() const {return min;}
		double Max() const {return max;}
		double GetPos(double x) const;

		int GetnTicks() const {return nTicks;}
		double GetTickStep() const {return TickStep;}
		double GetFirstTick() const {return FirstTick;}
		void SetTitle(const char* _title);
		char* GetTitle() const {return title;}
        bool IsTicksOn() const {return Ticks;}
        bool IsLabelsOn() const {return Labels;}

	private:
		double min;
		double max;
		int nTicks;
		double TickStep;
		double FirstTick;
		char* title;
        bool Ticks;
        bool Labels;
		void CalcTicks();
};

class MyPlot
{
	public :
		MyPlot();
		~MyPlot();
		void SetLim(int _left, int _top, int _right, int _bottom);
        void SetLim(int ClientWidth, int ClientHeight);
//		void SetLim(const TRect& WindowRect);
		int GetTop() {return top;}
		int GetLeft() {return left;}
		int GetBottom() {return bottom;}
		int GetRight() {return right;}

//  	int WithinLimits(TPoint point) const;
        bool WithinLimits(int X, int Y) const;

    	void PlotTheAxes(TCanvas* canvas);
		void SetMargins(float tm=0.05, float lm=0.15, float bm=0.15, float rm=0.10);

		int GetAutoRange() const {return autorange;}
		void SetAutoRange(int _mode) {autorange = _mode;}
		void SetAutoX();
		void SetAutoY1();
		void SetAutoY2();
		void SetRanges(DataPoint min, DataPoint max);
		void SetFixedRanges(DataPoint min, DataPoint max);
		DataPoint GetMinRanges();
		DataPoint GetMaxRanges();
		TPoint GetCoor(int mode, double x, double y);
        int GetXCoor(int mode, double x);
        int GetYCoor(int mode, double y);
		DataPoint GetUserCoordinates(TPoint point) const;

		void PlotArray(TCanvas* canvas, const DataArray& D);
													// X ,Y1 and Y2 axes
		int PointsInRange(const DataArray& D);
        void LineToLast(TCanvas* canvas, const DataPoint Previous, const DataPoint Current);
		void SetAxisTitle(const char*, int axis);
		char* GetAxisTitle(int axis) const;
		void SetDataSize(int size);
		void SetDataColumn(int ix, int iy, int iref);
		int GetDataColumn(int i) const;

        TColor GetFillColor() {return FillBrushColor;}
        void SetFillColor(TColor color) {FillBrushColor = color;}
        TColor GetAxisColor() {return AxisPenColor;}
        void SetAxisColor(TColor color) {AxisPenColor = color;}

        PlotAxis GetAxis(int i);
		int TickSize;

	private :
		// The limits of the plotting rectangle
		int	top;
		int bottom;
		int left;
		int	right;

		// plot margin with respect to window surface
		float topmargin;
		float bottommargin;
		float leftmargin;
		float rightmargin;

		PlotAxis Xaxis;
		PlotAxis Y1axis;
		PlotAxis Y2axis;

		TColor AxisPenColor;
		TColor DataPenColor;
		TColor Data2PenColor;
		TColor Data3PenColor;
		TColor FillBrushColor;

        int LabelSize;
        int TitleSize;
		int autorange; // This number gives which axes are
                        // AutoRange 0x0001 for X, 0x0002 for Y2,
                        // and 0x0004 for Y2
		int DataSize; // The number of y-data in the associated Data-array
		int DataColumn[3]; // Contains the associated Data columns with
};
#endif

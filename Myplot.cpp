//#include <owl\owlpch.h>
//#include <owl\point.h>

#include <math.h>
#include <stdio.h>
#include <windows.h>
#include "mydata.h"
#include "myplot.h"

const int MAXPLOTPOINTS = 0;

///////////////////////////////////////////////////////
// DEFINITIONS FOR Class PlotAXis

//Constructor
PlotAxis::PlotAxis(double _min, double _max)
						:min(_min), max(_max)
{
	title = new char[20];
	strcpy(title,"Undefined");
    Ticks = 1;
    Labels = 1;
}

PlotAxis::PlotAxis(const PlotAxis& Ax)           //Constructor by copy
{
	title = new char[20];
	SetTitle(Ax.GetTitle());
	min = Ax.Min();
	max = Ax.Max();
	nTicks = Ax.GetnTicks();
	TickStep = Ax.GetTickStep();
	FirstTick = Ax.GetFirstTick();
    Ticks = Ax.IsTicksOn();
    Labels = Ax.IsLabelsOn();
}

void PlotAxis::operator=(const PlotAxis& Ax)        //Copy other axis
{
	SetTitle(Ax.GetTitle());
	min = Ax.Min();
	max = Ax.Max();
	nTicks = Ax.GetnTicks();
	TickStep = Ax.GetTickStep();
	FirstTick = Ax.GetFirstTick();
    Ticks = Ax.IsTicksOn();
    Labels = Ax.IsLabelsOn();
}

void PlotAxis::AutoRange(double low, double high)
{
	double factor = 1.0;
	double range = high-low;
	if (range <= 0.0) return;
	while (range > 10.0)
	{
		factor /= 10.0;
		range /= 10.0;
	}
	while (range < 1)
	{
		factor *=10.0;
		range *= 10.0;
	}
	min = floor(factor*low)/factor;
	max = ceil(factor*high)/factor;
	CalcTicks();
}

void PlotAxis::Range(double _min, double _max)
{
	if (_max <= _min) return;
	min = _min;
	max = _max;
	CalcTicks();
}

void PlotAxis::CalcTicks()
{
	double range = max - min;
	double step = 1.0;
	while (range <= 1.0)
	{
		range *= 10.0;
		step /= 10.0;
	}
	while (range > 10.0)
	{
		range /= 10.0;
		step *= 10.0;
	}
	while (range < 4.0)
	{
		range *= 2.0;
		step /= 2.0;
	}
	TickStep = step;
	nTicks = 0;
	FirstTick = double(ceil(double(min)/step)) * step;
	while ((FirstTick + nTicks * step) <= max) nTicks++;
}

double PlotAxis::GetPos(double x) const
{
	if (x <= min) return 0.0;
	if (x >= max) return 1.0;
	return (x-min)/(max-min);
}

//
// SetTitle
//		Sets the title of the axis
void PlotAxis::SetTitle(const char* _title)
{
	delete[] title;
	title = new char[strlen(_title)+1];
	strcpy(title, _title);
}

//////////////////////////////////////////////////////////////////////////
// Definitions for class MyPlot

// default constructor
MyPlot::MyPlot()
		:top(0), bottom(1), left(0), right(1),
		leftmargin(0.1), rightmargin(0.1), topmargin(0.05), bottommargin(0.15)
{
	Xaxis.Range(0.0, 12.0);
	Y1axis.Range(-0.001, 0.001);
	Y2axis.Range(0.0, 10.0);
	AxisPenColor = clBlack;
	DataPenColor = clRed;
	Data2PenColor = clGreen;
	Data3PenColor = clBlue;
	FillBrushColor = clWhite;

	TickSize = 4;
    LabelSize = 6;
    TitleSize = 7;
	autorange = 0x0007;
	DataSize = 3;  //Default data with two y-columns
	for (int i=0; i<3; i++) DataColumn[i] = i;
}

MyPlot::~MyPlot()
{
}

void MyPlot::SetLim(int _left, int _top, int _right, int _bottom)
{
	left = _left + (int)(leftmargin*(_right-_left));
	top = _top + (int)(topmargin*(_bottom - _top));
	right = _right - (int)(rightmargin*(_right- _left));
	bottom = _bottom - (int)(bottommargin*(_bottom - _top));
}

void MyPlot::SetLim(int ClientX, int ClientY)
{
    if (DataSize > 2) rightmargin = 0.1;
       else rightmargin = 0.02;
	left = int(leftmargin * ClientX);
	right = ClientX - int(rightmargin * ClientX);
	top = int(topmargin * ClientY);
	bottom = ClientY - int(bottommargin * ClientY);
}

/*void MyPlot::SetLim(const TRect& WindowRect)
{
	left = WindowRect.left + int(leftmargin * WindowRect.Width());
	right = WindowRect.right - int(rightmargin * WindowRect.Width());
	top = WindowRect.top + int(topmargin * WindowRect.Height());
	bottom = WindowRect.bottom - int(bottommargin * WindowRect.Height());
}
*/
int MyPlot::PointsInRange(const DataArray& D)
{
	int n=0;
	for (int i=0; i<D.Getn(); i++)
		if ((D.Get(i).Get(DataColumn[0]) > Xaxis.Min())
				&& (D.Get(i).Get(DataColumn[0])< Xaxis.Max())) n++;
	return n;
}

void MyPlot::PlotTheAxes(TCanvas* canvas)
{
	int xi, yi, stringwidth;
	double value;
	char* str;
	str = new char[16];

  // draw the Plot rectangle
	canvas->Pen->Color = AxisPenColor;     //Take the Pen for the axes
    canvas->Brush->Color = FillBrushColor; //Take the brush for the background
	canvas->Rectangle(left,top,right+1,bottom+1);      // Drax the box

  // Draw the labels and ticks of the x-axis
    canvas->Font->Size=LabelSize;
    canvas->Brush->Color = clBtnFace; //current window background
    if (right > 1500) canvas->Brush->Color = clWhite;  // printing to paper
	for (int i =0; i<Xaxis.GetnTicks(); i++)       // Draw ticks and labels
	{															  // of the x-axis
		value = Xaxis.GetFirstTick() + i*Xaxis.GetTickStep();
		xi = left + int((right-left) * Xaxis.GetPos(value));
		canvas->MoveTo(xi, bottom);
		canvas->LineTo(xi, bottom+TickSize);
		sprintf(str, "%f", value);
		while ((str[strlen(str)-1]=='0') && (str[strlen(str)-2] != '.') && (strlen(str) > 1))
			str[strlen(str)-1] = 0;
        stringwidth = canvas->TextWidth(str);
		canvas->TextOut(xi - stringwidth/2, bottom + TickSize, str);
	}

  // Draw X-axis title
    canvas->Font->Size=TitleSize;
    stringwidth = canvas->TextWidth(GetAxisTitle(0));
	canvas->TextOut((left+right-stringwidth)/2,
        bottom + canvas->TextHeight(str) + TickSize, GetAxisTitle(0));

	int CharacterHeight = canvas->TextHeight("6789");

  // Draw Y1 axis title
	char dummy[2];
	dummy[1] = '\0';
    int xtitle = 2;
    //	int xtitle = leftmargin*(right-left)/(3*(1-(leftmargin+rightmargin)));
	int nchar = strlen(GetAxisTitle(1));
	int yst = (bottom+top - CharacterHeight * nchar) / 2;
    canvas->Font->Size=TitleSize;
	for (int i=0; i<nchar; i++)
	{
		dummy[0] = GetAxisTitle(1)[i];
		canvas->TextOut(xtitle, yst+ceil(i*CharacterHeight),dummy);
	}

  // Draw the Y1 labels and Ticks
    canvas->Font->Size=LabelSize;
	for (int i =0; i<Y1axis.GetnTicks(); i++)
	{
		value = Y1axis.GetFirstTick() + i*Y1axis.GetTickStep();
		yi = bottom - int((bottom-top) * Y1axis.GetPos(value));
		canvas->MoveTo(left, yi);
		canvas->LineTo(left - TickSize, yi);
//		if ((fabs(Y1axis.Max()) < 0.010) && (fabs(Y1axis.Min()) < 0.010 ))
//			sprintf(str, "%ge-3", value*1000.0);
//		 else
			sprintf(str, "%#g", value);
		while ((str[strlen(str)-1]=='0') && (str[strlen(str)-2] != '.') && (strlen(str) > 1))
			str[strlen(str)-1] = 0;
		canvas->TextOut(left-TickSize-canvas->TextWidth(str), yi - CharacterHeight/2, str);
	}

// If the datasize > 2, a second Y axis is drawn on the right of the graph
// Otherwise the ticks of the left Y-axis are repeated
	if (DataSize > 2)
	{
		// Draw reference axis title
		dummy[1] = '\0';
        canvas->Font->Size=TitleSize;
		xtitle = right + 3*rightmargin*(right-left) /
									(4*(1-(leftmargin+rightmargin)));
		nchar = strlen(GetAxisTitle(2));
		yst = (bottom+top - CharacterHeight * nchar) / 2;
		for (int i=0; i<nchar; i++)
		{
			dummy[0] = GetAxisTitle(2)[i];
			canvas->TextOut(xtitle, yst+i*CharacterHeight,dummy);
		}

        canvas->Font->Size=LabelSize;
		for (int i =0; i<Y2axis.GetnTicks(); i++)
		{
			value = Y2axis.GetFirstTick() + i*Y2axis.GetTickStep();
			yi = bottom - int((bottom-top) * Y2axis.GetPos(value));
			canvas->MoveTo(right, yi);
			canvas->LineTo(right + TickSize, yi);
//			if ((fabs(Y2axis.Max()) < 0.010) && (fabs(Y2axis.Min()) < 0.010 ))
//				sprintf(str, "%ge-3", value*1000.0);
//			 else
            sprintf(str, "%#g", value);
			while ((str[strlen(str)-1]=='0') && (str[strlen(str)-2] != '.') && (strlen(str) > 1))
				str[strlen(str)-1] = 0;
			canvas->TextOut(right + 2*TickSize, yi - CharacterHeight/2, str);
		}
	}
	else
	{ // If no second Y data available draw Y1 axis on right as well
					// without title and labels
		for (int i=0; i<Y1axis.GetnTicks(); i++)
		{
			value = Y1axis.GetFirstTick() + i*Y1axis.GetTickStep();
			yi = bottom - int((bottom-top) * Y1axis.GetPos(value));
			canvas->MoveTo(right, yi);
			canvas->LineTo(right + TickSize, yi);
//			if ((fabs(Y1axis.Max()) < 0.010) && (fabs(Y1axis.Min()) < 0.010 ))
//				sprintf(str, "%ge-3", value*1000.0);
//			 else
//				sprintf(str, "%#g", value);
//			while ((str[strlen(str)-1]=='0') && (str[strlen(str)-2] != '.') && (strlen(str) > 1))
//				str[strlen(str)-1] = 0;
//			canvas->TextOut(right + 3*TickSize/2, yi - CharacterHeight/2, str);
		}
	}
    delete[] str;
}


void MyPlot::SetRanges(DataPoint min, DataPoint max)
{
	int mode = autorange;
	if ((mode & 0x0001) == 1)
		Xaxis.AutoRange(min.Get(DataColumn[0]), max.Get(DataColumn[0]));
	if ((mode & 0x0002) == 2)
		Y1axis.AutoRange(min.Get(DataColumn[1]), max.Get(DataColumn[1]));
	if ((DataSize > 2) && ((mode & 0x0004) == 4))
		Y2axis.AutoRange(min.Get(DataColumn[2]), max.Get(DataColumn[2]));
}

void MyPlot::SetFixedRanges(DataPoint min, DataPoint max)
{
	int mode = autorange;
	if ((mode & 0x0001) != 1)
		Xaxis.Range(min.Get(DataColumn[0]), max.Get(DataColumn[0]));
	if ((mode & 0x0002) != 2)
		Y1axis.Range(min.Get(DataColumn[1]), max.Get(DataColumn[1]));
	if ((DataSize > 2) && ((mode & 0x0004) != 4))
		Y2axis.Range(min.Get(DataColumn[2]), max.Get(DataColumn[2]));
}

void MyPlot::SetAutoX()
{
	if ((autorange & 0x0001) != 1) autorange += 1;
}

void MyPlot::SetAutoY1()
{
	if ((autorange & 0x0002) != 2) autorange += 2;
}

void MyPlot::SetAutoY2()
{
	if ((autorange & 0x0004) != 4) autorange += 4;
}


DataPoint MyPlot::GetMinRanges()
{
	DataPoint min(DataSize-1);
	min.Set(DataColumn[0],Xaxis.Min());
	min.Set(DataColumn[1],Y1axis.Min());
	if (DataSize > 2) min.Set(DataColumn[2],Y2axis.Min());
	return min;
}

DataPoint MyPlot::GetMaxRanges()
{
	DataPoint max(DataSize-1);
	max.Set(DataColumn[0],Xaxis.Max());
	max.Set(DataColumn[1],Y1axis.Max());
	if (DataSize > 2) max.Set(DataColumn[2],Y2axis.Max());
	return max;
}

//int MyPlot::WithinLimits(TPoint point) const
//{
//	if ((point.x >= left) && (point.x <= right)
//			&& (point.y >= top) && (point.y <= bottom)) return 1;
//	  else return 0;
//}

bool MyPlot::WithinLimits(int X, int Y) const
{
	if ((X >= left) && (X <= right)
			&& (Y >= top) && (Y <= bottom)) return true;
	  else return false;
}

TPoint MyPlot::GetCoor(int mode, double x, double y)
{
	TPoint p;
	p.x = left + Xaxis.GetPos(x)*(right - left);
	if (mode == 2) p.y = bottom - Y2axis.GetPos(y)*(bottom - top);
	  else	p.y = bottom - Y1axis.GetPos(y)*(bottom - top);
	return p;
}

int MyPlot::GetXCoor(int mode, double x)
{
	return (left + Xaxis.GetPos(x)*(right - left));
}

int MyPlot::GetYCoor(int mode,  double y)
{
    int Y;
	if (mode == 2) Y = bottom - Y2axis.GetPos(y)*(bottom - top);
	  else	Y= bottom - Y1axis.GetPos(y)*(bottom - top);
	return Y;
}




DataPoint MyPlot::GetUserCoordinates(TPoint point) const
{
	DataPoint P(DataSize-1);
//	if (!WithinLimits(point)) return P;
	double x = Xaxis.Min()
			+ (point.x - left) * (Xaxis.Max() - Xaxis.Min()) /(right-left);
	double y1 = Y1axis.Min()
			+ (bottom - point.y) * (Y1axis.Max() - Y1axis.Min())/(bottom-top);
	P.Set(DataColumn[0],x);
	P.Set(DataColumn[1],y1);
	if (DataSize > 2)
	{
		double y2 = Y2axis.Min()
			+ (bottom - point.y) * (Y2axis.Max() - Y2axis.Min())/(bottom-top);
		P.Set(DataColumn[2],y2);
	}
	return P;
}

// PlotArray plots the data points of a complete data array
void MyPlot::PlotArray(TCanvas* canvas, const DataArray& D)
{
	int i,skip = 0;
	if (MAXPLOTPOINTS != 0) skip = PointsInRange(D) / MAXPLOTPOINTS;
	// Plot the Y2 data (if any)

	if (DataSize > 2)
	{
     	canvas->Pen->Color = Data2PenColor;     //Take the Pen for the axes
		canvas->MoveTo(GetXCoor(2,D.Get(0).Get(DataColumn[0])),GetYCoor(2,
										D.Get(0).Get(DataColumn[2])));
		for (i=1; i<D.Getn(); i += 1+skip)
			canvas->LineTo(GetXCoor(2, D.Get(i).Get(DataColumn[0])),
								GetYCoor(2,D.Get(i).Get(DataColumn[2])));
	}

	// Plot the Y1 data
 	canvas->Pen->Color = DataPenColor;     //Take the Pen for the axes
		canvas->MoveTo(GetXCoor(1,D.Get(0).Get(DataColumn[0])),GetYCoor(1,
										D.Get(0).Get(DataColumn[1])));
	for (i=1; i<D.Getn(); i+=1+skip)
	{
		if ((D.Get(i).Get(DataColumn[0]) >= Xaxis.Min())
					&& (D.Get(i).Get(DataColumn[0]) <= Xaxis.Max()))
			canvas->LineTo(GetXCoor(1, D.Get(i).Get(DataColumn[0])),
								GetYCoor(1,D.Get(i).Get(DataColumn[1])));
			else canvas->MoveTo(GetXCoor(1, D.Get(i).Get(DataColumn[0])),
								GetYCoor(1,D.Get(i).Get(DataColumn[1])));
	}
}

void MyPlot::LineToLast(TCanvas* canvas, const DataPoint Previous, const DataPoint Current)
{
	if (DataSize > 2)
	{
        canvas->Pen->Color = Data2PenColor;
		canvas->MoveTo(GetXCoor(2,Previous.Get(DataColumn[0])),GetYCoor(2,
										Previous.Get(DataColumn[2])));
		canvas->LineTo(GetXCoor(2,Current.Get(DataColumn[0])),
					 GetYCoor(2,Current.Get(DataColumn[2])));
	}

       canvas->Pen->Color = DataPenColor;
		canvas->MoveTo(GetXCoor(1,Previous.Get(DataColumn[0])),GetYCoor(1,
										Previous.Get(DataColumn[1])));
		canvas->LineTo(GetXCoor(1,Current.Get(DataColumn[0])),
					 GetYCoor(1,Current.Get(DataColumn[1])));
}


void MyPlot::SetMargins(float tm, float lm, float bm, float rm)
{
	topmargin = tm;
	leftmargin = lm;
	bottommargin = bm;
	rightmargin = rm;
}

//
// SetAxisTitle
//		Sets the title of one of the axes
void MyPlot::SetAxisTitle(const char* _title, int axis)
{
	switch (axis)
	{
		case 0: 	Xaxis.SetTitle(_title);
					break;
		case 1: 	Y1axis.SetTitle(_title);
					break;
		case 2:	Y2axis.SetTitle(_title);
					break;
		default:	break;
	}
}

//
// GetAxisTitle
// 	Returns a pointer to the title of one of the axes
char*	MyPlot::GetAxisTitle(int axis) const
{
	switch (axis)
	{
		case 0: 	return Xaxis.GetTitle();
		case 1: 	return Y1axis.GetTitle();
		case 2:	return Y2axis.GetTitle();
		default:	return Xaxis.GetTitle();
	}
}

// Sets the columns of the Data to be used in the plot
void MyPlot::SetDataColumn(int ix, int iy1, int iy2)
{
	if (ix < DataSize) DataColumn[0] = ix;
	if (iy1 < DataSize) DataColumn[1] = iy1;
	if (iy2 < DataSize) DataColumn[2] = iy2;
}

int MyPlot::GetDataColumn(int i) const
{
	if ((i < 0 ) || (i >= 3)) return 0;
	return DataColumn[i];
}

void MyPlot::SetDataSize(int size)
{
	DataSize = size;
	if (DataSize < 3)
	{
		Y2axis = Y1axis;
		DataColumn[2]=DataColumn[1];
	}
	  else
	  if (DataSize == 3) DataColumn[2] = 2;
}

PlotAxis MyPlot::GetAxis(int i)
{
    switch (i)
    {
        case 0: return Xaxis;
        case 1: return Y1axis;
        case 2: return Y2axis;
        default: return Xaxis;
    }
}

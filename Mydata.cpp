// Version 1.1
// 6/9/00 Added data linearisation routine DataArray::Linearize()

#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
//#include <owl\filedoc.h>
#include "mydata.h"
//#include "myindial.h"

/////////////////////////////////////////////////////////////////////////////
//
//	Class DataPoint
//

// this constructor sets the values of
// a series of y variables.
// Retained for compatibility with existing code
DataPoint::DataPoint(double _x, double _y1, double _y2)
				:x(_x)
{
	ny = 2;
	y = new double[ny];
	y[0] = _y1;
	y[1] = _y2;
}

// Defaultconstructor
DataPoint::DataPoint(int _ny)
		: ny(_ny)
{
	if (ny > MAXNY) ny = MAXNY;
	if (ny < 1) ny = 1;				//ny should be in range 1...MAXNY
	y = new double[ny];
	x = 0.0;						//Reset all to zero	
	for (int i=0; i<ny; i++) y[i] = 0.0;
}

//Destructor
DataPoint::~DataPoint()
{
	delete[] y;
}

// constructor by copy
DataPoint::DataPoint(const DataPoint& P)
{
	ny = P.GetNy();
	y = new double[ny];
	x = P.GetX();
	for (int i=0; i<ny; i++) y[i] = P.GetY(i);
}

// copy operator
void DataPoint::operator=(const DataPoint& P)
{
	delete[] y;
	ny = P.GetNy();
	y = new double[ny];
	x = P.GetX();
	for (int i=0; i<ny; i++) y[i] = P.GetY(i);
}

// simulates the y-values from a given x value
void DataPoint::Simulate(double _x)
{
	x = _x;
	for (int i=0; i<ny; i++)
		y[i] = double(random(100)) * sin(x*i);
}

// returns k-th y-value
double DataPoint::GetY(int k) const
{
	if ((k<0) || (k>=ny)) return 0.0;
	return y[k];
}

double DataPoint::Get(int k) const
{
	if ((k<0) || (k>ny)) return 0.0;
	if (k==0) return x;
	  else return y[k-1];
}

// Old function retained for compatibility
// Sets a datapoint with two y-values
void DataPoint::Set(double _x, double _y, double _z)
{
	x = _x;
	y[0] = _y;
	if (ny > 1) y[1]=_z;
}

void DataPoint::Set(int k, double el)       // Sets a datapoint
{                                           // without distinction of x or y
	if ((k<0) || (k>ny)) return;             // values
	if (k==0) x = el;
	  else y[k-1] = el;
	return;
}

//Sets the k-th y-value
void DataPoint::SetY(int k, double _y)
{
	if ((k>=0) && (k<ny)) y[k] = _y;
}

DataPoint DataPoint::operator+(const DataPoint& P) const
{
	int n = ny;
	if ( P.GetNy() < ny ) n=P.GetNy();
	DataPoint PNew(n);
	PNew.SetX(x + P.GetX());
	for (int i=0; i<n; i++)
		PNew.SetY(i, y[i] + P.GetY(i));
	return PNew;
}

DataPoint& DataPoint::operator+=(const DataPoint& P)
{
	if ( P.GetNy() != ny ) return *this;   //do nothing if dimensions are different
	x += P.GetX();
	for (int i=0; i<ny; i++)
		y[i] += P.GetY(i);
	return *this;
}

DataPoint DataPoint::operator-(const DataPoint& P) const
{
	int n = ny;
	if ( P.GetNy() < ny ) n=P.GetNy();
	DataPoint PNew(n);
	PNew.SetX(x - P.GetX());
	for (int i=0; i<n; i++)
		PNew.SetY(i, y[i] - P.GetY(i));
	return PNew;
}

DataPoint DataPoint::operator/(double divide) const
{
	DataPoint PNew(ny);
	if (divide < 1e-30 ) return PNew;
	PNew.SetX(x/divide);
	for (int i=0; i<ny; i++)
		PNew.SetY(i, y[i]/divide);
	return PNew;
}

DataPoint& DataPoint::operator/=(double divide)
{
	if (divide < 1e-30 ) return *this;
	x /= divide;
	for (int i=0; i<ny; i++)
		y[i]/=divide;
	return *this;
}

DataPoint DataPoint::operator*(double mult) const
{
	DataPoint PNew(ny);
	PNew.SetX(x * mult);
	for (int i=0; i<ny; i++)
		PNew.SetY(i, y[i]*mult);
	return PNew;
}

ostream& operator <<(ostream& os, const DataPoint& p)
{
	char* line;
    int len = (p.GetNy() + 1 ) * 21;
    line = new char[len];
	char yValue[20];
	sprintf(line, "%15.8g \0",p.Get(0));
	for (int i=0; i<p.GetNy(); i++)
	{
		sprintf(yValue,"%15.8g \0", p.Get(i+1)),
		strcat(line, yValue);
	}
	os << line;
    delete[] line;
	return os;
}


/////////////////////////////////////////////////
//
//   This operator reads a datapoint from a stream
//  it reads a line from the stream, determines the number of
//  reals and stores this in the DataPoint parameter p, which is
//  send as a reference
//
istream& operator >>(istream& is, DataPoint& p)
{
	if (is.eof())return is;
	double x;
	double *y;
        y = new double[MAXNY];
        is >> x;
        int k = 0;
        while ((is.peek() != '\n') && ( k < MAXNY) && (!is.eof()))
        {
             is >> y[k];
             k++;
             while (is.peek() == ' ') is.ignore();
        }
        if (k>0)
       	{
        	DataPoint P(k);
                P.SetX(x);
	        for (int i=0; i<k; i++) P.SetY(i,y[i]);
		    p=P;
	    }
        delete[] y;
	return is;
}

/////////////////////////////////////////////////////////////////////
//
//  Class DataArray
//
// which is a class that contains an array of DataPoints
// However, to be able to have a reasonable number of points
// It is not a strict array of datapoints, but a series of arrays


// constructor, the default for npts sets the maximum
DataArray::DataArray(int npts, int nY)
{
	n = 0;
	ny = nY;
	x = new double[npts];
	y = new double*[ny];
	for (int i=0; i<ny; i++)
		y[i] = new double[npts];
}

// constructor by copy. NB it is not a true copy because only the
// amount of space is reserved for the new DataArray equal to the
// actual occupied data in the array to be copied. That means that
// no data can be added without reserving new space.
DataArray::DataArray(const DataArray& DataToCopy)
{
	int npts = DataToCopy.Getn();
	ny = DataToCopy.GetNy();
	x = new double[npts];
	y = new double*[ny];
	for (int j=0; j<ny; j++)
		y[j] = new double[npts];
	n = npts;
	for (int i=0; i<n;i++)
			Set(i,DataToCopy.Get(i));
}

// destructor
DataArray::~DataArray()
{
	delete[] x;
	for (int j=0; j<ny; j++) delete[] y[j];
	delete[] y;
}

void DataArray::operator=(const DataArray& DataToCopy)
{
	int j;

	delete[] x;
	for (j=0; j<ny; j++)
		delete[] y[j];
	delete[] y;

	int npts = DataToCopy.Getn();     // thus no more points may be added !!
	ny = DataToCopy.GetNy();       
	
	x = new double[npts];
	y = new double*[ny];
	for (j=0; j<ny; j++)
		y[j] = new double[npts];
	n = npts;
	for (int i=0; i<n;i++)
			Set(i,DataToCopy.Get(i));
}

void DataArray::Add(const DataPoint P)
{
	if (n >= MAXPNT ) return;
	x[n] = P.GetX();
	for (int i=0; i<ny; i++) y[i][n] = P.GetY(i);
	n++;
	return;
}

int DataArray::AddColumn(int ncol)        //adds n columns
{
    if ((ny+ncol) > MAXNY) return -1;
    double **tempy;             //make copy of current data
    tempy = new double*[ny];
    for (int i=0; i<ny;i++)
    {
        tempy[i]=new double[n];
        for (int j=0;j<n;j++)
            tempy[i][j] = y[i][j];
        delete[] y[i];
    }
    delete[] y;
    ny += ncol;
    y = new double*[ny];
    for (int i=0; i<ny;i++)
    {
        y[i]=new double[n];
        for (int j=0;j<n;j++)
           if (i<(ny-ncol)) y[i][j] = tempy[i][j];
              else y[i][j] = 0.0;
        if (i<ny-ncol) delete[] tempy[i];
    }
    delete[] tempy;
    return ny;
}




void DataArray::Sum(int i, const DataPoint P)
{
	if (i >= n ) return;
	x[i] += P.GetX();
	for (int k=0; k<ny; k++) y[k][i] += P.GetY(k);
	return;
}

void DataArray::AddToAverage(int i, int k, const DataPoint P)
// i is the index of the point
// k is the index indicating the number of the averaging cycle
// P is the DataPoint to be included in the average
{
	if (k<2) Add(P);   // If the first averaging cycle
	 else
	 {
		  x[i] = ((k-1)*x[i] + P.GetX()) / k;
		  for (int j=0; j<ny; j++)	y[j][i] = ((k-1)*y[j][i] + P.GetY(j)) / k;
	 }
}

int DataArray::AddToAverage(int k, DataArray* Temp)

// k is the index indicating the number of the averaging cycle
// Temp is the DataArray to be added to the average
{
    // test if arrays are right size
    if ((Temp->Getn() != n) || (Temp->GetNy() != ny)) return -1;

	if (k<1) return -1;   // If k not in range
    DataPoint P(ny);

    for (int i=0; i<n; i++)
    {
          P = Temp->Get(i);
//  we assume all x values are identical
//		  x[i] = ((k-1)*x[i] + P.GetX()) / k;
		  for (int j=0; j<ny; j++)	y[j][i] = ((k-1)*y[j][i] + P.GetY(j)) / k;
    }
    return k;
}




void DataArray::Set(int i, const DataPoint P)
{
	if (i >= n) return;
	x[i] = P.GetX();
	for (int j=0; j<ny; j++) y[j][i] = P.GetY(j);
	return;
}

// calculates the minima of the data, which it returns as a Datapoint
DataPoint DataArray::Minima() const
{
	DataPoint Pmin(ny);
	if (n<1) return Pmin;

	Pmin = Get(0);
	double xmin = Pmin.GetX();
	double *ymin;
	ymin = new double[ny];
	for (int j=0; j<ny; j++) ymin[j]=Pmin.GetY(j);

	for (int i=1; i<n; i++)
	{
		if (x[i]  < xmin ) xmin = x[i];
		for (int j=0; j<ny; j++) if (y[j][i] < ymin[j]) ymin[j] = y[j][i];
	}
	Pmin.SetX(xmin);
	for (int j=0; j<ny; j++) Pmin.SetY(j,ymin[j]);
	delete[] ymin;
	return Pmin;
}

// calculates the maxima of the data, which it returns as a Datapoint
DataPoint DataArray::Maxima() const
{
	DataPoint Pmax(ny);
	if (n<1) return Pmax;

	Pmax = Get(0);
	double xmax = Pmax.GetX();
	double *ymax;
	ymax = new double[ny];
	for (int j=0; j<ny; j++) ymax[j]=Pmax.GetY(j);

	for (int i=1; i<n; i++)
	{
		if (x[i]  > xmax ) xmax = x[i];
		for (int j=0; j<ny; j++) if (y[j][i] > ymax[j]) ymax[j] = y[j][i];
	}
	Pmax.SetX(xmax);
	for (int j=0; j<ny; j++) Pmax.SetY(j,ymax[j]);
	delete[] ymax;
	return Pmax;
}

// returns the DataPoint with the given index
// If the index is out of range it returns the default Datapoint
DataPoint DataArray::Get(int i) const
{
	DataPoint P(ny);
	if ((i<0) || (i>n-1))
	{			 //  If point does not exist return default Datapoint
		return P;
	}
	P.SetX(x[i]);
	for (int j=0; j<ny; j++) P.SetY(j,y[j][i]);
	return P;
}

// simulates _n Datapoints from xmin to xmax
// it calls the DataPoint simulation subroutine
void DataArray::Simulate(double xmin, double xmax, int _n)
{
	if (_n > MAXPNT) _n = MAXPNT;  // If n > maximum   n=maximum
	n = _n;

	DataPoint P(ny);
	for (int i=0; i<n ; i++)
	{
		x[i] = xmin + i * (xmax-xmin)/n;
		P.Simulate(double(x[i]));
		for (int j=0; j<ny; j++) y[j][i] = P.GetY(j);
	}
}
/*
/////////////////////////////////////////////////////////////////
//
// Definitions of the ENDORParameters class
//

// Constructor
ENDORParameters::ENDORParameters()
{
	Start = 10.0;       // default  10 MHz
	Stop = 20.0;        // default  20 MHz
	Step = 2000.0;      // default  2000 kHz
	Dwell = 0;          // default  0 seconds
	Power = -60;        // default -60 dB power
	FM = TRUE;          // default ON
	AM = FALSE;         // default OFF
	FMmodulation = 20.0;	// default 20 kHz FM modulation
	AMmodulation = 99.0;	// default 99% modulation
	CurveAverage = 1;    // default only one curveaverage
}

// Copy operator
void ENDORParameters::operator=(const ENDORParameters& Par)
{
	Start = Par.GetStart();
	Stop = Par.GetStop();
	Step = Par.GetStep();
	Dwell = Par.GetDwell();
	FM = Par.FMon();
	AM = Par.AMon();
	FMmodulation = Par.GetFMmodulation();
	AMmodulation = Par.GetAMmodulation();
	Power = Par.GetPower();
	CurveAverage = Par.GetCurveAverage();
}

void ENDORParameters::Set(char* start, char* stop,
									char* step, char* dwell)
{
	Start = atof(start);
	Stop = atof(stop);
	Step = atof(step);
	Dwell = atoi(dwell);
}

void ENDORParameters::Set(char* start, char* stop,
									char* step, char* dwell,
									char* power, char* fm,
									char* am, int av, char modul)
{
	Start = atof(start);
	Stop = atof(stop);
	Step = atof(step);
	Dwell = atoi(dwell);
	Power = atoi(power);
	FMmodulation = atof(fm);
	AMmodulation = atof(am);
	CurveAverage = av;
	if (modul & 0x01) FM = TRUE; else FM = FALSE;
	if (modul & 0x02) AM = TRUE; else AM = FALSE;
}

int ENDORParameters::Dialog(TWindow* parent)
{
	const int size = 15;
	char start[size];
	char stop[size];
	char step[size];
	char dwell[size];
	char power[size];
	char fm[size];
	char am[size];
	char modul;
	sprintf(start,"%f",Start);
	sprintf(stop,"%f",Stop);
	sprintf(step,"%f",Step);
	sprintf(dwell,"%d",Dwell);
	sprintf(power,"%d",Power);
	sprintf(fm,"%f",FMmodulation);
	sprintf(am,"%f",AMmodulation);
	modul = 0;
	if (AM) modul+=2;
	if (FM) modul +=1;
	int average = CurveAverage;

	TENDORParamDialog Dialog(parent , "Set Measurement parameters",
			start, stop, step, dwell, power, fm, am, &modul, &average, size);

	if (Dialog.Execute() == IDOK )
	{
		Set(start,stop,step,dwell, power, fm, am, average, modul);
		return 0;
	}
	return -1;
}

void ENDORParameters::AsciiOutput(char *output)
{
	char line[128];
	strcpy(output,"Parameters ENDOR : \n");
	sprintf(line," From %8.3f to %8.3f MHz in steps of %10.2f kHz \n",
			Start, Stop, Step);
	strcat(output, line);
	sprintf(line," Power: %d dBm, with 50dB amplifier corresponding to %f Watt \n"
					, Power, pow(10.0, double(Power)/10+2));
	strcat(output, line);
	if (FM)
	{
		sprintf(line," FM modulation %f  kHz ", FMmodulation);
		strcat(output, line);
	}
	if (AM)
	{
		sprintf(line," AM modulation %f  \% ", FMmodulation);
		strcat(output, line);
	}
	strcat(output, "\n");
}

//////////////////////////////////////////////////////////////////////////
//
//	class FileParameters
//

FileParameters::FileParameters()  // default constructor
{
	strcpy(Sample,"Samplename");
	strcpy(Orientation,"");
	LaserFrequency = 244.996;
	Modulation = 0.0;
	Temperature = 0.0;
	strcpy(Comment,"No comment");
}

FileParameters::FileParameters(const FileParameters &ParToCopy)
										  // Constructor by copy
{
	strcpy(Sample, ParToCopy.GetSample());
	strcpy(Orientation, ParToCopy.GetOrientation());
	strcpy(Comment, ParToCopy.GetComment());
	LaserFrequency = ParToCopy.GetLaserFrequency();
	Modulation = ParToCopy.GetModulation();
	Temperature = ParToCopy.GetTemperature();
}

void FileParameters::operator=(const FileParameters &ParToCopy)
{
	strcpy(Sample, ParToCopy.GetSample());
	strcpy(Orientation, ParToCopy.GetOrientation());
	strcpy(Comment, ParToCopy.GetComment());
	LaserFrequency = ParToCopy.GetLaserFrequency();
	Modulation = ParToCopy.GetModulation();
	Temperature = ParToCopy.GetTemperature();
}

void FileParameters::AsciiOutput(char* output)
{
	char c1[80];

	strcpy(output,"Sample: ");
	strcat(output,Sample);
	strcat(output,", orientation: ");
	strcat(output,Orientation);
	strcat(output,"\n");	//endline
	sprintf(c1,"FIR frequency %f GHz, Field Modulation %f mA \n",
					LaserFrequency, Modulation);
	strcat(output, c1);
	sprintf(c1,"Sample Temperature %f K\n", Temperature);
	strcat(output, c1);
	strcat(output, Comment);
	strcat(output,"\n"); //endline
}

/////////////////////////////////////////////////////////////////
//
// Definitions of the FieldParameters class
//

// Constructor
FieldParameters::FieldParameters()
{
	StartField = 0.0;       // default  0 T
	EndField = 1.0;        // default  1 T
	SweepSpeed = 25.0;      // default  25 mT/min
	CurveAverage = 1;    // default only one curveaverage
}

// Copy operator
void FieldParameters::operator=(const FieldParameters& Par)
{
	StartField = Par.GetStartField();
	EndField = Par.GetEndField();
	SweepSpeed = Par.GetSweepSpeed();
	CurveAverage = Par.GetCurveAverage();
}

void FieldParameters::Set(char* start, char* stop, char* speed)
{
	StartField = atof(start);
	EndField = atof(stop);
	SweepSpeed = atof(speed);
}

int FieldParameters::Dialog(TWindow* parent)
{
	const int size = 15;
	char start[size];
	char stop[size];
	char speed[size];
	sprintf(start,"%f",StartField);
	sprintf(stop,"%f",EndField);
	sprintf(speed,"%f",SweepSpeed);
	int average = CurveAverage;

	TFieldSweepDialog FieldDialog(parent , "Set Field sweep",
			start, stop, speed, &average, size);

	if (FieldDialog.Execute() == IDOK )
	{
		Set(start,stop,speed);
		CurveAverage = average;
		return 0;
	}
	return -1;
}

void FieldParameters::AsciiOutput(char *output)
{
	char line[128];
	strcpy(output,"Sweep parameters : \n");
	sprintf(line," From %8.3f to %8.3f T at %10.2f mT/s \n",
			StartField, EndField, SweepSpeed);
	strcat(output, line);
}

//////////////////////////////////////////////////////////////////////////
//
//		class MeasParameters
//
// This class contains data concerning the measurement
//	conditions

// The default constructor:
MeasParameters::MeasParameters()
	: SetPointTemp(0.0), CryostatTemp(0.0), Modulation(0.0), Field(0.0)
{
	time(&Time);
	strcpy(Sensitivity, "0 mV");
}

void MeasParameters::SetSensitivity(const char *S)
{
	strcpy(Sensitivity,S);
}


void MeasParameters::SetTime()
{
	time(&Time);
}
*/

double DataArray::HilbertTransForm(int datcol,int imagcol,int resultcol,double angle)
{
    // first we calculate the 90 degree out of phase
    if ((datcol>=ny) || (imagcol>=ny) || (resultcol>=ny)) return 0.0;
    angle *= 3.1412/180.0;
    double sum, temp;
    double integral=0.0;
    double doubleintegral=0.0;
    int low, high;
    for (int i=0;i<n;i++)
    {
        sum = 0;
        if ((i!=0) && (i!=n-1))   // if not last or first point
        {
            temp = y[datcol][i-1]-y[datcol][i+1];
            temp *= 2;
            sum += temp;
        }
        for (int j=0; j<n; j++)
            if (j!=i)
                sum += y[datcol][j]/(j-i);
        y[imagcol][i] = sum/3.1412;
        y[resultcol][i] = cos(angle)*y[datcol][i] + sin(angle)*y[imagcol][i];
        integral += y[resultcol][i];
        doubleintegral += integral;
    }
    return doubleintegral;
}

double DataArray::ChangeHilbertAngle(int datcol,int imagcol,int resultcol,double angle)
{
    // first we calculate the 90 degree out of phase
    if ((datcol>=ny) || (imagcol>=ny) || (resultcol>=ny)) return -1;
    if ((datcol<0) || (imagcol<0) || (resultcol<0)) return -1;

    angle *= 3.1412/180.0;
    double integral=0.0;
    double doubleintegral=0.0;
    for (int i=0;i<n;i++)
    {
        y[resultcol][i] = cos(angle)*y[datcol][i] + sin(angle)*y[imagcol][i];
        integral += y[resultcol][i];
        doubleintegral += integral;
    }
    return doubleintegral;
}

int DataArray::SubstractLinRegres(int col)
{
    if ((n==0) || (col >= ny)) return -1;
    double S,Sx,Sxx,Sy,Sxy;
    S = 0.0;
    Sx = 0.0;
    Sy = 0.0;
    Sxx = 0.0;
    Sxy = 0.0;
    for (int i=0;i<n;i++)
    {
        S += 1.0;
        Sx += x[i];
        Sy += y[col][i];
        Sxx += x[i]*x[i];
        Sxy += x[i]*y[col][i];
    }
    double a= (Sxx*Sy - Sx*Sxy)/(S*Sxx-Sx*Sx);
    double b= (S*Sxy-Sx*Sy)/(S*Sxx-Sx*Sx);
    for (int i=0;i<n;i++)
        y[col][i] -= ( a  + b*x[i]);
    return 0;
}

double DataArray::SubstractMean(int col)
{
    if ((n==0) || (col >= ny)) return -1.0;
    double Mean;
    Mean = 0.0;
    for (int i=0;i<n;i++)
            Mean += y[col][i];
    Mean /= (double)n;
    for (int i=0;i<n;i++)
        y[col][i] -= Mean;
    return Mean;
}

int DataArray::SubstractLine(int col, int points, int first, int last)
{
    if ((n==0) || (col >= ny)) return -1;
    if ((first <0) || (first > n-2))
        first = 0;
    if ((last <= first) || (last > n-1))
        last = n-1;
    if (points > n/2) points = n/2;
    if (points < 1) points =1;
    double S,Sx,Sxx,Sy,Sxy;
    S = 0.0;
    Sx = 0.0;
    Sy = 0.0;
    Sxx = 0.0;
    Sxy = 0.0;
    for (int i=first;i<first+points;i++)
    {
        S += 1.0;
        Sx += x[i];
        Sy += y[col][i];
        Sxx += x[i]*x[i];
        Sxy += x[i]*y[col][i];
    }
    for (int i=last+1-points;i<last+1;i++)
    {
        S += 1.0;
        Sx += x[i];
        Sy += y[col][i];
        Sxx += x[i]*x[i];
        Sxy += x[i]*y[col][i];
    }
    double a= (Sxx*Sy - Sx*Sxy)/(S*Sxx-Sx*Sx);
    double b= (S*Sxy-Sx*Sy)/(S*Sxx-Sx*Sx);
    for (int i=0;i<n;i++)
        y[col][i] -= ( a  + b*x[i]);
    return 0;
}

DataArray DataArray::ReSample(double MinX, double MaxX, int npts)
{
    DataArray LinDat(npts, ny);  // make array of npts points
    if (npts<2) return LinDat;
    double NewX;
    DataPoint NewP(ny);
    int k,l;
    //  Now we have to seek for each x-value the corresponding position
//  in the array. We can do that by some "hunt" routine.
//  We can make that also a method of DataArray
//
//  Problems can arise if the measured array is not monotonic ascending
//  or descending. We should make sure of that.

    bool mono = true;
    if (x[0] > x[n-1])
    {
        for (int i=1; i<n; i++)
            if (x[i] > x[i-1]) mono = false;
    }
     else
        {
            for (int i=1; i<n; i++)
            if (x[i] < x[i-1]) mono = false;
        }
//
//  if (mono == false) we'll have to do something
//  e.g. check a linear regression curve
//
//  Now we assume that we are monotonic in x
    if (!mono) return LinDat;

    l = 0;
    for (int i=0; i<npts;i++)
    {
        NewX = MinX + i*(MaxX-MinX)/(npts-1);
//        LinDat.Set(0, NewX);  //set X-values
        k = Hunt(NewX,l);     //Hunt for point index
                              //for which x[k]<NewX<x[k+1}
                              // or x[k]>NewX>x[k+1]

        NewP = Interpolate(NewX,k);
        LinDat.Add(NewP);
        l = k+1;
    }
    return LinDat;
}

DataPoint DataArray::Interpolate(double Xval, int k)
{
    DataPoint P(ny);
    double A;
    if (k<0) P = Get(0);
      else
      {
         if (k>=n-1) P = Get(n-1);
            else
            {
                A = (x[k+1]-Xval)/(x[k+1]-x[k]);
                P = Get(k)*A + Get(k+1)*(1-A);
            }
      }

    P.Set(0,Xval);

    return P;
}

///////////////////////////////////////////
//
//  Hunt should return index of point for which
//    x(i) < NewX < x(i+1)  or x(i) > NewX > x(i+1)
//
int DataArray::Hunt(double NewX, int guess)
{
    if (n<2) return -2; // return if array too short

    int inc, jlo, jhi, jm;
    jlo = guess;

    bool ascend = (x[0]<x[n-1]);
    if ((jlo >= 0) && (jlo < n))    // if guess in data range
    {
        inc = 1;  //Set hunt increment
        if (ascend)
        {
            if (NewX > x[jlo])
            {
                if (jlo == (n-1)) return (n-1); // NewX outside array
                jhi = jlo+1;
                while ((jhi <= n-1) && (NewX >= x[jhi]))
                {
                    jlo = jhi;
                    inc += inc;
                    jhi = jlo + inc;
                    if (jhi > (n-1)) jhi = n;
                }
            }
              else
              {
                if (jlo == 0) return -1;
                jhi = jlo--;
                while ((jlo >= 0) && (NewX < x[jlo]))
                {
                    jhi = jlo;
                    inc += inc;
                    if (inc > jhi) jlo = -1;
                      else jlo = jhi-inc;
                }
              }
        }
          else // if descending values (or downsweep)
          {
            if (NewX < x[jlo])
            {
                if (jlo == (n-1)) return (n-1); // NewX outside array
                jhi = jlo+1;
                while ((jhi <= n-1) && (NewX < x[jhi]))
                {
                    jlo = jhi;
                    inc += inc;
                    jhi = jlo + inc;
                    if (jhi > (n-1)) jhi = n;
                }
            }
              else
              {
                if (jlo == 0) return -1;
                jhi = jlo--;
                while ((jlo >= 0) && (NewX >= x[jlo]))
                {
                    jhi = jlo;
                    inc += inc;
                    if (inc > jhi) jlo = -1;
                      else jlo = jhi-inc;
                }
              }
        }   // end if (ascend) loop
    }  // end hunting loop
// now start binominal search
      else
      {
        jlo = -1;
        jhi = n;
      }
// remember, in the binomial search only
//      x[jm] is evaluated, and that only if jhi differs
//      by at least 2
    while ((jhi-jlo) != 1)
    {
        jm = (jhi+jlo) >> 1;
        if ((NewX > x[jm]) == ascend)
            jlo = jm;
          else
            jhi = jm;
    }
    return jlo;  //NB range of jlow is from
    // -1 to n-1
    // -1 indicates out of range
    // n-1 indicates out of range
}
//////////////////////////////////////

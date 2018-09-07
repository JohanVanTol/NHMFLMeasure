#ifndef __IOSTREAM.H
#include <iostream.h>
#endif
#include <time.h>
#ifndef __MYDATA_H
#define __MYDATA_H

const int MAXPNT=65536;
const int MAXNY =200;

class DataPoint
{
	public:
		DataPoint(int _ny = 1);                // Default constructor
		DataPoint(const DataPoint& P);         // Constructor by copy

		~DataPoint();				   // Destructor

		void operator=(const DataPoint& P);    // Copy operator

		DataPoint operator+(const DataPoint& P) const;   //Arithmetic operators
		DataPoint& operator+=(const DataPoint& P);
		DataPoint operator-(const DataPoint& P) const;
		DataPoint operator/(double divide) const;
		DataPoint& operator/=(double divide);
		DataPoint operator*(double mult) const;

		int GetNy() const {return ny;}
		double GetX() const {return x;}
		double Get(int k=0) const;

		void SetX(double _x) {x=_x;}
		void Set(int k, double el);

		void Simulate(double _x);

		friend ostream& operator <<(ostream& os, const DataPoint& p);
		friend istream& operator >>(istream& is, DataPoint& p);

//	The following functions are included for compatibility with old version
//	Do not use in new applications

		DataPoint(double _x, double _y1, double _y2);  //For compatibility only
		double GetRef() const {if (ny>1) return y[1]; else return 0.0;}
		double GetY(int k=0) const;
		void SetY(double _y1) {y[0]=_y1;}
		void SetRef(double _y2) {if (ny>1) y[1]=_y2;}
		void SetY(int k, double _y);
		void Set(double _x, double _y, double _z);

	private:
		double x;
		double *y;
		int ny;
};

class DataArray
{
	private:
		double *x;             //array x-values
		double **y;            //arrays of y-values
		int ny;                //number of y-arrays
		int n;                 //number of valid data points

	public:
		DataArray(int npts = MAXPNT, int nY = 1);
		DataArray(const DataArray& DataToCopy);
		~DataArray();

		void operator=(const DataArray& DataToCopy);		
		void Add(const DataPoint p);
		void Sum(int i, const DataPoint p);
		void AddToAverage(int i, int k, const DataPoint P);
        int AddToAverage(int k, DataArray* Temp);

		DataPoint Minima() const;
		DataPoint Maxima() const;
		void Set(int i, const DataPoint p);

		DataPoint Get(int i) const;
		int Getn() const {return n;}
		int GetNy() const {return ny;}
		void Simulate(double xmin, double xmax, int _n=100);
		void flush() {n=0;}
        int AddColumn(int ncol=1);
        double HilbertTransForm(int datcol,int imagcol, int finalcol, double angle);
        double ChangeHilbertAngle(int datcol,int imagcol, int finalcol, double angle);
        int SubstractLinRegres(int col);
        double SubstractMean(int col);
        int SubstractLine(int col,int first=0, int last=0, int points=1);
        DataArray ReSample(double MinX, double MaxX, int npts);
        DataPoint Interpolate(double xval, int pos);
        int Hunt(double newx, int guess);
};
/*
class ENDORParameters
{
	public:
		ENDORParameters();
		double GetStart() const {return Start;}
		double GetStop() const {return Stop;}
		double GetStep() const {return Step;}
		int GetDwell() const {return Dwell;}
		BOOL FMon() const {return FM;}
		BOOL AMon() const {return AM;}
		double GetFMmodulation() const {return FMmodulation;}
		double GetAMmodulation() const {return AMmodulation;}
		double GetPower() const {return Power;}
		int GetNumberOfPoints() const {return int(1000.0*fabs(Stop-Start)/Step);}

		void operator=(const ENDORParameters& Par);
		void SetStart(char* start) {Start = atof(start);}
		void SetStop(char* stop) {Stop = atof(stop);}
		void SetStep(char* step) {Step = atof(step);}
		void SetDwell(char* dwell) {Dwell = atoi(dwell);}
		void Set(char* start, char* stop,
									char* step, char* dwell,
									char* power, char* fm,
									char* am, int av, char modul);
		void Set(char* start,char* stop,
									char* step, char* dwell);
 //		int Dialog(TWindow* parent);
		void AsciiOutput(char* output);
		int GetCurveAverage() const {return CurveAverage;}
		void SetCurveAverage(int av) {CurveAverage = av;}

	private:
		double Start;
		double Stop;
		double Step;
		int Dwell;
		double FMmodulation;
		double AMmodulation;
		BOOL FM;
		BOOL AM;
		int Power;
		int CurveAverage;
};

class FieldParameters
{
	public:
		FieldParameters();
		double GetStartField() const {return StartField;}
		double GetEndField() const {return EndField;}
		double GetSweepSpeed() const {return SweepSpeed;}
		void operator=(const FieldParameters& Par);
		void SetStartField(char* start) {StartField = atof(start);}
		void SetEndField(char* stop) {EndField = atof(stop);}
		void SetSweepSpeed(char* speed) {SweepSpeed = atof(speed);}
		void Set(char* start,char* stop,char* speed);
 //		int Dialog(TWindow* parent);
		void AsciiOutput(char* output);
		int GetCurveAverage() const {return CurveAverage;}
		void SetCurveAverage(int av) {CurveAverage = av;}

	private:
		double StartField;
		double EndField;
		double SweepSpeed;
		int CurveAverage;
};


class FileParameters
{
	private:
		char Sample[80];
		char Orientation[80];
		double LaserFrequency;
		double Modulation;
		double Temperature;
		char Comment[255];

	public:
		FileParameters();
		FileParameters(const FileParameters &ParToCopy);
		void operator=(const FileParameters &ParToCopy);

		void SetSample(const char* samplename) {strcpy(Sample,samplename);}
		const char* GetSample() const {return Sample;}
		void SetOrientation(const char* orient) {strcpy(Orientation, orient);}
		const char* GetOrientation() const {return Orientation;}
		void SetLaserFrequency(double freq) {LaserFrequency = freq;}
		double GetLaserFrequency() const {return LaserFrequency;}
		void SetModulation(double modula) {Modulation = modula;}
		double GetModulation() const {return Modulation;}
		void SetTemperature(double temp) {Temperature = temp;}
		double GetTemperature() const {return Temperature;}
		void SetComment(const char* comm) {strcpy(Comment,comm);}
		const char* GetComment() const {return Comment;}

		void AsciiOutput(char* output);
};

class MeasParameters
{
	private:
		double SetPointTemp;   	//The set-temperature in K
		double CryostatTemp;         //The cryostat temperature in K
		double Modulation;   //The modulation current in mA
		double Field;			//The fieldvalue in Tesla
		char Sensitivity[10];			//The sensitivity in V
		time_t Time;			//The time (in seconds since a certain date)

	public:
		MeasParameters();

		void SetSetPointTemp(double T) {SetPointTemp = T;}
		void SetCryostatTemp(double T) {CryostatTemp = T;}
		void SetModulation(double Mod) {Modulation = Mod;}
		void SetField(double B) {Field = B;}
		void SetSensitivity(const char *S);
		void SetTime();

		double GetSetPointTemp() const {return SetPointTemp;}
		double GetCryostatTemp() const {return CryostatTemp;}
		double GetModulation() const {return Modulation;}
		double GetField() const {return Field;}
		const char* GetSensitivity() const {return Sensitivity;}
		time_t GetTime() const {return Time;}
};
*/
#endif

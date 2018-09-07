//---------------------------------------------------------------------------

#ifndef ThermometerClassH
#define ThermometerClassH

class Thermometer
{
	private:
		double a[6];    // polynominial coefficients to 6th order
		double Tmin;
		double Tmax;
	public:
		Thermometer(double _Tmin = 1.0, double _Tmax=300.0, double a0=0.0, double a1=0.0, double a2=0.0, double a3=0.0, double a4=0.0, double a5=0.0);
		double GetTemperature(double R);
		double GetTemperature(double R, double B);
//		double GetCoefficient(int _i);
};



//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------


#pragma hdrstop
#include <math.h>
#include "ThermometerClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Thermometer::Thermometer(double _Tmin, double _Tmax, double a0, double a1, double a2, double a3, double a4, double a5)
{
	a[0] = a0;
	a[1] = a1;
	a[2] = a2;
	a[3] = a3;
	a[4] = a4;
	a[5] = a5;
	Tmin = _Tmin;
	Tmax = _Tmax;
}

double Thermometer::GetTemperature(double _R)
{
	if (_R<1) return 0.0;

	double lR = log10(_R);
	double lT = a[0] + a[1]*lR + a[2]*lR*lR + a[3]*lR*lR*lR + a[4]*lR*lR*lR*lR + a[5]*lR*lR*lR*lR*lR;
	double Tem = pow(10.0, lT);
	if (Tem < Tmin) return 0.0;
	if (Tem > Tmax) return 999.0;
	return Tem;
}

double Thermometer::GetTemperature(double _R, double _B)
{
	if (_R<1) return 0.0;
	if (_B>0.6)_R /= 1+(_B-0.6)*(_R-500)*(_R-500)*0.000001;        // This is still wrong. Change !!
	double lR = log10(_R);
	double lT = a[0] + a[1]*lR + a[2]*lR*lR + a[3]*lR*lR*lR + a[4]*lR*lR*lR*lR + a[5]*lR*lR*lR*lR*lR;
	double Tem = pow(10, lT);
	if (Tem < Tmin) return 0.0;
	if (Tem > Tmax) return 999.0;
	return Tem;
}


#pragma once
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

template<class R>
R sqr(R x)
{ return x*x; }


template<class R>
void real_fft(std::vector<R>& a, bool inversefft=false)

/*************************************************************************
Быстрое преобразование Фурье

Алгоритм проводит быстрое преобразование Фурье вещественной
функции, заданной n отсчетами на действительной оси.

В зависимости от  переданных параметров, может выполняться
как прямое, так и обратное преобразование.

Входные параметры:
    tnn  -   Число значений функции. Должно  быть  степенью
            двойки. Алгоритм   не  проверяет  правильность
            переданного значения.
    a   -   array [0 .. nn-1] of Real
            Значения функции.
    InverseFFT
        -   направление преобразования.
            True, если обратное, False, если прямое.
            
Выходные параметры:
    a   -   результат   преобразования.   Подробнее    см.
            описание на сайте.
*************************************************************************/
{

    R c1;
    R c2;
    R h1r;
    R h1i;
    R h2r;
    R h2i;
    R wrs;
    R wis;
    R wtemp;
    R wr;
    R wpr;
    R wpi;
    R wi;
    R theta;
    R tempr;
    R tempi;
	int tnn = static_cast<int>(a.size());

    if( tnn==1 )
    {
        return;
    }
	R ttheta;
    if( !inversefft )
    {
        ttheta = 2*M_PI/tnn;
        c1 = 0.5;
        c2 = -0.5;
    }
    else
    {
        ttheta = 2*M_PI/tnn;
        c1 = 0.5;
        c2 = 0.5;
        ttheta = -ttheta;
        R twpr = -2.0*sqr(sin(0.5*ttheta));
        R twpi = sin(ttheta);
	    R twr = 1.0+twpr;
	    R twi = twpi;
        for(int i = 2; i <= tnn/4+1; i++)
        {
            int i1 = i+i-2;
            int i2 = i1+1;
            int i3 = tnn+1-i2;
            int i4 = i3+1;
            wrs = twr;
            wis = twi;
            h1r = c1*(a[i1]+a[i3]);
            h1i = c1*(a[i2]-a[i4]);
            h2r = -c2*(a[i2]+a[i4]);
            h2i = c2*(a[i1]-a[i3]);
            a[i1] = h1r+wrs*h2r-wis*h2i;
            a[i2] = h1i+wrs*h2i+wis*h2r;
            a[i3] = h1r-wrs*h2r+wis*h2i;
            a[i4] = -h1i+wrs*h2i+wis*h2r;
            R twtemp = twr;
            twr = twr*twpr-twi*twpi+twr;
            twi = twi*twpr+twtemp*twpi+twi;
        }
        h1r = a[0];
        a[0] = c1*(h1r+a[1]);
        a[1] = c1*(h1r-a[1]);
    }
	int isign;
    if( inversefft )
    {
        isign = -1;
    }
    else
    {
        isign = 1;
    }
    int n = tnn;
    int nn = tnn/2;
    int j = 1;
    for(int ii = 1; ii <= nn; ii++)
    {
        int i = 2*ii-1;
        if( j>i )
        {
            tempr = a[j-1];
            tempi = a[j];
            a[j-1] = a[i-1];
            a[j] = a[i];
            a[i-1] = tempr;
            a[i] = tempi;
        }
        int m = n/2;
        while(m>=2&&j>m)
        {
            j = j-m;
            m = m/2;
        }
        j = j+m;
    }
    int mmax = 2;
    while(n>mmax)
    {
        int istep = 2*mmax;
        theta = 2*M_PI/(isign*mmax);
        wpr = -2.0*sqr(sin(0.5*theta));
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for(int ii = 1; ii <= mmax/2; ii++)
        {
            int m = 2*ii-1;
            for(int jj = 0; jj <= (n-m)/istep; jj++)
            {
                int i = m+jj*istep;
                j = i+mmax;
                tempr = wr*a[j-1]-wi*a[j];
                tempi = wr*a[j]+wi*a[j-1];
                a[j-1] = a[i-1]-tempr;
                a[j] = a[i]-tempi;
                a[i-1] = a[i-1]+tempr;
                a[i] = a[i]+tempi;
            }
            wtemp = wr;
            wr = wr*wpr-wi*wpi+wr;
            wi = wi*wpr+wtemp*wpi+wi;
        }
        mmax = istep;
    }
    if( inversefft )
    {
        for(int i = 1; i <= 2*nn; i++)
        {
            a[i-1] = a[i-1]/nn;
        }
    }
    if( !inversefft )
    {
        R twpr = -2.0*sqr(sin(0.5*ttheta));
        R twpi = sin(ttheta);
        R twr = 1.0+twpr;
        R twi = twpi;
        for(int i = 2; i <= tnn/4+1; i++)
        {
            int i1 = i+i-2;
            int i2 = i1+1;
            int i3 = tnn+1-i2;
            int i4 = i3+1;
            wrs = twr;
            wis = twi;
            h1r = c1*(a[i1]+a[i3]);
            h1i = c1*(a[i2]-a[i4]);
            h2r = -c2*(a[i2]+a[i4]);
            h2i = c2*(a[i1]-a[i3]);
            a[i1] = h1r+wrs*h2r-wis*h2i;
            a[i2] = h1i+wrs*h2i+wis*h2r;
            a[i3] = h1r-wrs*h2r+wis*h2i;
            a[i4] = -h1i+wrs*h2i+wis*h2r;
            R twtemp = twr;
            twr = twr*twpr-twi*twpi+twr;
            twi = twi*twpr+twtemp*twpi+twi;
        }
        h1r = a[0];
        a[0] = h1r+a[1];
        a[1] = h1r-a[1];
    }
}

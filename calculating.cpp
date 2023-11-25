#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QString>
#include <QObject>
#include <QDebug>
#include <QtGui>
#include <math.h>
#include "calculating.h"

#define Rm 14960000000.
//#define Reuq 6378137.
//#define Rnorth 6356752.314
#define d 1392000.

#define pi 3.1415926535897932384626433832795
#define rad pi/180.

Calculator::Calculator()
{
   Reuq = 6378137.;
   Rnorth = 6356752.314;
}

Calculator::~Calculator()
{
}

void Calculator::defElips(double a, double b)
{
   Reuq = a;
   Rnorth = b;
}

double Calculator::Delta(double &day, double &mounth, bool leap)
{
   int year_day_number = 0;
   int mounth_days_number[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

   if(leap) 
      mounth_days_number[1] = 29;

   if(mounth > 1) 
   {
     for(int i = 0; i < mounth - 1; i++) 
        year_day_number+=mounth_days_number[i];
   }
   year_day_number+=day;
   double delta = 23.45*sin(rad*(180./183.)*(year_day_number-81));
   qDebug()<<"Today: "<<day<<" - "<<mounth<<"; number: "<<year_day_number<<"; delta = "<<delta;
   return delta;
}

double Calculator::omega(double &hour, double &minute, double &second, int zone)
{
   double local_time = hour + minute/60. + second/3600.;
   double w = (12 + zone - local_time)*15;
   double GT;
	
   if(local_time > double(zone)) 
      GT = local_time - zone;
   else 
      GT = 24 - fabs(local_time - zone);
   qDebug()<<"zone: "<<zone<<"; time in decimal: "<<local_time<<", Grinwich time: "<<GT<<", w = "<<w;
   return w;
}

double Calculator::azimut(double phi, int z, double Tc, double teta)//(double Tr, double Ts, double Tc)
{
	/*double Xp = A*c;
	double Yp = A*s;
	double Zp = A*t;

	double zNQ = A*((Rnorth*Rnorth/(Reuq*Reuq*t))+t);

	double zMQ = (Reuq*Reuq*t/(Rnorth*Rnorth*Rnorth*Rnorth+Reuq*Reuq*Reuq*Reuq*t*t))*( A*(Rnorth*Rnorth+Reuq*Reuq*t*t) - Rm*Rnorth*Rnorth*(  (cos(rad*(phi-W)))*(cos(rad*delta)) - (Rnorth*Rnorth*(sin(rad*delta))/(Reuq*Reuq*t))
		                                                                                                                                  )
		                                                                             );
	double yMQ = n + ( Rnorth*Rnorth*s*(zMQ - k)/(Reuq*Reuq*t) );
	double xMQ = m + (c/s)*(yMQ - n);
	
	double a = sqrt(xMQ*xMQ + yMQ*yMQ + (zMQ-zNQ)*(zMQ-zNQ));
	double b = sqrt(Xp*Xp + Yp*Yp + (Zp-zNQ)*(Zp-zNQ));
	double c_ = sqrt((xMQ-Xp)*(xMQ-Xp) + (yMQ-Yp)*(yMQ-Yp) + (zMQ-Zp)*(zMQ-Zp));

	qDebug()<<"a = "<<a<<", b = "<<b<<", c = "<<c_;

	//double Az = (1/rad)*acos((b*b+c_*c_-a*a)/(2*b*c_));

	

	//qDebug()<<"az["<<i<<"] = "<<az[i];

	double azimuth;

	//прямая а - направлениеп на Север
	double ax = Xp;
	double ay = Yp;
	double az = Zp - zNQ;

	//прямая b - направление тени
	double bx = xMQ - Xp;
	double by = yMQ - Yp;
	double bz = zMQ - Zp;

	double Az = (100000./rad)*acos(abs(ax*bx+ay*by+az*bz)/( (sqrt(ax*ax+ay*ay+az*az))*(sqrt(bx*bx+by*by+bz*bz)) ));
	qDebug()<<Az;
	//double Az = 2*(1/rad)*asin(sqrt((a*a - (b-c_)*(b-c_))/(4*b*c_) ));
	/*double azi[5];

	azi[0] = Az - 360;//(powf(-1., -3))*Az -3*180;
	azi[1] = Az + 360;//(powf(-1., -2))*Az -2*180;
	azi[2] = -Az - 360;//(powf(-1., -1))*Az -180;
	azi[3] = -Az + 360;//(powf(-1., 1))*Az +180;
	azi[4] = Az;
	//azi[5] = (powf(-1., 2))*Az +2*180;
	//azi[6] = (powf(-1., 3))*Az +3*180;
	int i = 0;

	while( abs( (cos(rad*azi[i]/2))*(cos(rad*azi[i]/2)) - ( ((b+c_)*(b+c_)-a*a)/(4*b*c_) ) ) >= 0.001 && i < 7) {
		qDebug()<<"az["<<i<<"] = "<<azi[i];
		i++;
	}

	double AZ = 180 - azi[i];

	if(W < phi) azimuth = AZ;
	else azimuth = 360 - AZ;

	qDebug()<<"W - phi = "<<W-phi<<", azimuth = "<<azimuth;
	
	double gradus = 180/(Ts-Tr);
	double Th = Tr + (Ts-Tr)/2;
	double azimuth;
	if(Tc <= Th) azimuth = 270 + (Tc - Tr)*gradus;
	else azimuth = (Tc - Tr)*gradus-90;
	*/
   double azimuth;
   double W_null=phi;
   double Tnull = 12-(W_null/15);

   if(Tnull < 0.) 
      Tnull += 24;
   if(Tnull >= 24.) 
      Tnull = Tnull-24;
   double Tn = Tnull + z;
   int hh_n, mm_n;
   double ss_n;
   hh_n = int(Tn);
   mm_n = int(60*(Tn-hh_n));
   ss_n = 60*(60*(Tn-hh_n)-mm_n);
   qDebug()<<"Tn when azimuth = 0: "<<Tn<<". E. c. "<<hh_n<<":"<<mm_n<<":"<<ss_n;
	
   if(Tn < 0.) 
      Tn += 24;
   if(Tn >= 24.) 
      Tn = Tn-24;
   if(
      (teta > 0 && teta >= delta)||
      (teta <=0 && teta <= delta)
   ) 
   {
      azimuth = (Tc - Tn)*15;
      
      if(azimuth < 0) 
         azimuth+=360;
   }
   else 
      azimuth = 180 - (Tc - Tn)*15;
   return azimuth;
}

void Calculator::init(
   double &lati, 
   double &longi, 
   double &alti, 
   double &hour, 
   double &minute, 
   double &second, 
   double &day, 
   double &mounth, 
   double &length, 
   bool    leap, 
   int     zone, 
   double  decl
)
{
   if(day != 0.) 
      delta = Delta(day, mounth, leap);
   else 
   {
      delta = decl;
      qDebug()<<"delta = "<<delta;
   }
   W = omega(hour,minute,second, zone);
   A = (Reuq*Rnorth/sqrt(Reuq*Reuq*(tan(rad*lati))*(tan(rad*lati))+Rnorth*Rnorth))+
       alti/sqrt(1+(tan(rad*lati))*(tan(rad*lati)));
   s = sin(rad*longi);
   c = cos(rad*longi);
   t = tan(rad*lati);
   m = Rm*(cos(rad*delta))*(cos(rad*W));
   n = Rm*(cos(rad*delta))*(sin(rad*W));
   k = Rm*sin(rad*delta);
   r = Reuq*Reuq;
   u = Rnorth*Rnorth;
   B_y = (A*(t*(s*(A*t*(r-u)-k*(r-2*u))-n*r*t)-c*u*(c*n-m*s))+u*(c*m*n-s*(k*k+m*m))+k*n*r*t)/
         (A*(A*c*t*t*(r-u)+c*(k*t*(2*u-r)+n*s*u)-m*(r*t*t+s*s*u))-c*u*(k*k+n*n)+m*(k*r*t+n*s*u));
   B_z = -(c*(A*t*(u-r)-k*u)+m*r*t)*
          (A*(t*(s*(A*t*(r-u)-k*(r-2*u))-n*r*t)-c*u*(c*n-m*s))+u*(c*m*n-s*(k*k+m*m))+k*n*r*t)/
          ((A*(A*c*t*t*(r-u)+c*(k*t*(2*u-r)+s*n*u)-m*(r*t*t+s*s*u))-c*u*(k*k+n*n)+m*(k*r*t+n*s*u))*u*(c*n-m*s))-
          (s*(A*t*(r-u)+k*u)-n*r*t)/(u*(c*n-m*s));
   C_y = (A*(A*t*t*(c*n*(r-u)+m*s*u)+m*(c*c*n*u+s*(r*t*(k-t*t)-u*(c*m+2*k*t+n*s)))+c*n*(n*s*u+k*t*(2*u-r)))+u*(k*k+m*m+n*n)*(m*s-c*n))/
         (A*(A*c*t*t*(r-u)+c*(k*t*(2*u-r)+n*s*u)-m*(r*t*t+s*s*u))-c*u*(k*k+n*n)+m*(k*r*t+n*s*u));
   C_z = -(c*(A*t*(u-r)-k*u)+m*r*t)*
          (A*(A*t*t*(c*n*(r-u)+m*s*u)+m*(c*c*n*u+s*(r*t*(-t*t+k)-u*(c*m+2*k*t+n*s)))+c*n*(k*t*(2*u-r)+s*n*u))+u*(k*k+m*m+n*n)*(-c*n+m*s))/
          ((A*(A*c*t*t*(r-u)+c*(k*t*(2*u-r)+s*n*u)-m*(r*t*t+s*s*u))-c*u*(k*k+n*n)+m*(k*r*t+n*s*u))*u*(c*n-m*s))-A*t*(r-u)/u;
   x_1 = 
      (
         2*(B_y*(C_y-n)+B_z*(C_z-k))-
         sqrt(
            4*C_z*(2*k-C_z)*(B_y*B_y+1)+
            B_y*B_y*(d*d-4*(k*k+m*m))+
            8*B_z*(C_z-k)*(B_y*(C_y-n)-m)+
            4*C_y*(2*n-C_y)*(B_z*B_z+1)+
            B_z*B_z*(d*d-4*(m*m+n*n))+
            8*B_y*m*(n-C_y)+
            d*d-4*(k*k+n*n)
         )-
         2*m
      )/
      (-2*(B_y*B_y+B_z*B_z+1));
   x_2 = 
      (
         2*(B_y*(C_y-n)+B_z*(C_z-k))+
         sqrt(
            4*C_z*(2*k-C_z)*(B_y*B_y+1)+
            B_y*B_y*(d*d-4*(k*k+m*m))+
            8*B_z*(C_z-k)*(B_y*(C_y-n)-m)+
            4*C_y*(2*n-C_y)*(B_z*B_z+1)+
            B_z*B_z*(d*d-4*(m*m+n*n))+
            8*B_y*m*(n-C_y)+
            d*d-
            4*(k*k+n*n)
         )-
         2*m
      )/
      (-2*(B_y*B_y+B_z*B_z+1));
   y_1 = B_y*x_1+C_y;
   y_2 = B_y*x_2+C_y;
   z_1 = B_z*x_1+C_z;
   z_2 = B_z*x_2+C_z;
   A_ = Rnorth*c/Reuq;
   B_ = Rnorth*s/Reuq;
   C_ = Reuq*t/Rnorth;
   D_=-(
      sqrt(
         Rnorth*Rnorth*Rnorth*Rnorth + 
         Reuq*Reuq*Reuq*Reuq*t*t
      )
   )*
   (alti+(d/2.))/
   (Reuq*Rnorth) - 
   sqrt(Rnorth*Rnorth + Reuq*Reuq*t*t);
   Y_r = -(
      B_*C_*Rm*sin(rad*delta)+
      B_*D_
      -sqrt(
         -A_*A_*A_*A_*(sin(rad*delta))*(sin(rad*delta))*Rm*Rm-A_*A_*B_*B_*(sin(rad*delta))*(sin(rad*delta))*Rm*Rm-
         A_*A_*C_*C_*(sin(rad*delta))*(sin(rad*delta))*Rm*Rm+A_*A_*A_*A_*Rm*Rm+A_*A_*B_*B_*Rm*Rm-2*A_*A_*C_*D_*(sin(rad*delta))*Rm-A_*A_*D_*D_
      )
   )/
   (A_*A_+B_*B_);
   Y_s = -(
      B_*C_*Rm*sin(rad*delta)+
      B_*D_+
      sqrt(
         -A_*A_*A_*A_*(sin(rad*delta))*(sin(rad*delta))*Rm*Rm-
         A_*A_*B_*B_*(sin(rad*delta))*(sin(rad*delta))*Rm*Rm-
         A_*A_*C_*C_*(sin(rad*delta))*(sin(rad*delta))*Rm*Rm+
         A_*A_*A_*A_*Rm*Rm+
         A_*A_*B_*B_*Rm*Rm-
         2*A_*A_*C_*D_*(sin(rad*delta))*Rm-A_*A_*D_*D_
      )
   )/
   (A_*A_+B_*B_);
   double X_r = -(C_*Rm*sin(rad*delta)+B_*Y_r+D_)/A_;
   double X_s = -(C_*Rm*sin(rad*delta)+B_*Y_s+D_)/A_;
   double epsil = 0.001;
   
   if(X_r != 0.) 
      Wr = atan(Y_r/X_r);
   else 
   {
      if(Y_r < 0) 
         Wr = - pi/2.;
      else if(Y_r > 0)
         Wr = pi/2.;
      else 
         Wr = 0;
   }

   if(
      abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
      abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
   ) 
   {
      double W[6];
      W[0] = Wr-3*pi;
      W[1] = Wr-2*pi;
      W[2] = Wr-pi;
      W[3] = Wr+pi;
      W[4] = Wr+2*pi;
      W[5] = Wr+3*pi;
      int i=0;

      while(
         abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
         abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
      ) 
      {
         Wr = W[i];
	 i++;
	
	 if(i == 6) 
	    break;
      }

      if(
         abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
         abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
      ) 
      {
         Wr = -atan(Y_r/X_r);
	 
	 if(
	    abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
	    abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
	 ) 
	 {
	    Wr = asin(Y_r/(Rm*cos(rad*delta)));
	    
	    if(
	       abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
	       abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
	    ) 
	    {
	       W[0] = (powf(-1., -3))*Wr -3*pi;
	       W[1] = (powf(-1., -2))*Wr -2*pi;
	       W[2] = (powf(-1., -1))*Wr -pi;
	       W[3] = (powf(-1., 1))*Wr +pi;
	       W[4] = (powf(-1., 2))*Wr +2*pi;
	       W[5] = (powf(-1., 3))*Wr +3*pi;
	       int j = 0;
	       
	       while(
	          abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
	          abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
	       ) 
	       {
	          Wr = W[j];
		  j++;
		
		  if(j == 6) 
		     break;
	       }
					
	       if(
	          abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
	          abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
	       ) 
	       {
	          Wr = acos(X_r/(Rm*cos(rad*delta)));
		  
		  if(
		     abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
		     abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
		  ) 
		  {
		     W[0] = Wr -4*pi;
		     W[1] = Wr -2*pi;
		     W[3] = Wr = Wr + 2*pi;
		     W[4] = Wr + 4*pi;
		     int k = 0;
		     
		     while(
		        abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
		        abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
		     ) 
		     {
		        Wr = W[k];
			k++;
			
			if(k == 4) 
			   break;
		     }
		     
		     if(
		        abs(Rm*(cos(rad*delta))*cos(Wr) - X_r) >= epsil || 
		        abs(Rm*(cos(rad*delta))*sin(Wr) - Y_r) >= epsil
		     ) 
		        qDebug()<<"angle Wr is still not found!";
		     else 
		        qDebug()<<"Wr is found by arccosinus, i = "<<k<<".";
		  }
		  else 
		     qDebug()<<"Wr is found by arccosinus.";
	       }
	       else 
	          qDebug()<<"Wr is found by arcsinus, i = "<<j;
	    }
	    else 
	       qDebug()<<"Wr is found by arcsinus";
	 }
	 else 
	    qDebug()<<"Wr is found by -arctangens";
      }
      else 
         qDebug()<<"Wr is found by arctangens, i = "<<i<<".";
   }
   else 
      qDebug()<<"Wr is found by arctangens.";
   qDebug()<<"Xr by Wr: "<<Rm*(cos(rad*delta))*cos(Wr)<<", Xr wrighte: "<<X_r;
   qDebug()<<"Yr by Wr: "<<Rm*(cos(rad*delta))*sin(Wr)<<", Yr wrighte: "<<Y_r;
   
   if(X_s != 0.) 
      Ws = atan(Y_s/X_s);
   else 
   {
      if(Y_s < 0) 
         Ws = - pi/2;
      else if(Y_s > 0)
         Ws = pi/2;
      else 
         Ws = 0;
   }
   
   if(
      abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
      abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
   ) 
   {
      double W_[6];
      W_[0] = Ws-3*pi;
      W_[1] = Ws-2*pi;
      W_[2] = Ws-pi;
      W_[3] = Ws+pi;
      W_[4] = Ws+2*pi;
      W_[5] = Ws+3*pi;
      int i_=0;
      
      while(
         abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
         abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
      ) 
      {
         Ws = W_[i_];
	 i_++;
         
         if(i_ == 6) 
            break;
      }
		 
      if(
         abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
         abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
      ) 
      {
         Ws = -atan(Y_s/X_s);
	 
	 if(
	    abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
	    abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
	 ) 
	 {
	    Ws = asin(Y_s/(Rm*cos(rad*delta)));
	    
	    if(
	       abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
	       abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
	    ) 
	    {
	       W_[0] = (powf(-1., -3))*Ws -3*pi;
	       W_[1] = (powf(-1., -2))*Ws -2*pi;
	       W_[2] = (powf(-1., -1))*Ws -pi;
	       W_[3] = (powf(-1., 1))*Ws +pi;
	       W_[4] = (powf(-1., 2))*Ws +2*pi;
	       W_[3] = (powf(-1., 3))*Ws +3*pi;
	       int j_ = 0;
	       
	       while(
	          abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
	          abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
	       ) 
	       {
	          Ws = W_[j_];
		  j_++;
		
		  if(j_ == 6) 
		     break;
	       }
	       
	       if(
	          abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
	          abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
	       ) 
	       {
	          Ws = acos(X_s/(Rm*cos(rad*delta)));
		  
		  if(
		     abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
		     abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
		  ) 
		  {
		     W_[0] = Ws -4*pi;
		     W_[1] = Ws -2*pi;
		     W_[2] = Ws +2*pi;
		     W_[3] = Ws +4*pi;
		     int k_ = 0;
			
		     while(
		        abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
		        abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
		     ) 
		     {
			 Ws = W_[k_];
			 k_++;
			 
			 if(k_ == 4) 
			   break;
		     }
		     
		     if(
		        abs(Rm*(cos(rad*delta))*cos(Ws) - X_s) >= epsil || 
		        abs(Rm*(cos(rad*delta))*sin(Ws) - Y_s) >= epsil
		     ) 
		        qDebug()<<"angle Ws is still not found!";
		     else 
		        qDebug()<<"Ws is found by arccosinus, i = "<<k_<<".";
		  }
		  else 
		     qDebug()<<"Ws is found by arccosinus.";
	       }
	       else 
	          qDebug()<<"Ws is found by arcsinus, i = "<<j_;
	    }
	    else 
	       qDebug()<<"Ws is found by arcsinus";
	 }
	 else 
	    qDebug()<<"Ws is found by -arctangens";
      }
      else 
         qDebug()<<"Ws is found by arctangens, i = "<<i_<<".";
   }
   else 
      qDebug()<<"Ws is found by arctangens.";
   qDebug()<<"Xs by Ws: "<<Rm*(cos(rad*delta))*cos(Ws)<<", Xs wrighte: "<<X_s;
   qDebug()<<"Ys by Ws: "<<Rm*(cos(rad*delta))*sin(Ws)<<", Ys wrighte: "<<Y_s;
   qDebug()<<"Wr = "<<Wr;
   qDebug()<<"Ws = "<<Ws;
   qDebug()<<"s = "<<s<<", c = "<<c<<", t = "<<t;
   qDebug()<<"By = "<<B_y<<", Cy = "<<C_y<<", Bz = "<<B_z<<", Cz = "<<C_z;
   qDebug()<<"Solar disk coordinates:1).{"<<x_1<<"; "<<y_1<<"; "<<z_1<<"},";
   qDebug()<<"Solar center:             {"<<m<<"; "<<n<<"; "<<k<<"}";
   qDebug()<<"Solar disk coordinates:2).{"<<x_2<<"; "<<y_2<<"; "<<z_2<<"}";
   qDebug()<<"Citi coordinates: {"<<A*c<<"; "<<A*s<<"; "<<A*t<<"}";
}

double Calculator::L(double x, double y, double z, double length)
{
   double el = fabs((Rnorth*c/Reuq)*(x - A*c) + (Rnorth*s/Reuq)*(y - A*s) + (Reuq*t/Rnorth)*(z - A*t))/
               sqrt(
                  (Rnorth*Rnorth/(Reuq*Reuq) + Reuq*Reuq*t*t/(Rnorth*Rnorth))*((x-A*c)*(x-A*c) + 
                  (y-A*s)*(y-A*s) + (z-A*t)*(z-A*t))
               );
   return length*sqrt(1./(el*el) - 1.);
}

double Calculator::L_simple(double &l1, double &l2)
{
   if(l1 >= l2) 
      return l1;
   else 
      return l2;
}

double Calculator::L_total(double &l1, double &l2)
{
   if(l1 >= l2) 
      return l2;
   else 
      return l1;
}

double Calculator::L_half(double &l1, double &l2)
{
   return fabs(l1 - l2);
}

void Calculator::onCalc(
   double &lati, 
   double &longi, 
   double &alti, 
   double &hour, 
   double &minute, 
   double &second, 
   double &day, 
   double &mounth, 
   double &length, 
   bool    leap, 
   int     zone, 
   double  decl
)
{
   init(lati, longi, alti, hour, minute, second, day, mounth, length, leap, zone, decl);
   
   if(abs(lati) != 90.)
   {
      L_1 = L(x_1, y_1, z_1, length);
      L_2 = L(x_2, y_2, z_2, length);
   }
   else 
   {
      if(
         (lati == -90. && delta <= 0.) || 
         (lati == 90. && delta >= 0)
      ) 
      {
         L_1 = length*sqrt(
            (
               (Rm*Rm-2*Rm*Rnorth*(sin(rad*delta))+Rnorth*Rnorth)/
               ((Rm*(sin(rad*delta))-Rnorth)*(Rm*(sin(rad*delta))-Rnorth))
            )-1.
         );
         L_2 = 0;
      }
      else 
      {
         emit Error();
	 QMessageBox::information(
	    this, 
	    "Attention!",
            "At setted time in this region shadow is absent!"
         );
	 return;
      }
   }
   double ls = L_simple(L_1, L_2);
   double lt = L_total(L_1, L_2);
   double lh = L_half(L_1, L_2);
   double Tgrin_r, Tgrin_s,Tr, Ts;
   Tgrin_r = 12-(Wr/(15*rad));
   qDebug()<<"Tgrin_r before transformation: "<<Tgrin_r;
   
   if(Tgrin_r < 0.) 
      Tgrin_r += 24;
   
   if(Tgrin_r >= 24.) 
      Tgrin_r = Tgrin_r-24;
   Tgrin_s = 12-(Ws/(15*rad));
   qDebug()<<"Tgrin_s before transformation: "<<Tgrin_s;

   if(Tgrin_s < 0.) 
      Tgrin_s += 24;
   
   if(Tgrin_s >= 24.) 
      Tgrin_s = Tgrin_s-24;
   Tr = Tgrin_r + zone;
   qDebug()<<"Tr before transformation: "<<Tr;
   
   if(Tr < 0.) 
      Tr += 24;
   
   if(Tr >= 24.) 
      Tr = Tr-24;
   Ts = Tgrin_s + zone;
   qDebug()<<"Ts before transformation: "<<Ts;
   
   if(Ts < 0.) 
      Ts += 24;
   
   if(Ts >= 24.) 
      Ts = Ts-24;
	
   if(Tr > Ts) 
   {
      double rise = Ts;
      Ts = Tr;
      Tr = rise;
      rise = Tgrin_s;
      Tgrin_s = Tgrin_r;
      Tgrin_r = rise;
      rise = Ws;
      Ws = Wr;
      Wr = rise;
   }
   qDebug()<<"Tgrin r = "<<Tgrin_r;
   qDebug()<<"Tgrin s = "<<Tgrin_s;
   qDebug()<<"T r = "<<Tr;
   qDebug()<<"T s = "<<Ts;
   double Th = ((Ts-Tr)/2) + Tr;
   double current_Time = hour + minute/60. + second/3600.;	
   int hh_r, mm_r;
   double ss_r;
   hh_r = int(Tr);
   mm_r = int(60*(Tr-hh_r));
   ss_r = 60*(60*(Tr-hh_r)-mm_r);
   int hh_s, mm_s;
   double ss_s;
   hh_s = int(Ts);
   mm_s = int(60*(Ts-hh_s));
   ss_s = 60*(60*(Ts-hh_s)-mm_s);
   int hh_h, mm_h;
   double ss_h;
   hh_h = int(Th);
   mm_h = int(60*(Th-hh_h));
   ss_h = 60*(60*(Th-hh_h)-mm_h);
   QString ans1, ans2, ans3, ans4,ans5, ans6, ans7;
   
   if(current_Time <= Tr || current_Time >= Ts) 
   {
      //emit Error();
      QMessageBox::information(
         this, 
         "Attention!",
         "At setted time in this region shadow is absent!"
      );
      QTextStream(&ans6)<<"Today rise is at "<<hh_r<<":"<<mm_r<<":"<<ss_r<<", and sunset is at "<<hh_s<<":"<<mm_s<<":"<<ss_s<<".";
      QTextStream(&ans1)<<"Object is "<<length<<" metrs; latitude: "<<lati<<", longitude: "<<longi<<";";
      QTextStream(&ans2)<<"altitude: "<<alti<<", time - "<<hour<<":"<<minute<<":"<<second<<"; Solar declination: "<<delta;
      QTextStream(&ans3)<<"Date: "<<day<<"."<<mounth<<". Year is leap: "<<leap<<". Shadow is absent.";
      QTextStream(&ans4)<<"#############";
      QTextStream(&ans5)<<"#############";
      QTextStream(&ans7)<<"The highest point: "<<hh_h<<":"<<mm_h<<":"<<ss_h<<"; Azimut of shadow is ####";//<<azimut(Ts-Tr,current_Time)<<" grad."; 
   }
   else 
   {
      QTextStream(&ans6)<<"Today rise is at "<<hh_r<<":"<<mm_r<<":"<<ss_r<<", and sunset is at "<<hh_s<<":"<<mm_s<<":"<<ss_s<<".";
      QTextStream(&ans1)<<"Object is "<<length<<" metrs; latitude: "<<lati<<", longitude: "<<longi<<";";
      QTextStream(&ans2)<<"altitude: "<<alti<<", time - "<<hour<<":"<<minute<<":"<<second<<"; Solar declination: "<<delta;
      QTextStream(&ans3)<<"Date: "<<day<<"."<<mounth<<". Year is leap: "<<leap<<". Shadow is "<<ls<<" metrs;";
      QTextStream(&ans4)<<"Total shadow from this: "<<lt<<" metrs,";
      QTextStream(&ans5)<<"half-shadow: "<<lh<<" metrs.";
      QTextStream(&ans7)<<"The highest point: "<<hh_h<<":"<<mm_h<<":"<<ss_h<<"; Azimut of shadow is "<<azimut(longi,zone, current_Time, lati)<<" grad.";
   }
   emit anser(ans1,ans2,ans3,ans4,ans5,ans6, ans7);
}

#ifndef CALCULATING_H
#define CALCULATING_H

#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QLineEdit>
#include <QComboBox>
#include <QString>
#include <QObject>
#include <QtGui>

class Calculator : public QWidget
{
   Q_OBJECT
public:
   Calculator();
   ~Calculator();
   double Delta(double &day, double &mounth, bool leap);
   double omega(double &hour, double &minute, double &second, int zone);
   double L(double x, double y, double z, double length);
   //double L_2(double &lati, double &longi, double &alti, double &hour, double &minute, double &second, double &day, double &mounth, double &length, bool leap, int zone);
   double L_total(double &l1, double &l2);
   double L_simple(double &l1, double &l2);
   double L_half(double &l1, double &l2);
   void init(double &lati, double &longi, double &alti, double &hour, double &minute, double &second, double &day, double &mounth, double &length, bool leap, int zone, double decl);
   double azimut(double phi, int z, double Tc, double teta);//(double Tr, double Ts, double Tc);
public slots:
   void onCalc(double &lati, double &longi, double &alti, double &hour, double &minute, double &second, double &day, double &mounth, double &length, bool leap, int zone, double decl);
   void defElips(double,double);
signals:
   void anser(QString &t1,QString &t2,QString &t3,QString &t4,QString &t5,QString &t6,QString &t7);
   void Error();
public:
   double delta; 
   double A; 
   double W; 
   double s; 
   double c; 
   double m; 
   double n; 
   double t; 
   double k; 
   double r; 
   double u; 
   double B_y; 
   double C_y; 
   double C_z; 
   double B_z; 
   double x_1; 
   double x_2; 
   double y_1; 
   double y_2; 
   double z_1; 
   double z_2; 
   double L_1; 
   double L_2; 
   double A_; 
   double B_; 
   double C_; 
   double D_; 
   double Y_r; 
   double Y_s; 
   double Wr; 
   double Ws; 
   double Reuq; 
   double Rnorth;
};
#endif

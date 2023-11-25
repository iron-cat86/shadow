#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QRadioButton>
#include <QCheckBox>
#include "calculating.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   MainWindow();
   ~MainWindow();
   void createInterface();
   double textToDecimal(QString&);
Q_SIGNALS:
   void Calc(
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
      double decl
   );
   void Error();
   void Elips(double, double);
public slots:
   void onClickCalc();
   void about();
   void onAnser(
      QString &t1,
      QString &t2,
      QString &t3,
      QString &t4,
      QString &t5, 
      QString &t6, 
      QString &t7
   );
   void onError();
   void onDeclin(bool decl);
   void changeElips(const QString &ellips);

public:
   Calculator *calc;
   QPushButton *calcButton;
   QAction *aboutAction;
   QMenu *helpMenu;
   
   QLineEdit *entryLatitude;//широта
   QLineEdit *entryLongitude;//долгота
   QLineEdit *entryAltitude;//высота над ур.моря
   QLineEdit *entryHours;
   QLineEdit *entryMinutes;
   QLineEdit *entrySeconds;
   QLineEdit *entryDay;
   QLineEdit *entryMounth;
   QLineEdit *entryLength;
   
   QCheckBox *entryLeap;
   QCheckBox *declin;
   
   QLineEdit *entryZone;
   QLineEdit *entryDeclin;
   
   QLabel *anserWindow1;
   QLabel *anserWindow2;
   QLabel *anserWindow3;
   QLabel *anserWindow4;
   QLabel *anserWindow5;
   QLabel *anserWindow6;
   QLabel *anserWindow7;
   QLabel *nameLatitude;//широта
   QLabel *nameLongitude;//долгота
   QLabel *nameAltitude;//высота над ур.моря
   QLabel *nameHours;
   QLabel *nameMinutes;
   QLabel *nameSeconds;
   QLabel *nameDay;
   QLabel *nameMounth;
   QLabel *nameLength;
   QLabel *nameZone;
   QLabel *refName;
   
   QComboBox *ref_elips;    
};
#endif

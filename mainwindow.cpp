#include <math.h>
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
#include <QtGui>
#include <QDebug>
#include <QTime>
#include <QDate>
#include "calculating.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
   this->setGeometry(50,50, 900, 400);
   calc = new Calculator;
   setCentralWidget(calc);
   setWindowIcon(QIcon("../images/owl.jpg"));
   aboutAction = new QAction(tr("About"), this);
   aboutAction->setStatusTip(tr("about application"));
   connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
   helpMenu = new QMenu;
   helpMenu = menuBar()->addMenu(tr("&Help"));
   helpMenu->addAction(aboutAction);
   createInterface();
   setAttribute(Qt::WA_DeleteOnClose);
   connect(
      calcButton, SIGNAL(clicked()),
      this,       SLOT(onClickCalc())
   );
   connect(
      this, SIGNAL(Calc(double&, double&, double&, double&, double&, double&, double&, double&, double&, bool, int, double)),
      calc, SLOT(onCalc(double&, double&, double&, double&, double&, double&, double&, double&, double&, bool, int, double))
   );
   connect(
      calc, SIGNAL(anser(QString&,QString&,QString&,QString&,QString&,QString&,QString&)), 
      this, SLOT(onAnser(QString&,QString&,QString&,QString&,QString&,QString&,QString&))
   );
   connect(this, SIGNAL(Error()), this, SLOT(onError()));
   connect(calc, SIGNAL(Error()), this, SLOT(onError()));
   connect(this, SIGNAL(Elips(double,double)), calc, SLOT(defElips(double,double)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Shadow-calculating"),
                       tr("<h2>shadow 1.1,</h2>"
                          "<p>Copyright in 2016 by Anna Belova."
                          "<p>''Shadow-calculating'' is a small application that "
                          "apply you to calculate merages of shadow and half-shadow "
                          "in depending on your latitude, longitude, altitude above "
                          "see lavel and your local time."
                          "<p>====================================================="
                          "<p>Entry all your attributes and click botton ''Calculate''."));
}

void MainWindow::createInterface()
{
   calcButton = new QPushButton(this);
   calcButton->setGeometry(0,50,150,50);
   calcButton->setEnabled(true);
   calcButton->setText("&Calculate");
   calcButton->show();

   declin = new QCheckBox(this);
   declin->setGeometry(20, 130, 200, 30);
   declin->setText("Set declination: ");

   connect(declin, SIGNAL(clicked(bool)), this, SLOT(onDeclin(bool)));
   
   entryDeclin = new QLineEdit(this);
   entryDeclin->setGeometry(230,130,60,30);
   entryDeclin->setEnabled(true);
   entryDeclin->setModified(true);
   entryDeclin->show();

   refName = new QLabel(this);
   refName->setGeometry(430, 70, 110, 30);
   refName->setText("Choose Earth Ellipsoid:");	
   refName->show();

   ref_elips = new QComboBox(this);
   ref_elips->setGeometry(540, 70, 100, 30);
   ref_elips->addItem("Valbek 1819");
   ref_elips->addItem("Bessel 1841");
   ref_elips->addItem("Tenner 1844");
   ref_elips->addItem("Heiford 1910");
   ref_elips->addItem("Krassovskiy 1936");
   ref_elips->addItem("Krassovskiy 1942");
   ref_elips->addItem("PZ-90 1990");
   ref_elips->addItem("GRS80 1980");
   ref_elips->addItem("VGS84 1984");
   ref_elips->addItem("IERS 1996");
   ref_elips->setEnabled(true);
   connect(ref_elips, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(changeElips(const QString&)));
   ref_elips->setCurrentIndex(7);

   entryLeap = new QCheckBox(this);
   entryLeap->setGeometry(450, 50, 100, 30);
   entryLeap->setText("leap-year");
	
   nameLength = new QLabel(this);
   nameLength->setGeometry(160, 50, 200, 50);
   nameLength->setText("Length of the object (metrs): ");
   nameLength->show();

   entryLength = new QLineEdit(this);
   entryLength->setGeometry(330, 50, 50, 50);
   entryLength->setModified(true);
   
   entryLatitude = new QLineEdit(this);
   entryLatitude->setGeometry(150, 160, 50, 30);
   entryLatitude->update();
   entryLatitude->setModified(true);
   entryLatitude->setText("56.73305");
   entryLatitude->show();

   nameLatitude = new QLabel(this);
   nameLatitude->setGeometry(50, 160, 80, 30);
   nameLatitude->setText("Latitude: ");
   nameLatitude->show();
   
   entryLongitude = new QLineEdit(this);
   entryLongitude->setGeometry(150, 210, 50, 30);
   entryLongitude->update();
   entryLongitude->setModified(true);
   entryLongitude->setText("37.01");
   entryLongitude->show();

   nameLongitude = new QLabel(this);
   nameLongitude->setGeometry(50, 210, 80, 30);
   nameLongitude->setText("Longitude: ");
   nameLongitude->show();
   
   entryAltitude = new QLineEdit(this);
   entryAltitude->setGeometry(150, 260, 50, 30);
   entryAltitude->update();
   entryAltitude->setModified(true);
   entryAltitude->setText("126.00");
   entryAltitude->show();

   nameAltitude = new QLabel(this);
   nameAltitude->setGeometry(0, 260, 110, 30);
   nameAltitude->setText("Altitude above sea lavel: ");
   nameAltitude->show();

   QTime time;
   QDate date;
   QString hh, mm, ss, dd, mo;
   QTextStream(&hh)<<time.currentTime().hour();
   QTextStream(&mm)<<time.currentTime().minute();
   QTextStream(&ss)<<time.currentTime().second();
   QTextStream(&dd)<<date.currentDate().day();
   QTextStream(&mo)<<date.currentDate().month();
   
   entryHours = new QLineEdit(this);
   entryHours->setGeometry(150, 310, 30, 30);
   entryHours->update();
   entryHours->setModified(true);
   entryHours->setText(hh);
   entryHours->show();

   nameZone = new QLabel(this);
   nameZone->setGeometry(10, 310, 50, 30);
   nameZone->setText("Time zone: ");
   nameZone->show();

   entryZone = new QLineEdit(this);
   entryZone->setGeometry(70, 310, 40, 30);
   entryZone->update();
   entryZone->setText("3");
   entryZone->setModified(true);
   entryZone->show();

   nameHours = new QLabel(this);
   nameHours->setGeometry(120, 310, 30, 30);
   nameHours->setText("hh: ");
   nameHours->show();

   entryMinutes = new QLineEdit(this);
   entryMinutes->setGeometry(220, 310, 30, 30);
   entryMinutes->update();
   entryMinutes->setModified(true);
   entryMinutes->setText(mm);
   entryMinutes->show();

   nameMinutes = new QLabel(this);
   nameMinutes->setGeometry(190, 310, 30, 30);
   nameMinutes->setText("mm: ");
   nameMinutes->show();

   entrySeconds = new QLineEdit(this);
   entrySeconds->setGeometry(290, 310, 30, 30);
   entrySeconds->update();
   entrySeconds->setModified(true);
   entrySeconds->setText(ss);
   entrySeconds->show();

   nameSeconds = new QLabel(this);
   nameSeconds->setGeometry(260, 310, 30, 30);
   nameSeconds->setText("ss: ");
   nameSeconds->show();

   entryDay = new QLineEdit(this);
   entryDay->setGeometry(150, 360, 30, 30);
   entryDay->update();
   entryDay->setModified(true);
   entryDay->setText(dd);
   entryDay->show();

   nameDay = new QLabel(this);
   nameDay->setGeometry(110, 360, 40, 30);
   nameDay->setText("Day: ");
   nameDay->show();

   entryMounth = new QLineEdit(this);
   entryMounth->setGeometry(240, 360, 100, 30);
   entryMounth->update();
   entryMounth->setModified(true);
   entryMounth->setText(mo);
   entryMounth->show();

   nameMounth = new QLabel(this);
   nameMounth->setGeometry(190, 360, 100, 30);
   nameMounth->setText("Mounth: ");
   nameMounth->show();

   anserWindow1 = new QLabel(this);
   anserWindow1->setGeometry(350, 160, 500, 30);
   anserWindow1->setUpdatesEnabled(true);
   anserWindow1->setFrameStyle(2);
   anserWindow1->show();

   anserWindow2 = new QLabel(this);
   anserWindow2->setGeometry(350, 200, 500, 30);
   anserWindow2->setUpdatesEnabled(true);
   anserWindow2->setFrameStyle(2);
   anserWindow2->show();

   anserWindow3 = new QLabel(this);
   anserWindow3->setGeometry(350, 240, 500, 30);
   anserWindow3->setUpdatesEnabled(true);
   anserWindow3->setFrameStyle(2);
   anserWindow3->show();

   anserWindow4 = new QLabel(this);
   anserWindow4->setGeometry(350, 280, 500, 30);
   anserWindow4->setUpdatesEnabled(true);
   anserWindow4->setFrameStyle(2);
   anserWindow4->show();

   anserWindow5 = new QLabel(this);
   anserWindow5->setGeometry(350, 320, 500, 30);
   anserWindow5->setFrameStyle(2);
   anserWindow5->setUpdatesEnabled(true);
   anserWindow5->show();

   anserWindow6 = new QLabel(this);
   anserWindow6->setGeometry(350, 120, 500, 30);
   anserWindow6->setUpdatesEnabled(true);
   anserWindow6->setFrameStyle(2);
   anserWindow6->show();
   
   anserWindow7 = new QLabel(this);
   anserWindow7->setGeometry(350, 360, 500, 30);
   anserWindow7->setFrameStyle(2);
   anserWindow7->setUpdatesEnabled(true);
   anserWindow7->show();
}

void MainWindow::changeElips(const QString &ellips)
{
   if(ellips == "Valbek 1819") 
   {
      QMessageBox::information(this, "Reference ellipsoid, 1819, Finland, Russion Imperia",
			              "Reuq = 6 375 653 m, Rnorth =	6 356 564,219; f = 0,002994012, e = 0,077324381, e' = 0,077556586"
      );
      emit Elips(6375653.,6356564.219);
   }
   else if(ellips == "Bessel 1841") 
   {
      QMessageBox::information(this, "Reference ellipsoid, 1841, Germany, Russia",
                                     "Reuq = 6 377 397,155 m, Rnorth = 6 356 078,963 m; f = 0,003342773, e = 0,081696831, e' = 0,081970841"
      );
      emit Elips(6377397.155, 6356078.963);
   }
   else if(ellips == "Tenner 1844") 
   {
      QMessageBox::information(this, "Reference ellipsoid, 1844,Russia",
                                     "Reuq = 6 377 096 m, Rnorth = 6 356 014,691 m; f = 0,003305785, e = 0,081244335, e' =	0,081513803"
      );
      emit Elips(6377096.,6356014.691);
   }
   else if(ellips == "Heiford 1910") 
   {
      QMessageBox::information(this, "Reference ellipsoid, 1910, Europe, Asia, South America, Antarctica",
                                     "Reuq = 6 378 388 m, Rnorth = 6 356 911,946 m; f = 0,003367003, e = 0,08199189, e' = 0,08226889"
      );
      emit Elips(6378388., 6356911.946);
   }
   else if(ellips == "Krassovskiy 1936") 
   {
      QMessageBox::information(this, "Reference ellipsoid, 1936, USSR",
                                     "Reuq = 6 378 210 m, Rnorth = 6 356 849,618 m; f = 0,003348962, e = 0,081772294, e' = 0,082047067"
      );
      emit Elips(6378210.,6356849.618);
   }
   else if(ellips == "Krassovskiy 1942") 
   {
      QMessageBox::information(this, "Reference ellipsoid, 1942, USSR, Russia, CIS, East Europe, Antarctica",
                                     "Reuq = 6 378 245 m, Rnorth = 6 356 863,019 m; f = 0,00335233, e = 0,081813334, e' = 0,082088522"
      );
      emit Elips(6378245, 6356863.019);
   }
   else if(ellips == "PZ-90 1990") 
   {
      QMessageBox::information(this, "General Earth ellipsoid, 1990, Russia",
                                     "Reuq = 6 378 136 m, Rnorth = 6 356 751,373 m; f = 0,003352802, e = 0,081819085, e' = 0,082094331"
      );
      emit Elips(6378136., 6356751.373);
   }
   else if(ellips == "GRS80 1980") 
   {
      QMessageBox::information(this, "General Earth ellipsoid, 1980, IUGG",
                                     "Reuq = 6 378 137 m, Rnorth = 6 356 752,314 m; ellips zoom percent for semiaxises: ±2, f = 0,003352811,", 
                                     "accuracy of Earth ellipticity: ±0,001; e = 0,081819191, e' = 0,082094438. Ellipsoid was developed by ",
                                     "Geographic and Geodesic Union, and is recomended for geodesic searching."
      );
      emit Elips(6378137., 6356752.314);
   }
   else if(ellips == "WGS84 1984") 
   {
      QMessageBox::information(this, "General Earth, 1984, USA",
                                     "Reuq = 6 378 137 m, Rnorth = 6 356 752,314 m; ellips zoom percent for semiaxises: ±2, f = 0,003352811,",
                                     " accuracy of Earth ellipticity: ±0,001, e = 0,081819191, e' = 0,082094438. Ellipsoid is used in system ",
                                     "of satellite navigation GPS."
      );
      emit Elips(6378137., 6356752.314);
   }
   else if(ellips == "IERS 1996") 
   {
      QMessageBox::information(this, "General Earth ellipsoid, 1996, IERS",
                                     "Reuq = 6 378 136, 49 m, Rnorth = 6 356 751,75 m; f = 0,003352819, e = 0,081819297, e' = 0,082094545. ",
                                     "Ellipsoid is recomended by International Polar Motion Service for observation treatment."
      );
      emit Elips(6378136.49, 6356751.75);
   }
}

void MainWindow::onDeclin(bool decl)
{
   if(decl ==  true) 
   {
      entryLeap->setEnabled(false);
      entryMounth->setEnabled(false);
      entryDay->setEnabled(false);
      entryDeclin->setEnabled(true);
   }
   else 
   {
      entryLeap->setEnabled(true);
      entryMounth->setEnabled(true);
      entryDay->setEnabled(true);
      entryDeclin->setEnabled(false);
   }
}

double MainWindow::textToDecimal(QString &t)
{
   double decimal = 0;
   bool decimalPoint = false;
   int pastdecimalPoint = 0;
   int start;
   if(t.at(0) == '+' || t.at(0) == '-') 
      start = 1;
   else if(
      t.at(0) =='.' || 
      t.at(0) == ','|| 
      t.at(0) =='0' || 
      t.at(0) == '1'|| 
      t.at(0) == '2'|| 
      t.at(0) == '3'|| 
      t.at(0) == '4'|| 
      t.at(0) == '5'|| 
      t.at(0) == '6'|| 
      t.at(0) == '7'|| 
      t.at(0) == '8'|| 
      t.at(0) == '9'
   )
      start = 0;
   else 
   {
      emit Error();
      return 0;
   }
   int j = start;
   
   while(t.at(j) != '.' && t.at(j) != ',' && j < t.count()) 
      j++;
   if (t.at(j) == ',' || t.at(j) == '.') 
   {
      decimalPoint = true;
      pastdecimalPoint++;
   }

   for(int i = start; i < j; i++) 
   {
      if(t.at(i) == '0') 
         decimal+= 0*(powf(10,j-1-i));
      else if(t.at(i) == '1') 
         decimal+= 1*(powf(10,j-1-i));
      else if(t.at(i) == '2') 
         decimal+= 2*(powf(10,j-1-i));
      else if(t.at(i) == '3') 
         decimal+= 3*(powf(10,j-1-i));
      else if(t.at(i) == '4') 
         decimal+= 4*(powf(10,j-1-i));
      else if(t.at(i) == '5') 
         decimal+= 5*(powf(10,j-1-i));
      else if(t.at(i) == '6') 
         decimal+= 6*(powf(10,j-1-i));
      else if(t.at(i) == '7') 
         decimal+= 7*(powf(10,j-1-i));
      else if(t.at(i) == '8') 
         decimal+= 8*(powf(10,j-1-i));
      else if(t.at(i) == '9') 
         decimal+= 9*(powf(10,j-1-i));
      else 
      {
         emit Error();
	 return 0;
      }
      //qDebug()<<"t.at("<<i<<") = "<<t.at(i)<<";   10^"<<i<<" = "<<powf(10,i)<<", lati = "<<decimal<<", t-count: "<<t.count();
   }

   if(decimalPoint == true) 
   {
      for(int i = j+1; i < t.count(); i++) 
      {
	  if(t.at(i) == '0') 
	  {
	     decimal+= 0*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '1') 
	  {
	     decimal+= 1*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '2') 
	  {
	     decimal+= 2*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '3') 
	  {
	     decimal+= 3*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '4') 
	  {
	     decimal+= 4*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '5') 
	  {
	     decimal+= 5*(powf(10,-pastdecimalPoint));				
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '6') 
	  {
	     decimal+= 6*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '7') 
	  {
	     decimal+= 7*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '8') 
	  {
	     decimal+= 8*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else if(t.at(i) == '9') 
	  {
	     decimal+= 9*(powf(10,-pastdecimalPoint));
	     pastdecimalPoint++;
	  }
	  else 
	  {
	     emit Error();
	     return 0;
	  }
	  //qDebug()<<"t.at("<<i<<") = "<<t.at(i)<<", lati = "<<decimal<<", t-count: "<<t.count()<<"simbol past point: "<<pastdecimalPoint<<", otr: "<<powf(10,-pastdecimalPoint);
      }
   }
   
   if(t.at(0) == '-') 
      decimal*=(-1.);
   return decimal;
}

void MainWindow::onClickCalc()
{
   QString lat=entryLatitude->text();
   double lati = textToDecimal(lat);
   
   if(lati < -90 || lati > 90) 
   {
      emit Error();
      return;
   }
   QString longit=entryLongitude->text();
   double longi = textToDecimal(longit);
	
   if(longi < -180 || longi > 180) 
   {
      emit Error();
      return;
   }
   QString alt=entryAltitude->text();
   double alti = textToDecimal(alt);
   QString hou=entryHours->text();
   double hour = textToDecimal(hou);
   
   if(hour < 0 || hour >= 24) 
   {
      emit Error();
      return;
   }
   QString minu=entryMinutes->text();
   double minute = textToDecimal(minu);
   
   if(minute < 0 || minute >= 60) 
   {
      emit Error();
      return;
   }
   QString seco=entrySeconds->text();
   double second = textToDecimal(seco);
	
   if(second < 0 || second >= 60) 
   {
      emit Error();
      return;
   }
   QString len=entryLength->text();
   double length = textToDecimal(len);
   QString zon=entryZone->text();
   int zone = int(textToDecimal(zon));
   
   if(zone < -12 || zone > 12) 
   {
      emit Error();
      return;
   }

   if(!declin->isChecked()) 
   {
      QString da=entryDay->text();
      double day = textToDecimal(da);
      
      if(day <= 0 || day > 31) 
      {
         emit Error();
	 return;
      }
      double mounth;
		
      if(
         entryMounth->text() == "january" || 
         entryMounth->text() == "January" || 
         entryMounth->text() == "1"       || 
         entryMounth->text() == "01"      || 
         entryMounth->text() == "JANUARY"
      ) 
         mounth = 1;		 
      else if(
         entryMounth->text() == "february" || 
         entryMounth->text() == "February" || 
         entryMounth->text() == "2"        || 
         entryMounth->text() == "02"       || 
         entryMounth->text() == "FEBRUARY"
      ) 
      {
         mounth = 2;
	 
	 if(day > 29) 
	 {
	    emit Error();
	    return;
	 }
      }
      else if(
         entryMounth->text() == "march" || 
         entryMounth->text() == "March" || 
         entryMounth->text() == "3"     || 
         entryMounth->text() == "03"    || 
         entryMounth->text() == "MARCH"
      ) 
         mounth = 3;
      else if(
         entryMounth->text() == "april" || 
         entryMounth->text() == "April" || 
         entryMounth->text() == "4"     || 
         entryMounth->text() == "04"    || 
         entryMounth->text() == "APRIL"
      ) 
      {
         mounth = 4;
	 
	 if(day > 30) 
	 {
	    emit Error();
	    return;
         }
      }
      else if(
         entryMounth->text() == "may" || 
         entryMounth->text() == "May" || 
         entryMounth->text() == "5"   || 
         entryMounth->text() == "05"  || 
         entryMounth->text() == "MAY"
      ) 
         mounth = 5;
      else if(
         entryMounth->text() == "june" || 
         entryMounth->text() == "June" || 
         entryMounth->text() == "6"    || 
         entryMounth->text() == "06"   || 
         entryMounth->text() == "JUNE"
      ) 
      {
         mounth = 6;
	 
	 if(day > 30) 
	 {
	    emit Error();
	    return;
	 }
      }
      else if(
         entryMounth->text() == "july" || 
         entryMounth->text() == "July" || 
         entryMounth->text() == "7"    || 
         entryMounth->text() == "07"   || 
         entryMounth->text() == "JULY"
      ) 
         mounth = 7;
      else if(
         entryMounth->text() == "august" || 
         entryMounth->text() == "August" || 
         entryMounth->text() == "8"      || 
         entryMounth->text() == "08"     || 
         entryMounth->text() == "AUGUST"
      ) 
         mounth = 8;
      else if(
         entryMounth->text() == "september" || 
         entryMounth->text() == "September" || 
         entryMounth->text() == "9"         || 
         entryMounth->text() == "09"        || 
         entryMounth->text() == "SEPTEMBER"
      ) 
      {
         mounth = 9;
	 
	 if(day > 30) 
	 {
	    emit Error();
           return;
         }
      }
      else if(
         entryMounth->text() == "october" || 
         entryMounth->text() == "October" || 
         entryMounth->text() == "10"      || 
         entryMounth->text() == "OCTOBER"
      ) 
         mounth = 10;
      else if(
         entryMounth->text() == "november" || 
         entryMounth->text() == "November" || 
         entryMounth->text() == "11"       || 
         entryMounth->text() == "NOVEMBER"
      ) 
      {
         mounth = 11;
	 
	 if(day > 30) 
	 {
	    emit Error();
	    return;
	 }
      }
      else if(
         entryMounth->text() == "december" || 
         entryMounth->text() == "December" || 
         entryMounth->text() == "12"       || 
         entryMounth->text() == "DECEMBER"
      ) 
         mounth = 12;
      else 
      {
         emit Error();
	 return;
      }
      bool leap;
      
      if(entryLeap->isChecked()) 
         leap = true;
      else 
         leap = false;
      emit Calc(lati, longi, alti, hour, minute, second, day, mounth, length, leap, zone, 0);
   }
   else 
   {
      double day = 0.;
      double mounth = 0.;
      QString dec=entryDeclin->text();
      double declination = textToDecimal(dec);
      
      if(declination < -23.45 || declination > 23.45) 
      {
	 emit Error();
         return;
      }
      else 
         emit Calc(lati, longi, alti, hour, minute, second, day, mounth, length, false, zone, declination);
   }
}

void MainWindow::onError() 
{
   QMessageBox::information(this, "Error!",
                                  "Some fields are empty or invalid entryes. Please, entry all correctly!"
   );	
}

void MainWindow::onAnser(QString &t1,QString &t2,QString &t3,QString &t4,QString &t5,QString &t6, QString &t7)
{
   anserWindow1->clear();
   anserWindow1->setText(t1);
   anserWindow1->update();

   anserWindow2->clear();
   anserWindow2->setText(t2);
   anserWindow2->update();

   anserWindow3->clear();
   anserWindow3->setText(t3);
   anserWindow3->update();

   anserWindow4->clear();
   anserWindow4->setText(t4);
   anserWindow4->update();

   anserWindow5->clear();
   anserWindow5->setText(t5);
   anserWindow5->update();

   anserWindow6->clear();
   anserWindow6->setText(t6);
   anserWindow6->update();

   anserWindow7->clear();
   anserWindow7->setText(t7);
   anserWindow7->update();
}

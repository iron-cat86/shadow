#include <stdio.h>
#include <cstdio>
#include <cassert>
#include <QSplashScreen>
#include <QObject>
#include <QtGui>
#include <QPixmap>
#include <QApplication>
#include <QTime>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication *app= new QApplication(argc, argv);
    QSplashScreen *splash = new QSplashScreen(QPixmap("../images/zastava.jpg"));
    QTime *ti=new QTime;
    MainWindow *win=new MainWindow();
    win->show();
    splash->show();
    ti->start();
    while(ti->elapsed() <= 3000);
    delete ti;
    delete splash;
    return app->exec();
}

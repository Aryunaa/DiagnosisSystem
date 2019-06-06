#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QtGui>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   w.show();
   QString aryuna;
   w.setWindowIcon(QIcon("/home/aryuna/Downloads/Fokin/cute_ico/sgmu.png"));
//QLabel *label = new QLabel(ar); это мне ненужно! строчка 11 и 13 нужны!

//label->show();



    return a.exec();
}

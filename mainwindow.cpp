#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QApplication>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <QStandardItemModel>
#include <QTextStream>
#include <QStringList>
#include <QTableWidgetItem>
#include <QCoreApplication>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//global variabels
QString aryuna; //глобальная переменная
QString ar;
QStandardItemModel k;
QList<float > floatss;
int str_kol;
int stolb_kol;


float ** matrix;//данные с диагнозом
int N0,N1;//количество в каждой выборке из двух
float ** matr0; //матрицы каждого признака
float ** matr1;
float * centr0;//центры классов
float * centr1;
double * disp0;//дисперсии
double * disp1;

float d0,d1;//внутримножественные расстояния
float d12;//межклассовое расстояние

float * w;//веса для ЛДФ
QString * priznaki;//набор признаков для вывода;

float * sample;//то, что надо распознать

float *percw;//перцептрона весы
float **dataPerc;//видоизмененные данные с 1

int c;//данные для шага обучения
float dperc;
int k_cycle;//количество циклов обучения



//QVector2D <float> myMatr;


void MainWindow::on_action_triggered()
{
    //организация считывания, открытия и отображения
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Text Files (*.csv);;C++ Files (*.cpp *.h);;Text Files (*.txt)"));

       if (fileName != "")
       {
           QFile file(fileName);
           if (!file.open(QIODevice::ReadOnly))
             {
                 QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
                 return;
             }

           QTextStream in(&file);

          /*
           * не включать
           *  QTextEdit textEdit;
           textEdit.setText(in.readAll());
           textEdit.show();
           QFile file_out("file_out_perceptron.txt");
           не включать
          */

           /* не включать
             QString str;
             if (file_out.open(QIODevice::WriteOnly))
           {
               QTextStream stream(&file_out);
               str="this is a test";
               stream << str.toUpper(); //Запишет-THIS IS A TEST теперь должно записать SOMETHING

               file_out.close();
               if (stream.status() != QTextStream::Ok)
               {
                   qDebug() << "Ошибка записи файла";
               }
           }
           не включать
          */





          csvModel = new QStandardItemModel();

          // Создаём поток для извлечения данных из файла
          QTextStream intest(&file);


          // Считываем данные до конца файла
          QString metaline=intest.readLine();
          aryuna=metaline;
          // csvModel->setColumnCount(6);  //возвращайся сюда!!!
          //  QStringList wordList;
          //  wordList = file.readAll().split(QRegExp("[\r\n]"),QString::SkipEmptyParts); //reading file and splitting it by lines


          csvModel->setHorizontalHeaderLabels(QStringList()<<metaline.split(','));
          ui->tableView->setModel(csvModel); // Устанавливаем модель в таблицу


          // Открываем файл из ресурсов. Вместо данного файла
          // необходимо указывать путь к вашему требуемому файлу
          //QFile file("/home/1.csv");
          // if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
          //     qDebug() << "File not exists";
         //} else {


                   while (!intest.atEnd())
                     {
                          // ... построчно
                          QString line = intest.readLine();

                          // Добавляем в модель по строке с элементами
                          QList<QStandardItem *> standardItemsList;
                          // учитываем, что строка разделяется запятой на колонки
                          for (QString item : line.split(","))
                             {
                                 standardItemsList.append(new QStandardItem(item));
                                 floatss.append(item.toFloat());
                                 qDebug()<<floatss;

                                 //myMatr.append()

                                 // standardItemsList.append(new QStandardItem(floats[0]));

                             }


                          QVector <float> myVect;


                          //QVector2D <float> mine;

                          //int massive[4] = {1, 2, 3, 4};
                          //ar= new **int;


                          for(int i = 0; i < floatss.length(); i++)
                            {
                                myVect.append(floatss[i]);

                                qDebug() << "Value " << i << ": " << myVect.value(i)<<" "<<floatss[i];
                            }


                          //myVector.destruct();
                          csvModel->insertRow(csvModel->rowCount(), standardItemsList);
                          qDebug()<<csvModel->columnCount();//stolbtsi priznaki
                          qDebug()<<csvModel->rowCount();//stroki ,sample
                          int a = csvModel->columnCount();
                          int t ;t= csvModel->rowCount();
                          int b; b=t;

                          float ** mas = new float*[b];
                          for (int i = 0; i < b; i++)
                            {
                                 mas[i] = new float[a];
                            }

                          for (int i = 0; i < b; i++)
                            {
                                for (int j=0;j<a;j++)
                                    {
                                        mas[i][j] = floatss[i*a+j];
                                        qDebug()<<mas[i][j];
                                     }

                                qDebug()<<"end of sample";
                            }

                          //working with global matrix
                          str_kol=b;
                          stolb_kol=a;
                          matrix = new float*[str_kol];
                          for (int i = 0; i < str_kol; i++)
                            {
                                matrix[i] = new float[stolb_kol];
                            }

                          for (int i = 0; i < str_kol; i++)
                            {
                                for (int j=0;j<stolb_kol;j++)
                                    {
                                        matrix[i][j] = floatss[i*stolb_kol+j];
                                        qDebug()<<matrix[i][j];

                                    }
                            }


                          //deleting of massives
                          for (int i = 0; i < b; i++)
                            {
                                delete [] mas[i];
                            }
                          delete [] mas;

                          //очищать память в этом месте было плохой идеей! больше этого не делать!!!
                          //qDeleteAll(floatss.begin(),floatss.end());
                          //floatss.clear();
                          //qDeleteAll(standardItemsList.begin(),standardItemsList.end());
                          //standardItemsList.clear();




                          //file.close();
                    }
        //хочу сделать манипуляцию с данными и индексацию, чтобы потом работать с массивами и матрицами ---->done
        //ar=(csvModel->index(2,3));
        qDebug()<<"end of intest";
        in.atEnd();
        intest.atEnd();


           file.close();
           qDebug()<<"file is closed";
       }
       else
       {qDebug()<<"no file!";}

       qDebug()<<"end of action_triggered";

       ui->action_12->setEnabled(true);

       ui->action_4->setEnabled(true);

       ui->pushButton->setEnabled(true);
       ui->pushButton_2->setEnabled(true);
       ui->tableWidget->setEnabled(true);





}




void MainWindow::on_action_12_triggered()//calculation
{
    qDebug()<<"knopka nazhata";

    //количество в выборках N0,N1

    for (int i = 0; i < str_kol; i++)
    {
        if(matrix[i][stolb_kol-1]==0)
            {N0+=1;

            }
        else
            {N1+=1;

            }

    }
    qDebug()<<"kol-vo viborka: "<<"N0 "<<N0<<",N1 "<<N1; //количество в выборке 1 и 0 группах на последнем столбце


    //matr0
    matr0=new float*[N0];

    for (int i=0;i<N0;i++)
        {matr0[i]=new float[stolb_kol-1];}
    int k=0;
    qDebug()<<"matr0: ";
    for (int i=0;i<str_kol;i++)
        {if ((matrix[i][stolb_kol-1]==0) && (k<N0))
            {
            for(int j=0;j<(stolb_kol-1);j++)
                {matr0[k][j]=matrix[i][j];qDebug()<<matr0[k][j];}
            k++;qDebug()<<k;
            }
        }

    for(int i=0;i<k;i++)
        {for(int j=0;j<(stolb_kol-1);j++)
            {qDebug()<<matr0[i][j];}

        }


     //matr1
     matr1=new float*[N1];
     for (int i=0;i<N1;i++)
         {matr1[i]=new float[stolb_kol-1];}
     int u=0;
     qDebug()<<"matr1: ";
     for (int i=0;i<str_kol;i++)
         {if ((matrix[i][stolb_kol-1]==1) && (u<N1))
             {
             for(int j=0;j<(stolb_kol-1);j++)
                 {matr1[u][j]=matrix[i][j];qDebug()<<matr1[u][j];}
             u++;qDebug()<<u;
             }
         }

     for(int i=0;i<u;i++)
         {for(int j=0;j<(stolb_kol-1);j++)
             {qDebug()<<matr1[i][j];}

         }

    //центр класса, рассчеты
    float* Scentr0;float* Scentr1;
    Scentr0=new float[stolb_kol-1];
    Scentr1=new float[stolb_kol-1];

    for(int j=0;j<(stolb_kol-1);j++)
        {Scentr0[j]=0;Scentr1[j]=0;}

    for (int j=0;j<(stolb_kol-1);j++)
        {for (int i=0;i<N0;i++)
            {if(i<N0)
                {Scentr0[j]+=matr0[i][j];qDebug()<<Scentr0[j];}
            }

        }
    qDebug()<<"----";
    for (int j=0;j<(stolb_kol-1);j++)
        {for (int i=0;i<N1;i++)
            {if(i<N1)
                {Scentr1[j]+=matr1[i][j];qDebug()<<Scentr1[j];}
            }

        }

    centr0=new float [stolb_kol-1];
    centr1=new float [stolb_kol-1];
    for (int j=0;j<(stolb_kol-1);j++)
        {qDebug()<<"0 "<<Scentr0[j]<<","<<"1 "<<Scentr1[j];

        centr0[j]=Scentr0[j]/N0;
        centr1[j]=Scentr1[j]/N1;
        qDebug()<<j<<"|0 "<<centr0[j]<<","<<"1 "<<centr1[j];


        }


    //дисперсии признаков
    int feature_num=stolb_kol-1;
    disp0=new double[feature_num];
    disp1=new double[feature_num];
    for(int j=0;j<feature_num;j++)
        {for(int i=0;i<N0;i++)
            {disp0[j]+=(matr0[i][j]-centr0[j])*(matr0[i][j]-centr0[j]);

            }
        if (N0>1)
            {disp0[j]/=(N0-1);}
        qDebug()<<j<<" "<<disp0[j];
        }
    qDebug()<<"----";
    for(int j=0;j<feature_num;j++)
        {for(int i=0;i<N1;i++)
            {disp1[j]+=(matr1[i][j]-centr1[j])*(matr1[i][j]-centr1[j]);

            }
        if (N1>1)
            {disp1[j]/=(N1-1);}
        qDebug()<<j<<" "<<disp1[j];
        }


       //qDebug()<<"wtf "<<int(disp0[0]);


    //внутримножественные расстояния
    d0=0;d1=0;
    for (int j=0;j<feature_num;j++)
        {d0+=disp0[j];
         d1+=disp1[j];

        }
    d0*=2;d1*=2;
    qDebug()<<"vnuri rassoyznie "<<d0<<" "<<d1;

    //расстояние между классами
    float temp_d12=0;
    for(int j=0;j<feature_num;j++)
        {temp_d12+=(centr0[j]-centr1[j])*(centr0[j]-centr1[j]);

        }
    d12=sqrt(temp_d12);
    qDebug()<<"raast mezh classami "<<d12;



   //вывод рассчетов (внутримножественное расстояние, межклассоввое расстояние)
   QString kol_vo0,kol_vo1,vnutr_rasst0,vnutr_rasst1,mezh_rasst;
   kol_vo0="Количество в группе с исходом '0' : ";
   QString strN0 = QString::number(N0);
   kol_vo0.append(strN0);
   kol_vo1="Количество в группе с исходом '1' : ";
   QString strN1=QString::number(N1);
   kol_vo1.append(strN1);

   vnutr_rasst0="Внутримножественные расстояния в группе '0' : ";
   QString vn_r0=QString::number(d0);
   QString vn_r1=QString::number(d1);
   vnutr_rasst0.append(vn_r0);
   vnutr_rasst0.append(" ");

   vnutr_rasst1="Внутримножественные расстояния в группе '1' : ";
   vnutr_rasst1.append(vn_r1);

   mezh_rasst="Межклассовое расстояние : ";
   QString mzh_r=QString::number(d12);
   mezh_rasst.append(mzh_r);


   ui->txt->setText("Проведенные расчеты: ");
   ui->txt->append(kol_vo0);
   ui->txt->append(kol_vo1);
   ui->txt->append(vnutr_rasst0);
   ui->txt->append(vnutr_rasst1);
   ui->txt->append(mezh_rasst);



   //vivod v tablitsu
   QStandardItemModel *rs_model = new QStandardItemModel();
   rs_model = new QStandardItemModel();
   rs_model->setHorizontalHeaderLabels(QStringList()<<aryuna.split(','));
   ui->tableView_2->setModel(rs_model);

        //centr0
   QList<QStandardItem *> centri0_into_table;
   for (int i=0;i<feature_num;i++)
        {

         QString str = QString::number(centr0[i]);
         centri0_into_table.append(new QStandardItem(str));
        }
   centri0_into_table.append(new QStandardItem("0"));
   rs_model->insertRow(rs_model->rowCount(), centri0_into_table);
        //centr1
   QList<QStandardItem *> centri1_into_table;
   for (int i=0;i<feature_num;i++)
        {

         QString str = QString::number(centr1[i]);
         centri1_into_table.append(new QStandardItem(str));
        }
   centri1_into_table.append(new QStandardItem("1"));
   rs_model->insertRow(rs_model->rowCount(), centri1_into_table);
        //disp0
   QList<QStandardItem *> disp0_into_table;
   for (int i=0;i<feature_num;i++)
        {

         QString str = QString::number(disp0[i]);
         disp0_into_table.append(new QStandardItem(str));
        }
   disp0_into_table.append(new QStandardItem("0"));
   rs_model->insertRow(rs_model->rowCount(), disp0_into_table);

   //disp0
    QList<QStandardItem *> disp1_into_table;
    for (int i=0;i<feature_num;i++)
        {

            QString str = QString::number(disp1[i]);
            disp1_into_table.append(new QStandardItem(str));
        }
    disp1_into_table.append(new QStandardItem("1"));
    rs_model->insertRow(rs_model->rowCount(), disp1_into_table);

    //deleting было плохой идеей!
    //delete  rs_model;



    /*удалять это было плохой идеей!
    qDeleteAll(centri0_into_table.begin(),centri0_into_table.end());*/
    //centri0_into_table.clear();
    /*qDeleteAll(centri1_into_table.begin(),centri1_into_table.end());
    centri1_into_table.clear();
    qDeleteAll(disp0_into_table.begin(),disp0_into_table.end());
    disp0_into_table.clear();
    qDeleteAll(disp1_into_table.begin(),disp1_into_table.end());
    disp1_into_table.clear();
    */

   ui->action_10->setEnabled(true);//оценка информативности признаков
   ui->action_3->setEnabled(true);//обучение лдф

}

void MainWindow::on_action_12_changed()//something
{ qDebug()<<"changed";

}

void MainWindow::on_action_12_destroyed()
{
    qDebug()<<"distruction happened";
}

void MainWindow::on_action_5_triggered()//author
{
    qDebug()<<"Ariuna Aiusheeva, Siberian State Medical University";
    QTextEdit *txt_au = new QTextEdit();
     txt_au->setText("Программу сделал студент 4 курса СибГМУ ");
     txt_au->append("Аюшеева Арюна, группа 4508, специальность медицинская кибернетика");
     txt_au->append(" ");
     txt_au->append("______________________________________");
     txt_au->append("mail: aryuna.ayusheeva.1998@mail.ru ");
     txt_au->append("tel: +79138700781 ");


    txt_au->show();


}

void MainWindow::on_pushButton_clicked()//вывод ячейки с признаками
{   QStringList prizn;
    prizn=aryuna.split(',',QString::SkipEmptyParts);
    qDebug()<<prizn;
    ui->tableWidget->setColumnCount(stolb_kol-1);
    ui->tableWidget->setRowCount(1);
    QStringList horzHeaders;
    for (int i=0;(i<stolb_kol-1);i++)
        {
        horzHeaders << prizn[i];

        }
    ui->tableWidget->setHorizontalHeaderLabels(horzHeaders);

    //deleting
    prizn.clear();



}

void MainWindow::on_pushButton_2_clicked()//сохранение сэмпла
{      //tableWidget.show();
    QTableWidgetItem *itab = ui->tableWidget->item(0,0);
    QString itabtext = itab->text();


    //better to do:
    //QString itabtext = tableWidget->item(0,0)->text();
    qDebug()<<itabtext;

    sample=new float[stolb_kol-1];
    QString * sample_text;
    sample_text=new QString[stolb_kol-1];
    QTableWidgetItem **tabl=new QTableWidgetItem*[stolb_kol-1];

    for(int i =0;i<(stolb_kol-1);i++)
        {
         tabl[i]=ui->tableWidget->item(0,i);
         sample_text[i]=tabl[i]->text();
         sample[i]=sample_text[i].toFloat();
         qDebug()<<sample[i]<<" ";
        }

    ui->action_15->setEnabled(true);
    ui->action_16->setEnabled(true);


}

void MainWindow::on_action_3_triggered()//обучение методом наименьших расстояний ЛДФ
{
    //ЛДФ. рассчет весов

    int w_dlin=stolb_kol;
    w=new float [w_dlin];
    w[0]=0;
    for (int i=1;i< w_dlin;i++)
        {w[i]=2*(centr1[i-1]-centr0[i-1]);
         w[0]+=(centr0[i-1]*centr0[i-1])-(centr1[i-1]*centr1[i-1]);
        }


    QTextEdit *txt_LDF = new QTextEdit();

     txt_LDF->setText("Получившаяся линейная дискриминантная функция : ");
     txt_LDF->append(" ");



     priznaki =new QString[stolb_kol-1];
     //priznaki=aryuna.split(',');
     qDebug()<<" priznaki: "<<aryuna;





     QString ss;
     QStringList prizn;
     prizn=aryuna.split(',',QString::SkipEmptyParts);

     for(int i=0;i<w_dlin;i++)
        {ss=QString::number(w[i]);
         if(i>=1)
            {ss.append(" * ");
             ss.append(prizn[i-1]);
            }


         txt_LDF->append(ss);
         //txt_LDF->append(" ");
         if(i<(w_dlin-1))
            {txt_LDF->append(" + ");}


        }

    txt_LDF->show();



    //сделай оценку качества!!!
    //считаем лдф для каждой строки и делаем прогноз для нее
    float *LDF4ocenka;
    LDF4ocenka=new float[str_kol];

    for(int i=0;i<str_kol;i++)
        {LDF4ocenka[i]+=w[0];
         for(int j=1;j<stolb_kol;j++)
            {LDF4ocenka[i]+=w[i]*matrix[i][j-1];

            }

        }

    //считаем лдф для эталонов
    float l0,l1;l0=0;l1=0;
    l0+=w[0];
    for(int i=1;i<stolb_kol;i++)
        {
         l0+= w[i]*centr0[i-1];
        }

    l1+=w[0];
    for(int i=0;i<stolb_kol;i++)
        {
         l1+=w[i]*centr1[i-1];
        }

    //делаем прогноз для каждой строки
    float * answer;
    answer=new float [str_kol];
    for(int i =0;i<str_kol;i++)
        {
        if((l1>0) && (LDF4ocenka[i]>0) && (l0<0))
            {
            answer[i]=1;
            }
        if((l1<0) && (LDF4ocenka[i]<0) && (l0>0))
            {
            answer[i]=1;
            }
        if((l0>0) && (LDF4ocenka[i]>0) && (l1<0))
            {
            answer[i]=0;
            }
        if((l0<0) && (LDF4ocenka[i]<0) && (l1>0))
            {
            answer[i]=0;
            }

        }

    float ** ocenka;
    ocenka=new float *[2];
    for (int i=0;i<2;i++)
        {ocenka[i]=new float [2];}
    for(int i=0;i<2;i++)
        {for(int j=0;j<2;j++)
            {ocenka[i][j]=0;}

        }

    for(int i=0;i<str_kol;i++)
        {if ((matrix[i][stolb_kol-1]==0) && (answer[i]==0))
            {ocenka[0][0]+=1;}                                  //true negative
         else if((matrix[i][stolb_kol-1]==1) && (answer[i]==0)) // ложноотрицательный
            {ocenka[0][1]+=1;}

         else if((matrix[i][stolb_kol-1]==1) && (answer[i]==1))
            {ocenka[1][1]+=1;}                                  //true positive
         else if ((matrix[i][stolb_kol-1]==0) && (answer[i]==1))//ложноположительный
            {ocenka[1][0]+=1;}


        }
    float sensivity,specificity;//чувствительность ==true positive rate,специфичность==true negative rate
    //sensivity=true positive/positive=tp/(tp+fn)
    //specificity=true negative/negative=tn/(tn+fp)
    sensivity=(ocenka[1][1]/(ocenka[1][1]+ocenka[0][1]))*100;
    specificity=(ocenka[0][0]/(ocenka[0][0]+ocenka[1][0]))*100;

    QTextEdit *txt_ocenka = new QTextEdit();
    txt_ocenka->setText("Оценка качества: ");
    QString true_negative,true_positive,false_positive,false_negative,spec,sens;
    QString tn,tp,fp,fn,sp,se;
    true_negative="Количество правда отрицательных : ";
    false_positive="Количество ложноположительных: ";
    spec="Специфичность: ";

    true_positive="Количество правда положительных: ";
    false_negative="Количество ложнонегативных: ";
    sens="Чувствительность: ";

    tn=QString::number(ocenka[0][0]);
    tp=QString::number(ocenka[1][1]);
    fn=QString::number(ocenka[0][1]);
    fp=QString::number(ocenka[1][0]);
    sp=QString::number(specificity);
    se=QString::number(sensivity);


    true_negative.append(tn);
    false_positive.append(fp);
    true_positive.append(tp);
    false_negative.append(fn);

    spec.append(sp);
    spec.append('%');
    sens.append(se);
    sens.append('%');

    txt_ocenka->append(true_negative);
    txt_ocenka->append(false_positive);

    txt_ocenka->append(spec);
    txt_ocenka->append("-------------");
    txt_ocenka->append(true_positive);
    txt_ocenka->append(false_negative);

    txt_ocenka->append(sens);

    txt_ocenka->show();














    //удаление массивов .плохая идея!!!
    //delete [] txt_LDF;

}

void MainWindow::on_action_15_triggered()//распознавание методом ЛДФ наименьших расстояний
{


    float lq;
    lq=0;
    lq+=w[0];

    for(int i=1;i<stolb_kol;i++)
        {lq+=w[i]*sample[i-1];

        }


    //for centres
    float l0,l1;l0=0;l1=0;
    l0+=w[0];
    for(int i=1;i<stolb_kol;i++)
        {
         l0+= w[i]*centr0[i-1];
        }

    l1+=w[0];
    for(int i=0;i<stolb_kol;i++)
        {
         l1+=w[i]*centr1[i-1];
        }

    //preparing diagnoz=answer
    QString answer;
    if((l1>0) && (lq>0) && (l0<0))
        {
        answer='1';
        }
    if((l1<0) && (lq<0) && (l0>0))
        {
        answer='1';
        }
    if((l0>0) && (lq>0) && (l1<0))
        {
        answer='0';
        }
    if((l0<0) && (lq<0) && (l1>0))
        {
        answer='0';
        }
    if (lq==0)
        {answer="Пример попал в область непринятия решения ";}



    qDebug()<<lq;
    QString ldf; ldf=QString::number(lq);

    QTextEdit *txt_lq = new QTextEdit();
    QString txt_ldf="Значение d(x):  ";
    txt_ldf.append(ldf);
    txt_lq->setText(txt_ldf);
    txt_lq->append(answer);
    txt_lq->show();



    //удаление массивов
    //delete [] itab;
    //delete [] sample_text;
    //sample_text->clear();
    //for (int i=0;i<(stolb_kol-1);i++)
       // {delete [] tabl[i];}

    //delete [] tabl;

}

//сделать нормальную очистку только массивов!!!!!!!!!!!!!!!!!!!!!!

void MainWindow::on_action_2_triggered()//адекватный выход из программы, удаление массивов
{

    /* вспоминаю,как удалять массивы
    //deleting of massives
    for (int i = 0; i < b; i++)
      {
          delete [] mas[i];
      }
    delete [] mas;
    */

    for (int i=0;i<str_kol;i++)
        {delete [] matrix[i];


        }
    delete [] matrix;

    for (int i=0;i<N0;i++)
        {
         delete [] matr0[i];

        }
    delete [] matr0;

    delete [] centr0;
    delete [] disp0;

    for (int i=0;i<N1;i++)
        {
         delete [] matr1[i];

        }
    delete [] matr1;
    delete [] centr1;
    delete [] disp1;


    delete [] w;
    delete [] priznaki;
    delete [] sample;




    qDebug()<<"successfull deleting and closing";

    QCoreApplication::quit();

}

void MainWindow::on_action_16_triggered()//распознавание методом розенблата
{ int sample_answer;
  float sample_d; sample_d=0;
  float * chan_sample; chan_sample=new float [stolb_kol];

  for(int i;i<(str_kol-1);i++)
    {if(i==0)
        {chan_sample[i]=1;}
      else
        {chan_sample[i]=sample[i-1];}

    }

  for(int j=0;j<stolb_kol;j++)
    { sample_d+=chan_sample[j]*percw[j];

    }
  if (sample_d>0)
    {sample_answer=0;

    }
  else
    {
     sample_answer=1;
    }

  QTextEdit *txt_recogn = new QTextEdit();
  QString txt_perc_recogn="Значение d:  ";
  QString answerP="Случай относится к классу: ";
  txt_perc_recogn.append(QString::number(sample_d));
  answerP.append(QString::number(sample_answer));
  txt_recogn->setText(txt_perc_recogn);
  txt_recogn->append(answerP);
  txt_recogn->show();



}



void MainWindow::on_action_4_triggered()//обучение методом розенблата. активация процесса
{
    //создаем новый массив. лобавляем новый признак для 0 веса- 1
    //float **dataPerc;
    //int feature_num;feature_num=stolb_kol-1;
    //int **ocenkaPerc;

    k_cycle=0;

    dataPerc=new float*[str_kol];
    for(int i=0;i<str_kol;i++)
        {dataPerc[i]=new float[stolb_kol];

        }


    for(int i=0;i<str_kol;i++)
        { for(int j=0;j<stolb_kol;j++)
            {if(j==0)
                {dataPerc[i][j]=1;}
             else
                {dataPerc[i][j]=matrix[i][j-1];}
             qDebug()<<dataPerc[i][j]<<" "<<i<<" "<<j;

            }

        }

    //активируем кнопку продолжения обучения

    ui->pushButton_3->setEnabled(true);

    //считываем коэффициент с или делаем его сами
    //int c;
    ui->spinBox->setEnabled(true);
    ui->label_3->setEnabled(true);

    //c=ui->spinBox->setReadOnly();
    //ui->spinBox->setPrefix("ggg");
    ui->spinBox->setSingleStep(1);
    ui->spinBox->setRange(1,4);//потом подумаем что с этим делать
    c=ui->spinBox->value();
    qDebug()<<c<<" spinbox";

    ui->textEdit->setEnabled(true);
    qDebug()<<" textedit ";




}

void MainWindow::on_pushButton_3_clicked()//обучение методом розенблата.
{  //обнуляем массив w
    percw=new float[stolb_kol];
    for(int i=0;i<stolb_kol;i++)
        {percw[i]=0;}


    c=ui->spinBox->value();
    qDebug()<<c;


    ui->pushButton_4->setEnabled(true);
    qDebug()<<"enabled_true ";

    int * answerPerc;
    int Errperc;
    Errperc=0;
    answerPerc=new int [str_kol];

    for (int i=0;i<str_kol;i++)
        {dperc=0;
         for(int j=0;j<stolb_kol;j++)
            { dperc+=percw[j]*dataPerc[i][j];


            }
         qDebug()<<"dperc "<<dperc;
         if(dperc>0)
            {answerPerc[i]=0;}
         else
            {answerPerc[i]=1;}

         if((answerPerc[i]==0) && (matrix[i][stolb_kol-1]==1)) //|| ((dperc==0) && (matrix[i][stolb_kol-1]==1)))
            {
             Errperc+=1;
             for(int j=0;j<stolb_kol;j++)
                {percw[j]-=c*dataPerc[i][j];
                }
            }

         if((answerPerc[i]==1) && (matrix[i][stolb_kol-1]==0)) //|| ((dperc==0) && (matrix[i][stolb_kol-1]==0)))
            {
             Errperc+=1;
             for(int j=0;j<stolb_kol;j++)
                {percw[j]+=c*dataPerc[i][j];
                }
            }



        }

    qDebug()<<"до обучения дошли "; k_cycle+=1;

    //ocenka raboti perceptrona
    float **ocenkaPerc;
    ocenkaPerc=new float *[2];
    for(int i=0;i<2;i++)
        {ocenkaPerc[i]=new float [2];}
    for(int i=0;i<2;i++)
        {for (int j=0;j<2;j++)
            {ocenkaPerc[i][j]=0;

            }

        }


    for(int i=0;i<str_kol;i++)
        {if ((matrix[i][stolb_kol-1]==0) && (answerPerc[i]==0) )
            {ocenkaPerc[0][0]+=1;}
        else if((matrix[i][stolb_kol-1]==1) && (answerPerc[i]==0)) // ложноотрицательный
           {ocenkaPerc[0][1]+=1;}

        else if((matrix[i][stolb_kol-1]==1) && (answerPerc[i]==1))
           {ocenkaPerc[1][1]+=1;}                                  //true positive
        else if ((matrix[i][stolb_kol-1]==0) && (answerPerc[i]==1))//ложноположительный
           {ocenkaPerc[1][0]+=1;}


        }

    qDebug()<<" ocenka proizvedena ";

    for(int i=0;i<2;i++)
        {for(int j=0;j<2;j++)
            {qDebug()<<"ocenka "<<ocenkaPerc[i][j]<<" coord "<<i<<" "<<j;}
        }

    float sensivityperc,specificityperc;//чувствительность ==true positive rate,специфичность==true negative rate
    //sensivity=true positive/positive=tp/(tp+fn)
    //specificity=true negative/negative=tn/(tn+fp)
    sensivityperc=float(ocenkaPerc[1][1]/(ocenkaPerc[1][1]+ocenkaPerc[0][1]))*100;
    specificityperc=float(ocenkaPerc[0][0]/(ocenkaPerc[0][0]+ocenkaPerc[1][0]))*100;

    //QTextEdit *txt_ocenkaPerc = new QTextEdit();
    //txt_ocenkaPerc->setText("Оценка качества: ");
    ui->textEdit->setText("Оценка качества: ");

    QString true_negative,true_positive,false_positive,false_negative,spec,sens;
    QString tn,tp,fp,fn,sp,se;
    true_negative="Количество правда отрицательных : ";
    false_positive="Количество ложноположительных: ";
    spec="Специфичность: ";

    true_positive="Количество правда положительных: ";
    false_negative="Количество ложнонегативных: ";
    sens="Чувствительность: ";

    tn=QString::number(ocenkaPerc[0][0]);
    tp=QString::number(ocenkaPerc[1][1]);
    fn=QString::number(ocenkaPerc[0][1]);
    fp=QString::number(ocenkaPerc[1][0]);
    sp=QString::number(specificityperc);
    se=QString::number(sensivityperc);


    true_negative.append(tn);
    false_positive.append(fp);
    true_positive.append(tp);
    false_negative.append(fn);

    spec.append(sp);
    spec.append('%');
    sens.append(se);
    sens.append('%');

    ui->textEdit->append(true_negative);
    ui->textEdit->append(false_positive);

    ui->textEdit->append(spec);
    ui->textEdit->append("-------------");
    ui->textEdit->append(true_positive);
    ui->textEdit->append(false_negative);

    ui->textEdit->append(sens);


    QString kol_error;kol_error="Количество ошибок ";kol_error.append(QString::number(Errperc));
    QString kol_cycle;kol_cycle="Количество циклов обучения";kol_cycle.append(QString::number(k_cycle));

    ui->textEdit->append("_____________");
    ui->textEdit->append(kol_cycle);
    ui->textEdit->append(kol_error);


     ui->textEdit->append("Получившийся вектор весов : ");
     ui->textEdit->append(" ");



     QString ss;
     QStringList prizn;
     prizn=aryuna.split(',',QString::SkipEmptyParts);

     for(int i=0;i<stolb_kol;i++)
        {ss=QString::number(percw[i]);
         if(i>=1)
            {ss.append(" * ");
             ss.append(prizn[i-1]);
            }


         ui->textEdit->append(ss);

         if(i<(stolb_kol-1))
            {ui->textEdit->append(" + ");}


        }





    ui->textEdit->show();






}

void MainWindow::on_pushButton_4_clicked()//продолжаем обучение методом розенблата
{   ui->pushButton_3->setEnabled(false);
    ui->spinBox->setEnabled(false);


    int * answerPerc;
    int Errperc;
    Errperc=0;
    answerPerc=new int [str_kol];




    for (int i=0;i<str_kol;i++)
    {dperc=0;
     for(int j=0;j<stolb_kol;j++)
        { dperc+=percw[j]*dataPerc[i][j];


        }
     qDebug()<<"dperc "<<dperc;
     if(dperc>0)
        {answerPerc[i]=0;}
     else
        {answerPerc[i]=1;}

     if((answerPerc[i]==0) && (matrix[i][stolb_kol-1]==1))
        {
         Errperc+=1;
         for(int j=0;j<stolb_kol;j++)
            {percw[j]-=c*dataPerc[i][j];
            }
        }

     if((answerPerc[i]==1) && (matrix[i][stolb_kol-1]==0))
        {
         Errperc+=1;
         for(int j=0;j<stolb_kol;j++)
            {percw[j]+=c*dataPerc[i][j];
            }
        }

    }

qDebug()<<"до обучения дошли ";k_cycle+=1;

//ocenka raboti perceptrona
float **ocenkaPerc;
ocenkaPerc=new float *[2];
for(int i=0;i<2;i++)
    {ocenkaPerc[i]=new float [2];}
for(int i=0;i<2;i++)
    {for (int j=0;j<2;j++)
        {ocenkaPerc[i][j]=0;

        }

    }


for(int i=0;i<str_kol;i++)
    {if ((matrix[i][stolb_kol-1]==0) && (answerPerc[i]==0) )
        {ocenkaPerc[0][0]+=1;}
    else if((matrix[i][stolb_kol-1]==1) && (answerPerc[i]==0)) // ложноотрицательный
       {ocenkaPerc[0][1]+=1;}

    else if((matrix[i][stolb_kol-1]==1) && (answerPerc[i]==1))
       {ocenkaPerc[1][1]+=1;}                                  //true positive
    else if ((matrix[i][stolb_kol-1]==0) && (answerPerc[i]==1))//ложноположительный
       {ocenkaPerc[1][0]+=1;}


    }

qDebug()<<" ocenka proizvedena ";

for(int i=0;i<2;i++)
    {for(int j=0;j<2;j++)
        {qDebug()<<"ocenka "<<ocenkaPerc[i][j]<<" coord "<<i<<" "<<j;}
    }

float sensivityperc,specificityperc;//чувствительность ==true positive rate,специфичность==true negative rate
//sensivity=true positive/positive=tp/(tp+fn)
//specificity=true negative/negative=tn/(tn+fp)
sensivityperc=float(ocenkaPerc[1][1]/(ocenkaPerc[1][1]+ocenkaPerc[0][1]))*100;
specificityperc=float(ocenkaPerc[0][0]/(ocenkaPerc[0][0]+ocenkaPerc[1][0]))*100;

//QTextEdit *txt_ocenkaPerc = new QTextEdit();
//txt_ocenkaPerc->setText("Оценка качества: ");
ui->textEdit->setText("Оценка качества: ");

QString true_negative,true_positive,false_positive,false_negative,spec,sens;
QString tn,tp,fp,fn,sp,se;


true_negative="Количество правда отрицательных : ";
false_positive="Количество ложноположительных: ";
spec="Специфичность: ";

true_positive="Количество правда положительных: ";
false_negative="Количество ложнонегативных: ";
sens="Чувствительность: ";

tn=QString::number(ocenkaPerc[0][0]);
tp=QString::number(ocenkaPerc[1][1]);
fn=QString::number(ocenkaPerc[0][1]);
fp=QString::number(ocenkaPerc[1][0]);
sp=QString::number(specificityperc);
se=QString::number(sensivityperc);


true_negative.append(tn);
false_positive.append(fp);
true_positive.append(tp);
false_negative.append(fn);

spec.append(sp);
spec.append('%');
sens.append(se);
sens.append('%');

ui->textEdit->append(true_negative);
ui->textEdit->append(false_positive);
ui->textEdit->append(spec);
ui->textEdit->append("-------------");
ui->textEdit->append(true_positive);
ui->textEdit->append(false_negative);
ui->textEdit->append(sens);


QString kol_error;kol_error="Количество ошибок ";kol_error.append(QString::number(Errperc));
QString kol_cycle;kol_cycle="Количество циклов обучения";kol_cycle.append(QString::number(k_cycle));

ui->textEdit->append("_____________");
ui->textEdit->append(kol_cycle);
ui->textEdit->append(kol_error);


ui->textEdit->append("_____________");
ui->textEdit->append("Получившийся вектор весов :    ");
QString ss;
QStringList prizn;
prizn=aryuna.split(',',QString::SkipEmptyParts);
for(int i=0;i<stolb_kol;i++)
   {ss=QString::number(percw[i]);
    if(i>=1)
       {ss.append(" * ");
        ss.append(prizn[i-1]);
       }


    ui->textEdit->append(ss);

    if(i<(stolb_kol-1))
       {ui->textEdit->append(" + ");}
   }


ui->textEdit->show();




}

void MainWindow::on_action_10_triggered()//оценка информативности признаков
{

    float * infoness;
    float * temp_d12_out;
    float * d12_out;

    infoness=new float[stolb_kol-1];
    temp_d12_out=new float[stolb_kol-1];
    for(int j=0;j<(stolb_kol-1);j++)
        {temp_d12_out[j]=0;

        }

    d12_out=new float[stolb_kol-1];
    //float d12_rasst;d12_rasst=d12;

    for(int j=0;j<(stolb_kol-1);j++)
        {
         for(int i=0;i<(stolb_kol-1);i++)
            {if(j!=i)
                {temp_d12_out[j]+=(centr0[i]-centr1[i])*(centr0[i]-centr1[i]);

                }

            }

        }

    for(int j=0;j<(stolb_kol-1);j++)
        {
         d12_out[j]=sqrt(temp_d12_out[j]);
        }
    for(int j=0;j<(stolb_kol-1);j++)
        {
         infoness[j]=d12-d12_out[j];
        }




//vivod na ekran

    QTextEdit *txt_infoness = new QTextEdit();
    QString str_infoness="Информативность признаков:  ";
    txt_infoness->setText(str_infoness);
    QString ss;
    QStringList prizn;
    prizn=aryuna.split(',',QString::SkipEmptyParts);
    for(int i=0;i<(stolb_kol-1);i++)
       {ss=QString::number(infoness[i]);
        ss.append("    для признака    ");
        ss.append(prizn[i]);

        txt_infoness->append(ss);

       }


    txt_infoness->show();







}

void MainWindow::on_action_7_triggered()//инструкция
{

    QTextEdit *txt_instr = new QTextEdit();
    txt_instr->setFixedHeight(500);
    txt_instr->setFixedWidth(900);
     txt_instr->setText("Инструкция по использованию программы: ");
     txt_instr->append("1.Откройте файл csv-формата, в котором на последнем столбце представлены диагнозы в виде 0 и 1, а остальные данные представлены в числовом виде. ");
     txt_instr->append("2.Затем следует посчитать характеристики: дисперсии и центры классов. ");
     txt_instr->append(" ");

     txt_instr->append("3.Теперь вам доступно обучение методом ЛДФ и обучение методом перцептрон. ");
     txt_instr->append("4.Для того чтобы произвести обучение методом ЛДФ, нажмите соответвующую кнопку. ");
     txt_instr->append("Сразу после этого откроется диалоговое окно с результатами обучения. ");

     txt_instr->append("5.Для того чтобы произвести обучение методом перцептрон, нажмите соответствующую кнопку. ");
     txt_instr->append("Вам станет доступная возможность подобрать коэффициент обучения C .");
     txt_instr->append("Введите его (желательно оставить 1), далее нажмите на кнопку  <<С введен>>. ");
     txt_instr->append("Теперь вам стоит нажать кнопку <<продолжить обучение>>, чтобы пройти в перцептроном ещё один цикл обучения.");
     txt_instr->append("Рядом с упомянутыми кнопками вам будет виден текст о таких показателях, как специфичность, чувствительность, номер цикла обучения,разные виды ошибок и получившийся вектор весов. ");

     txt_instr->append(" ");
     txt_instr->append("6.Теперь вы можете на основе обученных моделей распознать ваш объект.");
     txt_instr->append("Нажмите кнопку <<Ввести случай>>. После этого появится таблица, куда вы можете ввести ваши данные ");
     txt_instr->append("Как только вы правильно введете данные, нажмите на кнопку <<Случай введен>> ");
     txt_instr->append("Теперь вы имеете возможность выбрать метод распознавания, нажмите для этого соответствующую кноку ");
     txt_instr->append("После нажатия кнопки вы увидите диалоговое окно, в котором написан класс, к которому относится объект");
     txt_instr->append(" ");
     txt_instr->append("7.Сразу после того, как вы посчитали характеристики на ваших данных, вы можете определить информативность каждого признака.");
     txt_instr->append("Для этого нажмите соответствующую кнопку. После нажатия кнопки вы увидитите диалоговое окно, в котором видны цифры и признаки");
     txt_instr->append("Вы можете интерпретировать результаты как: <<Чем выше цифра,тем информативнее признак>> ");
     txt_instr->append("Кнопки, что находятся в самом правом углу, вы можете нажимать в любое время при необходимости ");

     txt_instr->append("______________________________________");
     txt_instr->append("По любым вопросам, связанными с программой: ");
     txt_instr->append("mail: aryuna.ayusheeva.1998@mail.ru ");
     txt_instr->append("tel: +79138700781 ");


    txt_instr->show();


}

void MainWindow::on_action_8_triggered()//ссылки и ресурсы
{
    QTextEdit *txt_ssil = new QTextEdit();
    txt_ssil->setFixedHeight(300);
    txt_ssil->setFixedWidth(900);
    txt_ssil->setText("Ссылки и ресурсы: ");
    txt_ssil->append("1.Джайчибеков Н.Ж, Пеккер Я.С, Фокин В.А. Методы преобразования и анализа данных.Караганда: Изд-во ТОО <<Издатсервис>>, 2008. -248 с.");
    txt_ssil->append("2.Официальная документация по Qt5: https://doc.qt.io/qtcreator/index.html ");
    txt_ssil->append("3.Уроки по Qt5: https://www.youtube.com/watch?v=gfizc40fKfQ&list=LL3wkbCC9arkKpXTwdp03psg&index=3&t=0s");
    txt_ssil->append("4.Учебные видео по Qt5:  https://www.youtube.com/watch?v=EkjaiDsiM-Q&list=PLS1QulWo1RIZiBcTr5urECberTITj7gjA&index=2&t=2s");
    txt_ssil->show();


}

void MainWindow::on_action_6_triggered()//Принципы методов
{ QTextEdit *txt_prinz = new QTextEdit();

    txt_prinz->setFixedWidth(900);
    txt_prinz->setFixedHeight(300);
    txt_prinz->setText("Принципы методов: ");
    txt_prinz->append("Перцептрон -  (англ. perceptron от лат. perceptio — восприятие; нем. Perzeptron) — математическая или компьютерная модель восприятия информации мозгом (кибернетическая модель мозга), предложенная Фрэнком Розенблаттом в 1957 году и впервые реализованная в виде электронной машины «Марк-1» в 1960 году. Перцептрон стал одной из первых моделей нейросетей, а «Марк-1» — первым в мире нейрокомпьютером. ");
    txt_prinz->append("Идея построения алгоритма распознавания заключается в том, что "
                      "первоначально задается произвольная дискриминантная функция,затем"
                      "в процессе последовательного предьявления элементов обучающей выборки"
                      "производится исправление весовых коэффициентов "
                      "в зависимости от правильности решения(наказание за ощибку)."
                      "Процесс исправления весов и есть обучение, а скорость обучения будет зависеть от константы С- шага обучения."
                      "Таким образом, общая стратегия обучения состоит на основе обратной связи");
    txt_prinz->append(" ");
    txt_prinz->append("ЛДФ - линейная дискриминантная (решающая) функция,которая в нашем случае делит два класса по принципу "
                      "минимума расстояния.");
    txt_prinz->append("Расстояние между объектами двух классов, расстояние внутри классов и "
                      "центры классов считаются статистически. ");


    txt_prinz->show();



}

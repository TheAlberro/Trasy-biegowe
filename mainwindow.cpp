#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <Qtcore>   // do wyskakujacych okienek
#include <QtGui>

#include <QFile>    //do zapisu i odczytu
#include <QString>
#include <QDebug>
#include <QTextStream>
                        //STL
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>

#include <iostream>
//#include <string>
//#include <sstream>

#include <QVector>

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ukryj_paczki();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ukryj_paczki()
{
    ui->label->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->podaj_paczki->setVisible(false);
    ui->wyswietl_paczki->setVisible(false);
    var="0";
}
void MainWindow::on_pushButton_clicked()
{
    float pom=ui->podaj_paczki->text().toFloat()*40;
    ui->wyswietl_paczki->setText("Aby spalic te paczki \n potrzebujesz biec: \n"  +QString::number(pom)+" minut");
}
void MainWindow::on_klik_listy_clicked()
{
    if(ui->dodaj_trase->text()==NULL || ui->dodaj_dystans->text()==NULL)
    {
        QMessageBox::information(this,"komunikat","Uzupelnij wszystkie pola!");
    }
    else
    {
        if(ui->dodaj_dystans->text().toDouble()==false)
        {
            QMessageBox::information(this,"komunikat","Dystans musi \n byc cyfra");
        }
        else
        {
            ui->trasy->addItem(ui->dodaj_trase->text());
                        ui->dystanse->addItem(ui->dodaj_dystans->text());
                         ui->trudnosc->addItem(ui->comboBox->currentText());

            ui->trasy_text->append(ui->dodaj_trase->text());
            ui->dystanse_text->append(ui->dodaj_dystans->text());
             ui->trudnosc_text->append(ui->comboBox->currentText());
               QString pom1=ui->dodaj_trase->text();
                QString pom2=ui->comboBox->currentText();
             tab_trasy.push_back(pom1);
             tab_trudnosc.push_back(pom2);

             if(ui->miary_combo_box->currentText()=="Kilometry")
             {
                 tab_kilometry.push_back(ui->dodaj_dystans->text().toDouble());
                 tab_metry.push_back(ui->dodaj_dystans->text().toDouble()*1000);
                 tab_mile.push_back(ui->dodaj_dystans->text().toDouble()*0.62);
             }

             if(ui->miary_combo_box->currentText()=="Metry")
             {
                 tab_kilometry.push_back(ui->dodaj_dystans->text().toDouble()/1000);
                    tab_metry.push_back(ui->dodaj_dystans->text().toDouble());
                  tab_mile.push_back(ui->dodaj_dystans->text().toDouble()*0.62);
             }
             if(ui->miary_combo_box->currentText()=="Mile")
             {
                 tab_kilometry.push_back(ui->dodaj_dystans->text().toDouble()*1.6);
                tab_metry.push_back(ui->dodaj_dystans->text().toDouble()*1609);
                  tab_mile.push_back(ui->dodaj_dystans->text().toDouble());
             }
        }
    }
}

void MainWindow::on_zapisz_stan_clicked()
{
if(ostrzezenie_przed_zapisem_po_wyborze=="1")
{
    QMessageBox::information(this,"Informacja","Aby moc zapisac wyswietl wszystko");
}
else
{
    QMessageBox::StandardButton czy;
    czy = QMessageBox::question(this,"Informacja","Czy chcesz zapisac dane?",
                           QMessageBox::Yes |
                                QMessageBox::No);
    if(czy ==  QMessageBox::Yes)
    {
        QString filename1 = "trasy.txt";
         QFile file(filename1);
         // Trying to open in WriteOnly and Text mode
         if(!file.open(QFile::WriteOnly |
         QFile::Text))
         {
         qDebug() << " Could not open file for writing";
         return;
         }
         QTextStream out(&file);

         out<<ui->trasy_text->toPlainText();
         file.flush();
         file.close();

         QString filename2 = "trudnosc.txt";
          QFile file2(filename2);
          // Trying to open in WriteOnly and Text mode
          if(!file2.open(QFile::WriteOnly |
          QFile::Text))
          {
          qDebug() << " Could not open file for writing";
          return;
          }
          QTextStream out2(&file2);
          out2<<ui->trudnosc_text->toPlainText();
          file2.flush();
          file2.close();

          QString filename3 = "dystanse.txt";
           QFile file3(filename3);
           // Trying to open in WriteOnly and Text mode
           if(!file3.open(QFile::WriteOnly |
           QFile::Text))
           {
           qDebug() << " Could not open file for writing";
           return;
           }
           QTextStream out3(&file3);
           if(warunek=="kilometry")
           {
               QMessageBox::information(this,"Informacja","zapisujesz w kilometrach");
               out3<<"kilometry\n";
           }
               if(warunek=="metry")
           {
               QMessageBox::information(this,"Informacja","zapisujesz w metrach");
               out3<<"metry\n";
           }
           if(warunek=="mile")
           {
               QMessageBox::information(this,"Informacja","zapisujesz w milach");
               out3<<"mile\n";
           }
           out3<<ui->dystanse_text->toPlainText();
           file3.flush();
           file3.close();
    }

}
}
void MainWindow::on_odczytaj_stan_clicked()
{
    int pierwszy_obrot=0;
    std::string pierwsza_informacja;
    QMessageBox::StandardButton czy;
    czy = QMessageBox::question(this,"Informacja","Czy chcesz wczytac dane?",
                           QMessageBox::Yes |
                                QMessageBox::No);
    if(czy ==  QMessageBox::Yes)
    {//czyszczenie
        ui->dystanse->clear();
        ui->trasy->clear();
        ui->trudnosc->clear();
        ui->dystanse_text->clear();
        ui->trasy_text->clear();
        ui->trudnosc_text->clear();
        tab_kilometry.clear();
       tab_metry.clear();
       tab_mile.clear();
        tab_trasy.clear();
           tab_trudnosc.clear();

        QString filename = "trasy.txt";
         QFile file(filename);
         if(!file.open(QFile::ReadOnly |
         QFile::Text))
         {
         qDebug() << " Could not open the file for reading";
         return;
         }
          QTextStream in(&file);
         while(!in.atEnd()) {
         QString line = in.readLine();
         tab_trasy.push_back(line);
         ui->trasy_text->append(line);
         ui->trasy->addItem(line);
         }

         file.close();


         QString filename2 = "trudnosc.txt";
          QFile file2(filename2);
          if(!file2.open(QFile::ReadOnly |
          QFile::Text))
          {
          qDebug() << " Could not open the file for reading";
          return;
          }
           QTextStream in2(&file2);
          while(!in2.atEnd()) {
          QString line2 = in2.readLine();
          ui->trudnosc_text->append(line2);
          tab_trudnosc.push_back(line2);
          ui->trudnosc->addItem(line2);
          }
          file2.close();


    QString filename3 = "dystanse.txt";
    QFile file3(filename3);
    if(!file3.open(QFile::ReadOnly |
    QFile::Text))
    {
    qDebug() << " Could not open the file for reading";
    return;
    }
     QTextStream in3(&file3);
     pierwszy_obrot=0;
    while(!in3.atEnd()) {
    QString line3 = in3.readLine();


    if(line3=="kilometry" && pierwszy_obrot==0)
    {
        pierwszy_obrot=1;
        pierwsza_informacja="kilometry";
    }
    else
    {
        if(pierwsza_informacja=="kilometry")
        {
            tab_kilometry.push_back(line3.toDouble());
            tab_metry.push_back(line3.toDouble()*1000);
            tab_mile.push_back(line3.toDouble()*0.621371192);
        }
    }

    if(line3=="metry" && pierwszy_obrot==0)
    {
        pierwszy_obrot=1;
        pierwsza_informacja="metry";
    }
    else
    {
        if(pierwsza_informacja=="metry")
        {
            tab_kilometry.push_back(line3.toDouble()/1000);
            tab_metry.push_back(line3.toDouble());
            tab_mile.push_back(line3.toDouble()*0.000621371192);
        }
    }

    if(line3=="mile" && pierwszy_obrot==0)
    {
        pierwszy_obrot=1;
        pierwsza_informacja="mile";
    }
    else
    {
        if(pierwsza_informacja=="mile")
        {
            tab_kilometry.push_back(line3.toDouble()*1.609344);
            tab_metry.push_back(line3.toDouble()*1609.344);
            tab_mile.push_back(line3.toDouble());


        }
    }
                // W tym momencie mamy już uzupelnione wektory z dystansami w roznych miarach pora dodac je do listy

            ui->dystanse->clear();
            ui->dystanse_text->clear();
            if(ui->miary_combo_box->currentText()=="Kilometry")
            {
                for(int i=0;i<tab_kilometry.count();i++)
                {
                    ui->dystanse_text->append(QString::number(tab_kilometry.at(i)));
                    ui->dystanse->addItem(QString::number(tab_kilometry.at(i)));
                }


            }

            if(ui->miary_combo_box->currentText()=="Metry")
            {
                for(int i=0;i<tab_metry.count();i++)
                {
                    ui->dystanse_text->append(QString::number(tab_metry.at(i)));
                    ui->dystanse->addItem(QString::number(tab_metry.at(i)));
                }
            }
            if(ui->miary_combo_box->currentText()=="Mile")
            {
                for(int i=0;i<tab_mile.count();i++)
                {
                    ui->dystanse_text->append(QString::number(tab_mile.at(i)));
                    ui->dystanse->addItem(QString::number(tab_mile.at(i)));
                }
            }
    }
    file3.close();
    }
}
void MainWindow::on_wyczysc_clicked()
{
    QMessageBox::StandardButton czy;
    czy = QMessageBox::question(this,"Informacja","Czy chcesz zresetowac dane?",
                           QMessageBox::Yes |
                                QMessageBox::No);
    if(czy ==  QMessageBox::Yes)
    {//czyszczenie
        ui->dystanse->clear();
        ui->trasy->clear();
        ui->trudnosc->clear();
        ui->dystanse_text->clear();
        ui->trasy_text->clear();
        ui->trudnosc_text->clear();
        tab_kilometry.clear();
        tab_metry.clear();
        tab_mile.clear();
        tab_trasy.clear();
        tab_trudnosc.clear();

    }
}

void MainWindow::on_Usun_clicked()
{

    QMessageBox::StandardButton czy;
    czy = QMessageBox::question(this,"Informacja","Czy chcesz usunac podana trase ze wszystkimi jej skladnikami?",
                           QMessageBox::Yes |
                                QMessageBox::No);
    if(czy ==  QMessageBox::Yes)
    {
        QString usuwana=ui->do_usuniecia->text();
        int index_usuwanej=-1;
        for(int i=0;i<ui->trasy->count();i++)
        {
          if(usuwana==tab_trasy.at(i)){index_usuwanej=i;QMessageBox::information(this,"Informacja","Usuwam element "+ usuwana);}

        }
        if(index_usuwanej==-1)
        {
            QMessageBox::information(this,"Informacja","Na liscie nie ma pozycji o podanej nazwie");
          }
        else
        {
            tab_trasy.erase(tab_trasy.begin() + index_usuwanej);
            tab_trudnosc.erase(tab_trudnosc.begin() + index_usuwanej);
            tab_kilometry.erase(tab_kilometry.begin() + index_usuwanej);
             tab_mile.erase(tab_mile.begin() + index_usuwanej);
            tab_metry.erase(tab_metry.begin() + index_usuwanej);

        ui->dystanse->clear();
        ui->trasy->clear();
        ui->trudnosc->clear();
        ui->dystanse_text->clear();
        ui->trasy_text->clear();
        ui->trudnosc_text->clear();
        for(int i=0;i<tab_trasy.count();i++)
       {
            ui->trasy->addItem(tab_trasy.at(i));
            ui->trudnosc->addItem(tab_trudnosc.at(i));
            ui->trasy_text->append(tab_trasy.at(i));
            ui->trudnosc_text->append(tab_trudnosc.at(i));
            if(warunek=="kilometry")
            {
                ui->dystanse->addItem(QString::number(tab_kilometry.at(i)));
                ui->dystanse_text->append(QString::number(tab_kilometry.at(i)));
            }
            if(warunek=="metry")
            {
                ui->dystanse->addItem(QString::number(tab_metry.at(i)));
                ui->dystanse_text->append(QString::number(tab_metry.at(i)));
            }
            if(warunek=="mile")
            {
                ui->dystanse->addItem(QString::number(tab_mile.at(i)));
                ui->dystanse_text->append(QString::number(tab_mile.at(i)));
            }
        }
        }
    }
 }
void MainWindow::on_konwersja_clicked()
{
       if(ui->miary_combo_box->currentText()=="Kilometry")warunek="kilometry";
       if(ui->miary_combo_box->currentText()=="Metry")warunek="metry";
       if(ui->miary_combo_box->currentText()=="Mile")warunek="mile";
    int pom=ui->dystanse->count();
    ui->dystanse->clear();
    ui->dystanse_text->clear();

    if(ui->miary_combo_box->currentText()=="Kilometry")
    {
        for(int i=0;i<pom;i++)
        {
            ui->dystanse->addItem(QString::number(tab_kilometry.at(i)));
            ui->dystanse_text->append(QString::number(tab_kilometry.at(i)));
        }
    }
    if(ui->miary_combo_box->currentText()=="Metry")
    {
        for(int i=0;i<pom;i++)
        {
             ui->dystanse_text->append(QString::number(tab_metry.at(i)));
            ui->dystanse->addItem(QString::number(tab_metry.at(i)));
        }
    }
    if(ui->miary_combo_box->currentText()=="Mile")
    {
        for(int i=0;i<pom;i++)
        {
            ui->dystanse_text->append(QString::number(tab_mile.at(i)));
            ui->dystanse->addItem(QString::number(tab_mile.at(i)));
        }
    }
}
void MainWindow::on_guzik_latwe_clicked()
{
    if(ui->guzik_latwe->isChecked()==true)sprawdz="Latwy";
    on_box_trudnosci_clicked();
}
void MainWindow::on_guzik_srednie_clicked()
{
    if(ui->guzik_srednie->isChecked()==true)sprawdz="Sredni";
    on_box_trudnosci_clicked();
}
void MainWindow::on_guzik_trudne_clicked()
{
    if(ui->guzik_trudne->isChecked()==true)sprawdz="Trudny";
    on_box_trudnosci_clicked();
}

void MainWindow::on_guzik_ekstremalne_clicked()
{
    if(ui->guzik_ekstremalne->isChecked()==true)sprawdz="Ekstremalny";
    on_box_trudnosci_clicked();
}

void MainWindow::on_guzik_wszystkie_clicked()
{
    if(ui->guzik_wszystkie->isChecked()==true)sprawdz="Wszystkie";
    on_box_trudnosci_clicked();
}
void MainWindow::on_box_trudnosci_clicked()
{
    if(sprawdz!="Wszystkie")ostrzezenie_przed_zapisem_po_wyborze="1";
    else ostrzezenie_przed_zapisem_po_wyborze="0";
index_zaznaczonej_trudnosci.clear();
    for(int i=0;i<tab_trudnosc.count();i++)
    {
        if(sprawdz==tab_trudnosc.at(i))index_zaznaczonej_trudnosci.push_back(i);
    }
    ui->trasy->clear();
    ui->trudnosc->clear();
    ui->dystanse->clear();
    ui->dystanse_text->clear();
    if(sprawdz=="Wszystkie")
    {
        for(int i=0;i<tab_trasy.count();i++)
        {
            ui->trasy->addItem(tab_trasy.at(i));
           // ui->trasy_text->append(tab_trasy.at(i));
            ui->trudnosc->addItem(tab_trudnosc.at(i));
          //  ui->trudnosc_text->addItem(tab_trudnosc.at(i));
            if(warunek=="kilometry"){ui->dystanse->addItem(QString::number(tab_kilometry.at(i)));ui->dystanse_text->append(QString::number(tab_kilometry.at(i)));}
            if(warunek=="metry"){ui->dystanse->addItem(QString::number(tab_metry.at(i)));ui->dystanse_text->append(QString::number(tab_metry.at(i)));}
            if(warunek=="mile"){ui->dystanse->addItem(QString::number(tab_mile.at(i)));ui->dystanse_text->append(QString::number(tab_mile.at(i)));}
        }
    }
    else
    {
        if(index_zaznaczonej_trudnosci.isEmpty())QMessageBox::information(this,"informacja","Brak takiej pozycji na liscie ");
    for(int i=0;i<index_zaznaczonej_trudnosci.count();i++)
    {
        ui->trasy->addItem(tab_trasy.at(index_zaznaczonej_trudnosci.at(i)));
        ui->trudnosc->addItem(tab_trudnosc.at(index_zaznaczonej_trudnosci.at(i)));
        if(warunek=="kilometry")ui->dystanse->addItem(QString::number(tab_kilometry.at(index_zaznaczonej_trudnosci.at(i))));
        if(warunek=="metry")ui->dystanse->addItem(QString::number(tab_metry.at(index_zaznaczonej_trudnosci.at(i))));
        if(warunek=="mile")ui->dystanse->addItem(QString::number(tab_mile.at(index_zaznaczonej_trudnosci.at(i))));
    }
    }
}
void MainWindow::on_actionIle_zjadles_triggered()
{
    if(var=="0"){
    ui->label->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->podaj_paczki->setVisible(true);
    ui->wyswietl_paczki->setVisible(true);
    var="1";
    }
    else
    {
        ui->label->setVisible(false);
        ui->pushButton->setVisible(false);
        ui->podaj_paczki->setVisible(false);
        ui->wyswietl_paczki->setVisible(false);
        var="0";
    }
}

void MainWindow::on_button_kolokwium_clicked()
{

    if(ui->line_kolokwium->text().toDouble()==0)ui->lista_kolokwium->addItem("zero");
    if(ui->line_kolokwium->text().toDouble()==1)ui->lista_kolokwium->addItem("jeden");
    if(ui->line_kolokwium->text().toDouble()==2)
    {
        ile_dwojek++;
        ui->lista_kolokwium->addItem("dwa");
    }
    if(ui->line_kolokwium->text().toDouble()==3)ui->lista_kolokwium->addItem("trzy");
    if(ui->line_kolokwium->text().toDouble()==4)ui->lista_kolokwium->addItem("cztery");
    if(ui->line_kolokwium->text().toDouble()==5)ui->lista_kolokwium->addItem("piec");
    ui->informacja_kolokwium->setText("Podano dwojek: " + QString::number(ile_dwojek));
}

void MainWindow::on_klik_clicked()
{
    ui->label_10->setText("Wygrales  \n"  +QString::number(0)+"  albo jednak cie oszukano");
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTextStream>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QBitArray>
#include <QThread>
#include <QtAlgorithms>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    MainWindow::rowcount = 1;
    MainWindow::URL = "http://api.openweathermap.org/data/2.5/forecast?q=Delhi&units=metric&APPID=e0e1836d4c79d0fa264d648dc576a781&cnt=100";
    ui->setupUi(this);
    ui->data_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::feachdata(QNetworkReply*reply){
    QString json_string = reply->readAll();
    for (auto i=0; i!=1000;i++) {}
    QByteArray json_bytes = json_string.toLocal8Bit();
    auto json_doc=QJsonDocument::fromJson(json_bytes);
    auto table = ui->data_table;
    if(!json_doc.isObject()){
      //  MainWindow::loading.close();
        qDebug()<<"JSON is not an object.";
        QMessageBox  x;
        x.setIcon(QMessageBox::Critical);
        x.setText("You are not connected to Internet");
        x.setWindowTitle("Internet Problem");
        x.setStandardButtons(QMessageBox::Ok|QMessageBox::Retry|QMessageBox::Close);
        x.setDefaultButton(QMessageBox::Retry);
        x.show();
        int i = x.exec();
        switch (i) {
        case QMessageBox::Retry:
            MainWindow::on_Search_clicked();
            break;
        case QMessageBox::Ok:
            break;
        case QMessageBox::Close:
            close();
            break;
        }
        return;
    }else {
        //MainWindow::loading.close();


        QJsonObject json_obj=json_doc.object();

        QVariantMap json_map = json_obj.toVariantMap();

        qDebug()<< json_map["cod"].toString();
        if(json_map["cod"].toString()=="200"){
            QJsonArray list = json_obj["list"].toArray();
            //list loop
            for(int i = 0; i<list.count(); i++){
                QJsonObject list_obj = list[i].toObject();
                QJsonValue date = list_obj["dt_txt"].toString();
                MainWindow::date = date.toString()/*.split(" ")[0]*/;
                MainWindow::data = date.toString().split(" ")[0];
                //main
                QJsonArray Weather = list_obj["weather"].toArray();
                QJsonObject index = Weather[0].toObject();
                QVariantMap weather_main = index.toVariantMap();
                MainWindow::weather.append(weather_main["main"].toString());
                //qDebug()<<"=>"<<MainWindow::weather.back()<<endl;
                QJsonObject Main = list_obj["main"].toObject();
                if(Main.isEmpty()){
                    qDebug()<<"Main is empity2";
                }else {
                    QVariantMap Main_data = Main.toVariantMap();
                    MainWindow::data = Main_data["temp"].toString();
                    MainWindow::temp = Main_data["temp"].toString();
                    MainWindow::Max_temp = Main_data["temp_max"].toString();
                    MainWindow::Min_temp = Main_data["temp_min"].toString();
                    table->setRowCount(MainWindow::rowcount);
                    table->setItem(MainWindow::rowcount-1,0,new QTableWidgetItem(MainWindow::date));
                    table->setItem(MainWindow::rowcount-1,1,new QTableWidgetItem(MainWindow::weather.back()));
                    table->setItem(MainWindow::rowcount-1,2,new QTableWidgetItem(MainWindow::temp));
                    table->setItem(MainWindow::rowcount-1,3,new QTableWidgetItem(MainWindow::Max_temp));
                    table->setItem(MainWindow::rowcount-1,4,new QTableWidgetItem(MainWindow::Min_temp));
                    //qDebug()<< MainWindow::rowcount <<' '<<1<<" "<<MainWindow::weather.back()<<" "<<"\n";
                    MainWindow::rowcount++;
                }
            }
        } else {
            QMessageBox::critical(this,json_map["cod"].toString()+" Erro"," Try after the few time. ",QMessageBox::Ok);
         }

        //qDebug()<<"";
    }

}

void MainWindow::on_Search_clicked()
{
    if(MainWindow::rowcount>1){
        ui->data_table->clear();
        MainWindow::rowcount = 1;
    }
    if(ui->city->text().isEmpty()){
        MainWindow::URL = "http://api.openweathermap.org/data/2.5/forecast?q=Delhi&units=metric&APPID=e0e1836d4c79d0fa264d648dc576a781&cnt=100";
    }else{
        MainWindow::URL = QString("http://api.openweathermap.org/data/2.5/forecast?q=%1&units=metric&APPID=e0e1836d4c79d0fa264d648dc576a781&cnt=100").arg(ui->city->text());
    }
    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    //MainWindow::loading.setText("Loading");
    //MainWindow::loading.exec();
    connect(man,&QNetworkAccessManager::finished,this,&MainWindow::feachdata);
    QUrl url =  QUrl(MainWindow::URL);
    QNetworkRequest requst(url);
    man->get(requst);
}

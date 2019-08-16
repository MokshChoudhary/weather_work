#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QNetworkReply>
#include <QtDebug>
#include <QMessageBox>
#include <QMainWindow>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void feachdata(QNetworkReply*);

    void on_Search_clicked();

private:
    QString URL;
    QString data;
    QString temp;
    QString Max_temp;
    QString Min_temp;
    QString WD;
    QString date;
    QStringList weather;
    QMessageBox loading;
    int rowcount;

protected:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QUuid>
#include <QDateTime>
#include <QComboBox>
#include <QList>
#include <QSqlDatabase>
#include <QMessageBox>

#include "ordercard.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class OrderCard;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_LogoutBtn_clicked();
    void on_TablesBtn_clicked();
    void on_MenuBtn_clicked();
    void on_OrdersBtn_clicked();
    void on_BillingBtn_clicked();
    void on_ReservationsBtn_clicked();
    void on_InventoryBtn_clicked();

    void addOrder(OrderCard* card);


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    int currentRow = 0;

};
#endif // MAINWINDOW_H

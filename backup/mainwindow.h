#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QUuid>
#include <QDateTime>
#include "ordercard.h"
#include <QComboBox>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QListWidget>
#include <QTableWidgetItem>

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
    void setComboBoxColor(QComboBox *comboBox, const QString &status);
    void on_LogoutBtn_clicked();
    void on_TablesBtn_clicked();
    void on_MenuBtn_clicked();
    void on_OrdersBtn_clicked();
    void on_BillingBtn_clicked();
    void on_ReservationsBtn_clicked();
    void on_InventoryBtn_clicked();
    void on_FoodAddBtn_clicked();
    void on_FoodFinalizeBtn_clicked();
    void updateTableStatusCounts();
    void on_addButton_clicked();


    void on_btn_reserve_clicked();
    void loadTableStatuses();
    void updateTableStatus(int tableId, const QString& status);
    void loadMenuItems();


    void on_tableWidget_tables_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    void addOrderCards(OrderCard* card);
    void addOrder(OrderCard* card, int id = -1);
    void initializeOrders();
    void initializeReservations();
    int currentRow = 0;
    QComboBox *Table1_Status, *Table2_Status, *Table3_Status, *Table4_Status, *Table5_Status, *Table6_Status;
    QLineEdit *availableCount, *occupiedCount, *reservedCount;



};
#endif // MAINWINDOW_H

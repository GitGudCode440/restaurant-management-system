#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QUuid>
#include <QDateTime>
#include "ordercard.h"
#include "menuitemcard.h"
#include <QComboBox>
#include <QList>
#include <QMouseEvent>
#include <QPoint>
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
    
protected:
    // Override mouse events for window dragging
    // void mousePressEvent(QMouseEvent* event) override;
    // void mouseMoveEvent(QMouseEvent* event) override;
    // void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
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
    void on_AdditemBtn_clicked();
    void on_btn_reserve_clicked();
    void on_tableStatusChanged(int tableId, const QString& newStatus);
    void initializeFoodItems();

    
    // Window control buttons
    void on_pushButton_2_clicked(); // Minimize
    void on_pushButton_3_clicked(); // Maximize/Restore
    void on_pushButton_4_clicked(); // Close
    
    void loadMenuItems();

    void on_deleteButton_clicked();

    void on_reservation_btn_delete_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    void setComboBoxColor(QComboBox *comboBox, const QString &status);
    void updateTableStatus(int tableId, const QString& status);

    void addOrder(OrderCard* card, int id = -1);
    MenuItemCard* createMenuItemCard(const QString& name, const QString& price);

    void initializeOrders();
    void initializeReservations();
    void initializeTables();
    void initializeInventory();

    int currentRow = 0;
    int orderCount = 0;

    QPoint dragPosition;
    QComboBox *Table1_Status, *Table2_Status, *Table3_Status, *Table4_Status, *Table5_Status, *Table6_Status;
    QLineEdit *availableCount, *occupiedCount, *reservedCount;

};
#endif // MAINWINDOW_H

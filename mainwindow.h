#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

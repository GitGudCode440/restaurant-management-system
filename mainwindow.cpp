#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginpage.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(QSqlDatabase::addDatabase("QMYSQL"))
{
    ui->setupUi(this);

    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("restaurant_db");
    db.setUserName("root");
    db.setPassword("12345");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Unable to connect to database");
    }

    connect(ui->BillingPage, &Billing::addOrder, this, &MainWindow::addOrder);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_TablesBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(0);
}

void MainWindow::on_MenuBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(1);
}

void MainWindow::on_OrdersBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(2);
}

void MainWindow::on_BillingBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(3);
}

void MainWindow::on_ReservationsBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(4);
}

void MainWindow::on_InventoryBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(5);
}


void MainWindow::addOrder(OrderCard* card) {

    QString orderId = QString("Order %1-%2")
                          .arg(QDateTime::currentMSecsSinceEpoch())
                          .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    card->changeOrderLabelText(orderId);
    ui->OrdersPage->addWidgetToOrderList(card);
}


void MainWindow::on_LogoutBtn_clicked()
{
    loginpage *login = new loginpage();
    login->show();

    this->close();
}

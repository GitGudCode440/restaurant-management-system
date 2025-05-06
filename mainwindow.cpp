#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginpage.h"
#include <QSqlDatabase>

#include "globals.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Globals::db = QSqlDatabase::addDatabase("QMYSQL");

    Globals::db.setHostName("127.0.0.1");
    Globals::db.setPort(3306);
    Globals::db.setDatabaseName("restaurant_db");
    Globals::db.setUserName("root");
    Globals::db.setPassword("12345");



    if (!Globals::db.open()) {
        QMessageBox::critical(nullptr, "Database Error", "Unable to connect to database");
    }


    connect(ui->BillingPage, &Billing::addOrder, this, &MainWindow::addOrder);
    connect(ui->OrdersPage, &Orders::addOrder, this, &MainWindow::addOrder);

}

MainWindow::~MainWindow()
{
    Globals::db.close();
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


void MainWindow::addOrder(OrderCard* card, int id) {


    QSqlQuery query(Globals::db);
    query.prepare(R"(
        INSERT INTO Orders (order_type, order_details, order_price)
        VALUES (:type, :details, :price)
    )");

    query.bindValue(":type", OrderCard::convertOrderTypeToQString(card->getOrderType()));
    query.bindValue(":details", card->getDetailsText());
    query.bindValue(":price", card->getOrderPrice());


    QString orderId = QString("Order #%1").arg(id);
    card->changeOrderLabelText(orderId);
    ui->OrdersPage->addWidgetToOrderList(card);
}


void MainWindow::on_LogoutBtn_clicked()
{
    loginpage *login = new loginpage();
    login->show();

    this->close();
}

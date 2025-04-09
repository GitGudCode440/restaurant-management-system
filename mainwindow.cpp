#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_TablesBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_MenuBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_OrdersBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_BillingBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_ReservationsBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_InventoryBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::addOrderCards(OrderCard* card) {
    ui->OrderListLayout->addWidget(card);
}

void MainWindow::addOrder(OrderCard* card) {

    QString orderId = QString("Order %1-%2")
                          .arg(QDateTime::currentMSecsSinceEpoch())
                          .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    card->changeOrderLabelText(orderId);
    addOrderCards(card);
}

void MainWindow::on_FoodAddBtn_clicked()
{
    QComboBox* foodItemSelect = ui->FoodItemSelect;
    QComboBox* foodQuantitySelect = ui->FoodQuantitySelect;

    ui->FoodListWidget->addItem(
        QString("%1 %2")
            .arg(foodQuantitySelect->currentText(), foodItemSelect->currentText())
    );
}


void MainWindow::on_FoodFinalizeBtn_clicked()
{
    QListWidget* listWidget = ui->FoodListWidget;
    QComboBox* deliverySelect = ui->DeliveryTypeSelect;

    OrderCard* card = new OrderCard;
    OrderType type;

    if (deliverySelect->currentIndex() >= 0 && deliverySelect->currentIndex() <= 2) {
        type = static_cast<OrderType>(deliverySelect->currentIndex());
    }
    else type = OrderType::INVALID;

    card->changeOrderType(type);

    int count = listWidget->count();
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item = listWidget->item(i);
        if (item) {
            card->addTextToListWidget(item->text());
        }
    }

    addOrder(card);

    listWidget->clear();

}

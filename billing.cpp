#include "billing.h"
#include "ui_billing.h"

#include "mainwindow.h"
#include "ordercard.h"

#include <QSqlQuery>


Billing::Billing(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::Billing)
{
    ui->setupUi(this);


}

Billing::~Billing()
{
    delete ui;
}


void Billing::on_FoodAddBtn_clicked()
{
    QComboBox* foodItemSelect = ui->FoodItemSelect;
    QComboBox* foodQuantitySelect = ui->FoodQuantitySelect;

    ui->FoodListWidget->addItem(
        QString("%1 %2")
            .arg(foodQuantitySelect->currentText(), foodItemSelect->currentText())
        );
}


void Billing::on_FoodFinalizeBtn_clicked()
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

    QSqlQuery query;

    int id;

    if (query.exec("SELECT MAX(order_id) AS last_id FROM Orders;")) {
        query.next();
        id = query.value(0).toInt();
    }
    else
        id = -1;


    emit addOrder(card, id);

    listWidget->clear();

}


#include "ordercard.h"
#include "ui_ordercard.h"

OrderCard::OrderCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OrderCard)
{
    ui->setupUi(this);
    ui->OrderDetails->hide();
}

OrderCard::~OrderCard()
{
    delete ui;
}

void OrderCard::addTextToListWidget(const QString& text) {
    QLabel* label = new QLabel(text);
    QFont font("Arial", 10);
    label->setFont(font);
    ui->DetailsList->addWidget(label);
}

void OrderCard::changeOrderLabelText(const QString& text) {
    ui->OrderLabel->setText(text);
}

void OrderCard::changeDetails(const QString& text) {
    const QString prefixStr = "Order Details:\n";
    ui->DetailsLabel->setText(prefixStr + text);
}

void OrderCard::changeOrderType(const OrderType& type) {
    const QString prefixStr = "Order Type: ";

    switch(type) {
    case OrderType::TABLE:
        ui->OrderTypeLabel->setText(prefixStr + "Table");
        break;
    case OrderType::TAKE_OUT:
        ui->OrderTypeLabel->setText(prefixStr + "Take Out");
        break;
    case OrderType::DELIVERY:
        ui->OrderTypeLabel->setText(prefixStr + "Delivery");
        break;
    default:
        ui->OrderTypeLabel->setText(prefixStr + "INVALID");
        break;
    }
}

void OrderCard::changeOrderPrice(const float price) {
    const QString prefixStr = "ORDER PRICE: ";
    ui->OrderTypeLabel->setText(prefixStr + QString::number(price));
}

void OrderCard::on_OrderDetailsBtn_clicked()
{
    if (ui->OrderDetailsBtn->isChecked()) {
        ui->OrderDetails->show();
    }
    else {
        ui->OrderDetails->hide();
    }
}


void OrderCard::on_deleteBtn_clicked()
{
    delete this;
}


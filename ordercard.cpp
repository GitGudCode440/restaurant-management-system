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
    orderLabelText = text;
    ui->OrderLabel->setText(orderLabelText);

}

void OrderCard::changeDetailsText(const QString& text) {
    const QString str = "Order Details:\n" + text;
    detailsText = text;
    ui->DetailsLabel->setText(str);
}

void OrderCard::changeOrderType(const OrderType& type) {
    const QString prefixStr = "Order Type: ";

    switch(type) {
    case OrderType::TABLE:
        orderType = OrderType::TABLE;
        ui->OrderTypeLabel->setText(prefixStr + "Table");
        break;
    case OrderType::TAKE_OUT:
        orderType = OrderType::TAKE_OUT;
        ui->OrderTypeLabel->setText(prefixStr + "Take Out");
        break;
    case OrderType::DELIVERY:
        orderType = OrderType::DELIVERY;
        ui->OrderTypeLabel->setText(prefixStr + "Delivery");
        break;
    default:
        orderType = OrderType::INVALID;
        ui->OrderTypeLabel->setText(prefixStr + "INVALID");
        break;
    }
}

void OrderCard::changeOrderPrice(const float price) {
    const QString str = "Total Price: " + QString::number(price);
    orderPrice = price;
    ui->TotalPriceLabel->setText(str);
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


OrderType OrderCard::convertQStringToOrderType(const QString& str) {
    if (str == "Table") return OrderType::TABLE;
    else if (str == "Take Out") return OrderType::TAKE_OUT;
    else if (str == "Delivery") return OrderType::DELIVERY;
    else return OrderType::INVALID;
}

QString OrderCard::convertOrderTypeToQString(const OrderType& type) {
    if (type == OrderType::TABLE) return "Table";
    else if (type == OrderType::TAKE_OUT) return "Take Out";
    else if (type == OrderType::DELIVERY) return "Delivery";
    else return "Invalid";
}

void OrderCard::changeOrderStatus(const QString& status) {
    if (status == "Delivered") ui->OrderStatusSelect->setCurrentIndex(2);
    else if (status == "Out for Delivery") ui->OrderStatusSelect->setCurrentIndex(1);
    else ui->OrderStatusSelect->setCurrentIndex(0);
}



void OrderCard::on_deleteBtn_clicked()
{
    QSqlQuery query("DELETE FROM Orders WHERE order_id = ?;");
    query.addBindValue(orderId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", QString("Failed to delete order: %1").arg(query.lastError().text()));
        return;
    }

    this->deleteLater();
}


void OrderCard::on_OrderStatusSelect_currentIndexChanged(int index)
{
    QSqlQuery query("UPDATE Orders SET order_status = ? WHERE order_id = ?");
    QString orderStatus;

    switch(index) {
    case 0: orderStatus = "Being Prepared"; break;
    case 1: orderStatus = "Out for Delivery"; break;
    case 2: orderStatus = "Delivered"; break;
    }

    query.addBindValue(orderStatus);
    query.addBindValue(orderId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", QString("Failed to modify status of order: %1").arg(query.lastError().text()));
        return;
    }
}


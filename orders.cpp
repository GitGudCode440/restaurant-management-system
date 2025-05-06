#include "orders.h"
#include "ordercard.h"
#include "globals.h"

#include "ui_orders.h"


Orders::Orders(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Orders)
{
    ui->setupUi(this);


    QTimer::singleShot(0, this, [this]() {
        initializeOrders();
    });

}

void Orders::initializeOrders() {
    QSqlQuery query(Globals::db);

    if (query.exec("SELECT * FROM Orders;")) {

        std::cout << "Nice!" << std::endl;

        while(query.next()) {
            int id = query.value(0).toInt();
            QString orderType = query.value(1).toString();
            QString orderDetails = query.value(2).toString();
            float orderPrice = query.value(3).toFloat();


            OrderCard* card = new OrderCard;
            card->changeOrderType(OrderCard::convertQStringToOrderType(orderType));
            card->changeOrderPrice(orderPrice);
            card->addTextToListWidget(orderDetails);

            emit addOrder(card, id);

        }
    }
}

Orders::~Orders()
{
    delete ui;
}

void Orders::addWidgetToOrderList(QWidget* widget) {
    ui->OrderListLayout->addWidget(widget);

}

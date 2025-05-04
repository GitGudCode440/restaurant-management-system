#include "orders.h"
#include "ui_orders.h"

Orders::Orders(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Orders)
{
    ui->setupUi(this);
}

Orders::~Orders()
{
    delete ui;
}

void Orders::addWidgetToOrderList(QWidget* widget) {
    ui->OrderListLayout->addWidget(widget);
}

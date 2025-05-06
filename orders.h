#ifndef ORDERS_H
#define ORDERS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTimer>
#include <iostream>



QT_BEGIN_NAMESPACE
namespace Ui {
class Orders;
}
QT_END_NAMESPACE

class OrderCard;

class Orders : public QWidget
{
    Q_OBJECT


public:
    explicit Orders(QWidget *parent = nullptr);
    ~Orders();

    void addWidgetToOrderList(QWidget* widget);

signals:
    void addOrder(OrderCard* card, int id = -1);

private:
    Ui::Orders *ui;

    void initializeOrders();

};


#endif // ORDERS_H

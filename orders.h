#ifndef ORDERS_H
#define ORDERS_H

#include <QWidget>

namespace Ui {
class Orders;
}

class Orders : public QWidget
{
    Q_OBJECT

public:
    explicit Orders(QWidget *parent = nullptr);
    ~Orders();

    void addWidgetToOrderList(QWidget* widget);

private:
    Ui::Orders *ui;
};

#endif // ORDERS_H

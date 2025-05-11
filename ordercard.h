#ifndef ORDERCARD_H
#define ORDERCARD_H

#include <QWidget>

namespace Ui {
class OrderCard;
}

enum class OrderType {
    TABLE,
    TAKE_OUT,
    DELIVERY,
    INVALID
};

class OrderCard : public QWidget
{
    Q_OBJECT

public:
    explicit OrderCard(QWidget *parent = nullptr);
    ~OrderCard();

    void addTextToListWidget(const QString& text);

    void changeOrderLabelText(const QString& text);
    void changeDetails(const QString& text);
    void changeOrderType(const OrderType& type);
    void changeOrderPrice(const float price);

private slots:
    void on_OrderDetailsBtn_clicked();

    void on_deleteBtn_clicked();

private:
    Ui::OrderCard *ui;
};

#endif // ORDERCARD_H

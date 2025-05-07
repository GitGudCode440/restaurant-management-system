#ifndef ORDERCARD_H
#define ORDERCARD_H

#include <QWidget>
#include <QSqlError>

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

    QString getOrderLabelText() const { return orderLabelText; }
    QString getDetailsText() const { return detailsText; }
    OrderType getOrderType() const { return orderType; }
    float getOrderPrice() const { return orderPrice; }
    int getOrderId() const { return orderId; }

    void changeOrderLabelText(const QString& text);
    void changeDetailsText(const QString& text);
    void changeOrderType(const OrderType& type);
    void changeOrderPrice(const float price);
    void setOrderId(int id) { orderId = id; }

    static OrderType convertQStringToOrderType(const QString& str);
    static QString convertOrderTypeToQString(const OrderType& type);

private slots:
    void on_OrderDetailsBtn_clicked();

    void on_deleteBtn_clicked();

private:
    Ui::OrderCard *ui;

    QString orderLabelText;
    QString detailsText;
    OrderType orderType;
    float orderPrice;
    int orderId = -1;  // -1 indicates no valid order ID
};

#endif // ORDERCARD_H

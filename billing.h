#ifndef BILLING_H
#define BILLING_H

#include <QWidget>


namespace Ui {
class Billing;
}

class MainWindow;
class OrderCard;

class Billing : public QWidget
{
    Q_OBJECT

public:
    explicit Billing(QWidget* parent = nullptr);
    ~Billing();

signals:
    void addOrder(OrderCard* card, int id);

private slots:
    void on_FoodAddBtn_clicked();
    void on_FoodFinalizeBtn_clicked();

private:
    Ui::Billing *ui;
};

#endif // BILLING_H

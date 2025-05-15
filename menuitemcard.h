#ifndef MENUITEMCARD_H
#define MENUITEMCARD_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QMouseEvent>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <iostream>

class MenuItemCard : public QWidget
{
    Q_OBJECT

public:
    explicit MenuItemCard(const QString &name, const QString &price, QWidget *parent = nullptr);
    ~MenuItemCard();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QLabel *nameLabel;
    QLabel *priceLabel;
    QFrame *cardFrame;

signals:
    void initializeFoodItemsInBillingPage();
};

#endif // MENUITEMCARD_H 

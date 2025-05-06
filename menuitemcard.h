#ifndef MENUITEMCARD_H
#define MENUITEMCARD_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

class MenuItemCard : public QWidget
{
    Q_OBJECT

public:
    explicit MenuItemCard(const QString &name, const QString &description, const QString &price, QWidget *parent = nullptr);
    ~MenuItemCard();

private:
    QLabel *nameLabel;
    QLabel *priceLabel;
    QFrame *cardFrame;
};

#endif // MENUITEMCARD_H 
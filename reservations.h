#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include <QWidget>

namespace Ui {
class Reservations;
}

class Reservations : public QWidget
{
    Q_OBJECT

public:
    explicit Reservations(QWidget *parent = nullptr);
    ~Reservations();

private slots:
    void on_btn_reserve_clicked();

private:
    Ui::Reservations *ui;

    int currentRow;
};

#endif // RESERVATIONS_H

#include "reservations.h"
#include "ui_reservations.h"

Reservations::Reservations(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Reservations)
    , currentRow(0)
{
    ui->setupUi(this);
}

Reservations::~Reservations()
{
    delete ui;
}

void Reservations::on_btn_reserve_clicked()
{
    QDateEdit* dateEdit = ui->dateEdit_date;
    QTimeEdit* timeEdit = ui->timeEdit_time;
    QLineEdit* reservationName = ui->reservation_name;
    QLineEdit* reservationContact = ui->reservation_contact;
    QSpinBox* spinBox = ui->spinBox_guests;
    QComboBox* tableNo = ui->comboBox_selectTable;

    QTableWidget* table = ui->tableWidget_tables;
    QTableWidgetItem* tableItem;

    table->insertRow(currentRow);

    for (int i = 0; i < 6; i++) {
        switch(i) {
        case 0: tableItem = new QTableWidgetItem(reservationName->text()); break;
        case 1: tableItem = new QTableWidgetItem(tableNo->currentText()); break;
        case 2: tableItem = new QTableWidgetItem(dateEdit->text()); break;
        case 3: tableItem = new QTableWidgetItem(timeEdit->text()); break;
        case 4: tableItem = new QTableWidgetItem(spinBox->text()); break;
        case 5: tableItem = new QTableWidgetItem(reservationContact->text()); break;
        }

        table->setItem(currentRow, i, tableItem);
    }

    currentRow++;
}


#include "tables.h"
#include "ui_tables.h"

Tables::Tables(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tables)
{
    ui->setupUi(this);


    QList<QComboBox*> combos = ui->TableGrid->findChildren<QComboBox*>();

    for(QComboBox* combo : combos) {
        connect(
            combo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            [this, combo](int index) {
                setComboBoxColor(combo, combo->itemText(index));
            }
            );

    }
}

void Tables::setComboBoxColor(QComboBox *comboBox, const QString &status) {

    if (status == "Available") {
        comboBox->setStyleSheet("QComboBox { background-color: green;  }");
    } else if (status == "Occupied") {
        comboBox->setStyleSheet("QComboBox { background-color: red; }");
    } else if (status == "Reserved") {
        comboBox->setStyleSheet("QComboBox { background-color: orange; }");
    } else {
        comboBox->setStyleSheet("");
    }
}

Tables::~Tables()
{
    delete ui;
}

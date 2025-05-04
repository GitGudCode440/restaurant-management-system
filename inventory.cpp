#include "inventory.h"
#include "ui_inventory.h"

Inventory::Inventory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Inventory)
{
    ui->setupUi(this);
}

Inventory::~Inventory()
{
    delete ui;
}

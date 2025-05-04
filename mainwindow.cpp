#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginpage.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the signal to the slot
    connect(ui->Table1_Status, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        QString status = ui->Table1_Status->currentText();
        setComboBoxColor(ui->Table1_Status, status);
    });

    // Initialize combo boxes and line edits
    Table1_Status = ui->Table1_Status;
    Table2_Status = ui->Table2_Status;
    Table3_Status = ui->Table3_Status;
    Table4_Status = ui->Table4_Status;
    Table5_Status = ui->Table5_Status;
    Table6_Status = ui->Table6_Status;

    availableCount = ui->availableCount;
    occupiedCount = ui->occupiedCount;
    reservedCount = ui->reservedCount;

    // Connect the combo box changes to the update function
    connect(Table1_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table2_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table3_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table4_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table5_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table6_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));

    // Initialize the count display
    updateTableStatusCounts();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_TablesBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(0);
}

void MainWindow::on_MenuBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(1);
}

void MainWindow::on_OrdersBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(2);
}

void MainWindow::on_BillingBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(3);
}

void MainWindow::on_ReservationsBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(4);
}

void MainWindow::on_InventoryBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(5);
}

void MainWindow::addOrderCards(OrderCard* card) {
    ui->OrderListLayout->addWidget(card);
}

void MainWindow::addOrder(OrderCard* card) {

    QString orderId = QString("Order %1-%2")
                          .arg(QDateTime::currentMSecsSinceEpoch())
                          .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    card->changeOrderLabelText(orderId);
    addOrderCards(card);
}

void MainWindow::on_FoodAddBtn_clicked()
{
    QComboBox* foodItemSelect = ui->FoodItemSelect;
    QComboBox* foodQuantitySelect = ui->FoodQuantitySelect;

    ui->FoodListWidget->addItem(
        QString("%1 %2")
            .arg(foodQuantitySelect->currentText(), foodItemSelect->currentText())
    );
}


void MainWindow::on_FoodFinalizeBtn_clicked()
{
    QListWidget* listWidget = ui->FoodListWidget;
    QComboBox* deliverySelect = ui->DeliveryTypeSelect;

    OrderCard* card = new OrderCard;
    OrderType type;

    if (deliverySelect->currentIndex() >= 0 && deliverySelect->currentIndex() <= 2) {
        type = static_cast<OrderType>(deliverySelect->currentIndex());
    }
    else type = OrderType::INVALID;

    card->changeOrderType(type);

    int count = listWidget->count();
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item = listWidget->item(i);
        if (item) {
            card->addTextToListWidget(item->text());
        }
    }

    addOrder(card);

    listWidget->clear();

}


void MainWindow::on_LogoutBtn_clicked()
{
    // Create a new instance of loginpage
    loginpage *login = new loginpage();
    login->show();

    // Close the main window
    this->close();
}

void MainWindow::setComboBoxColor(QComboBox *comboBox, const QString &status)
{
    if (status == "Available") {
        comboBox->setStyleSheet("QComboBox { background-color: green;  }");
    } else if (status == "Occupied") {
        comboBox->setStyleSheet("QComboBox { background-color: red; ");
    } else if (status == "Reserved") {
        comboBox->setStyleSheet("QComboBox { background-color: orange; ");
    } else {
        comboBox->setStyleSheet(""); // Reset to default
    }
}

void MainWindow::on_btn_reserve_clicked()
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
void MainWindow::on_addButton_clicked()
{
    // Get references to your input fields
    QLineEdit* itemNameEdit = ui->itemNameEdit;
    QLineEdit* categoryEdit = ui->categoryEdit;
    QLineEdit* quantityEdit = ui->quantityEdit;
    QComboBox* statusCombo = ui->statusCombo;
    QTableWidget* table = ui->inventoryTable;

    // Create new items for the table
    QTableWidgetItem* tableItem;

    // Add a new row
    int currentRow = table->rowCount();
    table->insertRow(currentRow);

    // Fill in the cells
    for (int i = 0; i < 4; i++) {
        switch(i) {
        case 0: tableItem = new QTableWidgetItem(itemNameEdit->text()); break;
        case 1: tableItem = new QTableWidgetItem(categoryEdit->text()); break;
        case 2: tableItem = new QTableWidgetItem(quantityEdit->text()); break;
        case 3: tableItem = new QTableWidgetItem(statusCombo->currentText()); break;
        }
        table->setItem(currentRow, i, tableItem);
    }

    // Clear input fields after adding
    itemNameEdit->clear();
    categoryEdit->clear();
    quantityEdit->clear();
    statusCombo->setCurrentIndex(0); // Reset to default value
}

void MainWindow::updateTableStatusCounts()
{
    int available = 0;
    int occupied = 0;
    int reserved = 0;

    // Check the status of each table and update counts
    if (Table1_Status->currentText() == "Available") available++;
    if (Table1_Status->currentText() == "Occupied") occupied++;
    if (Table1_Status->currentText() == "Reserved") reserved++;

    if (Table2_Status->currentText() == "Available") available++;
    if (Table2_Status->currentText() == "Occupied") occupied++;
    if (Table2_Status->currentText() == "Reserved") reserved++;

    if (Table3_Status->currentText() == "Available") available++;
    if (Table3_Status->currentText() == "Occupied") occupied++;
    if (Table3_Status->currentText() == "Reserved") reserved++;

    if (Table4_Status->currentText() == "Available") available++;
    if (Table4_Status->currentText() == "Occupied") occupied++;
    if (Table4_Status->currentText() == "Reserved") reserved++;

    if (Table5_Status->currentText() == "Available") available++;
    if (Table5_Status->currentText() == "Occupied") occupied++;
    if (Table5_Status->currentText() == "Reserved") reserved++;

    if (Table6_Status->currentText() == "Available") available++;
    if (Table6_Status->currentText() == "Occupied") occupied++;
    if (Table6_Status->currentText() == "Reserved") reserved++;

    // Update the line edits with the new counts
    availableCount->setText(QString::number(available));
    occupiedCount->setText(QString::number(occupied));
    reservedCount->setText(QString::number(reserved));
}


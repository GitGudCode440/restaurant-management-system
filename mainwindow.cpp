#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginpage.h"

#include <iostream>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "Starting MainWindow constructor";
    qDebug() << "Available database drivers:" << QSqlDatabase::drivers();
    
    ui->setupUi(this);


    qDebug() << "Setting up database connection";
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("restaurant_db");
    db.setUserName("root");
    db.setPassword("12345");

    qDebug() << "Attempting to open database connection";
    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Unable to connect to database: %1").arg(db.lastError().text()));
        return;
    }
    qDebug() << "Database connection successful";

    QSqlQuery testQuery(db);
    qDebug() << "Testing database connection";
    if (!testQuery.exec("SELECT 1")) {
        qDebug() << "Database connection test failed:" << testQuery.lastError().text();
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Database connection test failed: %1").arg(testQuery.lastError().text()));
        return;
    }
    qDebug() << "Database connection test successful";

    QSqlQuery query(db);
    
    qDebug() << "Creating Tables table if not exists";
    if (!query.exec("CREATE TABLE IF NOT EXISTS Tables ("
                   "table_id INT PRIMARY KEY, "
                   "status VARCHAR(20) NOT NULL)")) {
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Failed to create Tables table: %1").arg(query.lastError().text()));
        return;
    }
    qDebug() << "Tables table created/verified";
    
    if (!query.exec("CREATE TABLE IF NOT EXISTS Orders ("
                   "order_id INT AUTO_INCREMENT PRIMARY KEY, "
                   "order_type VARCHAR(20) NOT NULL, "
                   "order_details TEXT NOT NULL, "
                   "order_price FLOAT NOT NULL, "
                   "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)")) {
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Failed to create Orders table: %1").arg(query.lastError().text()));
        return;
    }
    qDebug() << "Orders table created/verified";
    
    for (int i = 1; i <= 6; i++) {
        query.prepare("INSERT IGNORE INTO Tables (table_id, status) VALUES (?, 'Available')");
        query.addBindValue(i);
        if (!query.exec()) {
            QMessageBox::warning(nullptr, "Database Warning", 
                QString("Failed to initialize table %1: %2").arg(i).arg(query.lastError().text()));
        }
    }
    qDebug() << "Table statuses initialized";
    
    qDebug() << "Setting up UI elements";
    Table1_Status = ui->Table1_Status;
    Table2_Status = ui->Table2_Status;
    Table3_Status = ui->Table3_Status;
    Table4_Status = ui->Table4_Status;
    Table5_Status = ui->Table5_Status;
    Table6_Status = ui->Table6_Status;

    availableCount = ui->availableCount;
    occupiedCount = ui->occupiedCount;
    reservedCount = ui->reservedCount;

    qDebug() << "Connecting signals and slots";
    connect(Table1_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table2_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table3_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table4_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table5_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));
    connect(Table6_Status, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTableStatusCounts()));

    qDebug() << "Setting up combo box colors";
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

    qDebug() << "Loading initial table statuses";
    loadTableStatuses();
    qDebug() << "Initializing orders";
    initializeOrders();
    qDebug() << "Initializing reservations";
    initializeReservations();
    qDebug() << "Initializing inventory";
    initializeInventory();
    qDebug() << "Initializing menu items container";
    initializeMenuItems();
    qDebug() << "MainWindow constructor completed";
}

void MainWindow::initializeOrders() {
    QSqlQuery query(db);

    if (query.exec("SELECT * FROM Orders;")) {
        std::cout << "Nice" << std::endl;

        while(query.next()) {
            int id = query.value(0).toInt();
            QString orderType = query.value(1).toString();
            QString orderDetails = query.value(2).toString();
            float orderPrice = query.value(3).toFloat();


            OrderCard* card = new OrderCard;
            card->changeOrderType(OrderCard::convertQStringToOrderType(orderType));
            card->changeOrderPrice(orderPrice);
            card->addTextToListWidget(orderDetails);

            addOrder(card, id);
        }
    }
}

void MainWindow::initializeReservations() {
    QSqlQuery query(db);
    
    if (query.exec("SELECT * FROM Reservations ORDER BY reservation_date, reservation_time;")) {
        while(query.next()) {
            QString customerName = query.value(1).toString();
            int tableId = query.value(2).toInt();
            QDate date = query.value(3).toDate();
            QTime time = query.value(4).toTime();
            int guests = query.value(5).toInt();
            QString contact = query.value(6).toString();

            QTableWidget* table = ui->tableWidget_tables;

            table->insertRow(currentRow);

            table->setItem(currentRow, 0, new QTableWidgetItem(customerName));
            table->setItem(currentRow, 1, new QTableWidgetItem(QString("Table %1").arg(tableId)));
            table->setItem(currentRow, 2, new QTableWidgetItem(date.toString("yyyy-MM-dd")));
            table->setItem(currentRow, 3, new QTableWidgetItem(time.toString("hh:mm")));
            table->setItem(currentRow, 4, new QTableWidgetItem(QString::number(guests)));
            table->setItem(currentRow, 5, new QTableWidgetItem(contact));

            currentRow++;
        }
    } else {
        QMessageBox::warning(nullptr, "Database Warning", 
            QString("Failed to load reservations: %1").arg(query.lastError().text()));
    }
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::on_TablesBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(0);
}

void MainWindow::on_MenuBtn_clicked()
{
    ui->NavigationTabs->setCurrentIndex(1);
    loadMenuItems();
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
    loadInventoryItems();
}

void MainWindow::addOrderCards(OrderCard* card) {
    ui->OrderListLayout->addWidget(card);
}

void MainWindow::addOrder(OrderCard* card, int id) {
    QString orderId = QString("Order #%1").arg(id);
    card->changeOrderLabelText(orderId);
    card->setOrderId(id);
    ui->OrderListLayout->addWidget(card);
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

    QString str = "";
    int count = listWidget->count();
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item = listWidget->item(i);
        str += item->text() + ", ";

    }
    card->changeDetailsText(str);
    card->changeOrderPrice(0);

    addOrder(card);

    QSqlQuery query(db);
    
    if (!query.prepare(
        "INSERT INTO Orders (order_type, order_details, order_price) "
        "VALUES (:type, :details, :price);"
    )) {
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Failed to prepare order insertion: %1").arg(query.lastError().text()));
        return;
    }

    query.bindValue(":type", OrderCard::convertOrderTypeToQString(card->getOrderType()));
    query.bindValue(":details", card->getDetailsText());
    query.bindValue(":price", card->getOrderPrice());

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Failed to insert order: %1").arg(query.lastError().text()));
        return;
    }

    int newOrderId = query.lastInsertId().toInt();
    
    addOrder(card, newOrderId);

    listWidget->clear();
}


void MainWindow::on_LogoutBtn_clicked()
{
    loginpage *login = new loginpage();
    login->show();

    this->close();
}

void MainWindow::setComboBoxColor(QComboBox *comboBox, const QString &status)
{
    if (status == "Available") {
        comboBox->setStyleSheet("QComboBox { background-color: green;  }");
    } else if (status == "Occupied") {
        comboBox->setStyleSheet("QComboBox { background-color: red; }");
    } else if (status == "Reserved") {
        comboBox->setStyleSheet("QComboBox { background-color: orange; } ");
    } else {
        comboBox->setStyleSheet("");
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

    QString tableText = tableNo->currentText();
    int tableId = tableText.split(" ").last().toInt();
    
    if (tableId < 1 || tableId > 6) {
        QMessageBox::warning(this, "Warning", "Invalid table number selected");
        return;
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT status FROM Tables WHERE table_id = ?");
    checkQuery.addBindValue(tableId);
    
    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::critical(this, "Error", "Failed to check table status: " + checkQuery.lastError().text());
        return;
    }

    QString currentStatus = checkQuery.value(0).toString();
    if (currentStatus != "Available") {
        QMessageBox::warning(this, "Warning", 
            QString("Table %1 is currently %2. Please select another table.").arg(tableId).arg(currentStatus));
        return;
    }

    QDateTime reservationDateTime(dateEdit->date(), timeEdit->time());
    QDateTime currentDateTime = QDateTime::currentDateTime();

    QString tableStatus;
    if (reservationDateTime <= currentDateTime) {
        tableStatus = "Occupied";
    } else {
        tableStatus = "Reserved";
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO Reservations (customer_name, table_id, reservation_date, reservation_time, number_of_guests, contact_number) "
                       "VALUES (?, ?, ?, ?, ?, ?)");
    
    insertQuery.addBindValue(reservationName->text());
    insertQuery.addBindValue(tableId);
    insertQuery.addBindValue(dateEdit->date());
    insertQuery.addBindValue(timeEdit->time());
    insertQuery.addBindValue(spinBox->value());
    insertQuery.addBindValue(reservationContact->text());

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to store reservation: " + insertQuery.lastError().text());
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE Tables SET status = ? WHERE table_id = ?");
    query.addBindValue(tableStatus);
    query.addBindValue(tableId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update table status: " + query.lastError().text());
        return;
    }

    QComboBox* tableStatusCombo = nullptr;
    switch (tableId) {
        case 1: tableStatusCombo = Table1_Status; break;
        case 2: tableStatusCombo = Table2_Status; break;
        case 3: tableStatusCombo = Table3_Status; break;
        case 4: tableStatusCombo = Table4_Status; break;
        case 5: tableStatusCombo = Table5_Status; break;
        case 6: tableStatusCombo = Table6_Status; break;
    }

    if (tableStatusCombo) {
        int index = tableStatusCombo->findText(tableStatus);
        if (index >= 0) {
            tableStatusCombo->setCurrentIndex(index);
            setComboBoxColor(tableStatusCombo, tableStatus);
        }
    }

    QTableWidget* table = ui->tableWidget_tables;

    table->insertRow(currentRow);

    for (int i = 0; i < 6; i++) {
        switch(i) {
        case 0: table->setItem(currentRow, i, new QTableWidgetItem(reservationName->text())); break;
        case 1: table->setItem(currentRow, i, new QTableWidgetItem(tableNo->currentText())); break;
        case 2: table->setItem(currentRow, i, new QTableWidgetItem(dateEdit->text())); break;
        case 3: table->setItem(currentRow, i, new QTableWidgetItem(timeEdit->text())); break;
        case 4: table->setItem(currentRow, i, new QTableWidgetItem(spinBox->text())); break;
        case 5: table->setItem(currentRow, i, new QTableWidgetItem(reservationContact->text())); break;
        }
    }

    currentRow++;

    updateTableStatusCounts();

    reservationName->clear();
    reservationContact->clear();
    spinBox->setValue(1);
    dateEdit->setDate(QDate::currentDate());
    timeEdit->setTime(QTime::currentTime());
    tableNo->setCurrentIndex(0);

    QMessageBox::information(this, "Success", "Reservation added successfully");
}

void MainWindow::on_addButton_clicked()
{
    QLineEdit* itemNameEdit = ui->itemNameEdit;
    QLineEdit* categoryEdit = ui->categoryEdit;
    QLineEdit* quantityEdit = ui->quantityEdit;

    if (itemNameEdit->text().isEmpty() || quantityEdit->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Input Error", "Please fill in name and price fields");
        return;
    }

    bool ok;
    float price = quantityEdit->text().toFloat(&ok);
    if (!ok || price <= 0) {
        QMessageBox::warning(nullptr, "Input Error", "Please enter a valid price");
        return;
    }

    QString itemName = itemNameEdit->text();
    QString category = categoryEdit->text().isEmpty() ? "Uncategorized" : categoryEdit->text();
    QString priceText = QString::number(price, 'f', 2);

    MenuItemCard* card = new MenuItemCard(itemName, category, priceText);
    menuItemsLayout->addWidget(card);
    
    clearMenuInputs();
}

void MainWindow::clearMenuInputs()
{
    ui->itemNameEdit->clear();
    ui->categoryEdit->clear();
    ui->quantityEdit->clear();
}

void MainWindow::loadTableStatuses() {
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection is not open");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT table_id, status FROM Tables ORDER BY table_id");
    
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Database Warning", 
            QString("Failed to load table statuses: %1").arg(query.lastError().text()));
        return;
    }

    while (query.next()) {
        int tableId = query.value(0).toInt();
        QString status = query.value(1).toString();
        
        QComboBox* tableStatus = nullptr;
        switch (tableId) {
            case 1: tableStatus = Table1_Status; break;
            case 2: tableStatus = Table2_Status; break;
            case 3: tableStatus = Table3_Status; break;
            case 4: tableStatus = Table4_Status; break;
            case 5: tableStatus = Table5_Status; break;
            case 6: tableStatus = Table6_Status; break;
        }
        
        if (tableStatus) {
            int index = tableStatus->findText(status);
            if (index >= 0) {
                tableStatus->setCurrentIndex(index);
                setComboBoxColor(tableStatus, status);
            }
        }
    }
}

void MainWindow::updateTableStatus(int tableId, const QString& status) {
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection is not open");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE Tables SET status = ? WHERE table_id = ?");
    query.addBindValue(status);
    query.addBindValue(tableId);
    
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Database Warning", 
            QString("Failed to update table %1 status: %2").arg(tableId).arg(query.lastError().text()));
    }
}

void MainWindow::updateTableStatusCounts()
{
    int available = 0;
    int occupied = 0;
    int reserved = 0;

    if (Table1_Status->currentText() == "Available") available++;
    if (Table1_Status->currentText() == "Occupied") occupied++;
    if (Table1_Status->currentText() == "Reserved") reserved++;
    updateTableStatus(1, Table1_Status->currentText());

    if (Table2_Status->currentText() == "Available") available++;
    if (Table2_Status->currentText() == "Occupied") occupied++;
    if (Table2_Status->currentText() == "Reserved") reserved++;
    updateTableStatus(2, Table2_Status->currentText());

    if (Table3_Status->currentText() == "Available") available++;
    if (Table3_Status->currentText() == "Occupied") occupied++;
    if (Table3_Status->currentText() == "Reserved") reserved++;
    updateTableStatus(3, Table3_Status->currentText());

    if (Table4_Status->currentText() == "Available") available++;
    if (Table4_Status->currentText() == "Occupied") occupied++;
    if (Table4_Status->currentText() == "Reserved") reserved++;
    updateTableStatus(4, Table4_Status->currentText());

    if (Table5_Status->currentText() == "Available") available++;
    if (Table5_Status->currentText() == "Occupied") occupied++;
    if (Table5_Status->currentText() == "Reserved") reserved++;
    updateTableStatus(5, Table5_Status->currentText());

    if (Table6_Status->currentText() == "Available") available++;
    if (Table6_Status->currentText() == "Occupied") occupied++;
    if (Table6_Status->currentText() == "Reserved") reserved++;
    updateTableStatus(6, Table6_Status->currentText());

    availableCount->setText(QString::number(available));
    occupiedCount->setText(QString::number(occupied));
    reservedCount->setText(QString::number(reserved));
}

void MainWindow::loadMenuItems()
{
    QSqlQuery query(db);
    if (!query.exec("SELECT item_name, category, price, description FROM Menu ORDER BY category, item_name")) {
        QMessageBox::warning(nullptr, "Database Error", 
            QString("Failed to load menu items: %1").arg(query.lastError().text()));
        return;
    }

    ui->FoodListWidget->clear();

    QString currentCategory;
    while (query.next()) {
        QString itemName = query.value(0).toString();
        QString category = query.value(1).toString();
        float price = query.value(2).toFloat();
        QString description = query.value(3).toString();

        if (category != currentCategory) {
            currentCategory = category;
            QListWidgetItem* categoryItem = new QListWidgetItem(category);
            categoryItem->setBackground(QColor("#E0E0E0"));
            categoryItem->setFlags(categoryItem->flags() & ~Qt::ItemIsSelectable);
            ui->FoodListWidget->addItem(categoryItem);
        }

        QString itemText = QString("%1 - $%2").arg(itemName).arg(price, 0, 'f', 2);
        if (!description.isEmpty()) {
            itemText += QString("\n    %1").arg(description);
        }

        QListWidgetItem* item = new QListWidgetItem(itemText);
        ui->FoodListWidget->addItem(item);
    }
}

void MainWindow::on_tableWidget_tables_itemDoubleClicked(QTableWidgetItem *item)
{
    int row = item->row();
    
    QString tableText = ui->tableWidget_tables->item(row, 1)->text();
    int tableId = tableText.split(" ").last().toInt();

    QString customerName = ui->tableWidget_tables->item(row, 0)->text();
    QDate date = QDate::fromString(ui->tableWidget_tables->item(row, 2)->text(), "yyyy-MM-dd");
    QTime time = QTime::fromString(ui->tableWidget_tables->item(row, 3)->text(), "hh:mm");

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Deletion",
        "Are you sure you want to delete this reservation?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM Reservations WHERE customer_name = ? AND table_id = ? AND reservation_date = ? AND reservation_time = ?");
    deleteQuery.addBindValue(customerName);
    deleteQuery.addBindValue(tableId);
    deleteQuery.addBindValue(date);
    deleteQuery.addBindValue(time);

    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to delete reservation from database: " + deleteQuery.lastError().text());
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE Tables SET status = 'Available' WHERE table_id = ?");
    query.addBindValue(tableId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update table status: " + query.lastError().text());
        return;
    }

    QComboBox* tableStatusCombo = nullptr;
    switch (tableId) {
        case 1: tableStatusCombo = Table1_Status; break;
        case 2: tableStatusCombo = Table2_Status; break;
        case 3: tableStatusCombo = Table3_Status; break;
        case 4: tableStatusCombo = Table4_Status; break;
        case 5: tableStatusCombo = Table5_Status; break;
        case 6: tableStatusCombo = Table6_Status; break;
    }

    if (tableStatusCombo) {
        int index = tableStatusCombo->findText("Available");
        if (index >= 0) {
            tableStatusCombo->setCurrentIndex(index);
            setComboBoxColor(tableStatusCombo, "Available");
        }
    }

    ui->tableWidget_tables->removeRow(row);

    updateTableStatusCounts();

    QMessageBox::information(this, "Success", "Reservation deleted successfully");
}

void MainWindow::initializeInventory() {
    ui->inventoryTable->setColumnCount(5);
    ui->inventoryTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Item Name" << "Category" << "Quantity" << "Status");
    ui->inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->inventoryTable->setColumnHidden(0, true);
    
    QPushButton* inventoryAddBtn = ui->addButton;
    if (inventoryAddBtn) {
        disconnect(inventoryAddBtn, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
        connect(inventoryAddBtn, SIGNAL(clicked()), this, SLOT(on_inventoryAddButton_clicked()));
    }
    
    loadInventoryItems();
    
    clearInventoryInputs();
}

void MainWindow::loadInventoryItems() {
    ui->inventoryTable->setRowCount(0);
    
    QSqlQuery query(db);
    if (query.exec("SELECT inventory_id, item_name, category, quantity, status FROM Inventory ORDER BY category, item_name")) {
        int row = 0;
        while (query.next()) {
            ui->inventoryTable->insertRow(row);
            
            ui->inventoryTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            
            ui->inventoryTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->inventoryTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->inventoryTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            ui->inventoryTable->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
            
            QTableWidgetItem* statusItem = ui->inventoryTable->item(row, 4);
            if (query.value(4).toString() == "In Stock") {
                statusItem->setBackground(QColor("#c8e6c9"));
            } else if (query.value(4).toString() == "Low Stock") {
                statusItem->setBackground(QColor("#ffecb3"));
            }
            
            row++;
        }
    } else {
        QMessageBox::warning(this, "Database Error", 
            QString("Failed to load inventory items: %1").arg(query.lastError().text()));
    }
}

void MainWindow::on_inventoryTable_itemClicked(QTableWidgetItem *item) {
    int row = item->row();
    
    currentSelectedInventoryId = ui->inventoryTable->item(row, 0)->text().toInt();
    
    ui->itemNameEdit->setText(ui->inventoryTable->item(row, 1)->text());
    ui->categoryEdit->setText(ui->inventoryTable->item(row, 2)->text());
    ui->quantityEdit->setText(ui->inventoryTable->item(row, 3)->text());
    
    QString status = ui->inventoryTable->item(row, 4)->text();
    int statusIndex = ui->statusCombo->findText(status);
    if (statusIndex >= 0) {
        ui->statusCombo->setCurrentIndex(statusIndex);
    }
}

void MainWindow::clearInventoryInputs() {
    ui->itemNameEdit->clear();
    ui->categoryEdit->clear();
    ui->quantityEdit->clear();
    ui->statusCombo->setCurrentIndex(0);
    currentSelectedInventoryId = -1;
}

void MainWindow::on_inventoryAddButton_clicked() {
    QString itemName = ui->itemNameEdit->text().trimmed();
    QString category = ui->categoryEdit->text().trimmed();
    QString quantityText = ui->quantityEdit->text().trimmed();
    QString status = ui->statusCombo->currentText();
    
    if (itemName.isEmpty() || category.isEmpty() || quantityText.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please fill in all fields.");
        return;
    }
    
    bool ok;
    int quantity = quantityText.toInt(&ok);
    if (!ok || quantity < 0) {
        QMessageBox::warning(this, "Validation Error", "Quantity must be a positive number.");
        return;
    }
    
    QSqlQuery query(db);
    
    if (currentSelectedInventoryId > 0) {
        query.prepare("UPDATE Inventory SET item_name = ?, category = ?, quantity = ?, status = ? WHERE inventory_id = ?");
        query.addBindValue(itemName);
        query.addBindValue(category);
        query.addBindValue(quantity);
        query.addBindValue(status);
        query.addBindValue(currentSelectedInventoryId);
        
        if (query.exec()) {
            QMessageBox::information(this, "Success", "Inventory item updated successfully.");
        } else {
            QMessageBox::critical(this, "Database Error", 
                QString("Failed to update inventory item: %1").arg(query.lastError().text()));
        }
    } else {
        query.prepare("INSERT INTO Inventory (item_name, category, quantity, status) VALUES (?, ?, ?, ?)");
        query.addBindValue(itemName);
        query.addBindValue(category);
        query.addBindValue(quantity);
        query.addBindValue(status);
        
        if (query.exec()) {
            QMessageBox::information(this, "Success", "Inventory item added successfully.");
        } else {
            QMessageBox::critical(this, "Database Error", 
                QString("Failed to add inventory item: %1").arg(query.lastError().text()));
        }
    }
    
    loadInventoryItems();
    clearInventoryInputs();
}

void MainWindow::on_deleteButton_clicked() {
    if (currentSelectedInventoryId <= 0) {
        QMessageBox::warning(this, "Selection Error", "Please select an item to delete.");
        return;
    }
    
    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirm Deletion", 
        "Are you sure you want to delete this inventory item?", 
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        
    if (confirm != QMessageBox::Yes) {
        return;
    }
    
    QSqlQuery query(db);
    query.prepare("DELETE FROM Inventory WHERE inventory_id = ?");
    query.addBindValue(currentSelectedInventoryId);
    
    if (query.exec()) {
        QMessageBox::information(this, "Success", "Inventory item deleted successfully.");
        loadInventoryItems();
        clearInventoryInputs();
    } else {
        QMessageBox::critical(this, "Database Error", 
            QString("Failed to delete inventory item: %1").arg(query.lastError().text()));
    }
}

void MainWindow::initializeMenuItems() {
    menuItemsContainer = new QWidget(ui->Menu);
    menuItemsLayout = new QVBoxLayout(menuItemsContainer);
    menuItemsLayout->setSpacing(10);
    menuItemsLayout->setContentsMargins(10, 10, 10, 10);
    
    QScrollArea* scrollArea = new QScrollArea(ui->Menu);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(menuItemsContainer);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    ui->verticalLayout_22->addWidget(scrollArea);
}


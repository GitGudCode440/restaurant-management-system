#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginpage.h"
#include "menuitemcard.h"

#include <iostream>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "Starting MainWindow constructor";
    
    ui->setupUi(this);
    
    // Set up frameless window
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    ui->dateEdit_date->setDate(QDate::currentDate());
    ui->timeEdit_time->setTime(QTime::currentTime());
    
    // Initialize UI elements
    availableCount = ui->availableCount;
    occupiedCount = ui->occupiedCount;
    reservedCount = ui->reservedCount;

    // Initialize table status combo boxes
    Table1_Status = ui->Table1_Status;
    Table2_Status = ui->Table2_Status;
    Table3_Status = ui->Table3_Status;
    Table4_Status = ui->Table4_Status;
    Table5_Status = ui->Table5_Status;
    Table6_Status = ui->Table6_Status;

    // Setup database connection
    qDebug() << "Setting up database connection";
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("restaurant_db");
    db.setUserName("root");
    db.setPassword("12345");

    qDebug() << "Attempting to open database connection";
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", 
            QString("Unable to connect to database: %1").arg(db.lastError().text()));
        return;
    }
    qDebug() << "Database connection successful";

    // Setup combo box connections
    qDebug() << "Setting up combo box connections";
    QList<QComboBox*> combos = ui->TableGrid->findChildren<QComboBox*>();
    for(QComboBox* combo : std::as_const(combos)) {
        // Extract table ID from combo box name (e.g., "Table1_Status" -> 1)
        QString name = combo->objectName();
        int tableId = name.mid(5, 1).toInt();  // Extract the number after "Table"
        
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this, combo, tableId](int index) {
                QString newStatus = combo->itemText(index);
                on_tableStatusChanged(tableId, newStatus);
            }
        );
    }

    // Load initial data
    qDebug() << "Loading initial table statuses";
    initializeTables();
    updateTableStatusCounts();

    initializeOrders();
    initializeReservations();
    initializeInventory();
    initializeFoodItems();

    loadMenuItems();
    
    qDebug() << "MainWindow constructor completed successfully";

}

void MainWindow::initializeOrders() {
    ui->noOrdersLabel->setVisible(true);
    QSqlQuery query(db);

    if (query.exec("SELECT * FROM Orders;")) {
        std::cout << "Nice" << std::endl;

        while(query.next()) {
            int id = query.value(0).toInt();
            QString orderType = query.value(1).toString();
            QString orderStatus = query.value(2).toString();
            QString orderDetails = query.value(3).toString();
            float orderPrice = query.value(4).toFloat();


            OrderCard* card = new OrderCard;
            card->changeOrderType(OrderCard::convertQStringToOrderType(orderType));
            card->changeOrderStatus(orderStatus);
            card->changeOrderPrice(orderPrice);
            card->addTextToListWidget(orderDetails);

            connect(card, &QObject::destroyed, this, [this]() {
                orderCount--;
                ui->noOrdersLabel->setVisible(orderCount <= 0);
            });

            addOrder(card, id);
            ui->noOrdersLabel->setVisible(false);
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

            // Add to UI table
            QTableWidget* table = ui->tableWidget_tables;

            table->insertRow(currentRow);

            // Set items in the table
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
    loadMenuItems();  // Reload menu items when switching to Menu tab
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


void MainWindow::addOrder(OrderCard* card, int id) {
    orderCount++;
    QString orderId = QString("Order #%1").arg(orderCount);
    card->setId(id);
    card->changeOrderLabelText(orderId);
    ui->OrderListLayout->addWidget(card);
    ui->noOrdersLabel->setVisible(false);
}

void MainWindow::on_FoodAddBtn_clicked()
{
    QComboBox* foodItemSelect = ui->FoodItemSelect;
    QComboBox* foodQuantitySelect = ui->FoodQuantitySelect;

    // Add the selected item and quantity to the list widget
    ui->FoodListWidget->addItem(
        QString("%1 %2")
            .arg(foodQuantitySelect->currentText(), foodItemSelect->currentText())
    );
}

void MainWindow::on_FoodFinalizeBtn_clicked()
{
    QListWidget* listWidget = ui->FoodListWidget;
    QComboBox* deliverySelect = ui->DeliveryTypeSelect;

    // Only proceed if there are items in the list
    if (listWidget->count() == 0) {
        QMessageBox::warning(nullptr, "Warning", "Please add items before finalizing the order");
        return;
    }

    OrderCard* card = new OrderCard;

    connect(card, &QObject::destroyed, this, [this]() {
        orderCount--;
        ui->noOrdersLabel->setVisible(orderCount <= 0);
    });

    OrderType type;

    if (deliverySelect->currentIndex() >= 0 && deliverySelect->currentIndex() <= 2) {
        type = static_cast<OrderType>(deliverySelect->currentIndex());
    }
    else type = OrderType::INVALID;

    card->changeOrderType(type);

    QString str = "";
    float totalPrice = 0.0;
    int count = listWidget->count();
    
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item = listWidget->item(i);
        QString itemText = item->text();
        
        // Extract quantity and price from the item text
        // Format is "1x Chicken Tandoori - $12.99"
        QStringList parts = itemText.split(" - $");
        if (parts.size() == 2) {
            // Get the quantity and food name from the first part
            QString firstPart = parts[0];  // This contains "1x Chicken Tandoori"
            float itemPrice = parts[1].toFloat();
            
            // Calculate total price
            int quantity = firstPart.split("x")[0].trimmed().toInt();
            totalPrice += quantity * itemPrice;

            // Add the quantity and food name to the string
            str += firstPart;
            if (i < count - 1) {
                str += ", ";
            }
        }
    }
    
    card->changeDetailsText(str);
    card->changeOrderPrice(totalPrice);

    QSqlQuery query(db);
    
    if (!query.prepare(
        "INSERT INTO Orders (order_type, order_status, order_details, order_price) "
        "VALUES (:type, :status, :details, :price);"
    )) {
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Failed to prepare order insertion: %1").arg(query.lastError().text()));
        return;
    }

    query.bindValue(":type", OrderCard::convertOrderTypeToQString(card->getOrderType()));
    query.bindValue(":status", "Being Prepared");
    query.bindValue(":details", card->getDetailsText());
    query.bindValue(":price", totalPrice);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Failed to insert order: %1").arg(query.lastError().text()));
        return;
    }

    int newOrderId = query.lastInsertId().toInt();

    card->setId(newOrderId);
    
    addOrder(card, newOrderId);

    // Clear the list widget after finalizing
    listWidget->clear();
    ui->noOrdersLabel->setVisible(false);
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
        comboBox->setStyleSheet("QComboBox { background-color: green; color: white; }");
    } else if (status == "Occupied") {
        comboBox->setStyleSheet("QComboBox { background-color: red; color: white; }");
    } else if (status == "Reserved") {
        comboBox->setStyleSheet("QComboBox { background-color: orange; color: white; }");
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

    // Get the selected table number - extract number from text like "Table 1"
    QString tableText = tableNo->currentText();
    int tableId = tableText.split(" ").last().toInt();
    
    if (tableId < 1 || tableId > 6) {
        QMessageBox::warning(this, "Warning", "Invalid table number selected");
        return;
    }

    // Check if the table is available
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

    // Get the reservation date and time
    QDateTime reservationDateTime(dateEdit->date(), timeEdit->time());
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Determine table status based on reservation time
    QString tableStatus;
    if (reservationDateTime <= currentDateTime) {
        tableStatus = "Occupied";
    } else {
        tableStatus = "Reserved";
    }

    // Store reservation in database
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

    // Update the table status in the database
    QSqlQuery query(db);
    query.prepare("UPDATE Tables SET status = ? WHERE table_id = ?");
    query.addBindValue(tableStatus);
    query.addBindValue(tableId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update table status: " + query.lastError().text());
        return;
    }

    // Update the UI table status
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

    // Update the reservations table
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

    // Update table status counts
    updateTableStatusCounts();

    // Clear input fields
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
    // Get references to your input fields
    QLineEdit* itemNameEdit = ui->itemNameEdit;
    QLineEdit* categoryEdit = ui->categoryEdit;
    QLineEdit* quantityEdit = ui->quantityEdit;
    QComboBox* statusCombo = ui->statusCombo;
    QTableWidget* table = ui->inventoryTable;

    // Validate input
    if (itemNameEdit->text().isEmpty() || categoryEdit->text().isEmpty() || quantityEdit->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Input Error", "Please fill in all fields");
        return;
    }

    // Convert quantity to price (assuming quantityEdit is being used for price)
    bool ok;
    float price = quantityEdit->text().toFloat(&ok);
    if (!ok || price <= 0) {
        QMessageBox::warning(nullptr, "Input Error", "Please enter a valid price");
        return;
    }

    // Insert into database
    QSqlQuery query(db);
    query.prepare("INSERT INTO Inventory (item_name, category, quantity, status) "
                 "VALUES (:name, :category, :price, :status)");
    
    query.bindValue(":name", itemNameEdit->text());
    query.bindValue(":category", categoryEdit->text());
    query.bindValue(":price", price);
    query.bindValue(":status", statusCombo->currentText());  // Using statusCombo for description

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", 
            QString("Failed to add menu item: %1").arg(query.lastError().text()));
        return;
    }

    // Add to table widget
    int currentRow = table->rowCount();
    table->insertRow(currentRow);

    QTableWidgetItem* tableItem;
    for (int i = 0; i < 4; i++) {
        switch(i) {
        case 0: tableItem = new QTableWidgetItem(itemNameEdit->text()); break;
        case 1: tableItem = new QTableWidgetItem(categoryEdit->text()); break;
        case 2: tableItem = new QTableWidgetItem(QString::number(price, 'f', 2)); break;
        case 3: tableItem = new QTableWidgetItem(statusCombo->currentText()); break;
        }
        table->setItem(currentRow, i, tableItem);
    }

    // Clear input fields
    itemNameEdit->clear();
    categoryEdit->clear();
    quantityEdit->clear();
    statusCombo->setCurrentIndex(0);

    // Reload menu items
    loadMenuItems();
}

void MainWindow::initializeTables() {
    QSqlQuery query(db);
    query.prepare("SELECT table_id, status FROM Tables ORDER BY table_id");
    query.exec();

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

void MainWindow::updateTableStatusCounts()
{
    QSqlQuery query(db);
    query.prepare("SELECT status, COUNT(*) as count FROM Tables GROUP BY status");
    query.exec();

    int available = 0;
    int occupied = 0;
    int reserved = 0;

    while (query.next()) {
        QString status = query.value(0).toString();
        int count = query.value(1).toInt();
        
        if (status == "Available") available = count;
        else if (status == "Occupied") occupied = count;
        else if (status == "Reserved") reserved = count;
    }

    // Update the line edits with the new counts
    availableCount->setText(QString::number(available));
    occupiedCount->setText(QString::number(occupied));
    reservedCount->setText(QString::number(reserved));
}

void MainWindow::on_tableStatusChanged(int tableId, const QString& newStatus)
{
    // Update database
    QSqlQuery query(db);
    query.prepare("UPDATE Tables SET status = ? WHERE table_id = ?");
    query.addBindValue(newStatus);
    query.addBindValue(tableId);
    query.exec();

    // Update UI
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
        setComboBoxColor(tableStatusCombo, newStatus);
    }

    // Update counts
    updateTableStatusCounts();
}

void MainWindow::loadMenuItems()
{
    QSqlQuery query(db);
    if (!query.exec("SELECT food_name, food_price FROM Menu ORDER BY food_name")) {
        QMessageBox::warning(nullptr, "Database Error", 
            QString("Failed to load menu items: %1").arg(query.lastError().text()));
        return;
    }

    // Clear existing items from the combo box and menu items layout
    ui->FoodItemSelect->clear();
    QLayout* menuLayout = ui->menuItemsLayout;
    if (menuLayout) {
        QLayoutItem* item;
        while ((item = menuLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
    }

    while (query.next()) {
        QString foodName = query.value(0).toString();
        float price = query.value(1).toFloat();

        // Add item to the combo box
        ui->FoodItemSelect->addItem(QString("%1 - $%2").arg(foodName).arg(price, 0, 'f', 2));
        
        // Create and add menu item card
        createMenuItemCard(foodName, "", QString::number(price, 'f', 2));
    }
}

void MainWindow::on_tableWidget_tables_itemDoubleClicked(QTableWidgetItem *item)
{
    // Get the row of the double-clicked item
    int row = item->row();
    
    // Get the table number from the second column (index 1)
    QString tableText = ui->tableWidget_tables->item(row, 1)->text();
    int tableId = tableText.split(" ").last().toInt();

    // Get the reservation details for deletion
    QString customerName = ui->tableWidget_tables->item(row, 0)->text();
    QDate date = QDate::fromString(ui->tableWidget_tables->item(row, 2)->text(), "yyyy-MM-dd");
    QTime time = QTime::fromString(ui->tableWidget_tables->item(row, 3)->text(), "hh:mm");

    // Confirm deletion with the user
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Deletion",
        "Are you sure you want to delete this reservation?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Delete from database
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

    // Update the table status to Available in the database
    QSqlQuery query(db);
    query.prepare("UPDATE Tables SET status = 'Available' WHERE table_id = ?");
    query.addBindValue(tableId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update table status: " + query.lastError().text());
        return;
    }

    // Update the UI table status
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

    // Remove the row from the reservations table
    ui->tableWidget_tables->removeRow(row);

    // Update table status counts
    updateTableStatusCounts();

    QMessageBox::information(this, "Success", "Reservation deleted successfully");
}

void MainWindow::on_AdditemBtn_clicked()
{
    // Get references to your input fields
    QLineEdit* itemName = ui->ItemName;
    QLineEdit* price = ui->Price;
    
    // Check if the fields are not empty
    if (!itemName->text().isEmpty() && !price->text().isEmpty()) {
        // Convert price to float
        bool ok;
        float priceValue = price->text().toFloat(&ok);
        if (!ok || priceValue <= 0) {
            QMessageBox::warning(nullptr, "Input Error", "Please enter a valid price");
            return;
        }

        // Insert into database
        QSqlQuery query(db);
        query.prepare("INSERT INTO Menu (food_name, food_price) VALUES (:name, :price)");
        query.bindValue(":name", itemName->text());
        query.bindValue(":price", priceValue);

        if (!query.exec()) {
            QMessageBox::critical(nullptr, "Database Error", 
                QString("Failed to add menu item: %1").arg(query.lastError().text()));
            return;
        }

        // Create a new menu item card
        createMenuItemCard(itemName->text(), "", price->text());
        
        // Update the food items combo box
        initializeFoodItems();
        
        // Clear the input fields
        itemName->clear();
        price->clear();
        
        // Set focus back to item name field
        itemName->setFocus();
    }
}

void MainWindow::createMenuItemCard(const QString& name, const QString& description, const QString& price)
{
    // Create a new menu item card widget
    MenuItemCard* card = new MenuItemCard(name, description, price, this);
    
    // Add the card to the menu items layout
    ui->menuItemsLayout->addWidget(card);
}

// Window control buttons implementation
void MainWindow::on_pushButton_2_clicked()
{
    // Minimize button
    this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::on_pushButton_3_clicked()
{
    // Maximize/Restore button
    if(this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    // Close button
    this->close();
}


void MainWindow::on_deleteButton_clicked()
{

}

void MainWindow::initializeInventory() {
    QSqlQuery query(db);
    QTableWidget* table = ui->inventoryTable;
    
    // Clear existing items
    table->setRowCount(0);
    
    if (query.exec("SELECT item_name, category, quantity, status FROM Inventory ORDER BY item_name")) {
        while (query.next()) {
            int row = table->rowCount();
            table->insertRow(row);
            
            // Add items to the table
            table->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // Item Name
            table->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // Category
            table->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // Quantity
            table->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // Status
        }
    } else {
        QMessageBox::warning(nullptr, "Database Warning", 
            QString("Failed to load inventory: %1").arg(query.lastError().text()));
    }
}

void MainWindow::initializeFoodItems() {
    QComboBox* foodItemSelect = ui->FoodItemSelect;
    foodItemSelect->clear();  // Clear existing items
    
    QSqlQuery query(db);
    if (query.exec("SELECT food_name, food_price FROM Menu ORDER BY food_name")) {
        while (query.next()) {
            QString foodName = query.value(0).toString();
            float price = query.value(1).toFloat();
            
            // Add item with name and price
            foodItemSelect->addItem(QString("%1 - $%2").arg(foodName).arg(price, 0, 'f', 2));
        }
    } else {
        QMessageBox::warning(nullptr, "Database Warning", 
            QString("Failed to load menu items: %1").arg(query.lastError().text()));
    }
}


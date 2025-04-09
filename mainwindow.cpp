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


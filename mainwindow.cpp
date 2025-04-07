#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_TablesBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_MenuBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_OrdersBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_BillingBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_ReservationsBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_InventoryBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


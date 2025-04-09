#include "loginpage.h"
#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList drivers = QSqlDatabase::drivers();
    std::cout << "Available Qt SQL Drivers:\n";
    for (const QString &driver : drivers) {
        std::cout << driver.toStdString() << std::endl;
    }

    // Show login page first
    loginpage login;
    login.show();


    // Optional: If login succeeds, you can show MainWindow
    // MainWindow w;
    // w.show();

    return a.exec();
}

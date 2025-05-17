#include "loginpage.h"
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

    loginpage login;
    login.show();

    return a.exec();
}

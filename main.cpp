#include "loginpage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create and show the login page first
    loginpage login;
    login.show();

    return a.exec();
}

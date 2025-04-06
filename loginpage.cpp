#include "loginpage.h"
#include "ui_loginpage.h"
#include <QMessageBox>

loginpage::loginpage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginpage)
{
    ui->setupUi(this);
}

loginpage::~loginpage()
{
    delete ui;
}

void loginpage::on_SubmitBtn_clicked()
{
    // Retrieve entered username and password
    QString enteredUsername = ui->usernameLineEdit->text();
    QString enteredPassword = ui->passwordLineEdit->text();

    // Validate credentials (replace with your actual logic)
    QString correctUsername = "admin"; // Example username
    QString correctPassword = "12345"; // Example password

    if (enteredUsername == correctUsername && enteredPassword == correctPassword) {
        // Credentials are correct, show the main window
        MainWindow *mainWindow = new MainWindow(); // Ensure MainWindow is included
        mainWindow->show();
        this->close(); // Optional: Close the login page
    } else {
        ui->label->setText("Invalid Username or Password");
    }
}



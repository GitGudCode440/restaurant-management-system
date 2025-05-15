#include "menuitemcard.h"

MenuItemCard::MenuItemCard(const QString &name, const QString &price, QWidget *parent)
    : QWidget(parent)
{
    // Create the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Create the card frame
    cardFrame = new QFrame(this);
    cardFrame->setFrameShape(QFrame::StyledPanel);
    cardFrame->setFrameShadow(QFrame::Raised);
    cardFrame->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border: 2px solid green;"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "}"
    );
    cardFrame->setCursor(Qt::CursorShape::PointingHandCursor);
    
    // Create a layout for the card content
    QHBoxLayout *cardLayout = new QHBoxLayout(cardFrame);
    
    // Create and style the name label
    nameLabel = new QLabel(name, cardFrame);
    nameLabel->setStyleSheet(
        "QLabel {"
        "   font-family: 'Franklin Gothic';"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   color: black;"
        "   padding: 5px;"
        "}"
    );
    
    // Create and style the price label
    priceLabel = new QLabel(price + "$", cardFrame);
    priceLabel->setStyleSheet(
        "QLabel {"
        "   font-family: 'Franklin Gothic';"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   color: green;"
        "   padding: 5px;"
        "}"
    );
    priceLabel->setAlignment(Qt::AlignRight);
    
    // Add widgets to layout
    cardLayout->addWidget(nameLabel);
    cardLayout->addStretch();
    cardLayout->addWidget(priceLabel);
    
    // Add card frame to main layout
    mainLayout->addWidget(cardFrame);
    setLayout(mainLayout);
}

void MenuItemCard::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {

        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Confirm Deletion",
            "Are you sure you want to delete this menu item?", QMessageBox::Yes | QMessageBox::No);

        if (reply != QMessageBox::Yes) return;

        QSqlQuery q("DELETE FROM Menu WHERE food_name = ? AND food_price = ?");
        std::cout << "Hello! It workds" << std::endl;
        q.addBindValue(nameLabel->text());
        q.addBindValue(priceLabel->text());
        if (!q.exec()) {
            QMessageBox::critical(nullptr, "Database Error", QString("Failed to delete menu item: %1").arg(q.lastError().text()));
            return;
        }
    }

    emit initializeFoodItemsInBillingPage();
    delete this;
}

MenuItemCard::~MenuItemCard()
{
    // Qt will handle destruction of child widgets
} 

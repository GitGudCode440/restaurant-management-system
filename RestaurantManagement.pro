QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    billing.cpp \
    inventory.cpp \
    loginpage.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    ordercard.cpp \
    orders.cpp \
    reservations.cpp \
    tables.cpp

HEADERS += \
    billing.h \
    inventory.h \
    loginpage.h \
    mainwindow.h \
    menu.h \
    ordercard.h \
    orders.h \
    reservations.h \
    tables.h

FORMS += \
    billing.ui \
    inventory.ui \
    loginpage.ui \
    mainwindow.ui \
    menu.ui \
    ordercard.ui \
    orders.ui \
    reservations.ui \
    tables.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    loginpage.cpp \
    main.cpp \
    mainwindow.cpp \
    menuitemcard.cpp \
    ordercard.cpp

HEADERS += \
    loginpage.h \
    mainwindow.h \
    menuitemcard.h \
    ordercard.h

FORMS += \
    loginpage.ui \
    mainwindow.ui \
    ordercard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

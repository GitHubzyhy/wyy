QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++17
RC_ICONS = $$PWD/images/ohter/logoRed.ico
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    wyy.cpp\

HEADERS += \
    loadfileqss.h\
    wyy.h \

FORMS += \
    wyy.ui \

INCLUDEPATH +=$$PWD/titleCode
include ($$PWD/titleCode/titleCode.pri)

INCLUDEPATH +=$$PWD/page1Code
include ($$PWD/page1Code/page1Code.pri)

INCLUDEPATH +=$$PWD/page2Code
include ($$PWD/page2Code/page2Code.pri)

INCLUDEPATH +=$$PWD/cartoonCode
include ($$PWD/cartoonCode/cartoonCode.pri)

INCLUDEPATH += $$PWD/gptCode
include ($$PWD/gptCode/gptCode.pri)

INCLUDEPATH +=$$PWD/listenCode
include ($$PWD/listenCode/listenCode.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

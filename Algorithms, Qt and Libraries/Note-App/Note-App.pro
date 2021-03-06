#-------------------------------------------------
#
# Project created by QtCreator 2018-11-03T13:16:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Note-App
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    singlenote.cpp \
    notedialog.cpp \
    singlenoteview.cpp \
    archivednotes.cpp \
    utilityclass.cpp \
    exportdialog.cpp

HEADERS += \
        mainwindow.h \
    singlenote.h \
    notedialog.h \
    singlenoteview.h \
    archivednotes.h \
    utilityclass.h \
    exportdialog.h

FORMS += \
        mainwindow.ui \
    notedialog.ui \
    singlenoteview.ui \
    archivednotes.ui \
    exportdialog.ui

RESOURCES +=
        themes/style.qrc

# Add themes folder
copy_to_build.path = $$OUT_PWD/themes
copy_to_build.files = themes/*

install_exe.path = $$OUT_PWD/pdf
install_exe.files = pdf/*
INSTALLS += \
    install_exe

INSTALLS += \
    copy_to_build

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

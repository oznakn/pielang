QT += core
QT -= gui

CONFIG += c++11

TARGET = pilang
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pilang.cpp \
    processor.cpp \
    helper.cpp \
    variable.cpp \
    blockprocessor.cpp \
    functionrunner.cpp

HEADERS += \
    pilang.h \
    processor.h \
    helper.h \
    variable.h \
    blockprocessor.h \
    functionrunner.h

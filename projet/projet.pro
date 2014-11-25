#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T10:51:43
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = projet
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann

TEMPLATE = app


SOURCES += main.cpp \
    traitementimage.cpp \
    percepunit.cpp

HEADERS += \
    traitementimage.h \
    percepunit.h

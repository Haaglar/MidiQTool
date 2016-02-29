#-------------------------------------------------
#
# Project created by QtCreator 2016-02-27T10:03:19
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MidiQTool
TEMPLATE = app


SOURCES += main.cpp\
        midiqtool.cpp \
    midifile/Binasc.cpp \
    midifile/MidiEvent.cpp \
    midifile/MidiEventList.cpp \
    midifile/MidiFile.cpp \
    midifile/MidiMessage.cpp \
    adjustmid.cpp

HEADERS  += midiqtool.h \
    midifile/Binasc.h \
    midifile/MidiEvent.h \
    midifile/MidiEventList.h \
    midifile/MidiFile.h \
    midifile/MidiMessage.h \
    adjustmid.h \
    constants.h

FORMS    += midiqtool.ui

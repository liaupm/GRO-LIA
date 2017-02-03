QT       -= core

TARGET = cellprot
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/erreenege.cpp \
    src/GenCell.cpp \
    src/GenListPlasmid.cpp \
    src/GenOperon.cpp \
    src/GenPlasmid.cpp \
    src/GenPromoter.cpp

HEADERS += \
    include/erreenege.h \
    include/GenCell.h \
    include/GenListPlasmid.h \
    include/GenOperon.h \
    include/GenPlasmid.h \
    include/GenPromoter.h

#-------------------------------------------------
#
# Project created by QtCreator 2012-05-23T09:21:54
#
#-------------------------------------------------

# uncomment this line to compile grong (the no gui version)
# CONFIG += nogui

CONFIG += warn_off
#CONFIG += c++11

#contains ( CONFIG, nogui ) {
#  QMAKE_CXXFLAGS += -DNOGUI -std=c99
#}

contains ( CONFIG, nogui ) {
  QMAKE_CXXFLAGS += -DNOGUI -std=c++11
}

win32 {
  QMAKE_CXXFLAGS += -O3 -DWIN -DNDEBUG
  contains ( CONFIG, nogui ) {
    CONFIG += console
  }
  CONFIG -= app_bundle
}

macx {
  QMAKE_CXXFLAGS -= -std=gnu++11
  QMAKE_CXXFLAGS += -fast -std=c++11
  #LIBS += -stdlib=libstdc++
  #QMAKE_LFLAGS -= -stdlib=libc++
  #QMAKE_LFLAGS += -stdlib=libstdc++
  QMAKE_MAC_SDK = macosx10.11

}

contains ( CONFIG, nogui ) {
  QT -= core gui
  TARGET = grong
  TEMPLATE = app
} else {
  QT += core gui widgets
  TARGET = gro
  TEMPLATE = app
}

ICON = groicon.icns

SOURCES += main.cpp\
    gui.cpp \
    GroThread.cpp \
    GroWidget.cpp \
    Messages.cpp \
    #Programs.cpp \
    Signal.cpp \
    Themes.cpp \
    Utility.cpp \
    World.cpp \
    EColi.cpp \
    Gro.cpp \
    GroPainter.cpp \
    Cell.cpp \
    reaction.cpp \
    Yeast.cpp

HEADERS  += gui.h \
    GroThread.h \
    GroWidget.h \
    Defines.h \
    Programs.h \
    Utility.h \
    Micro.h \
    GroPainter.h \
    Theme.h \
    EColi.h \
    Cell.h \
    ui_gui.h \
    Yeast.h

contains ( CONFIG, nogui ) {
  SOURCES -= GroThread.cpp GroWidget.cpp GroPainter.cpp gui.cpp Themes.cpp
  HEADERS -= gui.h GroThread.h GroWidget.h GroPainter.h
}

!contains ( CONFIG, nogui ) {
  FORMS    += gui.ui
}

macx {
  CONFIG(debug, debug|release) {
        LIBS += -L../ccl_Debug/ -lccl -L ../CellEngine/build/mac/debug -lcellengine -L../cellsignals/build/mac/debug -lsignals -L../CellNutrient/build/mac/debug -lcellnutrient -L../CellProt/build/mac/debug -lcellprot
        PRE_TARGETDEPS += ../ccl_Debug/libccl.a
  } else {
        LIBS += -L../ccl_Release/ -lccl -L ../CellEngine/build/mac/release -lcellengine -L../cellsignals/build/mac/release -lsignals -L../CellNutrient/build/mac/release -lcellnutrient -L../CellProt/build/mac/release -lcellprot
        PRE_TARGETDEPS += ../ccl_Release/libccl.a
  }

  #DEPENDPATH += ../Chipmunk-5.3.5/
  INCLUDEPATH += ../ccl-master/ ../CellEngine/include ../cellsignals/include ../CellNutrient/include ../CellProt/include

  #makelinks.commands += echo Making links in $$OUT_PWD;
  #makelinks.commands += ln -s -f ../GRO/examples;
  #makelinks.commands += ln -s -f ../GRO/include;
}

linux {
  CONFIG(debug, debug|release) {
        LIBS += -L../ccl_Debug/ -lccl -L../CellEngine/build/linux/debug -lcellengine -L../CellNutrient/build/linux/debug -lcellnutrient -L../CellProt/build/linux/debug -lcellprot -L../cellsignals/build/linux/debug -lsignals -lrt
        PRE_TARGETDEPS += ../ccl_Debug/libccl.a
  } else {
        LIBS += -L../ccl_Release/ -lccl -L../CellEngine/build/linux/release -lcellengine -L../CellNutrient/build/linux/release -lcellnutrient -L../CellProt/build/linux/release -lcellprot -L../cellsignals/build/linux/release -lsignals -lrt
        PRE_TARGETDEPS += ../ccl_Release/libccl.a
  }
  #LIBS += -L/home/tin/copyfiles/PhD/EvLIT/PLASWIRES/GRO/BiGRO2/CellEngine/build/ -lcellengine -L/home/tin/copyfiles/PhD/EvLIT/PLASWIRES/GRO/BiGRO2/cellsignal/cellsignal-0.0.3/build/ -lcellsignal
  #LIBS += -L../CellEngine/build/ -lcellengine -L../cellsignal/cellsignal-0.0.3/build/ -lcellsignal

  #../CellEngine/build/libcellengine.a ../cellsignal/cellsignal-0.0.3/build/libcellsignal.a
  #DEPENDPATH += ../Chipmunk-5.3.5/
  INCLUDEPATH += ../ccl-master/ ../CellEngine/include/ ../CellNutrient/include/ ../cellsignals/include/ ../CellProt/include

  makelinks.commands += echo Making links in $$OUT_PWD;
  makelinks.commands += ln -s -f ../GRO/examples;
  makelinks.commands += ln -s -f ../GRO/include;
}

win32 {
  CONFIG(debug, debug|release) {
        LIBS += -L../ccl_Debug/debug -lccl -L../CellEngine/build/win/debug -lcellengine -L../CellNutrient/build/win/debug -lcellnutrient -L../cellsignals/build/win/debug -lsignals -L../CellProt/build/win/debug -lcellprot
        PRE_TARGETDEPS += ../ccl_Debug/debug/libccl.a
  } else {
        LIBS += -L../ccl_Release/release -lccl -L../CellEngine/build/win/release -lcellengine -L../CellNutrient/build/win/release -lcellnutrient -L../cellsignals/build/win/release -lsignals -L../CellProt/build/win/release -lcellprot
        PRE_TARGETDEPS += ../ccl_Release/release/libccl.a
  }

  INCLUDEPATH += ../ccl-master/ ../CellEngine/include/ ../CellNutrient/include/ ../cellsignals/include/ ../CellProt/include
  QMAKE_POST_LINK = copy ..\GRO\include\*.* ..\..\;
}

QMAKE_EXTRA_TARGETS += makelinks
POST_TARGETDEPS += makelinks


OTHER_FILES += \
    examples/wave.gro \
    examples/skin.gro \
    examples/morphogenesis.gro \
    examples/inducer.gro \
    examples/growth.gro \
    examples/gfp.gro \
    examples/game.gro \
    examples/foreach.gro \
    examples/edge.gro \
    examples/dilution.gro \
    examples/chemotaxis.gro \
    examples/bandpass.gro \
    include/gro.gro \
    include/standard.gro \
    icons/stop.png \
    icons/step.png \
    icons/start.png \
    icons/reload.png \
    icons/open.png \
    examples/signal_demo.gro \
    examples/maptocells.gro \
    examples/game.gro \
    changelog.txt \
    error.tiff \
    examples/signal_grid.gro \
    examples/coupled_oscillator.gro \
    examples/spots.gro \
    examples/spatial_oscillations.gro \
    examples/symbiosis.gro \
    LICENSE.txt \
    examples/yeast_example.gro

!contains ( CONFIG, nogui ) {
  RESOURCES += icons.qrc
}

QMAKE_CLEAN += Makefile moc_GroPainter.cpp

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-Statistics-Debug/release/ -lStatistics
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-Statistics-Debug/debug/ -lStatistics
#else:unix: LIBS += -L$$PWD/../build-Statistics-Debug/ -lStatistics

#INCLUDEPATH += $$PWD/../Statistics
#DEPENDPATH += $$PWD/../Statistics

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-Statistics-Debug/release/Statistics.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-Statistics-Debug/debug/Statistics.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../build-Statistics-Debug/libStatistics.a

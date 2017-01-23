#-------------------------------------------------
#
# Project created by QtCreator 2012-05-26T17:40:42
# And then edited some by Eric Klavins.
#-------------------------------------------------

win32 {
  QMAKE_CXXFLAGS += -O3 -DWIN -w
}

macx {
  QMAKE_CXXFLAGS += -fast -w
  QMAKE_MAC_SDK = macosx10.12
}

#QT       -= core x86_64
QT       -= core

TARGET = ccl
TEMPLATE = lib
CONFIG += staticlib

SOURCES += TypeCheck.cpp \
    Type.cpp \
    SymbolTable.cpp \
    Symbol.cpp \
    Scope.cpp \
    Schedulers.cpp \
    Program.cpp \
    List.cpp \
    Internals.cpp \
    Expr.cpp \
    Eval.cpp \
    Debug.cpp \
    Clause.cpp \
    App.cpp \
    Normal.cpp \
    Hill.cpp \
    Rands.cpp

HEADERS += Expr.h \
    SymbolTable.h \
    Program.h \
    parser.h \
    Schedulers.h \
    Type.h \
    ccl.h \
    Normal.h \
    Hill.h \
    Rands.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    parser.y \
    lexer.l


include(flex.pri)
include(bison.pri)
FLEXSOURCES = lexer.l
BISONSOURCES = parser.y

QMAKE_CLEAN += Makefile libccl.a

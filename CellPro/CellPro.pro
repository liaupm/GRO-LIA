QT       -= core

TARGET = cellpro
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/cg/Gene.cpp \
    src/cg/Genome.cpp \
    src/cg/Operon.cpp \
    src/cg/Plasmid.cpp \
    src/cg/PlasmidPool.cpp \
    src/cg/Promoter.cpp \
    src/cg/Protein.cpp

HEADERS += \
    include/cg/Gene.hpp \
    include/cg/Genome.hpp \
    include/cg/Operon.hpp \
    include/cg/Plasmid.hpp \
    include/cg/PlasmidPool.hpp \
    include/cg/Promoter.hpp \
    include/cg/Protein.hpp

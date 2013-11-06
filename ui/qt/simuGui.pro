################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
################################################################

include( $${PWD}/../examples.pri )

TARGET       = simuGui

HEADERS = \
    mainwindow.h \
    scrollzoomer.h \
    scrollbar.h \
    incrementalplot.h \
    randomplot.h \
    parser.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    scrollzoomer.cpp \
    scrollbar.cpp \
    incrementalplot.cpp \
    randomplot.cpp \
    parser.cpp


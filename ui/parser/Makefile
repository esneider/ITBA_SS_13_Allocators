#-----------------------------------------------------------------------------------------
# Makefile for GCC & gmake (Linux, Windows/MinGW, OpenSolaris, etc).
#-----------------------------------------------------------------------------------------
# Copyright (c) 2010-2012 Marcus Geelnard
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#     1. The origin of this software must not be misrepresented; you must not
#     claim that you wrote the original software. If you use this software
#     in a product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#
#     2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#
#     3. This notice may not be removed or altered from any source
#     distribution.
#-----------------------------------------------------------------------------------------

# A simple hack to check if we are on Windows or not (i.e. are we using mingw32-make?)
ifeq ($(findstring mingw32, $(MAKE)), mingw32)
WINDOWS=1
endif

# Compiler settings
CPP = g++
CPPFLAGS = -W -O3 -c -I./TinyThread++-1.1/source
LFLAGS =
LIBS =

# Non-windows systems need pthread
ifndef WINDOWS
LIBS += -lpthread
endif

# MinGW32 GCC 4.5 link problem fix
ifdef WINDOWS
ifeq ($(findstring 4.5.,$(shell g++ -dumpversion)), 4.5.)
LFLAGS += -static-libstdc++ -static-libgcc
endif
endif

# Misc. system commands
ifdef WINDOWS
RM = del /Q
else
RM = rm -f
endif

# File endings
ifdef WINDOWS
EXE = .exe
else
EXE =
endif

# Object files for the parser program
PARSER_OBJS = parser.o testParser.o

# TinyThread++ object files
TINYTHREAD_OBJS = tinythread.o

all: parser$(EXE)

clean:
	$(RM) parser$(EXE)  $(PARSER_OBJS) $(TINYTHREAD_OBJS)


parser$(EXE): $(PARSER_OBJS) $(TINYTHREAD_OBJS)
	$(CPP) $(LFLAGS) -o $@ $(PARSER_OBJS) $(TINYTHREAD_OBJS) $(LIBS)
	
parser.o: testParser.cpp parser.cpp ./TinyThread++-1.1/source/tinythread.h  ./TinyThread++-1.1/source/fast_mutex.h
	$(CPP) $(CPPFLAGS) testParser.cpp parser.cpp

tinythread.o: ./TinyThread++-1.1/source/tinythread.cpp
	$(CPP) $(CPPFLAGS) $<

# Dependencies
#parser.o: testParser.cpp parser.cpp
#    $(CPP) $(LFLAGS) $(LIBS) testParser.cpp parser.cpp ./TinyThread++-1.1/source/tinythread.h  ./TinyThread++-1.1/source/fast_mutex.h
#tinythread.o: ./TinyThread++-1.1/source/tinythread.cpp ./TinyThread++-1.1/source/tinythread.h


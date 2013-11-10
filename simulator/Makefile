ALLOCATOR := lifo
CFLAGS    := -std=c99 -Wall -Wextra -Wno-overflow -Wno-unused
LIBS      := -lm
UNAME     := $(shell uname -s)


ifeq ($(UNAME), Linux)
	LIBS += -lrt
endif

ifdef NAME
	ALLOCATOR := $(NAME)
endif


.PHONY: all debug release compile


all: debug

debug: CFLAGS += -g
debug: compile

release: CFLAGS += -O2 -DNDEBUG
release: compile

compile:
	gcc $(CFLAGS) -I. *.c allocators/$(ALLOCATOR).c -o ../bin/simulator/run_$(ALLOCATOR) $(LIBS)

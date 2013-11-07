
LIBS  :=
UNAME := $(shell uname -s)

ifeq ($(UNAME), Linux)
	LIBS += -lrt
endif

ALL: debug

debug:
	gcc -std=c99 -Wall -Wextra -g -I. *.c allocators/lifo.c -o run $(LIBS)

release:
	gcc -std=c99 -Wall -Wextra -O2 -DNDEBUG -I. *.c allocators/lifo.c -o run $(LIBS)

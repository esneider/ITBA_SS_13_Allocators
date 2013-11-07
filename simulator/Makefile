
LIBS  :=
UNAME := $(shell uname -s)

ifeq ($(UNAME), Linux)
	LIBS += -lrt
endif

ALLOCATOR := lifo

ifdef NAME
	ALLOCATOR := $(NAME)
endif

ALL: debug

debug:
	gcc -std=c99 -Wall -Wextra -Wno-overflow -g -I. *.c allocators/$(ALLOCATOR).c -o run $(LIBS)

release:
	gcc -std=c99 -Wall -Wextra -Wno-overflow -Wno-unused-result -O2 -DNDEBUG -I. *.c allocators/$(ALLOCATOR).c -o run $(LIBS)

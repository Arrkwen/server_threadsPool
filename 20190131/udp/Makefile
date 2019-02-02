CC:=gcc
SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)
LINKERFLAG := -lpthread
all: ${BINS}

%: %.c
	${CC} $< -o $@ ${LINKERFLAG}
clean:
	rm -rf ${BINS}

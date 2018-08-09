CFLAGS=-std=gnu11 -Wall -Wextra
LDFLAGS=-lm

BIN=tone

.PHONY: all play clean

all: ${BIN}

clean:
	-rm -f ${BIN} out.wav

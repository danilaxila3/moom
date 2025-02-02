CC := gcc
CFLAGS := -Wall -Wextra
SRCS := $(wildcard src/*.c)
PROGS := $(patsubst src/%.c, bin/%, $(SRCS))

all: $(PROGS)

$(PROGS): $(SRCS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf bin

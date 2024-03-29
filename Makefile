CC=cc
OPT=-O2
STD=-std=gnu18
LDFLAGS=-L/usr/local/lib/ -lgmp
INC=-I/usr/local/include/
WARNING=-Werror -Wall -Wextra -Wpedantic -Wfloat-equal -Wundef -Wshadow \
		-Wpointer-arith -Wcast-align -Wstrict-prototypes -Wmissing-prototypes \
		-Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual \
		-Wswitch-enum -Wunreachable-code -Wformat -Wformat-security -Wvla \

FLAGS=-fstack-protector-all -fPIE -pipe
CFLAGS=$(WARNING) $(STD) $(OPT) $(FLAGS) $(INC)

.PHONY: release
release: OPT=-O2 -D_FORTIFY_SOURCE=2
release: all

.PHONY: debug
debug: OPT=-O0 -ggdb3
debug: all

.PHONY: sanitize
sanitize: OPT=-O0 -ggdb3 -fsanitize=address,undefined,leak
sanitize: all

.PHONY: all
all: isprime

isprime: isprime.c
	$(CC) -o isprime isprime.c $(CFLAGS) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: lint
lint:
	clang-tidy *.c *.h

.PHONY: fmt
fmt:
	clang-format -i *.c *.h

.PHONY: clean
clean:
	rm -f isprime
	rm -f *.o

CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline -Werror

decoder: decoder.c

debug: CFLAGS+=-g
debug: decoder.c

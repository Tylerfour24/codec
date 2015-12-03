CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline -Werror

all: decoder encoder
encoder: encoder.c

debug: CFLAGS+=-g
debug: all

.PHONY: clean debug profile

debug: CFLAGS+=-g
debug: $(TARGET)

clean:
	-$(RM) decoder encoder

profile: CFLAGS+=-pg
profile: LDFLAGS+=-pg
profile: $(TARGET)

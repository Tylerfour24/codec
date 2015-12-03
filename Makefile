CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline -Werror

decoder: decoder.c
encoder: encoder.c

debug: CFLAGS+=-g
debug: decoder.c

TARGET=decoder
OBJS=decoder.c

.PHONY: clean debug profile

$(TARGET): $(OBJS)

debug: CFLAGS+=-g
debug: $(TARGET)


clean:
	-$(RM) $(TARGET) $(OBJS)

profile: CFLAGS+=-pg
profile: LDFLAGS+=-pg
profile: $(TARGET)

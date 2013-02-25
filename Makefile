CC         = clang
CFLAGS    += --std=c99 -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -pedantic
LDFLAGS   += -I./external/msgpack-c/src
LDLIBS    += 
OBJS       = simple-fluentd-client.o
OBJS      += external/msgpack-c/src/objectc.o
OBJS      += external/msgpack-c/src/unpack.o
OBJS      += external/msgpack-c/src/version.o
OBJS      += external/msgpack-c/src/vrefbuffer.o
OBJS      += external/msgpack-c/src/zone.o
TARGETS    = main

main: main.c $(OBJS)
	$(CC) -o main main.c $(OBJS) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(ARGS)

simple-fluentd-client.o: simple-fluentd-client.c
	$(CC) -c simple-fluentd-client.c $(CFLAGS) $(LDFLAGS) $(ARGS)

simple-fluentd-client.c: simple-fluentd-client.h


.PHONY: clean
clean:
	$(RM) *.o $(TARGETS)

.PHONY: check-syntax
check-syntax:
	$(CC) $(CFLAGS) -fsyntax-only $(CHK_SOURCES)

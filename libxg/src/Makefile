.POSIX:

.SUFFIXES:
CC=/opt/irix/sgug/bin/mips-sgi-irix6.5-gcc
CFLAGS=-O3 -mips3
PREFIX=/usr
LDFLAGS=-shared

OBJS= asprintf.o forkpty.o getopt.o setenv.o strndup.o strtonum.o strnlen.o strerror_r.o posix_memalign.o

all: libxg.a libxg.so

libxg.a: $(OBJS)
	ar cq libxg.a $(OBJS)

libxg.so: $(OBJS)
	$(CC) $(CFLAGS) -o libxg.so $(OBJS) $(LDFLAGS)

asprintf.o: asprintf.c
	$(CC) $(CFLAGS) -c asprintf.c

forkpty.o: forkpty.c
	$(CC) $(CFLAGS) -c forkpty.c

getopt.o: getopt.c
	$(CC) $(CFLAGS) -c getopt.c

setenv.o: setenv.c
	$(CC) $(CFLAGS) -c setenv.c

strndup.o: strndup.c
	$(CC) $(CFLAGS) -c strndup.c

strtonum.o: strtonum.c
	$(CC) $(CFLAGS) -c strtonum.c

strnlen.o: strnlen.c
	$(CC) $(CFLAGS) -c strnlen.c

strerror_r.o: strerror_r.c
	$(CC) $(CFLAGS) -c strerror_r.c

posix_memalign.o: posix_memalign.c
	$(CC) $(CFLAGS) -c posix_memalign.c

clean:
	rm -rf *.o *.a

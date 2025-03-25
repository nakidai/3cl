OBJS += cccl.o
OBJS += executor.o
OBJS += main.o
OBJS += parser.o
OBJS += readfile.o
OBJS += str.o
OBJS += tokenizer.o

RM ?= rm -f
DESTDIR ?= /usr/local


all: 3cl

${OBJS}: cccl.h

3cl: ${OBJS}
	${CC} ${LDFLAGS} -o 3cl ${OBJS} ${LDLIBS}

install: all
	install -d ${DESTDIR}/bin ${DESTDIR}/share/man/man1
	install -d ${DESTDIR}/bin ${DESTDIR}/share/man/man7
	install -m755 3cl ${DESTDIR}/bin
	install -m644 3cl.1 ${DESTDIR}/share/man/man1
	install -m644 ccl.7 ${DESTDIR}/share/man/man7

uninstall:
	${RM} ${DESTDIR}/bin/3cl
	${RM} ${DESTDIR}/share/man/man1/3cl.1
	${RM} ${DESTDIR}/share/man/man7/ccl.7

clean:
	${RM} ${OBJS} 3cl

README: README.7
	mandoc -Tascii README.7 | col -b > README

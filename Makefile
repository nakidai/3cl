OBJS += allocfile.o
OBJS += cccl.o
OBJS += executor.o
OBJS += main.o
OBJS += parser.o
OBJS += str.o
OBJS += tokenizer.o

RM ?= rm -f
PREFIX ?= /usr/local
BINDIR ?= ${PREFIX}/bin
MANDIR ?= ${PREFIX}/man


all: 3cl

${OBJS}: cccl.h

3cl: ${OBJS}
	${CC} ${LDFLAGS} -o 3cl ${OBJS} ${LDLIBS}

test: all
	make -C examples

install: all
	install -d ${BINDIR} ${MANDIR}/man1
	install -m755 3cl ${BINDIR}
	install -m644 3cl.1 ${MANDIR}/man1
	install -m644 ccl.7 ${MANDIR}/man7

uninstall:
	${RM} ${BINDIR}/3cl
	${RM} ${MANDIR}/man1/3cl.1
	${RM} ${MANDIR}/man7/ccl.7

clean:
	${RM} ${OBJS} 3cl

README: README.7
	mandoc -Tascii README.7 | col -b > README

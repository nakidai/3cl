OBJS += cccl.o
OBJS += main.o
OBJS += parser.o
OBJS += readfile.o
OBJS += tokenizer.o

RM ?= rm -f


all: 3cl

${OBJS}: cccl.h

3cl: ${OBJS}
	${CC} ${LDFLAGS} -o 3cl ${OBJS} ${LDLIBS}

clean:
	${RM} ${OBJS} 3cl

README: README.7
	mandoc -Tascii README.7 | col -b > README

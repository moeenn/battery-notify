PROJECT = battery-notify

CC = cc
SRC = src/main.c
OBJ = ${SRC:.c=.o}
LIBS = 
CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations
BINARY = bin/${PROJECT}
INSTALL_DIR = /usr/local/bin

.c.o:
	${CC} -c ${CFLAGS} $<

main: ${OBJ}
	${CC} -o ${BINARY} *.o ${LIBS}

install:
	sudo cp -v ${BINARY} ${INSTALL_DIR}

uninstall:
	sudo rm -v ${INSTALL_DIR}${PROJECT}

clean:
	@echo "Cleaning Executables ..."
	@rm *.o
	rm ${BINARY}

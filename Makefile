all: netSim1 netSim2

netSim1.o: src/netSim1.c
	${CC} ${CFLAGS} -Isrc -c src/netSim1.c

netSim1: netSim1.o
	${CC} ${CFLAGS} -Isrc -o netSim1 netSim1.o

netSim2.o: src/netSim2.c
	${CC} ${CFLAGS} -Isrc -c src/netSim2.c

netSim2: netSim2.o
	${CC} ${CFLAGS} -Isrc -o netSim2 netSim2.o

clean:
	rm *.o netSim1 netSim2

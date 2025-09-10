MyBitApp: bits.o main.o
	gcc -o MyBitApp bits.o main.o

bits.o: bits.c bits.h
	gcc -c bits.c

main.o: main.c bits.h
	gcc -c main.c

clean:
	rm -f *.o MyBitApp
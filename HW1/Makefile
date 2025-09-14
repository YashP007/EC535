#// file: Makefile
#// Author/Date: Yash Patel (pately@bu.edu), 09/10/25
#// EC535, HW1, Question 3

# recompile main.c, bits.c into executable called MyBitApp
MyBitApp: bits.o main.o mylist.o
	gcc -o MyBitApp bits.o main.o mylist.o

bits.o: bits.c bits.h
	gcc -c bits.c -o bits.o

# ensure mylist.o is built and linked
mylist.o: mylist.c mylist.h
	gcc -c mylist.c -o mylist.o

main.o: main.c bits.h mylist.h
	gcc -c main.c -o main.o

clean:
	rm -f *.o MyBitApp
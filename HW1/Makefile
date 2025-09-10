#// file: Makefile
#// Author/Date: Yash Patel (pately@bu.edu), 09/10/25
#// EC535, HW1, Question 3

# recompile main.c, bits.c into executable called MyBitApp
MyBitApp: bits.o main.o
	gcc -o MyBitApp bits.o main.o

bits.o: bits.c bits.h
	gcc -c bits.c

main.o: main.c bits.h
	gcc -c main.c

# recompile mylist files into executable called mylist
mylist: mylist.c mylist.h
	gcc -c mylist.c -o mylist

clean:
	rm -f *.o MyBitApp
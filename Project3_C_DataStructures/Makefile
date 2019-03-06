# Nicu Razvan-Alexandru 313CB
build: tema3
tema3: functii.o main.o functii.h
	gcc -o tema3 $^ -Wall -lm
functii.o: functii.c
	gcc -c $^
main.o: main.c
	gcc -c $^
clean:
	rm -rf *.o tema3

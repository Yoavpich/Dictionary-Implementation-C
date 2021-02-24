a.out: main.o dictionary.o
	gcc -std=c99 main.o dictionary.o
main.o: main.c
	gcc -c -std=c99 main.c
dictionary.o: dictionary.c dictionary.h
	gcc -c -std=c99 dictionary.c
clean:
	rm -f main.o dictionary.o a.out
run: a.out
	./a.out

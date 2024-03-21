main:
	gcc -c main.c
	gcc -o run main.o list.o

clean:
	rm run
	rm main.o
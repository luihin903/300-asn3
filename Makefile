main:
	gcc -c main.c
	gcc -o run -pthread main.o list.o

clean:
	rm run
	rm main.o


all:
	gcc -m32 -no-pie -nostdlib -lgcc -o fib fib.c
	gcc -m32 -no-pie -nostdlib -lgcc -o sum sum.c
	gcc -m32 -o smartLoader loader.c

run-fib:
	./smartLoader fib

run-sum:
	./smartLoader sum



clean:
	-@rm -f fib sum smartLoader *.o

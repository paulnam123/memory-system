all:
	gcc -g -Wall main.c cse320_malloc.c cse320_virt_to_phys.c -o main -pthread
	gcc -g -Wall mem.c -o mem
clean:
	rm -f main
	rm -f mem

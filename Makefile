all:
	gcc -g -Wall main.c cse320_malloc.c cse320_virt_to_phys.c -o main
clean:
	rm -f main

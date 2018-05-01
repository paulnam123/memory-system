#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void* cse320_malloc(size_t size);
void* cse320_virt_to_phys(void* addr);

void *my_thread(void *vargp);

sem_t mutex;

pthread_t tid[4];

typedef struct page_table{

}page_table;

#endif

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void* cse320_malloc(size_t size);
void* cse320_virt_to_phys(void* addr);

void *my_thread(void *vargp);

sem_t mutex;

pthread_t tid[4];

typedef struct second_table{

  int address[1024];

}second_table;

typedef struct first_table{

  unsigned long tid;
  second_table second_level_table[1024];

}first_table;

first_table first_level_table[4];

#endif

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

char* cse320_malloc(char *buffer, int index, int process_index);
void int_to_binary(char *buffer, int decimal);

int cse320_virt_to_phys(int process_index, int first, int second);

void *my_thread(void *vargp);

sem_t mutex;

pthread_t tid[4];

typedef struct addresses{

  int used;
  int addr;

}addresses;

typedef struct second_table{

  addresses address[1024];

}second_table;

typedef struct first_table{

  unsigned long tid;
  second_table second_level_table[1024];

}first_table;

typedef struct cache{

  int valid;
  int addr;
  int value;

}cache;

first_table first_level_table[4];

cache cache_table[4];

extern int threadcheck[4];

#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
#include "functions.h"

int main(int argc, char **argv){

  while(1){

  printf("What would you like to do?\n");

    char input[255];
    char **list = malloc(255 * sizeof(char));
    char *token = NULL;
    int count = 0, temp, i, j, found = 0;

    //user input
    fgets(input, 100, stdin);

    token = strtok(input, " \n");
    char *command = token;

    // place values after commands into a list
    token = strtok(NULL, " \n");
    while(token != NULL){
      list[count] = token;
      token = strtok(NULL, " \n");
      count++;
    }
    if(count <= 1){
      list[1] = 0x0;
    }

    // create
    if(!strcmp(command, "create")){
      for(i = 0;i < 4;i++){
	if(tid[i] == 0){
	  temp = i;
	  found = 1;
	  break;
	}
      }

      if(found){
        pthread_create(&tid[temp], NULL, my_thread, NULL);
	for(i = 0;i < 4;i++){
	  if(first_level_table[i].tid == 0){
	    first_level_table[i].tid = tid[i];
	  }
	}
      }else{
	printf("You have already reached the max number of processes(4).\n");
      }

    // kill
    }else if(!strcmp(command, "kill")){
      unsigned long id = strtoul(list[0], 0, 0);
      for(i = 0;i < 4;i++){
        if(id == (unsigned long) tid[i]){
	  pthread_cancel(tid[i]);
	  tid[i] = 0;
	  found = 1;
	  temp = i;
	  break;
	}
      }

      // reset tables and tid from first table
      first_level_table[temp].tid = 0;

      for(i = 0;i < 1024;i++){
        for(j = 0;j < 1024;j++){
	  first_level_table[temp].second_level_table[i].address[j].valid = 0;
        }
      }

      if(!found){
	printf("Process with id %s does not exist.\n", list[0]);
      }

    // list
    }else if(!strcmp(command, "list")){
      for(i = 0;i < 4;i++){
	if(tid[i] != 0){
	  printf("%lu\n", (unsigned long) tid[i]);
	}
      }

    // mem
    }else if(!strcmp(command, "mem")){
      // edge case for X = 0
      temp = atoi(list[0]);
      if(temp == 0){
        printf("Process with id %s does not exist.\n", list[0]);
        free(list);
        continue;
      }

      unsigned long id = strtoul(list[0], 0, 0);
      for(i = 0;i < 4;i++){
	if(id == (unsigned long) tid[i]){
	  temp = i;
	  found = 1;
	  break;
	}
      }

      if(!found){
	printf("Process with id %s does not exist.\n", list[0]);
      }else{
	char *arr = NULL;
	for(i = 0;i < 1024;i++){
	  for(j = 0;j < 1024;j++){
	    if(first_level_table[temp].second_level_table[i].address[j].valid == 1){
	      char first[11], second[11];
	      char *last = "000000000000\0";

	      int_to_binary(first, i);
	      int_to_binary(second, j);

	      arr = strcat(first, second);
	      arr = strcat(arr, last);
	      printf("%s\n", arr);
	    }
	  }
	}
      }

      /*for(i = 0;i < 4;i++){
	printf("tid: %lu\n", (unsigned long) tid[i]);
      }

      for(i = 0;i < 4;i++){
	printf("first_level_table: %lu\n", (unsigned long) first_level_table[i].tid);
      }*/

    // allocate
    }else if(!strcmp(command, "allocate")){
      // edge case for X = 0
      temp = atoi(list[0]);
      if(temp == 0){
        printf("Process with id %s does not exist.\n", list[0]);
        free(list);
        continue;
      }

      unsigned long id = strtoul(list[0], 0, 0);
      for(i = 0;i < 4;i++){
	if(id == (unsigned long) tid[i]){
	  temp = i;
	  found = 1;
	  break;
	}
      }

      if(!found){
	printf("Process with id %s does not exist.\n", list[0]);
      }else{
        //char arr[33];
        //arr[32] = '\0';
	char *arr = NULL;
        cse320_malloc(arr, temp);
      }

    // read
    }else if(!strcmp(command, "read")){

    // write
    }else if(!strcmp(command, "write")){

    // exit
    }else if(!strcmp(command, "exit")){
      free(list);

      for(i = 0;i < 4;i++){
	if(tid[i] != 0){
	  pthread_cancel(tid[i]);
	}
      }

      exit(0);
    }else{
      //if none of the commands exist, restart
      printf("Command does not exist, try again.\n");
      free(list);
      continue;
    }

    free(list);

  }

  return 0;

}

void *my_thread(void *vargp){

  while(1){

  }

  return NULL;

}

void int_to_binary(char *buffer, int decimal){
  int i;
  buffer[10] = '\0';
  for(i = 10;i > 0;i--){
    buffer[i-1] = (decimal & 1) + '0';
    decimal >>= 1;
  }
  printf("binary: %s\n", buffer);

  return;
}

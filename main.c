#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "functions.h"

int main(int argc, char **argv){


  while(1){

  printf("What would you like to do?\n");

    char input[255];
    char **list = malloc(255 * sizeof(char));
    char *token = NULL;
    int count = 0, temp, i, found = 0;

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
      }else{
	printf("You have already reached the max number of processes(4).\n");
      }

    // kill
    }else if(!strcmp(command, "kill")){
      unsigned long id = strtoul(list[0], 0, 0);
      for(i = 0;i < 4;i++){
        if(id == (long int) tid[i]){
	  pthread_join(tid[i], NULL);
	  tid[i] = 0;
	  found = 1;
	  break;
	}
      }

      if(!found){
	printf("Thread with id %s does not exist.\n", list[0]);
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

    // allocate
    }else if(!strcmp(command, "allocate")){

    // read
    }else if(!strcmp(command, "read")){

    // write
    }else if(!strcmp(command, "write")){

    // exit
    }else if(!strcmp(command, "exit")){
      free(list);
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

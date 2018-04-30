#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

int main(int argc, char **argv){

  printf("What would you like to do?\n");

  while(1){

    char input[255];
    int *list = malloc(100 * sizeof(int));
    char *token = NULL;
    int count = 0, temp; 
    //int i;

    //user input
    fgets(input, 50, stdin);

    token = strtok(input, " \n");
    char *command = token;

    // place values after commands into a list
    token = strtok(NULL, " \n");
    while(token != NULL){
      temp = atoi(token);
      list[count] = temp;
      token = strtok(NULL, " \n");
      count++;
    }
    if(count <= 1){
      list[1] = 0x0;
    }

    if(!strcmp(command, "create")){

    }else if(!strcmp(command, "kill")){

    }else if(!strcmp(command, "list")){

    }else if(!strcmp(command, "mem")){

    }else if(!strcmp(command, "allocate")){

    }else if(!strcmp(command, "read")){

    }else if(!strcmp(command, "write")){

    }else if(!strcmp(command, "exit")){
      free(list);
      exit(0);
    }else{
      //if none of the commands exist, restart
      printf("Command does not exist, try again.\n");
      free(list);
      continue;
    }
  }

  return 0;

}

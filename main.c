#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
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
	  first_level_table[temp].second_level_table[i].address[j].used = 0;
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
	    if(first_level_table[temp].second_level_table[i].address[j].used == 1){
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

    // allocate
    }else if(!strcmp(command, "allocate")){
      // edge case for X = 0
      unsigned long id = strtoul(list[0], 0, 0);
      if(id == 0){
        printf("Process with id %s does not exist.\n", list[0]);
        free(list);
        continue;
      }

      // checks the process exists
      for(i = 0;i < 4;i++){
	if(id == (unsigned long) tid[i]){
	  // temp is the process index
	  temp = i;
	  found = 1;
	  break;
	}
      }

      if(!found){
	printf("Process with id %s does not exist.\n", list[0]);
      }else{
        // go to mem to allocate memory

        int fd;
        char *pipe = "rdpipe";
        char buf[1024], pnum[20];

        sprintf(pnum, "%d", temp);

        char *str = NULL;
        char *allocate = " allocate";
        str = strcat(pnum, allocate);

        mkfifo(pipe, 0666);

        fd = open(pipe, O_WRONLY);
        write(fd, str, 1024);
        close(fd);

        unlink(pipe);

        while(1){
          char *pipe2 = "wrpipe";

          if((fd = open(pipe2, O_RDONLY)) > 0){
            read(fd, buf, 1024);

            close(fd);

	    int index = atoi(buf);

	    unlink(pipe2);

	    char *arr = NULL;
            cse320_malloc(arr, index, temp);

	    break;

          }else{
	    continue;
          }
        }   

      }

    // read
    }else if(!strcmp(command, "read")){
      
      

      int fd;
      char *pipe = "rdpipe";
      char buf[1024];

      mkfifo(pipe, 0666);

      fd = open(pipe, O_WRONLY);
      //write(fd, "hi", sizeof("hi"));
      close(fd);

      unlink(pipe);

      while(1){
        char *pipe2 = "wrpipe";

        if((fd = open(pipe2, O_RDONLY)) > 0){
          read(fd, buf, 1024);

          close(fd);

          //printf("Received message: %s\n", buf);
	  unlink(pipe2);
	  break;
        }else{
	  continue;
        }
      } 

    // write
    }else if(!strcmp(command, "write")){ 
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

	// check if virtual memory exists
	char part1[11];
	char part2[11];
	part1[10] = '\0';
	part2[10] = '\0';

	strncpy(part1, list[1], 10);
	strncpy(part2, list[1]+10, 10);

        // go to mem to read memory
        int fd;
        char *pipe = "rdpipe";
        char buf[1024], pnum[20];

        sprintf(pnum, "%d", temp);

        char *str = NULL;
        char *readmem = " read";

	//cse320_virt_to_phys();

        str = strcat(pnum, readmem);

        mkfifo(pipe, 0666);

        fd = open(pipe, O_WRONLY);
        write(fd, str, 1024);
        close(fd);

        unlink(pipe);

        while(1){
          char *pipe2 = "wrpipe";

          if((fd = open(pipe2, O_RDONLY)) > 0){
            read(fd, buf, 1024);

            close(fd);

	    int index = atoi(buf);

	    unlink(pipe2);

	    char *arr = NULL;
            cse320_malloc(arr, index, temp);

	    break;

          }else{
	    continue;
          }
        }   

        

      }

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

  return;
}

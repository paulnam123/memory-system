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

      if(!found){
	printf("Process with id %s does not exist.\n", list[0]);
      }else{

	// invalidate cache for killed process
	for(i = 0;i < 4;i++){
	  if(cache_table[i].addr >= (temp * 256) && cache_table[i].addr < ((temp * 256) + 256)){
	    cache_table[i].valid = 0;
	  } 
	}

        // reset tables and tid from first table
        first_level_table[temp].tid = 0;

        for(i = 0;i < 1024;i++){
          for(j = 0;j < 1024;j++){
	    first_level_table[temp].second_level_table[i].address[j].used = 0;
          }
        }

	// remove traces of killed process in memory
	  
        int fd;
        char *pipe = "rdpipe";
        char pnum[20];

        sprintf(pnum, "%d", temp);

        char *str = NULL;
        char *kill = " kill";
        str = strcat(pnum, kill);

        mkfifo(pipe, 0666);

        fd = open(pipe, O_WRONLY);
        write(fd, str, 1024);
        close(fd);

        unlink(pipe);
      }


    // list
    }else if(!strcmp(command, "list")){
      for(i = 0;i < 4;i++){
	if(tid[i] != 0){
	  printf("%lu\n", (unsigned long) tid[i]);
	  found = 1;
	}
      }
    if(!found){
      printf("There are no current processes.\n");
    }

    // mem
    }else if(!strcmp(command, "mem")){
      // edge case for X = 0
      unsigned long id = strtoul(list[0], 0, 0);
      if(id == 0){
        printf("Process with id %s does not exist.\n", list[0]);
        free(list);
        continue;
      }

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
	found = 0;
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

	      found = 1;
	    }
	  }
	}
	if(!found){
	  printf("No virtual memory addresses used by process %s\n", list[0]);
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
      // edge case for X = 0
      unsigned long id = strtoul(list[0], 0, 0);
      if(id == 0){
        printf("Process with id %s does not exist.\n", list[0]);
        free(list);
        continue;
      }

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

	int p1 = strtol(part1, NULL, 2);
	int p2 = strtol(part2, NULL, 2);

	// temp here is process index
	if(first_level_table[temp].second_level_table[p1].address[p2].used == 1){
	
	  // read cache first
	  int phys = cse320_virt_to_phys(temp, p1, p2);

	  // mod 4 to get index of cache (direct mapped)
	  int cache_index = (phys/4) % 4;
	  
	  if(cache_table[cache_index].valid == 1 && cache_table[cache_index].addr == phys){
	    printf("Cache hit\n");
	    printf("%d\n", cache_table[cache_index].value);
	  }else{
	    printf("Cache miss\n");
	    if(cache_table[cache_index].valid == 1){
	      printf("Eviction\n");
	    }

            // go to mem to read memory

            int fd;
            char *pipe = "rdpipe";
            char buf[1024], pnum[20];

            sprintf(pnum, "%d", temp);

            char *str = NULL;
            char *readmem = " read ";

	    //int phys = cse320_virt_to_phys(temp, p1, p2);

	    char phys_addr[20];
	    sprintf(phys_addr, "%d", phys);

            str = strcat(pnum, readmem);
	    str = strcat(str, phys_addr);

            mkfifo(pipe, 0666);

            fd = open(pipe, O_WRONLY);
            write(fd, str, 1024);
            close(fd);

            unlink(pipe);

	    int retval;

            while(1){
              char *pipe2 = "wrpipe";

              if((fd = open(pipe2, O_RDONLY)) > 0){
                read(fd, buf, 1024);

                close(fd);
		unlink(pipe2);

		char *tok = NULL;

		tok = strtok(buf, " \n");

	        if(!strcmp(tok, "error")){
	          char **ret = malloc(100 * sizeof(char));

		  if(buf[6] == 'a'){
		    printf("Requested address is not aligned.\n");
		  }else if (buf[6] == 'r'){
		    printf("Requested address out of range.\n");
		  }

		  free(ret);
		  break;
		}else{

	          retval = atoi(buf);

	          // update value in the cache table
	          cache_table[cache_index].addr = phys;
	          cache_table[cache_index].value = retval;
	          cache_table[cache_index].valid = 1;	    

	          printf("%d\n", retval);

	          break;
		}

              }else{
	        continue;
              }
            }  

	  }

	}else{
	  printf("Virtual memory address does not exist.\n");
	}
      } 

    // write
    }else if(!strcmp(command, "write")){ 
      // edge case for X = 0
      unsigned long id = strtoul(list[0], 0, 0);
      if(id == 0){
        printf("Process with id %s does not exist.\n", list[0]);
        free(list);
        continue;
      }

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

	int p1 = strtol(part1, NULL, 2);
	int p2 = strtol(part2, NULL, 2);

	// temp here is process index
	if(first_level_table[temp].second_level_table[p1].address[p2].used == 1){

	  // write to cache first (write through)
	  int phys = cse320_virt_to_phys(temp, p1, p2);

	  // mod 4 to get index of cache (direct mapped)
	  int cache_index = (phys/4) % 4;
	  
	  if(cache_table[cache_index].valid == 1 && cache_table[cache_index].addr == phys){
	    printf("Cache hit\n");
	  }else{
	    printf("Cache miss\n");
	    if(cache_table[cache_index].valid == 1){
	      printf("Eviction\n");
	    }
	  }



          // go to mem to write memory
          int fd;
          char *pipe = "rdpipe";
          char buf[1024], pnum[20];

          sprintf(pnum, "%d", temp);

          char *str = NULL;
          char *writemem = " write ";

	  //int phys = cse320_virt_to_phys(temp, p1, p2);

	  char phys_addr[20];
	  sprintf(phys_addr, "%d", phys);

	  // used for extra space to place writing value after
	  char *phys_addr_space = strcat(phys_addr, " ");

          str = strcat(pnum, writemem);
	  str = strcat(str, phys_addr_space);
	  str = strcat(str, list[2]);

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

	      unlink(pipe2);

	      // check if error is returned

	      char *tok = NULL;
	      //int c = 0;

	      tok = strtok(buf, " \n");

	      if(!strcmp(tok, "error")){
	        char **ret = malloc(100 * sizeof(char));

		if(buf[6] == 'a'){
		  printf("Requested address is not aligned.\n");
		}else if (buf[6] == 'r'){
		  printf("Requested address out of range.\n");
		}

		free(ret);
		break;
	      }else{
		
	        // update value in the cache table
	        cache_table[cache_index].addr = phys;
	        int value = atoi(list[2]);
	        cache_table[cache_index].value = value;
	        cache_table[cache_index].valid = 1;
	      }

	      break;

            }else{
	      continue;
            }
          }
	}else{
	  printf("Virtual memory adress does not exist.\n");
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
/*
    // print cache********************
    for(i = 0;i < 4;i++){
      printf("----------------------\n");
      printf("valid: %d, addr: %d, value: %d\n", cache_table[i].valid, cache_table[i].addr, cache_table[i].value);
    }
      printf("----------------------\n");
*/


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

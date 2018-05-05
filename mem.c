#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){

  void *memory = malloc(1024);
/*
  *(int*)(memory) = 10;
  int x = *(int*)(memory);
  int y = *(int*)(memory+1);

  printf("%d\n", x);
  printf("%d\n", y);
*/
  while(1){

    int fd;
    char *pipe = "rdpipe";
    char buf[1024];

    if((fd = open(pipe, O_RDONLY)) > 0){
      read(fd, buf, 1024);
 
      sleep(5);

      //printf("Received message: %s\n", buf);
      close(fd);

      unlink(pipe);

      // use the buf for allocate/read/write
      // first is process, then command
      char **list = malloc(255 * sizeof(char));
      char *token = NULL;
      int count = 0, i;

      token = strtok(buf, " \n");
      while(token != NULL){
	list[count] = token;
	token = strtok(NULL, " \n");
	count++;
      }
      if(count <= 1){
	list[1] = 0x0;
      }

      // get the process number
      int process = atoi(list[0]);

      if(!strcmp(list[1], "allocate")){
	
      }else if(!strcmp(list[1], "read")){

      }else if(!strcmp(list[1], "write")){

      }


      // for returning for read/write
      char *pipe2 = "wrpipe";

      mkfifo(pipe2, 0666);

      fd = open(pipe2, O_WRONLY);
      //write(fd, "bye", sizeof("bye"));
      close(fd);
    
      unlink(pipe2);


    }else{
      continue;
    }
  }

  free(memory);

  exit(0);
}

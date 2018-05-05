#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){

  void *memory = malloc(1024); 
  int n;
  for(n = 0;n < 1024;n++){
    *(int*)(memory+n) = 0;
  }

/*  *(int*)(memory) = 10;
  *(int*)(memory+4) = 54;
  *(int*)(memory+8) = 23;
  int x = *(int*)(memory);
  int y = *(int*)(memory+4);
  int z = *(int*)(memory+8);

  printf("%d\n", x);
  printf("%d\n", y);
  printf("%d\n", z);
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
      int count = 0, i, addr, segment_offset;

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
      segment_offset = process * 64;

      if(!strcmp(list[1], "allocate")){
	for(i = segment_offset;i < (segment_offset + 64);i++){
	   if((*(int*)(memory+i)) == 0){
	     addr = i;
	     break;
	   }
	}

	char addrback[100];
        char *pipe2 = "wrpipe";

	sprintf(addrback, "%d", addr);

        mkfifo(pipe2, 0666);

        fd = open(pipe2, O_WRONLY);
        write(fd, addrback, 100);
        close(fd);
    
        unlink(pipe2);

      }else if(!strcmp(list[1], "read")){

      }else if(!strcmp(list[1], "write")){

      }


    }else{
      continue;
    }
  }

  free(memory);

  exit(0);
}

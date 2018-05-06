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
  for(n = 0;n < 1024;n += 4){
    *(int*)(memory+n) = 0;
  }


  int valid[1024];

  for(n = 0;n < 1024;n++){
    valid[n] = 0;
  }

  while(1){

    int fd;
    char *pipe = "rdpipe";
    char buf[1024];

    if((fd = open(pipe, O_RDONLY)) > 0){
      read(fd, buf, 1024);

      // change sleep back to 5
      sleep(1);

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
      segment_offset = process * 256;

      if(!strcmp(list[1], "allocate")){
	for(i = segment_offset;i < (segment_offset + 64);i+=4){
	  if(valid[i] == 0){
	    addr = i;
	    valid[i] = 1;
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

      free(list);    

    }else{
      continue;
    }
  }

  free(memory);


  exit(0);
}

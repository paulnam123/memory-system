#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv){

  while(1){

    int fd;
    char *pipe = "rdpipe";
    char buf[1024];

    fd = open(pipe, O_RDONLY);
    read(fd, buf, 1024);
 
    sleep(5);

    printf("Received message: %s\n", buf);
    close(fd);

    /*char *pipe2 = "wrpipe";

    mkfifo(pipe2, 0666);

    fd = open(pipe2, O_WRONLY);
    write(fd, "bye", sizeof("bye"));
    close(fd);
    
    unlink(pipe2);
*/
  }

  exit(0);
}

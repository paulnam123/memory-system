#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include "functions.h"

char* cse320_malloc(char *buffer, int index){

  // argument takes in index of process

  int i, j;
  for(i = 0;i < 1024;i++){
    for(j = 0;j < 1024;j++){
      if(first_level_table[index].second_level_table[i].address[j].valid == 0){
	char first[11], second[11];
	char *last = "000000000000\0";

	int_to_binary(first, i);
	int_to_binary(second, j);

	buffer = strcat(buffer, first);
	buffer = strcat(buffer, second);
	buffer = strcat(buffer, last);
	printf("%s\n", buffer);
      }
    }
  }

  return buffer;

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

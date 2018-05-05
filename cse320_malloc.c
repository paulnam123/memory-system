#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include "functions.h"

char* cse320_malloc(char *buffer, int index){

  int i, j, found = 0;
  for(i = 0;i < 1024;i++){
    for(j = 0;j < 1024;j++){
      if(first_level_table[index].second_level_table[i].address[j].valid == 0){

	// show that it is used from now on
	first_level_table[index].second_level_table[i].address[j].valid = 1;
	found = 1;
	break;
      }
    }
    
    if(found){
      break;
    }

  }
  // use first_level_table[index].second_level_table[i].address[addr].addr = addr; 
  //check valid bit, if 1 move to next 2nd level table
  

  return buffer;

}

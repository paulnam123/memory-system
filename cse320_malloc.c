#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include "functions.h"

char* cse320_malloc(char *buffer, int index, int process_index){

  int i, found = 0, mod_index;

  mod_index = index - (process_index * 256);

  for(i = 0;i < 1024;i++){
    if(first_level_table[process_index].second_level_table[i].address[mod_index].used == 0){
  
      first_level_table[process_index].second_level_table[i].address[mod_index].used = 1;

      first_level_table[process_index].second_level_table[i].address[mod_index].addr = index; 

      found = 1;
      break;
    } 
  }

  if(!found){
    printf("Not enough space in secondary page table.\n");
  }


    //first_level_table[process_index].second_level_table[i].address[mod_index].addr = index; 

  // use first_level_table[index].second_level_table[i].address[addr].addr = addr; 
  //check valid bit, if 1 move to next 2nd level table
  

  return buffer;

}

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "functions.h"

int cse320_virt_to_phys(int process_index, int first, int second){

  int phys;

  if(first_level_table[process_index].second_level_table[first].address[second].used == 1){
    phys = first_level_table[process_index].second_level_table[first].address[second].addr;
  }

  //phys = phys * 256;

  // maybe return -1 for error? 

  return phys;

}

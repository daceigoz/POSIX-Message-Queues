#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "keys.h"
#include "message.h"

static char *rand_string(char *str, size_t size){
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

int main(){
  int init_=init();


  printf("Response value from the server is: %d\n", init_);
printf("\n");

//Inifnity loop inserting keys
/*
int set_, exist_,items;
char buffer [256];
char * str;
  for(;;){
    str=rand_string(buffer,5);
    set_=set_value(rand_string(buffer,5),"hola",1.1);
    if(set_==-1){
      printf("Error or key already exist\n");
    }
    else{
      printf("Key %s inserted succesfully\n",str);
    }
    exist_= exist(str);
    if(exist_== 0){
      printf("Triplet with the given key does not exist.\n");
    }
    else if(exist_ == 1){
      printf("A triplet with the given key exists\n");
    }
    else{
      printf("Error checking its existence\n");
    }
    items=num_items();
    if(items==-1){
      printf("Error on counting items\n");
    }
    else{
      printf("There are: %d items in the server.\n", items);
    }

  printf("\n");
}*/

  int set_=set_value("hola","hola",1.1);
  if(set_==-1){
    printf("Error or key already exist\n");
  }
  else{
    printf("Key inserted succesfully\n");
  }
printf("\n");
  int set_2=set_value("adios","adios",2.0);
  if(set_2==-1){
    printf("Error or key already exist\n");
  }
  else{
    printf("Key inserted succesfully\n");
  }
  printf("\n");
  char auxstring[256];
  float auxfloat=0.0f;

  int get_=get_value("hola", auxstring, &auxfloat);
  if(get_==-1){
    printf("Error getting the values of that key or key does not exist\n");
  }
  else{
    printf("Key values successfully retrieved:\n");
    printf("Value 1 received: %s\n", auxstring);
    printf("Value 2 received: %f\n", auxfloat);
  }
printf("\n");
  int set1=set_value("hola","hello",1.5);
  if(set1==-1){
    printf("Error or key already exists\n");
  }
  else{
    printf("Key inserted succesfully\n");
  }

printf("\n");
  int exist_= exist("hola");
  if(exist_== 0){
    printf("Triplet with the given key does not exist.\n");
  }
  else if(exist_ == 1){
    printf("A triplet with the given key exists\n");
  }
  else{
    printf("Error checking its existence\n");
  }

printf("\n");
  int exist_1= exist("hello");
  if(exist_1== 0){
    printf("Triplet with the given key does not exist.\n");
  }
  else if(exist_1 == 1){
    printf("A triplet with the given key exists\n");
  }
  else{
    printf("Error checking its existence\n");
  }
printf("\n");
  int items=num_items();
  if(items==-1){
    printf("Error on counting items\n");
  }
  else{
    printf("There are: %d items in the server.\n", items);
  }



}

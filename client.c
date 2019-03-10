
#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keys.h"
#include "message.h"

#define LOOP_SIZE 1000
#define RANDOM_STR_LENGTH 5 //It will be the number -1

//Random string generator for testing (inspire by code from the internet):
char *random_str(char *str, size_t size){
    char dictionary[] = "abcdefghijklmnopqrstuvwxyz";
    if(size){
        --size;
        for(size_t i=0;i<size;i++){
            int key = rand()%(int)(sizeof dictionary-1);
            str[i] = dictionary[key];
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

int set_, exist_,items,i;
char buffer [256];
char * str;
  for(i=0;i<LOOP_SIZE;i++){
    str=random_str(buffer,RANDOM_STR_LENGTH);
    set_=set_value(str,"hola",1.1);
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
}
int delete_, modify_, get_, j;
    for(j=0;j<LOOP_SIZE;j++){
      str=random_str(buffer,RANDOM_STR_LENGTH);
      modify_=modify_value(str, "adios", 0.0);
      if(modify_==-1){
        printf("Error or key doesn't exist\n");
      }
      else{
        printf("Key %s modified succesfully\n",str);
      }
      char get_v1[256];
      float get_v2=0.0f;
      get_=get_value(str, get_v1, &get_v2);
      if(get_==-1){
        printf("Error on get value, key may not exist.\n");
      }
      else{
      printf("Got value1: %s\n", get_v1);
      printf("Got value2: %f\n", get_v2);
    }
      delete_=delete_key(str);
      if(delete_==-1){
        printf("Error on delete key, key may not exist.\n");
      }
      else{
        printf("Key %s deleted succesfully\n",str);
        }
          printf("\n");
    items=num_items();
    if(items==-1){
      printf("Error on counting items\n");
    }
    else{
      printf("There are: %d items in the server.\n", items);
    }
    printf("\n");
}

/*
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

  int get_=get_value("adios", auxstring, &auxfloat);
  if(get_==-1){
    printf("Error getting the values of that key or key does not exist\n");
  }
  else{
    printf("Key values successfully retrieved:\n");
    printf("Value 1 received: %s\n", auxstring);
    printf("Value 2 received: %f\n", auxfloat);
  }
printf("\n");

  int modify_=modify_value("adios","adios",4.5);
  if(modify_==-1){
    printf("Error or key doesn't exist\n");
  }
  else{
    printf("Key modified succesfully\n");
  }
  printf("\n");

  int get1=get_value("hola", auxstring, &auxfloat);
  if(get1==-1){
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

  int delete=delete_key("hola");
  if(delete==-1){
    printf("Error or key doesn't exist\n");
  }
  else{
    printf("Key deleted succesfully\n");
  }
printf("\n");

  int exist_1= exist("hola");
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
*/


}

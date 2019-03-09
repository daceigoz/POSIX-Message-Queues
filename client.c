
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

int set_, exist_,items,i;
char buffer [256];
char * str;
  for(i=0;i<14;i++){
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
}
int delete_,j;
    for(j=0;j<14;j++){
      str=rand_string(buffer,5);
      delete_=delete_key(rand_string(buffer,5));
      if(delete_==-1){
        printf("Error or key doesn't exist\n");
      }
      else{
        printf("Key %s deleted succesfully\n",str);
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

#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "keys.h"
#include "message.h"

int main(){
  int init_=init();

  printf("Response value from the server is: %d\n", init_);

  int set_=set_value("hola","hola",1.1);
  if(set_==-1){
    printf("Error or key already exist\n");
  }
  else{
    printf("Key inserted succesfully%d\n",set_);
  }

  int set1=set_value("hola","hello",1.5);
  if(set1==-1){
    printf("Error or key already exists\n");
  }
  else{
    printf("Key inserted succesfully%d\n",set_);
  }

  int exist_= exist("hola");
  if(exist_== 0){
    printf("Triplet with the given key does not exist.\n");
  }
  else if(exist_ == 1){
    printf("A triplet with the given key exists\n");
  }
  else{
    printf("Error checking its existence %d\n",exist_);
  }


  int exist_1= exist("hello");
  if(exist_1== 0){
    printf("Triplet with the given key does not exist.\n");
  }
  else if(exist_1 == 1){
    printf("A triplet with the given key exists\n");
  }
  else{
    printf("Error checking its existence %d\n", exist_1);
  }

  int items=num_items();
  if(items==-1){
    printf("Error on counting items\n");
  }
  else{
    printf("There are: %d items in the server.\n", items);
  }
/*
  int get_=get_value("hola","hola",1.1);
  if(get_==-1){
    printf("Error or key already exist\n")
  }
  else{
    printf("Key inserted succesfully\n", );
  }


  int modify_=modify_value("hola", "hola", 1.1);
  if(modify_==-1){
    printf("Error on modifying the triplet with the given key.\n");
  }
  else{
    printf("Triplet with the given key successfully modified\n");
  }


  int delete_=delete_key("hola");
  if(delete_==-1){
    printf("Error deleting the triplet.\n");
  }
  else{
    printf("Triplet succesfully deleted.\n");
  }


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


  int items=num_items();
  if(items==-1){
    printf("Error on counting items\n");
  }
  else{
    printf("There are: %d items in the server.\n", items);
  }



  mqd_t server_queue;
  server_queue = mq_open("/server", O_WRONLY);

  struct message msg_local;
  msg_local.request_type = '0';
  strcpy(msg_local.queue_name, "/server");
  strcpy(msg_local.key, "key of the message");
  strcpy(msg_local.value1, "this is value 1");
  msg_local.value2 = 1.1;

  if(mq_send(server_queue, &msg_local, sizeof(struct message), 0) == -1){
    printf("Error sending the message\n");
    return -1;
  }*/


}

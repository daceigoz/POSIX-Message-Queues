#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "message.h"

int init(){

  //Creating the queue for init requests within the clients:
  mqd_t init_queue;

  struct mq_attr init_queue_attr;

  bzero(&queue_attr, sizeof(struct mq_attr));
  queue_attr.mq_msgsize = sizeof(int);
  queue_attr.mq_maxmsg = 10;
  queue_attr.mq_flags = 0;
  queue_attr.mq_curmsgs = 0;

  init_queue = mq_open("/init", O_CREAT | O_RDONLY,  0777, &init_queue_attr);
  if (init_server_queue == -1) {
    perror("Can't create init function queue.\n");
    return -1;
  }

  //Opening the server queue for sending requests:
  mqd_t server_queue;
  server_queue = mq_open("/server", O_WRONLY);
  if(server_queue == -1){
    perror("Error opening server queue in the init function process.\n");
  }

  //Defining the init request message:
  struct message msg_init;
  msg_init.request_type = '0';  //init will have the request type code '0'
  msg_local.queue_name = "/init"; //NOMBRE TEMPORAL PARA LA QUEUE.
  msg_local.key = "";
  msg_local.value1 = "");
  msg_local.value2 = 0.0;

  //Sending the init request:
  if(mq_send(server_queue, &msg_local, sizeof(struct message), 0) == -1){
    printf("Error sending the init message.\n");
    return -1;
  }

  int response_value; //This variable wil be the recipient for the result of the response.

  //Receiving the response from the server to the init function call:
  if(mq_receive(init_server_queue, &response_value, sizeof(int), 0) == -1){
    printf("Error receiving the result of the init call.\n");
    return -1;
  }

  //Closing the opened queues:
  mq_close(server_queue);

  mq_close(init_server_queue);
  mq_unlink("/init");

  return response_value;
}

int set_value(char*key, char *value1, float value2){
  //set_value will have the request type code '1'

}

int get_value(char *key){
  //get_value will have the request_type code '2'

}

int modify_value(char *key, char *value1, float *value2){
  //modify_value will have the request type code '3'

}

int delete_key(char *key){
  //delete_key will have the request type code '4'

}

int exist(char *key){
  //exist will have the request type code '4'

}

int num_items(){
  //num_items will have the request type code '5'

}

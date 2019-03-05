#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "keys.h"
#include "message.h"

int init(){

  char name_q[256];
  sprintf(name_q, "/i%d", getpid());
  //Creating the queue for init requests within the clients:
  mqd_t init_queue;
  struct mq_attr init_queue_attr;

  bzero(&init_queue_attr, sizeof(struct mq_attr));
  init_queue_attr.mq_msgsize = sizeof(int);
  init_queue_attr.mq_maxmsg = 10;
  init_queue_attr.mq_flags = 0;
  init_queue_attr.mq_curmsgs = 0;

  init_queue = mq_open(name_q, O_CREAT | O_RDONLY,  0777, &init_queue_attr);
  if (init_queue == -1) {
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
  strcpy(msg_init.queue_name, name_q);
  strcpy(msg_init.key, "");
  strcpy(msg_init.value1, "");
  msg_init.value2 = 0.0;

  //Sending the init request:
  if(mq_send(server_queue, (char *)&msg_init, sizeof(struct message), 0) == -1){
    printf("Error sending the init message.\n");
    return -1;
  }



  int response_value; //This variable wil be the recipient for the result of the response.

  //Receiving the response from the server to the init function call:
  if(mq_receive(init_queue, (char *)&response_value, sizeof(int), 0) == -1){
    printf("Error receiving the result of the init call.\n");
    return -1;
  }
  printf("Server was succesfully initialized.\n");

  //Closing the opened queues:
  mq_close(server_queue);

  mq_close(init_queue);
  mq_unlink(name_q);

  return response_value;
}

////////////////////////////////////////////////////////////////////////////////

int set_value(char*key, char *value1, float value2){
  //set_value will have the request type code '1'
  char name_s[256];
  sprintf(name_s, "/s%d", getpid());
  //Creating the queue for init requests within the clients:
  mqd_t set_queue;

  struct mq_attr set_queue_attr;

  bzero(&set_queue_attr, sizeof(struct mq_attr));
  set_queue_attr.mq_msgsize = sizeof(int);
  set_queue_attr.mq_maxmsg = 10;
  set_queue_attr.mq_flags = 0;
  set_queue_attr.mq_curmsgs = 0;

  set_queue = mq_open(name_s, O_CREAT | O_RDONLY,  0777, &set_queue_attr);
  if (set_queue == -1) {
    perror("Can't create set function queue.\n");
    return -1;
  }

  //Opening the server queue for sending requests:
  mqd_t server_queue;
  server_queue = mq_open("/server", O_WRONLY);
  if(server_queue == -1){
    perror("Error opening server queue in the set function process.\n");
  }

  //Defining the set request message:
  struct message msg_set;
  msg_set.request_type = '1';  //set will have the request type code '1'
  strcpy(msg_set.queue_name, name_s);
  strcpy(msg_set.key, key);
  strcpy(msg_set.value1, value1);
  msg_set.value2 = value2;

  //Sending the init request:
  if(mq_send(server_queue, (char *)&msg_set, sizeof(struct message), 0) == -1){
    printf("Error sending the set message.\n");
    return -1;
  }

  int response_value; //This variable wil be the recipient for the result of the response.

  //Receiving the response from the server to the init function call:
  if(mq_receive(set_queue, (char *)&response_value, sizeof(int), 0) == -1){
    printf("Error receiving the result of the set call.\n");
    return -1;
  }
  //printf("The triplet was correctly added.\n");

  //Closing the opened queues:
  mq_close(server_queue);

  mq_close(set_queue);
  mq_unlink(name_s);

  return response_value;
}

////////////////////////////////////////////////////////////////////////////////


int get_value(char *key, char *value1, float* value2){
  char name_g[256];
  sprintf(name_g, "/g%d", getpid());
  //Creating the queue for get requests within the clients:
  mqd_t get_queue;

  struct mq_attr get_queue_attr;

  bzero(&get_queue_attr, sizeof(struct mq_attr));
  get_queue_attr.mq_msgsize = sizeof(int);
  get_queue_attr.mq_maxmsg = 10;
  get_queue_attr.mq_flags = 0;
  get_queue_attr.mq_curmsgs = 0;

  get_queue = mq_open(name_g, O_CREAT | O_RDONLY,  0777, &get_queue_attr);
  if (get_queue == -1) {
    perror("Can't create get_value function queue.\n");
    return -1;
  }

  //Opening the server queue for sending requests:
  mqd_t server_queue;
  server_queue = mq_open("/server", O_WRONLY);
  if(server_queue == -1){
    perror("Error opening server queue in the init function process.\n");
  }

  //Defining the get request message:
  struct message msg_get;
  msg_get.request_type = '2';  //get will have the request type code '2'
  strcpy(msg_get.queue_name, name_g);
  strcpy(msg_get.key, key);
  strcpy(msg_get.value1, value1);
  msg_get.value2 = 0.0;

  //Sending the init request:
  if(mq_send(server_queue, (char *)&msg_get, sizeof(struct message), 0) == -1){
    printf("Error sending the get message.\n");
    return -1;
  }

  int response_value; //This variable wil be the recipient for the result of the response.

  //Receiving the response from the server to the get function call:
  if(mq_receive(get_queue, (char *)&response_value, sizeof(int), 0) == -1){
    printf("Error receiving the result of the get call.\n");
    return -1;
  }
  //printf("The values of the requested triplet are: blablabla.\n");

  //Closing the opened queues:
  mq_close(server_queue);

  mq_close(get_queue);
  mq_unlink(name_g);

  return response_value;
}

////////////////////////////////////////////////////////////////////////////////


int modify_value(char *key, char *value1, float value2){
  //modify_value will have the request type code '3'

  char name_m[256];
  sprintf(name_m, "/m%d", getpid());
  //Creating the queue for modify requests within the clients:
  mqd_t modify_queue;

  struct mq_attr modify_queue_attr;

  bzero(&modify_queue_attr, sizeof(struct mq_attr));
  modify_queue_attr.mq_msgsize = sizeof(int);
  modify_queue_attr.mq_maxmsg = 10;
  modify_queue_attr.mq_flags = 0;
  modify_queue_attr.mq_curmsgs = 0;

  modify_queue = mq_open(name_m, O_CREAT | O_RDONLY,  0777, &modify_queue_attr);
  if (modify_queue == -1) {
    perror("Can't create modify function queue.\n");
    return -1;
  }

  //Opening the server queue for sending requests:
  mqd_t server_queue;
  server_queue = mq_open("/server", O_WRONLY);
  if(server_queue == -1){
    perror("Error opening server queue in the modify function process.\n");
  }

  //Defining the modify request message:
  struct message msg_modify;
  msg_modify.request_type = '3';  //set will have the request type code '1'
  strcpy(msg_modify.queue_name, name_m);
  strcpy(msg_modify.key, key);
  strcpy(msg_modify.value1, value1);
  msg_modify.value2 = value2;

  //Sending the modify request:
  if(mq_send(server_queue, (char *)&msg_modify, sizeof(struct message), 0) == -1){
    printf("Error sending the modify message.\n");
    return -1;
  }

  int response_value; //This variable wil be the recipient for the result of the response.

  //Receiving the response from the server to the modify function call:
  if(mq_receive(modify_queue, (char *)&response_value, sizeof(int), 0) == -1){
    printf("Error receiving the result of the modify call.\n");
    return -1;
  }
  //printf("Values of the triplet with the given key have been correctly modified.\n");

  //Closing the opened queues:
  mq_close(server_queue);

  mq_close(modify_queue);
  mq_unlink(name_m);

  return response_value;
}

////////////////////////////////////////////////////////////////////////////////


int delete_key(char *key){
  char name_d[256];
  sprintf(name_d, "/d%d", getpid());
  //Creating the queue for init requests within the clients:
  mqd_t delete_queue;

  struct mq_attr delete_queue_attr;

  bzero(&delete_queue_attr, sizeof(struct mq_attr));
  delete_queue_attr.mq_msgsize = sizeof(int);
  delete_queue_attr.mq_maxmsg = 10;
  delete_queue_attr.mq_flags = 0;
  delete_queue_attr.mq_curmsgs = 0;

  delete_queue = mq_open(name_d, O_CREAT | O_RDONLY,  0777, &delete_queue_attr);
  if (delete_queue == -1) {
    perror("Can't create delete function queue.\n");
    return -1;
  }

  //Opening the server queue for sending requests:
  mqd_t server_queue;
  server_queue = mq_open("/server", O_WRONLY);
  if(server_queue == -1){
    perror("Error opening server queue in the set function process.\n");
  }

  //Defining the set request message:
  struct message msg_delete;
  msg_delete.request_type = '4';  //delete will have the request type code '4'
  strcpy(msg_delete.queue_name, name_d);
  strcpy(msg_delete.key, key);
  strcpy(msg_delete.value1, "");
  msg_delete.value2 = 0.0;

  //Sending the init request:
  if(mq_send(server_queue, (char *)&msg_delete, sizeof(struct message), 0) == -1){
    printf("Error sending the delete message.\n");
    return -1;
  }

  int response_value; //This variable wil be the recipient for the result of the response.

  //Receiving the response from the server to the init function call:
  if(mq_receive(delete_queue, (char *)&response_value, sizeof(int), 0) == -1){
    printf("Error receiving the result of the delete call.\n");
    return -1;
  }
  //printf("Triplet was successfully deleted on the server.\n");

  //Closing the opened queues:
  mq_close(server_queue);

  mq_close(delete_queue);
  mq_unlink(name_d);

  return response_value;
}

////////////////////////////////////////////////////////////////////////////////


int exist(char *key){
  char name_e[256];
  sprintf(name_e, "/e%d", getpid());
  //Creating the queue for exist requests within the clients:
  mqd_t exist_queue;

  struct mq_attr exist_queue_attr;

  bzero(&exist_queue_attr, sizeof(struct mq_attr));
  exist_queue_attr.mq_msgsize = sizeof(int);
  exist_queue_attr.mq_maxmsg = 10;
  exist_queue_attr.mq_flags = 0;
  exist_queue_attr.mq_curmsgs = 0;

  exist_queue = mq_open(name_e, O_CREAT | O_RDONLY,  0777, &exist_queue_attr);
  if (exist_queue == -1) {
    perror("Can't create exist function queue.\n");
    return -1;
  }

  //Opening the server queue for sending requests:
  mqd_t server_queue;
  server_queue = mq_open("/server", O_WRONLY);
  if(server_queue == -1){
    perror("Error opening server queue in the exist function process.\n");
    return -1;
  }

  //Defining the exist request message:
  struct message msg_exist;
  msg_exist.request_type = '5';  //exist will have the request type code '5'
  strcpy(msg_exist.queue_name, name_e);
  strcpy(msg_exist.key, key);
  strcpy(msg_exist.value1, "");
  msg_exist.value2 = 0.0;

  //Sending the exist request:
  if(mq_send(server_queue, (char *)&msg_exist, sizeof(struct message), 0) == -1){
    printf("Error sending the exist message.\n");
    return -1;
  }

  int response_value; //This variable wil be the recipient for the result of the response.

  //Receiving the response from the server to the exist function call:
  if(mq_receive(exist_queue, (char *)&response_value, sizeof(int), 0) == -1){
    printf("Error receiving the result of the exist call.\n");
    return -1;
  }

  //printf("The triplet exists: %d.\n", response_value);


  //Closing the opened queues:
  mq_close(server_queue);

  mq_close(exist_queue);
  mq_unlink(name_e);

  return response_value;
}

////////////////////////////////////////////////////////////////////////////////


int num_items(){
  char name_n[256];
  sprintf(name_n, "/n%d", getpid());
  //Creating the queue for num_items requests within the clients:
  mqd_t num_queue;

  struct mq_attr num_queue_attr;

  bzero(&num_queue_attr, sizeof(struct mq_attr));
  num_queue_attr.mq_msgsize = sizeof(int);
  num_queue_attr.mq_maxmsg = 10;
  num_queue_attr.mq_flags = 0;
  num_queue_attr.mq_curmsgs = 0;

  num_queue = mq_open(name_n, O_CREAT | O_RDONLY,  0777, &num_queue_attr);
  if (num_queue == -1) {
    perror("Can't create num items function queue.\n");
    return -1;
  }

  //Opening the server queue for sending requests:
  mqd_t server_queue;
  server_queue = mq_open("/server", O_WRONLY);
  if(server_queue == -1){
    perror("Error opening server queue in the num items function process.\n");
  }

  //Defining the exist request message:
  struct message msg_num;
  msg_num.request_type = '6';  //exist will have the request type code '6'
  strcpy(msg_num.queue_name, name_n);
  strcpy(msg_num.key, "");
  strcpy(msg_num.value1, "");
  msg_num.value2 = 0.0;

  //Sending the exist request:
  if(mq_send(server_queue, (char *)&msg_num, sizeof(struct message), 0) == -1){
    printf("Error sending the num items message.\n");
    return -1;
  }

  int response_value; //This variable wil be the recipient for the result of the response.

  //Receiving the response from the server to the exist function call:
  if(mq_receive(num_queue, (char *)&response_value, sizeof(int), 0) == -1){
    printf("Error receiving the result of the num items call.\n");
    return -1;
  }
  //printf("The number of items in the server is: %d.\n", response_value);

  //Closing the opened queues:
  mq_close(server_queue);

  mq_close(num_queue);
  mq_unlink(name_n);

  return response_value;
}

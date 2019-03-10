#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_t.h"
#include "message.h"
#include "get_result.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex_msg;
int msg_not_copied=1;
pthread_cond_t cond_msg;

struct node * head=NULL;

void *process_message(struct message * data){

  struct message msg_local;

  pthread_mutex_lock(&mutex_msg);
  memcpy((char *) &msg_local, (char *)data, sizeof(struct message));
  msg_not_copied = 0;
  pthread_cond_signal(&cond_msg);
  pthread_mutex_unlock(&mutex_msg);

  pthread_mutex_lock(&mutex);
  int response,counter;
  struct node * aux1 = head;
  struct node * aux2 = head;
  struct get_result results;
  results.op_result=0;
  strcpy(results.get_value1, "");
  results.get_value2=0.0f;

  //Opening client queue
  mqd_t client_queue;
  client_queue = mq_open(msg_local.queue_name, O_WRONLY);
  if(client_queue == -1){
    perror("Error opening client message queue.\n");
    printf("Thread terminated.\n");
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
  }

  switch (msg_local.request_type) {
    case '0': //Init function
    printf("Initializing the server...\n");
      //CONTENT OF THE INIT FUNCTION
      //Release memory previously used
      while(aux1!=NULL){
        aux2=aux1;
        free(aux2);
        aux1=aux1->next;
      }
      head=NULL;
      response=0; //init correct
      break;

    case '1': //Set value function
    printf("-------------Set function-------------\n");
        int exists=0;
        while(aux1!=NULL){ //checking if the key already exists in the server
          if(strcmp(aux1->key,msg_local.key)==0){
            exists=1;
          }
          aux1=aux1->next;
        }
        if(exists==0){
          struct node * temp=malloc(sizeof(struct node));
          strcpy(temp->key, msg_local.key);
          strcpy(temp->value1, msg_local.value1);
          temp->value2=msg_local.value2;
          temp->next=head;
          head=temp;
          response=0; //set correct
          printf("Key inserted succesfully.\n");
        }
        else{
          response=-1; //In this case the key already exists in the server
          printf("Key already exists, sending error...\n");
        }

      break;

    case '2': //Get value function
      printf("-------------Get function-------------\n");

      if(aux1==NULL){
        results.op_result=-1;
        printf("Key not found, sending error...\n");
        if(mq_send(client_queue, (char *)&results, sizeof(struct get_result), 0) == -1){
          printf("Error sending the response.\n");
        }
        mq_close(client_queue);
        printf("Thread terminated.\n");
        pthread_mutex_unlock(&mutex);
        pthread_exit(0);
      }

      results.op_result=0;
      while(aux1!=NULL&&strcmp(aux1->key,msg_local.key)!=0){ //search for the key
        aux1=aux1->next;
      }
      if(aux1==NULL){//not found, sending error
        printf("Key not found, sending error...\n");
        results.op_result=-1;
        if(mq_send(client_queue, (char *)&results, sizeof(struct get_result), 0) == -1){
          printf("Error sending the response\n");
        }
        mq_close(client_queue);
        printf("Thread terminated.\n");
        pthread_mutex_unlock(&mutex);
        pthread_exit(0);
      }
      printf("Key found, sending values...\n");
      strcpy(results.get_value1, aux1->value1);
      results.get_value2=aux1->value2;
      //Given that for the get function the kind of return message is a structure, the send message and queue closing will be done directly here:
      if(mq_send(client_queue, (char *)&results, sizeof(struct get_result), 0) == -1){
        printf("Error sending the response.\n");
      }
      mq_close(client_queue);
      printf("Thread terminated.\n");
      pthread_mutex_unlock(&mutex);
      pthread_exit(0);

    case '3': //Modify value function
      printf("-------------Modify function-------------\n");
      if(head==NULL){
        response=-1;
        printf("There is no triplet stored, no value can be modified.\n");
        break;
      }
      response=0;
      while(aux1!=NULL&&strcmp(aux1->key,msg_local.key)!=0){ //search for the key
        aux1=aux1->next;
      }
      if(aux1==NULL){//not found, sending error
        response=-1;
        printf("Key not found, sending error...\n");
        break;
      }
      strcpy(aux1->value1, msg_local.value1);
      aux1->value2=msg_local.value2;
      printf("The new values for the key %s are %s and %f.\n", aux1->key, aux1->value1, aux1->value2);
      break;

    case '4': //Delete key function
    printf("-------------Delete function-------------\n");
      if (aux1!=NULL&&strcmp(aux1->key,msg_local.key)==0){//check header
        head=aux1->next;
        free(aux1);
        response=0;
      }
      else{
        response=0;
        while(aux1!=NULL&&strcmp(aux1->key,msg_local.key)!=0 ){ //search for the key
          aux2=aux1;
          aux1=aux1->next;
        }
        if(aux1==NULL){
          printf("Key not found, sending error...\n");
          response=-1;
          break;
        }
        printf("Deleting the key...\n");
      aux2->next=aux1->next;//change pointers
      free(aux1);//delete the key
    }
    break;

    case '5': //Exist function
    printf("-------------Exist function-------------\n");
      response=1;
    if(aux1==NULL){
      response=0;
    }
    else{
      while(strcmp(aux1->key,msg_local.key)!=0){ //search for the key
        if(aux1->next==NULL){//not found, sending result
          response=0;
          break;
        }
        aux1=aux1->next;
      }
      if(response) printf("Key was found, sending result...\n");
      else printf("Key not found, sending result...\n");

  }
    break;

    case '6': //Num items function*/
    printf("-------------Number of items function-------------\n");
    counter=0;
      while(aux1!=NULL){
        counter++;
        aux1=aux1->next;
      }
      printf("There are: %d items in the server.\n", counter);
      response=counter;
      break;
  }

  pthread_mutex_unlock(&mutex);
  if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
    printf("Error sending the response.\n");
  }

  mq_close(client_queue);
  printf("Thread terminated.\n");
  pthread_exit(0);
}

  int main(){

    mqd_t server_queue;
    pthread_t thid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    struct message data;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_msg, NULL);
    pthread_cond_init(&cond_msg, NULL);


    struct mq_attr queue_attr;
    bzero(&queue_attr, sizeof(struct mq_attr));
    queue_attr.mq_msgsize = sizeof(struct message);
    queue_attr.mq_maxmsg = 10;
    queue_attr.mq_flags = 0;
    queue_attr.mq_curmsgs = 0;

    server_queue = mq_open("/server", O_CREAT | O_RDONLY,  0777, &queue_attr);
    if (server_queue == -1) {
      perror("Can't create server queue. \n");
      return -1;
    }

    bzero(&data, sizeof(struct message));



    while(1){
     msg_not_copied = 1;
      if(mq_receive(server_queue, (char *)&data, sizeof(struct message), 0) == -1){
        perror("Error receiving a message on the server side.\n");
        mq_close(server_queue);
        return -1;
      }

      if(pthread_create(&thid, &attr, (void*)process_message, &data) == -1){
        printf("Error creating the thread.\n");
        return -1;
      }

      /* wait for thread to copy message */
      pthread_mutex_lock(&mutex_msg);
      while (msg_not_copied){
      pthread_cond_wait(&cond_msg, &mutex_msg);
	    msg_not_copied = 0;
	    pthread_mutex_unlock(&mutex_msg);
      }
    }

    mq_close(server_queue);
    mq_unlink("/server");
}

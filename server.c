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


struct node * head=NULL;

void *process_message(struct message * data){
  int response,counter;
  struct node * aux1 = head;
  struct node * aux2 = head;
  //Opening client queue
  mqd_t client_queue;
  client_queue = mq_open(data->queue_name, O_WRONLY);
  if(client_queue == -1){
    perror("Error opening client message queue.\n");
    printf("Thread terminated\n");
    pthread_exit(0);
  }

  printf("request type: %c\n", data->request_type);

  switch (data->request_type) {
    case '0': //Init function
      printf("Client queue has been opened\n");
      //CONTENT OF THE INIT FUNCTION
      //Release memory previously used
      while(aux1!=NULL){
        aux1=aux1->next;
        if (aux2) free(aux2);
        aux2=aux1;
      }
      head = NULL;
      response=0; //init correct
      break;

    case '1': //Set value function
      if(aux1==NULL){//check header
        head=malloc(sizeof(struct node));
        strcpy(head->key, data->key);
        strcpy(head->value1, data->value1);
        head->value2=data->value2;
        head->next=NULL;
        response=0; //set correct
      }
      else{
        int exists=0;
        while(aux1!=NULL){ //checking if the key already exists in the server
          if(strcmp(aux1->key,data->key)==0){
            exists=1;
          }
          aux1=aux1->next;
        }
        if(exists==0){
          struct node * temp=malloc(sizeof(struct node));
          strcpy(temp->key, data->key);
          strcpy(temp->value1, data->value1);
          temp->value2=data->value2;
          temp->next=head;
          head=temp;
          response=0; //set correct
        }
        else{
          response=-1; //In this case the key already exists in the server
        }
      }
      break;

    case '2': //Get value function
      while(strcmp(head->key,data->key)!=0){ //search for the key
        if(aux1==NULL){//not found, sending error
          response=-1;
          if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
            printf("Error sending the response\n");
          }
          mq_close(client_queue);
          printf("Thread terminated\n");
          pthread_exit(0);
        }
        aux1=aux1->next;
      }
      response=0; //get correct
      break;

    case '3': //Modify value function
      while(strcmp(head->key,data->key)!=0){ //search for the key
        if(aux1==NULL){//not found, sending error
          response=-1;
          if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
            printf("Error sending the response\n");
          }
          mq_close(client_queue);
          printf("Thread terminated\n");
          pthread_exit(0);
        }
        aux1=aux1->next;
      }
      strcpy(aux1->value1, data->value1);
      aux1->value2=data->value2;
      printf("The new values for the key %s are %s and %f\n", aux1->key, aux1->value1, aux1->value2);

      response=0; //get correct
      break;

    case '4': //Delete key function
      if (strcmp(head->key,data->key)==0&&head!=NULL){//check header
        head=aux1->next;
        free(aux1);
        response=0;
        printf("head removed\n");
      }
      else{
        while(strcmp(aux1->next->key,data->key)!=0){ //search for the key
          if(aux1->next==NULL){//not found
            response = -1;
            if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
              printf("Error sending the response\n");
            }
            mq_close(client_queue);
            printf("Thread terminated\n");
            pthread_exit(0);
          }
          aux1=aux1->next;
        }
      aux2=aux1->next;
      if(aux2->next!=NULL){
        aux1->next=aux2->next;//change pointers
      }
      free(aux2);//delete the key
      response=0; //delete correct
    }
    break;

    case '5': //Exist function
    if(aux1==NULL){
      response=0;
    }
    else{
      while(strcmp(aux1->key,data->key)!=0){ //search for the key
        if(aux1->next==NULL){//not found, sending result
          response=0;
          printf("asd\n");
          if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
            printf("Error sending the response\n");
            response=-1;
          }
          mq_close(client_queue);
          printf("Thread terminated\n");
          pthread_exit(0);
        }
        aux1=aux1->next;
      }
    response=1; //key found
  }
    break;

    case '6': //Num items function*/
      counter = 0;
      while(aux1!=NULL){
        counter++;
        aux1=aux1->next;
      }
      response=counter;
      break;
  }



  if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
    printf("Error sending the response\n");
  }

  mq_close(client_queue);
  printf("Thread terminated\n");
  pthread_exit(0);
}

  int main(){

    mqd_t server_queue;
    pthread_t thid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    struct message data;


    struct mq_attr queue_attr;
    bzero(&queue_attr, sizeof(struct mq_attr));
    queue_attr.mq_msgsize = sizeof(struct message);
    queue_attr.mq_maxmsg = 10;
    queue_attr.mq_flags = 0;
    queue_attr.mq_curmsgs = 0;

    server_queue = mq_open("/server", O_CREAT | O_RDONLY,  0777, &queue_attr);
    if (server_queue == -1) {
      perror("Can't create server queue \n");
      return -1;
    }

    bzero(&data, sizeof(struct message));

    while(1){
      if(mq_receive(server_queue, (char *)&data, sizeof(struct message), 0) == -1){
        perror("Error receiving a message on the server side\n");
        mq_close(server_queue);
        return -1;
      }


      if(pthread_create(&thid, &attr, (void*)process_message, &data) == -1){
        printf("Error creating the thread,\n");
        return -1;
      }
    }

    mq_close(server_queue);
    mq_unlink("/server");
}

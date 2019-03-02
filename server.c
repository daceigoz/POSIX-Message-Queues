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
void process_message(struct message * data){
  int response,counter;
  struct node * aux1 = head;
  struct node * aux2 = head;
  //Opening client queue
  mqd_t client_queue;
  client_queue = mq_open(data->queue_name, O_WRONLY);
  if(client_queue == -1){
    perror("Error opening client message queue.\n");
    response=-1;//send error
    if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
      printf("Error sending the response\n");
    }
    mq_close(client_queue);
    printf("Thread terminated\n");
    pthread_exit(0);
  }

  switch (data->request_type) {
    case 0: //Init function
      printf("Client queue has been opened\n");
      //CONTENT OF THE INIT FUNCTION
      //Release memory previously used
      while(aux1!=NULL){
        aux1=aux1->next;
        free(aux2);
        aux2=aux1;
      }
      response=0; //init correct

    case 1: //Set value function
      while(aux1!=NULL){ //search for the first empty node
        aux1=aux1->next;
      }
      aux1=malloc(sizeof(node_t));
      strcpy(aux1->key, data->key);
      strcpy(aux1->value1, data->value1);
      aux1->value2=data->value2;
      aux1->next=NULL;
      response=0; //set corect

    case 2: //Get value function
      while(aux1->key!=data->key||aux1==NULL){ //search for the key
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
      // NO SE COMO RETURNEAR LOS DOS VALORES EN VALUE1 y VALUE2 como dice el enunciado
      strcpy(aux1->value1, data->value1);
      aux1->value2=data->value2;
      response=0; //get corect

    case 3: //Modify value function
      while(aux1->key!=data->key||aux1==NULL){ //search for the key
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
      response=0; //get corect

    case 4: //Delete key function
      if (aux1->key==data->key&&aux1!=NULL){//check header
        head=head->next;
        free(aux1);
        response=0;
      }
      else{
        while(aux1->next->key!=data->key||aux1->next==NULL){ //search for the key
          if(aux1->next==NULL){//not found, sending error
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
      aux2=aux1->next;
      if(aux2->next!=NULL){
        aux1->next=aux2->next;//change pointers
      }
      free(aux2);//delete the key
      response=0; //delete corect
    }

    case 5: //Exist function
      while(aux1->key!=data->key||aux1==NULL){ //search for the key
        if(aux1==NULL){//not found, sending result
          response=0;
          if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
            printf("Error sending the response\n");
          }
          mq_close(client_queue);
          printf("Thread terminated\n");
          pthread_exit(0);
        }
        aux1=aux1->next;
      }
    response=1; //key found
    case 6: //Num items function*/
      while(aux1!=NULL){
        counter++;
        aux1=aux1->next;
      }
      response=counter;
  }

  if(mq_send(client_queue, (char *)&response, sizeof(int), 0) == -1){
    printf("Error sending the response\n");
  }

  mq_close(client_queue);
  printf("Thread terminated\n");
  pthread_exit(0);

  }

  int main(){

    head=malloc(sizeof(node_t));
    head->next=NULL;

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


      if(pthread_create(&thid, &attr, process_message, &data) == -1){
        printf("Error creating the thread,\n");
        return -1;
      }
    }

    mq_close(server_queue);
    mq_unlink("/server");
}

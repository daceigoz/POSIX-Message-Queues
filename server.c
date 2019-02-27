#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "message.h"

void process_message(struct message * data){
  printf("A message has been received. its content is: %s\n", data->key);
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
  int rec;

  while(1){
    if(mq_receive(server_queue, &data, sizeof(struct message), 0) == -1){
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

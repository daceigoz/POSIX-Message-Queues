#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "message.h"

int main(){
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
  }

  mq_close(server_queue);
}

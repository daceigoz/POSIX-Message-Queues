#ifndef STRUCT_MESSAGE
#define STRUCT_MESSAGE
#define MAX_SIZE    256

struct message{
  char  request_type;
  char  queue_name[MAX_SIZE];
  char  key[MAX_SIZE];
  char  value1[MAX_SIZE];
  float * value2;

};

#endif

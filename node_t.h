#ifndef STRUCT_NODE_T
#define STRUCT_NODE_T
#define MAX_SIZE    256

typedef struct node{
  struct node * next;
  char key [MAX_SIZE];
  char value1 [MAX_SIZE];
  float value2;
} node_t;

#endif

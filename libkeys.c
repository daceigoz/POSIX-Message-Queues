#include <printf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "message.h"

int init(){}

int set_value(char*key, char *value1, float value2){}

int get_value(char *key){}

int modify_value(char *key, char *value1, float *value2){}

int delete_key(char *key){}

int exist(char *key){}

int num_items(){}

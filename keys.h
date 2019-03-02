#ifndef LIBRARY_KEYS_H
#define LIBRARY_KEYS_H

int init();
int set_value(char*key, char *value1, float value2);
int get_value(char *key);
int modify_value(char *key, char *value1, float *value2);
int delete_key(char *key);
int exist(char *key);
int num_items();

#endif

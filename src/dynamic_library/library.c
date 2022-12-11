/*
  If compile with -fvisibility=hidden(gcc -fPIC -shared -fvisibility=hidden library.c -o liblibrary.so),
  we need to add __attribute__((visibility("default"))) for add function
*/ 
#include "stdio.h"

__attribute__((constructor))
void init() {
  printf("init\n");
}

int add(int a, int b)
{
  return a + b;
}
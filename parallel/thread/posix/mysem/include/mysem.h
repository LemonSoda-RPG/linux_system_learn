#ifndef MYSEM_H
#define MYSEM_H
void *sem_init(int n);
void sem_add(void *st,int n);
void sem_sub(void *st,int n);
void sem_destory(void *st);
#endif


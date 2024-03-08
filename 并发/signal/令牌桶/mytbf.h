#ifndef MYTBF_H_
#define MYTBF_H_

#define MYTBF_MAX 1024  //桶的最大值
// typedef void mytbf_st;
struct mytbf_st *mytbf_init(int cps,int burst);

int mytbf_fetchtoken(struct mytbf_st *st,int n);

int mytbf_returntoken(struct mytbf_st *st,int n);

int mytbf_destory(struct mytbf_st *st);

#endif
#ifndef MYTBF_H_
#define MYTBF_H_

#define MYTBF_MAX 1024  //桶的最大值
typedef void mytbf_st;
mytbf_st *mytbf_init(int cps,int burst);

mytbf_fetchtoken();

mytbf_returntoken();







#endif
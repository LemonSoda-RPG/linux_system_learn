#ifndef MYTBF_H
#define MYTBF_H
#define MYTBF_MAX 1024
typedef void mytbf_t;
mytbf_t* mytbf_init(int cps,int burst);
int mytbf_fetchtoken(mytbf_t*,int);  //取token  
int mytbf_returntoken(mytbf_t*,int);  //还token
int mytbf_destory(mytbf_t* p);        //销毁
int mytbf_checktoken(mytbf_t *ptr);
#endif
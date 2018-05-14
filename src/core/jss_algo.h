//
// Created by heymi on 2018/4/27.
//

#ifndef JSS_JSS_ALGO_H

#include <stdbool.h>




typedef struct jss_exchange_s{
    int m,n;//m machines n jobs
    int cost;
    int *T;// 
    int *P;
    int *S;
    int* process_num;
    int length;
} JSSExchange;

typedef void (*reporter_func)(struct jss_exchange_s *);
typedef struct jss_algo_s {
    char name[50];
    void *udata;
    bool (*solve)(struct jss_algo_s *,struct  jss_exchange_s *,reporter_func);
} JSSAlgo;



#define JSS_JSS_ALGO_H

#endif //JSS_JSS_ALGO_H

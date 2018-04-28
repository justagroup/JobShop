//
// Created by heymi on 2018/4/27.
//

#include <stdlib.h>
#include "greedy.h"

#define GET(x,i,j) ((x)[(i-1)*m+(j-1)])
#define SET(x,i,j,v) ((x)[(i-1)*m+(j-1)] = (v))
#define O(i,j) (((i)<<32)+(j))
#define Oi(o) ((o)>>32)
#define Oj(o) ((o)&0xffffffff)


typedef struct greedy_data {
    unsigned int time;
    unsigned int *O;
} Data;


unsigned long long get_min_operate(JSSExchange *ex,unsigned long long *O,unsigned int time, unsigned int machine){
    unsigned int m = ex->m,n=ex->n;
    unsigned int cost = 0; //由于不许出现为0的cost
    unsigned long long result = 0;
    for(unsigned int i=1;i<=n;i++){
        unsigned int p = GET(ex->P,machine,i); //取出这台机器上的工序,即第i个工件的第p个工序
        unsigned  int state = GET(O,i,p); //0代表尚未加工，不是零代表着完成的时间
        if(state == 0 &&(p==1 || GET(O,i,p-1) <= time)){ //该工序尚未完成且上一道工序在此刻已经完成或者就是第一道工序
            if(cost == 0 || GET(ex->T,machine,i) < cost){
                cost =  GET(ex->T,machine,i);
                result = O(i,p);
            }
        }
    }
    return result;
}
static bool slove(JSSAlgo *algo,JSSExchange *ex,reporter_func reporter){
    unsigned int m = ex->m,n=ex->n;
    unsigned int time = 0;
    unsigned int time_add = 0;
    unsigned int *m_next_free = calloc(sizeof(unsigned int),n);
    unsigned int *O = calloc(sizeof(unsigned int),m*n);
    unsigned int finish = 0;
    while (!finish) {
        finish = 1;
        time_add = 0;
        for (int i = 1; i <= n; i++) {
            if (time >= m_next_free[i - 1]) {
                //机器空闲
                unsigned long long op = get_min_operate(ex, O, time, i);
                if (op != 0) {
                    finish = 0;
                    unsigned int cost = GET(ex->T, i, Oi(op));
                    SET(O, Oi(op), Oj(op), time+cost);
                    m_next_free[i-1]+=cost;
                    unsigned int k = 0;
                    while(GET(ex->S,i,++k) != 0);
                    SET(ex->S,i,k,op);
                    if (time_add == 0 || cost < time_add) time_add = cost;
                }
            }
        }
        time += time_add;
    }
    ex->cost = time;
}


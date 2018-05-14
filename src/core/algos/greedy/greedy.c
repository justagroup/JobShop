//
// Created by heymi on 2018/4/27.
//

#include <stdlib.h>
#include <string.h>
#include "greedy.h"

#define GET(x,i,j) ((x)[(i-1)*m+(j-1)])
#define SET(x,i,j,v) ((x)[(i-1)*m+(j-1)] = (v))
#define O(i,j) (((i)<<32)+(j))
#define Oi(o) ((o)>>32)
#define Oj(o) ((o)&0xffffffff)

static long long get_min_operate(JSSExchange* ex,long long*O,int time, int machine){
    int m = ex->m,n=ex->n;
    int cost = 0; //由于不许出现为0的cost
    long long result = 0;
    for(int i=1;i<=n;i++){
        int p = GET(ex->P,machine,i); //取出这台机器上的工序,即第i个工件的第p个工序
        unsigned  int state = GET(O,i,p); //0代表尚未加工，不是零代表着完成的时间
        if(state == 0 &&(p==1 ||  (GET(O,i,p-1)!=0 && GET(O,i,p-1) <= time))){ //该工序尚未完成且上一道工序在此刻已经完成或者就是第一道工序
            if(cost == 0 || GET(ex->T,machine,i) < cost){
                cost =  GET(ex->T,machine,i);
                result = O((long long)i,p);
            }
        }
    }
    return result;
}
static bool slove(JSSAlgo *algo,JSSExchange *ex,reporter_func reporter){
    int m = ex->m,n=ex->n;
    int time = 0;
    int time_add = 0;
    int *m_next_free = calloc(sizeof(int),n);
    long long *O = calloc(sizeof(long long),m*n);
    int finish = 0;
    while (!finish) {
        finish = 1;
        time_add = 0;
        for (int i = 1; i <= m; i++) {
            if (time >= m_next_free[i - 1]) {
                //机器空闲
                long long op = get_min_operate(ex, O, time, i);
                if (op != 0) {
                    finish = 0;
                    int cost = GET(ex->T, i, Oi(op));
                    SET(O, Oi(op), Oj(op), time+cost);
                    m_next_free[i-1]=time+cost;
                    int k = 0;
                    while(GET(ex->S,i,++k) != 0);
                    SET(ex->S,i,k,O(Oi(op),time));
                    if (time_add == 0 || cost < time_add) time_add = cost;
                }
            }
        }
        if(finish){
            //really finish????
            unsigned  int min = 0;
            for (int i = 1; i <= m; i++) {
                if(m_next_free[i-1] > time && (m_next_free[i-1] < min || min == 0)){
                    min = m_next_free[i-1];
                }
            }
            if(min !=0) { time = min; finish = 0;}

        } else time += time_add;

    }
    ex->cost = time;
}

JSSAlgo* jss_algo_create_greedy(){
    JSSAlgo *al = malloc(sizeof(JSSAlgo));
    strcpy(al->name,"greedy");
    al->solve = slove;
    return  al;

}
//
// Created by heymi on 2018/4/28.
//
#include <stdio.h>
#include "dump_excange_s.h"
#define Oi(o) ((o)>>32)
#define Oj(o) ((o)&0xffffffff)
#define GET(x,i,j) ((x)[(i-1)*m+(j-1)])
#define SET(x,i,j,v) ((x)[(i-1)*m+(j-1)] = (v))
void jss_dump_exchange_S(const JSSExchange *ex){
    unsigned int m = ex->m,n=ex->n;
    for(unsigned int i=0;i<=m;i++) {
        for(unsigned int j=0;j<=n;j++) {
            if(i==0&&j==0) printf("\t");
            else if(i==0 && j>0) printf("%d\t",j);
            else if(j==0 && i>0) printf("%d\t",i);
            else {
                unsigned long long jj = Oi(ex->S[(i-1)*m+j-1]);
                unsigned long long start = Oj(ex->S[(i-1)*m+j-1]);
                printf("%lld,%ld,%lld,%ld\t",jj,GET(ex->P,i,jj),start,GET(ex->T,i,jj));
            }
        }
        printf("\n");
    }21
    printf("%d",ex->cost);
}

void jss_dump_exchange_S_to_gantt(const JSSExchange *ex){

}
//
// Created by heymi on 2018/4/28.
//
#include <stdlib.h>
#include "file.h"

#define GET(x,i,j) ((x)[(i-1)*m+(j-1)])
#define SET(x,i,j,v) ((x)[(i-1)*m+(j-1)] = (v))
#define O(i,j) (((i)<<32)+(j))
#define Oi(o) ((o)>>32)
#define Oj(o) ((o)&0xffffffff)

unsigned int m,n,*_Tbegin,*_Tend;

static unsigned int Tbegin(unsigned int i, unsigned int j){
    if(GET(_Tbegin,i,j)!=0) return GET(_Tbegin,i,j);

}
bool jss_write_to_file(const JSSExchange *ex,FILE *fp){
    m = ex->m;n=ex->n;
    Tbegin = calloc(sizeof(unsigned int),m*n);
    Tend = calloc(sizeof(unsigned int),m*n);
    unsigned int count = 0;
    while(count<m*n){
        for(unsigned int j=1;j<=n;j++){
            for(unsigned int i=1;i<=m;i++) {
                //i为机器号
                unsigned int _O= GET(ex->S,i,j);
                unsigned int x = Oi(_O),y=Oj(_O); //第x个工件的第y个工序
                if(y==1 || GET(Tend,x,y-1)!=0){ //第一道工序或者上一道工序完成时间已经计算出来了

                }


            }}
    }
}
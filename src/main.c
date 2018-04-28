//
// Created by heymi on 2018/4/28.
//

#include <stdio.h>
#include <stdlib.h>
#include "core/jss_algo.h"
#include "core/algos/greedy/greedy.h"
#include "io/file.h"
#include "utils/dump_excange_s.h"

int main() {
    printf("Hello, World!\n");
    FILE *fp = fopen("../testdatas/1.txt","r");
    JSSExchange *ex = jss_load_from_file(fp);
    fclose(fp);
    JSSAlgo *al = jss_algo_create_greedy();
    al->solve(al,ex,NULL);
    jss_dump_exchange_S(ex);
    system("pause");
    return 0;
}
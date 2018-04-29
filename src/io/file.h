//
// Created by heymi on 2018/4/27.
//

#ifndef JSS_FILE_H
#define JSS_FILE_H

#include <stdio.h>
#include <stdbool.h>
#include "../core/jss_algo.h"

void emrge_sort(int *a[2], const JSSExchange* data, int process_num);
JSSExchange *jss_load_from_file(FILE *fp);
bool jss_write_to_file(const JSSExchange *data,FILE *fp);
void jss_destory(JSSExchange *ex);


#endif //JSS_FILE_H

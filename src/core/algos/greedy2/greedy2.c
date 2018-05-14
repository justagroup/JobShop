#include <stdlib.h>
#include <string.h>
#include "greedy2.h"
#define MAX_NUM 2147483633
JSSExchange* gd_get_operate(JSSExchange* data) {
    int m = data->m;
    int n = data->n;
    int* traveled = calloc(sizeof(int), n);
    bool* is_empty = malloc(sizeof(bool) * (m + 1));
    int i;
    for (i = 0; i < m + 1; i++)
        is_empty[i] = true;
    int* end_time = calloc(sizeof(int), m + 1);
    int first_end_time = 0;
    int last_min_time = 0;
    while (true) {
        int j;
        ///fill empty machines with the jobs that cost the least time in the same machine.
        for (j = 0; j < data->n; j++) {
            int current_positon = j * m + traveled[j];
            int current_machine = data->P[current_positon];
            int current_job = j;
            if (current_machine != 0 && is_empty[current_machine]) {
                is_empty[current_machine] = false;
                int k = 0;
                int min_time = data->T[current_positon];
                for (k = j + 1; k < n; k++)
                    if (current_machine == data->P[k * m + traveled[k]]) {
                        if (data->T[k * m + traveled[k]] < min_time) {
                            current_positon = k * m + traveled[k];
                            current_job = k;
                            min_time = data->T[current_positon];
                        }
                    }
                data->S[current_positon] = end_time[current_machine];
                end_time[current_machine] += min_time;
                traveled[current_job]++;
            }
        }
        ///find the machine that end first, and turn the clock to the first end time, then let the job-ended machines become empty
        int first_end_time = MAX_NUM;
        for (j = 1; j < m + 1; j++)
            if (end_time[j]>last_min_time && end_time[j] < first_end_time)
                first_end_time = end_time[j];
        last_min_time = first_end_time;
        for (j = 1; j < m + 1; j++)
            if (end_time[j] <= first_end_time)
                is_empty[j] = true;
        ///judge if all jobs have finished
        for (j = 0; j < n; j++)
            if (traveled[j] != data->process_num[j])
                break;
        if (j == n) return data;
    }
}

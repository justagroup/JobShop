#include "file.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*!
* jobs begin wtih 0
* machines begin with 1
* so that we can check whether the job has been finished by search for the machine number in the process.
*/
int jss_get_position(int i, int j, JSSExchange *data) {
	return i * data->m + j;
}
int jss_get_time_cost(int position, JSSExchange *data) {
	return data->T[position];
}
int jss_get_machine_num(int position, JSSExchange *data) {
	return data->P[position];
}
void jss_set_start_time(int value, int position, JSSExchange* data) {
	data->S[position] = value;
}

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



void jss_emrge_sort(int* a[2], const JSSExchange* data, int process_num) {
	int i;
	for (i = 0; i < process_num; i++) {
		int j;
		for (j = process_num - 2; j >= i; j--) {
			int this_position = jss_get_position(a[j][0], a[j][1], data);
			int next_position = jss_get_position(a[j + 1][0], a[j + 1][1], data);
			if (data->S[this_position] > data->S[next_position]) {
				int x = a[j][0];
				int y = a[j][1];
				a[j][0] = a[j + 1][0], a[j][1] = a[j + 1][1];
				a[j + 1][0] = x;
				a[j + 1][1] = y;
			}
		}
	}
}

int jss_change_to_num(FILE* fp) {
	int c = fgetc(fp);
	int coef = 1;
	if (c == '-') coef = -1;
	int num = 0;
	while (c >= '0' && c <= '9') {
		num = num * 10 + c - '0';
		c = fgetc(fp);
	}
	fseek(fp, -1, SEEK_CUR);
	return num * coef;
}


//tp间隔输入
JSSExchange* jss_load_from_file(FILE* fp) {
	int total_process_num = 0;
	int n, m; ///m machines n jobs
	rewind(fp);
	JSSExchange* data = (JSSExchange*)malloc(sizeof(JSSExchange));
	fscanf(fp, "%d %d\n", &n, &m);
	data->n = n;
	data->m = m;
	data->T = (int*)calloc(sizeof(int), m * n);
	data->P = (int*)calloc(sizeof(int), m * n);
	data->S = (int*)calloc(sizeof(int), m * n);
	data->process_num = calloc(sizeof(int), n);
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			fscanf(fp, "%d", &data->P[i*m + j]);
			fscanf(fp, "%d", &data->T[i*m + j]);
			
			data->P[i*m + j]++;
		}
	}
	for (i = 0; i<n; i++) {
		data->process_num[i] = m;
	}
	data->length = n * m;
	return data;
}

////T P分别输入
//JSSExchange* jss_load_from_file(FILE* fp) {
//	int total_process_num = 0;
//	int n, m; ///m machines n jobs
//	rewind(fp);
//	JSSExchange* data = (JSSExchange*)malloc(sizeof(JSSExchange));
//	fscanf(fp,"%d %d\n", &n, &m);
//	data->n = n;
//	data->m = m;
//	data->T = (int*)calloc(sizeof(int), m * n);
//	data->P = (int*)calloc(sizeof(int), m * n);
//	data->S = (int*)calloc(sizeof(int), m * n);
//	data->process_num = calloc(sizeof(int), n);
//	int i,j;
//	for(i=0;i<n;i++) {
//		for (j = 0; j < m; j++)
//			fscanf(fp, "%d", &data->T[i*m+j]);
//	}
//	for(i=0;i<n;i++) {
//		for(j=0;j<m;j++) {
//			fscanf(fp, "%d", &data->P[i*m + j]);
//		}
//	}
//	for(i=0;i<n;i++) {
//		data->process_num[i] = m;
//	}
//	data->length = n * m;
//	return data;
//}

//JSSExchange* jss_load_from_file(FILE* fp) {
//    int total_process_num = 0;
//	int n, m; ///m machines n jobs
//	rewind(fp);
//	JSSExchange* data = (JSSExchange*)malloc(sizeof(JSSExchange));
//	scanf("%d %d\n", &n, &m);
//    data->n = n;
//    data->m = m;
//	data->T = (int*)calloc(sizeof(int), m * n);
//	data->P = (int*)calloc(sizeof(int), m * n);
//	data->S = (int*)calloc(sizeof(int), m * n);
//	data->process_num = calloc(sizeof(int), n);
//	// int ordinal = jss_change_to_num(fp);
//	int ordinal;
//	scanf("%d", &ordinal);
//	while (ordinal != -1) {
//		int i = 0;
//		while (getchar() != '\n') {
//			scanf("(%d,%d)", &data->T[(ordinal -1) * m + i], &data->P[(ordinal -1) * m + i]);
//			/// fseek(fp, 1, SEEK_CUR);
//			/// jss_exchange->T[(ordinal - '1')*m + i] = jss_change_to_num(fp);
//			/// fseek(fp, 1, SEEK_CUR);
//			/// jss_exchange->P[(ordinal - '1')*m + i] = jss_change_to_num(fp);
//			/// fseek(fp, 1, SEEK_CUR);
//			i++;
//		}
//        total_process_num += i;
//		data->process_num[ordinal - 1] = i;
//		/// ordinal= jss_change_to_num(fp);
//		scanf("%d", &ordinal);
//	}
//	return data;
//}

bool jss_write_to_file(const JSSExchange* data, FILE* fp) {
	int i;
	int max_time = 0;
    int** process_location = calloc(sizeof(int), data->m);
	for (i = 1; i <= data->m; i++) {
		/// NO.i machine
		int j;
        memset(process_location, 0, sizeof process_location);
		for (j = 0; j < data->m; j++)
			process_location[j] = calloc(sizeof(int), 2);
		int process_num = 0;
		for (j = 0; j < data->n; j++) {
			///in j row k col ,totally n row m col
			int k;
			for (k = 0; k < data->m; k++)
				if (data->P[j * data->m + k] == i) {
					process_location[process_num][0] = j;
					process_location[process_num][1] = k;
					process_num++;
					break;
				}
		}
		jss_emrge_sort(process_location, data, process_num);
		printf("M%d", i);
		for (j = 0; j < process_num; j++) {
			int row = process_location[j][0];
			int col = process_location[j][1];
			int start_time = data->S[row * data->m + col];
			int end_time = start_time + data->T[row * data->m + col];
			if (end_time > max_time) max_time = end_time;
			printf(" (%d,%d-%d,%d)",
			       start_time,
			       row + 1,
			       col + 1,
			       end_time
			);
			putchar('\n');
			for (j = 0; j < data->m; j++)
				free(process_location[j]);

		}
	}
    free(process_location);
	printf("END %d", max_time);
}

void jss_destory(JSSExchange* ex) {
	free(ex);
	free(ex->P);
	free(ex->S);
	free(ex);
}


#include "file.h"
#include <stdlib.h>
#include <stdio.h>

void emrge_sort(int *a[2], const JSSExchange* data, int process_num) {
	int i;
	for (i = 0; i<process_num; i++) {
		int j;
		for (j = process_num - 1; j > i; j--)
			if (data->S[a[j][0] * data->m + a[j][1]]<data->S[a[j - 1][0] * data->m + a[j - 1][1]]) {
				int *temp_ptr = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp_ptr;
			}
	}
}

unsigned int jss_change_to_num(FILE* fp) {
	int c = fgetc(fp);
	unsigned int coef = 1;
	if (c == '-') coef = -1;
	unsigned int num = 0;
	while (c >= '0' && c <= '9') {
		num = num * 10 + c - '0';
		c = fgetc(fp);
	}
	fseek(fp, -1, SEEK_CUR);
	return num * coef;
}

JSSExchange* jss_load_from_file(FILE* fp) {
	int n, m; //m machines n jobs
	rewind(fp);
	JSSExchange* data = (JSSExchange*)malloc(sizeof(JSSExchange));
	scanf("%ud %ud\n", n, m);
	data->T = (unsigned int*)calloc(sizeof(unsigned int), m * n);
	data->P = (unsigned int*)calloc(sizeof(unsigned int), m * n);
	data->S = (unsigned int*)calloc(sizeof(unsigned int), m * n); //start
																  // int ordinal = jss_change_to_num(fp);
	int ordinal;
	scanf("%ud", &ordinal);
	while (ordinal != -1) {
		int i = 0;
		while (getchar() != '\n') {
			scanf("(%ud,%ud)", &data->T[(ordinal - '1') * m + i], &data->P[(ordinal - '1') * m + i]);
			// fseek(fp, 1, SEEK_CUR);
			// jss_exchange->T[(ordinal - '1')*m + i] = jss_change_to_num(fp);
			// fseek(fp, 1, SEEK_CUR);
			// jss_exchange->P[(ordinal - '1')*m + i] = jss_change_to_num(fp);
			// fseek(fp, 1, SEEK_CUR);
			i++;
		}
		// ordinal= jss_change_to_num(fp);
		scanf("%ud", &ordinal);
	}
	return data;
}

bool jss_write_to_file(const JSSExchange* data, FILE* fp) {
	int i;
	unsigned int max_time = 0;
	for (i = 1; i <= data->m; i++) {
		int j;
		int ** process_location = calloc(sizeof(int), data->m);
		for (j = 0; j < data->m; j++)
			process_location[j] = calloc(sizeof(int), 2);
		int process_num = 0;
		for (j = 0; j < data->n; j++) {//in j row k col ,totally n row m col
			int k;
			for (k = 0; k < data->m; k++)
				if (data->T[j*data->m + k] == i) {
					process_location[process_num][0] = j;
					process_location[process_num][1] = k;
					process_num++;
					break;
				}
		}
		emrge_sort(process_location, data, process_num);
		printf("M%d", i + 1);
		for (j = 0; j < process_num; j++) {
			int row = process_location[j][0];
			int col = process_location[j][1];
			unsigned int start_time = data->S[row*data->m + col];
			unsigned int end_time = start_time + data->T[row*data->m + col];
			if (end_time > max_time) max_time = end_time;
			printf(" (%ud,%d-%d,%ud)",
				start_time,
				row,
				col,
				end_time
			);
			putchar("\n");
			for (j = 0; j < data->m; j++)
				free(process_location[j]);
			free(process_location);
		}
	}
	printf("END %ud", max_time);
}

void jss_destory(JSSExchange* ex) {
	free(ex);
	free(ex->P);
	free(ex->S);
	free(ex);
}

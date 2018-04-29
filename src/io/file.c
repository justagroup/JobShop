#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	data->T = (unsigned int*)calloc(sizeof(unsigned int) , m * n);
	data->P = (unsigned int*)calloc(sizeof(unsigned int) , m * n);
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
	for (i = 0; i < data->m; i++) {
		printf("M%d ", i + 1);
		unsigned int** jss_temp_store = malloc(sizeof(unsigned int*) * data->n);
		int j;
		for (j = 0; j < data->m; j++) {
			jss_temp_store[i] = malloc(sizeof(unsigned int) * 4);
		}
		int k, l;
		for (k = 0; k < data->n; k++)
			for (l = 0; l < data->m; l++) {

			}
		printf("(%d,%d-%d,%d) ");//输出的是 机器号 (起始时间,产品号-工序号,结束时间)
		priintf("")

	}
}

void jss_destory(JSSExchange* ex) {
	free(ex->T);
	free(ex->P);
	free(ex->S);
	free(ex);
}

int main() {
	FILE* fp = fopen("1.txt", "r");
	JSSExchange* jss_data = jss_load_from_file(fp);

}

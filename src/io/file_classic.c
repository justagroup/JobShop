#include <stdlib.h>
#include "file.h"

JSSExchange *jss_load_from_file(FILE *fp) {
    int m, n;
    JSSExchange *ex = malloc(sizeof(JSSExchange));
    fscanf(fp, "%d%d", &m, &n);
    ex->m = m;
    ex->n = n;
    ex->T = calloc(sizeof(int), m * n);
    ex->P = calloc(sizeof(int), m * n);
    ex->S = calloc(sizeof(long long),m*n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            fscanf(fp, "%d", &ex->T[j * m + i]);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            fscanf(fp, "%d", &ex->P[j * m + i]);
    return ex;
}

bool jss_write_to_file(const JSSExchange *data, FILE *fp) {

}

void jss_destory(JSSExchange *ex) {
    free(ex->T);
    free(ex->S);
    free(ex->P);
    free(ex);
}
#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "../../jss_algo.h"

double jss_rand();
int* init_population(int population_size, JSSExchange* data);

bool delete_population(int population_size,int**population);

int format_solution(int* chromosome, JSSExchange* data);

int* crossover(int chromo_length, int* parent1, int* parent2);

int format_solution(int chromo_length, int * chromosome, JSSExchange * data);
    
int* mutation(int chromo_length, int* parent);

int compute_DAG(int* chromo, JSSExchange* data);
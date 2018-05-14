#include "inherit.h"
#include "../../../io/file.h"
#define MAX_JOB 105 
#define MAX_MACHINE 25
#define POPULATION_SIZE 200 
#define MUTIPLE 10
#define CHROMO_CHANGE_TIMES 20
#define MAX_LENGTH 1000
#define ENLARGE_TIMES 5
#define MUTATION_RATE 0.01
typedef struct
{
	int *time_cost;
	int** population;
	int population_size;
	int chromo_size;
	int current_chromo_num;
}Population;


double jss_rand() {
	double r;
	do {
		r = rand() / RAND_MAX;
	} while (r == 0.0 || r == 1.0);
	return r;
}


bool delete_population(int population_size, int ** population)
{
	int i;
	for (i = 0; i < population_size; i++) {
		free(population[i]);
	}
	free(population);
	return true;
}


int * crossover(int chromo_length, int * parent1, int * parent2, JSSExchange* data)
{
	int* temp_chromo = malloc(sizeof(int)*chromo_length * 2);
	int i;
	for (i = 0; i < chromo_length; i++) {
		temp_chromo[i] = parent1[i];
	}
	int begin;
	int end;
	do {
		begin = rand() % chromo_length;
		end = rand() % chromo_length + 1;
	} while (begin == end);
	if (begin > end) {
		int temp_begin = begin;
		begin = end;
		end = temp_begin;
	}
	int segment_length = end - begin;
	int insert_position;
	do {
		insert_position = rand() % chromo_length;
	} while (insert_position == 0);
	for (i = chromo_length - 1; i >= insert_position; i--) {
		temp_chromo[i + segment_length] = temp_chromo[i];
	}
	int* segment_num = calloc(sizeof(int), data->n);
	for (i = begin; i < end; i++, insert_position++) {
		segment_num[parent2[i]]++;
		temp_chromo[insert_position] = parent2[i];
	}
	int temp_chromo_length = segment_length + chromo_length;
	for (i = temp_chromo_length - 1; i > insert_position + segment_length; i--) {
		if (segment_num[temp_chromo[i]] != 0) {
			int j;
			temp_chromo_length--;
			segment_num[temp_chromo[i]]--;
			for (j = i; j < temp_chromo_length; j++) {
				temp_chromo[j] = temp_chromo[j + 1];
			}
		}
	}
	if (temp_chromo_length != chromo_length) {
		for (i = insert_position - 1; i > 0; i--) {
			if (segment_num[temp_chromo[i]] != 0) {
				int j;
				temp_chromo_length--;
				segment_num[temp_chromo[i]]--;
				for (j = i; j < temp_chromo_length; j++) {
					temp_chromo[j] = temp_chromo[j + 1];
				}
			}
		}
	}
	free(segment_num);
	//记得释放内存
	return temp_chromo;
}

int format_solution(int chromo_length, int * chromosome, JSSExchange * data)
{
	return 0;
}


int * mutation(int chromo_length, int * parent)
{
	int position1, position2;
	do {
		position1 = rand() % chromo_length;
		position2 = rand() % chromo_length;
	} while (position1 == position2);
	int temp_num = parent[position1];
	parent[position1] = parent[position2];
	parent[position2] = temp_num;
	return parent;
}

int get_position(int i, int j, JSSExchange *data) {
	return i * data->m + j;
}
int get_time_cost(int position, JSSExchange *data) {
	return data->T[position];
}
int get_machine_num(int position, JSSExchange *data) {
	return data->P[position];
}
void set_start_time(int value, int position, JSSExchange* data) {
	data->S[position] = value;
}

/// 对单条染色体进行解码,并返回时间的最大值
int compute_DAG(int* chromo, JSSExchange* data) {
	int traveled[MAX_JOB];
	bool is_empty[MAX_MACHINE];
	int m = data->m;
	int n = data->n;
	int i;
	for (i = 0; i < m + 1; i++)
		is_empty[i] = true;
	int end_time[MAX_MACHINE];
	int first_end_time = 0;
	int last_min_time = 0;
	for (i = 0; i < data->length; i++) {
		int position = get_position(chromo[i], traveled[chromo[i]], data);
		int current_machine = get_machine_num(position, data);
		int current_time_cost = get_time_cost(position, data);
		// if (is_empty[current_machine]) {
			//非第一步
		if (traveled[chromo[i]] > 0) {
			int pre_position = get_position(chromo[i], traveled[chromo[i]] - 1, data);
			int pre_machine = get_machine_num(pre_position, data);
			//如果工序上一步的结束时间大于现在机器的结束时间
			if (end_time[pre_machine] > end_time[current_machine]) {
				end_time[current_machine] = end_time[pre_machine] + current_time_cost;
			}
			else {
				end_time[current_machine] += current_time_cost;
			}
		}
		else {//为第一步
			end_time[current_machine] += current_time_cost;
		}
		// } else {
		// 	//机器非空且非第一步
		// 	if(traveled[chromo[i]]>0) {
  //
		// 	} else {//机器非空且为第一步
		// 		end_time[current_machine] += current_time_cost;
		// 	}
		// }
		traveled[chromo[i]]++;
	}
	int max_time = 0;
	for (i = 1; i <= m; i++) {
		if (end_time[i] > max_time)
			max_time = end_time[i];
	}
	return max_time;
}

Population* jss_init_population(int population_size, JSSExchange * const data)
{
	Population* population_data = calloc(sizeof(Population), 1);
	population_data->population_size = population_size;
	population_data->chromo_size = data->length;
	int i;
	int process_num = 0;
	///make the first ordered array
	int* ordered_list = calloc(sizeof(int), data->m*data->n);
	for (i = 0; i < data->n; i++) {
		int j = 0;
		for (j = 0; j < data->process_num[i]; j++) {
			int position = i * data->m + j;
			ordered_list[process_num] = i;
			process_num++;
		}
	}
	int *time_cost = calloc(sizeof(int), population_size*MUTIPLE);
	int ** population = malloc(sizeof(int*)* population_size*MUTIPLE);
	for (i = 0; i < population_size; i++) {
		population[i] = calloc(sizeof(int), process_num);
	}
	for (i = 0; i < population_size; i++) {
		int j;
		for (j = 0; j < CHROMO_CHANGE_TIMES; j++) {
			int rand1 = rand() % process_num;
			int rand2 = rand() % process_num;
			int temp = ordered_list[rand1];
			ordered_list[rand1] = ordered_list[rand2];
			ordered_list[rand2] = temp;
		}
		for (j = 0; j < process_num; j++) {
			population[i][j] = ordered_list[j];
		}
	}
	free(ordered_list);
	population_data->population = population;
	population_data->current_chromo_num = population_size;
	return population_data;
}

void enlarge_population(Population* population_data) {
	int i;
	for (i = 0; i < population_data->population_size*(MUTIPLE - 2); i++) {
		int chromo1, chromo2;
		do {
			chromo1 = rand() % population_data->population_size;
			chromo2 = rand() % population_data->population_size;
		} while (chromo1 != chromo2);
		population_data->population[population_data->current_chromo_num] =
			crossover(population_data->chromo_size, population_data->population[chromo1], population_data->population[chromo2], data);
		population_data->current_chromo_num++;
		if (jss_rand() < MUTATION_RATE)
			mutation(population_data->chromo_size, population_data->population[rand() % population_data->population_size]);
	}
}

void lessen_population(Population* population_data) {
	int i = population_data->population_size;
	for (; i < population_data->current_chromo_num; i++) {
		free(population_data->population[i]);
	}
	population_data->current_chromo_num = population_data->population_size;
}

void find_best_chromo(Population* population_data) {
	int i, j;
	int ** pop = population_data->population;
	int * t = population_data->time_cost;
	for (i = 0; i < population_data->current_chromo_num; i++) {
		for (j = population_data->current_chromo_num - 2; j >= i; j--) {
			if (t[j] < t[j + 1]) {
				int temp = t[j];
				t[j] = t[j + 1];
				t[j + 1] = temp;
				int *temp2 = pop[j];
				pop[j] = pop[j + 1];
				pop[j + 1] = temp2;

			}
		}
	}
}

void decode(Population* population_data, JSSExchange* data) {
	int i;
	for (i = 0; i < population_data->current_chromo_num; i++) {
		if (population_data->time_cost[i] != 0)
			population_data->time_cost[i] = compute_DAG(population_data->population[i], data);
	}
}

int circle_to_find_best(Population* population_data, JSSExchange* data) {
	int i;
	Population* population_data = jss_init_population(POPULATION_SIZE, data);
	enlarge_population(population_data);
	decode(population_data, data);
	find_best_chromo(population_data);
	lessen_population(population_data);
	int temp_time = population_data->time_cost[0];
	for (i = 0; i < 30; i++) {
		enlarge_population(population_data);
		decode(population_data, data);
		find_best_chromo(population_data);
		lessen_population(population_data);
		int cost = population_data->time_cost[0];
		if (cost == temp_time) i = 0;
		else temp_time = cost;
	}
	printf("%d", temp_time);
}


int main() {
	FILE* fp = fopen("C:\\Users\\Nihil\\Desktop\\JobShop\\src\\test.in", "r");
	JSSExchange* a = jss_load_from_file(fp);
	int** p = jss_init_population(POPULATION_SIZE, a);
	return 0;

}
#include "inherit.h"
#include "../../../io/file.h"
#include <string.h>
#include <stdint.h>
#include <time.h>
#define MAX_JOB 105 
#define MAX_MACHINE 25
#define POPULATION_SIZE 30
#define MUTIPLE 5
#define CHROMO_CHANGE_TIMES 3
#define MAX_LENGTH 1000
#define ENLARGE_TIMES 5
#define MUTATION_RATE 0.3
#define CORSSOVER_RATE 0.4
#define CIRCLE_TIME 1000
#define RESET 20
typedef struct
{
	int *time_cost;
	int** population;
	int population_size;
	int chromo_size;
	int current_chromo_num;
}Population;

/* The state word must be initialized to non-zero */
static unsigned long x =1, y=2 , z =3;

unsigned long rand_xorshift32(void) {          //period 2^96-1
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;
	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}
double jss_rand_xorshift32() {
	return (double)(rand_xorshift32() % 1000) / 1000;
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




bool delete_population(int population_size, int ** population)
{
	int i;
	for (i = 0; i < population_size; i++) {
		free(population[i]);
	}
	free(population);
	return true;
}


//int * crossover(int chromo_length, int * parent1, int * parent2, JSSExchange* data)
//{
//	int* temp_chromo = malloc(sizeof(int)*chromo_length * 2);
//	int i;
//	for (i = 0; i < chromo_length; i++) {
//		temp_chromo[i] = parent1[i];
//	}
//	int begin;
//	int end;
//	do {
//		begin = rand_xorshift32() % chromo_length;
//		end = rand_xorshift32() % chromo_length + 1;
//	} while (begin == end);
//	if (begin > end) {
//		int temp_begin = begin;
//		begin = end;
//		end = temp_begin;
//	}
//	int segment_length = end - begin;
//	int insert_position;
//	do {
//		insert_position = rand_xorshift32() % chromo_length;
//	} while (insert_position == 0);
//	for (i = chromo_length - 1; i >= insert_position; i--) {
//		temp_chromo[i + segment_length] = temp_chromo[i];
//	}
//	int* segment_num = calloc(sizeof(int), data->n);
//	for (i = begin; i < end; i++, insert_position++) {
//		segment_num[parent2[i]]++;
//		temp_chromo[insert_position] = parent2[i];
//	}
//	int temp_chromo_length = segment_length + chromo_length;
//	for (i = temp_chromo_length - 1; i > insert_position + segment_length; i--) {
//		if (segment_num[temp_chromo[i]] != 0) {
//			int j;
//			temp_chromo_length--;
//			segment_num[temp_chromo[i]]--;
//			for (j = i; j < temp_chromo_length; j++) {
//				temp_chromo[j] = temp_chromo[j + 1];
//			}
//		}
//	}
//	if (temp_chromo_length != chromo_length) {
//		for (i = insert_position - 1; i > 0; i--) {
//			if (segment_num[temp_chromo[i]] != 0) {
//				int j;
//				temp_chromo_length--;
//				segment_num[temp_chromo[i]]--;
//				for (j = i; j < temp_chromo_length; j++) {
//					temp_chromo[j] = temp_chromo[j + 1];
//				}
//			}
//		}
//	}
//	free(segment_num);
//	//记得释放内存
//	return temp_chromo;
//}
int * crossover(int *p1,int *p2,JSSExchange* data) {
	int i;
	int* offspring = malloc(sizeof(int)*(data->length + 1));
	memset(offspring, 0xC0, sizeof offspring);
	int process[MAX_JOB] = { 0 };
	for(i=0;i<data->length;i++) {
		if(jss_rand_xorshift32()>0.5) {
			offspring[i] = p1[i];
			process[p1[i]]++;
		}	
	}
	int j=0;
	for(i=0;i<data->length;i++) {
		if(process[p2[i]]<data->process_num[p2[i]]) {
			while (offspring[j] >= 0)j++;
			offspring[j] = p2[i];
			process[p2[i]]++;
		}
	}
	return offspring;
}

int format_solution(int chromo_length, int * chromosome, JSSExchange * data)
{
	return 0;
}


//
int * mutation(int chromo_length, int * parent)
{
	int position1, position2;
		position1 = rand_xorshift32() % chromo_length;
		position2 = rand_xorshift32() % chromo_length;
	int temp_num = parent[position1];
	parent[position1] = parent[position2];
	parent[position2] = temp_num;
	return parent;
}


/// 对单条染色体进行解码,并返回时间的最大值
int compute_DAG(int* chromo, JSSExchange* data) {
	int a[MAX_JOB*MAX_MACHINE] = { 0 };
	int traveled[MAX_JOB] = { 0 };
	int m = data->m;
	int i;
	int end_time[MAX_MACHINE] = { 0 };
	for (i = 0; i < data->length; i++) {
		int position = get_position(chromo[i], traveled[chromo[i]], data);
		int current_machine = get_machine_num(position, data);
		int current_time_cost = get_time_cost(position, data);
		// if (is_empty[current_machine]) {
		//非第一步
		if (traveled[chromo[i]] > 0) {
			int pre_position = get_position(chromo[i], traveled[chromo[i]] - 1, data);
			int pre_time_cost = get_time_cost(pre_position, data);
			int pre_machine = get_machine_num(pre_position, data);
			//如果工序上一步的结束时间大于现在机器的结束时间
			if (a[pre_position] + pre_time_cost> end_time[current_machine]) {
				end_time[current_machine] = a[pre_position] + pre_time_cost + current_time_cost;
				a[position] = a[pre_position] + pre_time_cost;
			}
			else {
				a[position] = end_time[current_machine];
				end_time[current_machine] += current_time_cost;
				
			}
		}
		else {//为第一步
			a[position] = end_time[current_machine];
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


void random_algo(JSSExchange* data) {
	int i;
	int process_num = 0;
	///make the first ordered array
	int  ordered_list[MAX_LENGTH];
	for (i = 0; i < data->n; i++) {
		int j = 0;
		for (j = 0; j < data->process_num[i]; j++) {
			int position = i * data->m + j;
			ordered_list[process_num] = i;
			process_num++;
		}
	}
	int max_num[MAX_LENGTH]={0};
	memcpy(max_num, ordered_list, sizeof(int)*data->length);
	int min_time = compute_DAG(max_num, data);
	
	for (i = 0; i < CIRCLE_TIME; i++) {
		
		int position1, position2;
		int j;
		for (j = 0; j <MUTIPLE; j++) {
				position1 = rand_xorshift32() % data->length;
				position2 = rand_xorshift32() % data->length;
			int temp_num = ordered_list[position1];
			ordered_list[position1] = ordered_list[position2];
			ordered_list[position2] = temp_num;
		}
		if(i%RESET==0) {
			memcpy(ordered_list, max_num, sizeof(int)* data->length);
		}
		int time = compute_DAG(ordered_list, data);
		if(time<min_time) {
			min_time = time;
			memcpy(max_num, ordered_list,sizeof(int)* data->length);
		}
	}
	add_time(max_num, data);
	jss_print_time(data);
}


///生成种群中的size个染色体
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
			int rand1 = (int)(rand_xorshift32() % process_num);
			int rand2 = (int)(rand_xorshift32() % process_num);
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
	population_data->time_cost = time_cost;
	population_data->current_chromo_num = population_size;
	return population_data;
}

///扩大种群
void enlarge_population(Population* population_data, JSSExchange* data) {
	int i;
	for (i = 0; i < population_data->population_size*(MUTIPLE - 2); i++) {
		int chromo1, chromo2;
		do {
			chromo1 = (rand_xorshift32() % population_data->population_size);
			chromo2 = (rand_xorshift32() % population_data->population_size);
		} while (chromo1 == chromo2);
		population_data->population[population_data->current_chromo_num] =
			crossover(population_data->population[chromo1], population_data->population[chromo2], data);
		population_data->current_chromo_num++;
	}
	if (jss_rand_xorshift32() < MUTATION_RATE)
		mutation(population_data->chromo_size, population_data->population[(rand_xorshift32() % population_data->population_size)]);
}

///缩小种群
void lessen_population(Population* population_data) {
	int i = population_data->population_size;
	for (; i < population_data->current_chromo_num; i++) {
		population_data->time_cost[i] = 0;
		free(population_data->population[i]);
	}
	population_data->current_chromo_num = population_data->population_size;
}


///找到最好的size个
void find_best_chromo(Population* population_data) {
	int best[10000] = { 0 };
	int i, j;
	int ** pop = population_data->population;
	int * t = population_data->time_cost;
	for (i = 0; i < population_data->current_chromo_num; i++) {
		for (j = population_data->current_chromo_num - 2; j >= i; j--) {
			if (t[j] > t[j + 1]) {
				int temp = t[j];
				t[j] = t[j + 1];
				t[j + 1] = temp;
				int *temp2 = pop[j];
				pop[j] = pop[j + 1];
				pop[j + 1] = temp2;

			}
		}
	}
	j = population_data->population_size;
	for (i = 0; i < population_data->population_size; i++) {
		best[t[i]]++;
		if (best[t[i]] > 2) {
			while (j<population_data->current_chromo_num&&best[t[j]]>2) 
				j++;
			
			if (j < population_data->current_chromo_num) {
					int temp = t[i];
					t[i] = t[j];
					t[j] = temp;
					int *temp2 = pop[i];
					pop[i] = pop[j];
					pop[j] = temp2;
					best[t[i]]++;
					best[t[j]]--;
					j++;
			}

		}
	}

}

///解码
void decode(Population* population_data, JSSExchange* data) {
	int i;
	for (i = 0; i < population_data->current_chromo_num; i++) {
		if (population_data->time_cost[i] == 0) {
			population_data->time_cost[i] = compute_DAG(population_data->population[i], data);
		}
	}

}

///进行迭代
int circle_to_find_best(Population* population_data, JSSExchange* data) {
	int i;
	enlarge_population(population_data, data);
	decode(population_data, data);
	find_best_chromo(population_data);
	lessen_population(population_data);
	int temp_time = population_data->time_cost[0];
	for (i = 0; i < CIRCLE_TIME; i++) {
		enlarge_population(population_data, data);
		decode(population_data, data);
		find_best_chromo(population_data);
		lessen_population(population_data);
		temp_time = population_data->time_cost[0];
		//int cost = population_data->time_cost[0];
		//if (cost != temp_time) i = 0;
		//else temp_time = cost;
	}
	
	printf("%d\n", temp_time);
}

///添加时间
void add_time(int *chromo, JSSExchange* data)
{
	memset(data->S, 0, sizeof(int)*data->m*data->n);
	int traveled[MAX_JOB] = { 0 };
	bool is_empty[MAX_MACHINE];
	int m = data->m;
	int n = data->n;
	int i;
	for (i = 0; i < m + 1; i++)
		is_empty[i] = true;
	int end_time[MAX_MACHINE] = { 0 };
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
			int pre_time_cost = get_time_cost(pre_position, data);
			//如果工序上一步的结束时间大于现在机器的结束时间
			if (data->S[pre_position]+pre_time_cost > end_time[current_machine]) {
				data->S[position] = data->S[pre_position] + pre_time_cost;
				end_time[current_machine] = data->S[pre_position] + pre_time_cost + current_time_cost;

			}
			else {
				data->S[position] = end_time[current_machine];
				end_time[current_machine] += current_time_cost;

			}
		}
		else {//为第一步
			data->S[position] = end_time[current_machine];
			end_time[current_machine] += current_time_cost;

		}
		traveled[chromo[i]]++;
	}
}

void jss_print_time(JSSExchange* data)
{
	//机器号 (起始,产品-工序,结束)
	int i;
	int max_time = 0;
	int** process_location = malloc(sizeof(int)* data->m);
	for (i = 1; i <= data->m; i++) {
		/// NO.i machine
		int j;
		memset(process_location, 0, sizeof(int)* data->m);
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
			int current_position = get_position(row, col, data);
			int start_time = data->S[current_position];
			int end_time = start_time + data->T[current_position];
			if (end_time > max_time) max_time = end_time;
			printf(" (%d,%d-%d,%d)",
				start_time,
				row + 1,
				col + 1,
				end_time
			);
		}

		putchar('\n');
		int k;
		for (k = 0; k < data->m; k++)
			free(process_location[k]);
	}
	//free(process_location);
	printf("END %d", max_time);
}

void inh_destroy(Population* pop) {
	int i;
	for (i = 0; i < pop->current_chromo_num; i++) {
		free(pop->population[i]);
	}
	free(pop->time_cost);
	free(pop);
}



int main() {
	srand(time(NULL));
	FILE* fp = fopen("C:\\Users\\Nihil\\Desktop\\JobShop\\src\\test.in", "r");
	JSSExchange* data = jss_load_from_file(fp);
	Population* population_data = jss_init_population(POPULATION_SIZE, data);
	circle_to_find_best(population_data, data);
	add_time(population_data->population[0], data);
	jss_print_time(data);
	jss_destory(data);
	inh_destroy(population_data);
	//random_algo(data);
	return 0;

}
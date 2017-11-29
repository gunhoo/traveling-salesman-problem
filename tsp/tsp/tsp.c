#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAXLENGTH 4000
#define NUMOFNODE 1000
#define NAN 2000

typedef struct Node{
	int cost;
	int prev, next;
}city;

int dist[NUMOFNODE][NUMOFNODE];
int visited_cities[NUMOFNODE];
city cities[NUMOFNODE];
time_t start_t = 0, end_t = 0;

void linkCity(int prev, int next){
	cities[prev].next = next;
	cities[next].prev = prev;
	cities[prev].cost = dist[prev][next];
}
void swapCity(int city1, int city2){
	linkCity(cities[city1].prev, cities[city1].next);
	linkCity(city1, cities[city2].next);
	linkCity(city2, city1);
}
int getCost(){
	int cost = 0;
	int i = 0;

	while (1){
		cost += cities[i].cost;
		i = cities[i].next;
		if (i == 0)
			break;
	}
	return cost;
}
void showResult(FILE* output){
	int i = 0;

	while (1){
		fprintf(output, "%d\n", i + 1);
		i = cities[i].next;
		if (i == 0){
			fprintf(output, "%d\n", i + 1);
			break;
		}
	}
}
void readFile(FILE* fp){
	int x = 0, y = 0;
	char str[MAXLENGTH];

	while (1){
		fgets(str, MAXLENGTH, fp);
		if (feof(fp)){
			break;
		}
		dist[x][y++] = atoi(strtok(str, ","));
		while (1){
			char* temp = strtok(NULL, ",");
			if (temp == NULL)
				break;
			else
				dist[x][y++] = atoi(temp);
		}
		x++; y = 0;
	}
}

// 현재 노드에서 가장 가까운 노드 찾아서 cost 더함
int tsp(int c){
	int count, nearest_city = NAN;
	int minimum = NAN, temp;
	for (count = 0; count < NUMOFNODE; count++){
		if (dist[c][count] != 0 && (visited_cities[count] == 0)){
			if (dist[c][count] + dist[count][c] < minimum){
				minimum = dist[count][c] + dist[c][count];
				temp = dist[c][count];
				nearest_city = count;
			}
		}
	}
	return nearest_city;
}
void minimum_cost(int city){
	int nearest_city;
	visited_cities[city] = 1;
	// 가장 가까운 노드 찾기
	nearest_city = tsp(city);
	if (nearest_city == NAN){
		// 마지막 노드인 경우
		nearest_city = 0;
		linkCity(city, nearest_city);
		return;
	}
	linkCity(city, nearest_city);
	minimum_cost(nearest_city);
}
void swap2opt(){
	while ((end_t - start_t) < 29000){
		int beforeCost, afterCost;
		int city1, city2;
		city temp1, temp2;

		// 랜덤하게 두 노드 선택
		while (1){
			city1 = rand() % NUMOFNODE;
			city2 = rand() % NUMOFNODE;
			if ((city1 != city2)
				&& (city1 != 0) && (city2 != 0)
				&& (city1 != cities[city2].next))
				break;
		}
		beforeCost = getCost();
		temp1 = cities[city1];
		temp2 = cities[city2];
		swapCity(city1, city2);
		afterCost = getCost();

		if (beforeCost < afterCost){
			linkCity(temp1.prev, city1);
			linkCity(city1, temp1.next);
			linkCity(city2, temp2.next);
		}
		end_t = clock();
	}
}
int main(int argc, char *argv[]){
	FILE* fp, *output;
	fp = fopen(argv[1], "r");
	if (fp == NULL){
		printf("There is no file!\n");
		return -1;
	}
	output = fopen("output.txt", "w");

	srand(time(NULL));

	start_t = clock();

	// Algorithm Starts
	readFile(fp);
	for (int i = 0; i < NUMOFNODE; i++){
		visited_cities[i] = 0;
	}
	minimum_cost(0);
	swap2opt();

	showResult(output);

	printf("Execution Time: %f sec\n", (double)((end_t - start_t) / (double)1000));
	printf("cost:%d", getCost());
	fclose(fp);
	fclose(output);
	return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAXLENGTH 4000
#define NUMOFNODE 1000
#define NAN 2000

int trial_num = 10;		// 시도 횟수
int timeCount = 29750;	// 실행 시간
int cost;

typedef struct Node {
	int cost;
	int prev, next;
}city;

int dist[NUMOFNODE][NUMOFNODE];
int visited_cities[NUMOFNODE] = { 0 };
city cities[NUMOFNODE];
int tempCity[NUMOFNODE];
int numOfTemp;

time_t start_t = 0, end_t = 0, end_t2 = 0;

void linkCity(int prev, int next) {
	cities[prev].next = next;
	cities[next].prev = prev;
	cities[prev].cost = dist[prev][next];
}
void swapCity(int city1, int city2) {
	numOfTemp = 0;

	// 링크 복사
	tempCity[numOfTemp] = cities[city1].next;
	while (tempCity[numOfTemp] != city2) {
		tempCity[numOfTemp + 1] = cities[tempCity[numOfTemp]].next;
		numOfTemp++;
	}
	// 링크 바꾸기
	for (int i = 0; i < numOfTemp - 1; i++) {
		linkCity(tempCity[i + 1], tempCity[i]);
	}
	linkCity(city1, tempCity[numOfTemp - 1]);
	linkCity(tempCity[0], city2);
}
void restoreCity(int city1, int city2) {
	// 링크 바꾸기
	for (int i = 0; i < numOfTemp - 1; i++) {
		linkCity(tempCity[i], tempCity[i + 1]);
	}
	linkCity(city1, tempCity[0]);
	linkCity(tempCity[numOfTemp - 1], city2);
}
int getCost() {
	int cost = 0;
	int i = 0;

	while (1) {
		cost += cities[i].cost;
		i = cities[i].next;
		if (i == 0)
			break;
	}
	return cost;
}
void saveResult(FILE* output) {
	int i = 0;

	while (1) {
		fprintf(output, "%d\n", i + 1);
		i = cities[i].next;
		if (i == 0) {
			fprintf(output, "%d\n", i + 1);
			break;
		}
	}
}
void readFile(FILE* fp) {
	int x = 0, y = 0;
	char str[MAXLENGTH];

	while (1) {
		fgets(str, MAXLENGTH, fp);
		if (feof(fp)) {
			break;
		}
		dist[x][y++] = atoi(strtok(str, ","));
		while (1) {
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
int tsp(int c) {
	int count, nearest_city = NAN;
	int minimum = NAN, temp;
	for (count = 0; count < NUMOFNODE; count++) {
		if (dist[c][count] != 0 && (visited_cities[count] == 0)) {
			if (dist[c][count] + dist[count][c] < minimum) {
				minimum = dist[count][c] + dist[c][count];
				temp = dist[c][count];
				nearest_city = count;
			}
		}
	}
	return nearest_city;
}
void minimum_cost(int city) {
	int nearest_city;
	visited_cities[city] = 1;
	// 가장 가까운 노드 찾기
	nearest_city = tsp(city);
	if (nearest_city == NAN) {
		// 마지막 노드인 경우
		nearest_city = 0;
		linkCity(city, nearest_city);
		return;
	}
	linkCity(city, nearest_city);
	minimum_cost(nearest_city);
}
void swap2opt() {
	cost = getCost();
	int afterCost;
	int city1, city2;
	while ((end_t - start_t) < timeCount) {
		// 랜덤하게 두 노드 선택
		while (1) {
			city1 = rand() % NUMOFNODE;
			city2 = rand() % NUMOFNODE;
			if ((city1 != city2)
				&& (city1 != 0) && (city2 != 0)
				&& (city2 != cities[city1].next)
				&& (cities[city1].next != cities[city2].prev))
				break;
		}
		swapCity(city1, city2);
		afterCost = getCost();

		if (cost < afterCost) {
			restoreCity(city1, city2);
		}
		else {
			cost = afterCost;
		}
		end_t = clock();
	}
}

int main(int argc, char *argv[]) {
	FILE* fp, *output;
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("There is no file!\n");
		return -1;
	}
	output = fopen("output.txt", "w");

	printf("Algorithm Executing...");
	srand(time(NULL));
	start_t = clock();

	// Algorithm Starts
	readFile(fp);
	minimum_cost(0);
	swap2opt();

	saveResult(output);

	printf("\nExecution Time: %f sec\n", (double)((end_t - start_t) / (double)1000));
	printf("cost:%d\n", getCost());

	fclose(fp);
	fclose(output);
	return 0;
}
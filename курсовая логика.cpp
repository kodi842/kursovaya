#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

void printSolution(int** dist, int V) {
    printf("Матрица кратчайших расстояний между каждой парой вершин:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INT_MAX)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

void floydWarshall(int** graph, int V) {
    int** dist = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        dist[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX
                    && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printSolution(dist, V);

    FILE* fptr;
    fptr = fopen("floyd_warshall_result.txt", "w");

    if (fptr == NULL) {
        printf("Ошибка при открытии файла!\n");
        return;
    }

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INT_MAX)
                fprintf(fptr, "%7s", "INF");
            else
                fprintf(fptr, "%7d", dist[i][j]);
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);
    printf("Результаты сохранены в файл floyd_warshall_result.txt\n");

    for (int i = 0; i < V; i++) {
        free(dist[i]);
    }
    free(dist);
}

void loadGraphFromFile(int*** graph, int* V, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", V);

    *graph = (int**)malloc(*V * sizeof(int*));
    for (int i = 0; i < *V; i++) {
        (*graph)[i] = (int*)malloc(*V * sizeof(int));
        for (int j = 0; j < *V; j++) {
            int value;
            fscanf(file, "%d", &value);
            if (value == -1) { // Используем -1 для обозначения "бесконечности"
                (*graph)[i][j] = INT_MAX;
            }
            else {
                (*graph)[i][j] = value;
            }
        }
    }

    fclose(file);
}

int main() {
    int V;
    int** graph;
    int choice;

    printf("Выберите способ задания графа:\n");
    printf("1. Сгенерировать случайный граф\n");
    printf("2. Загрузить граф из файла\n");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Введите количество вершин в графе: ");
        scanf("%d", &V);

        graph = (int**)malloc(V * sizeof(int*));
        for (int i = 0; i < V; i++) {
            graph[i] = (int*)malloc(V * sizeof(int));
        }

        srand(time(NULL));

        printf("Случайно сгенерированная матрица смежности:\n");
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (i == j) {
                    graph[i][j] = 0;
                }
                else {
                    graph[i][j] = rand() % 20 + 1;
                    if (rand() % 2 == 0) {
                        graph[i][j] = INT_MAX;
                    }
                }
                if (graph[i][j] == INT_MAX)
                    printf("%7s", "INF");
                else
                    printf("%7d", graph[i][j]);
            }
            printf("\n");
        }
    }
    else if (choice == 2) {
        char filename[100];
        printf("Введите имя файла: ");
        scanf("%s", filename);

        loadGraphFromFile(&graph, &V, filename);

        printf("Матрица смежности загружена из файла:\n");
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (graph[i][j] == INT_MAX)
                    printf("%7s", "INF");
                else
                    printf("%7d", graph[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("Неверный выбор!\n");
        return EXIT_FAILURE;
    }

    floydWarshall(graph, V);

    for (int i = 0; i < V; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}

#ifndef ALGO_GRAPH_H
#define ALGO_GRAPH_H

#include <stdio.h>
#include "vector.h"
#include "queue.h"

typedef struct edge {
	int from;
	int to;
	float weight;
} Edge;

typedef struct graph {
	int n;
	int m;
	int directed;
	int weighted;
	Vector *adj_list;
} Graph;


Graph *load_graph(char *filename);

void destroy_graph(Graph *g);

/* component array must be of size n
 */
int components(Graph *g, int component[]);

int *sssp_unweighted(Graph *g, int s);

int *construct_path(Graph *g, int e, int *sssp, int *len);

int *shortest_path_unweighted(Graph *g, int s, int e, int *len);

int acyclic(Graph *g);

#endif

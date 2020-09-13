#include <stdio.h>
#include "graph.h"

int show_components(Graph *g);

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Invalid number of args\n");
		return -1;
	}
	Graph *g = load_graph(argv[1]);
	if (!g) {
		printf("Unable to load graph\n");
		return 1;
	}

	int len;
	int *path = shortest_path_unweighted(g, 9, 13, &len);

	for (int i = 0; i < len; ++i) {
		printf("%d ", path[i]);
	}
	printf("\n");

	show_components(g);

	free(path);
	destroy_graph(g);
	return 0;
}

int show_components(Graph *g)
{
	int *comp = malloc(sizeof(*comp) * g->n);
	if (!comp)
		return 2;
	int num_comps = components(g, comp);
	printf("Number of components: %d\n", num_comps);
	for (int i = 0; i < g->n; ++i) {
		printf("%d ", comp[i]);
	}
	printf("\n");
	
	free(comp);
	return 0;
}


#include "graph.h"

static Edge *create_edge(int from, int to, float weight)
{
	Edge *e = malloc(sizeof(*e));
	if (!e)
		return NULL;
	e->from = from;
	e->to = to;
	e->weight = weight;
	return e;
}

static void *cpy_edge(void *e)
{
	Edge *edge = (Edge *)e;
	return create_edge(edge->from, edge->to, edge->weight);
}

Graph *load_graph(char *filename)
{
	FILE *f = fopen(filename, "r");
	if (!f)
		return NULL;
	Graph *g = malloc(sizeof(*g));
	if (!g) {
		fclose(f);
		return NULL;
	}
	/* n = |V|, m = |E| */
	int n, m;
	int ret = fscanf(f, "%d %d\n", &n, &m);
	if (ret != 2 || n < 0 || m < 0)
		goto load_graph_error;
	g->n = n;
	g->m = m;
	
	int d, w;
	ret = fscanf(f, "%d %d\n", &d, &w);
	if (ret != 2 && (d != 0 || d != 1) && (w != 0 || w!= 1))
		goto load_graph_error;
	g->directed = d;
	g->weighted = w;

	/* init adjacency lists */
	g->adj_list = malloc(sizeof(*(g->adj_list)) * g->n);
	if (!g->adj_list)
		goto load_graph_error;
	for (int i = 0; i < g->n; ++i) {
		g->adj_list[i] = create_vector(free, cpy_edge);
		if (!g->adj_list[i]) {
			/* memory allocation failure */
			for (int j = 0; j < i; ++j) {
				destroy_vector(g->adj_list[j]);
			}
			goto load_graph_error;
		}
	}
	/* add edges */
	int from, to, weight;
	for (int i = 0; i < g->m; ++i) {
		if (g->weighted)
			ret = fscanf(f, "%d %d %d\n", &from, &to, &weight);
		else
			ret = fscanf(f, "%d %d\n", &from, &to);
		if ((!g->weighted || ret != 3) && (g->weighted || ret != 2))
			goto load_graph_error;
		ret = append_vector(g->adj_list[from], create_edge(from, to, weight));
		if (ret != 0) {
			/* append error */
			goto load_graph_error;
		}
		if (!g->directed) {
			ret = append_vector(g->adj_list[to], create_edge(to, from, weight));
			if (ret != 0) {
				/* append error */
				goto load_graph_error;
			}
		}
	}
	/* remove unused space from vectors */
	for (int i = 0; i < g->n; ++i) {
		if (size_vector(g->adj_list[i]))
			capacity_vector(g->adj_list[i], 0);
	}

	fclose(f);

	return g;
load_graph_error:
	return NULL;
}

void destroy_graph(Graph *g)
{
	if (!g)
		return;
	for (int i = 0; i < g->n; ++i)
		destroy_vector(g->adj_list[i]);
	free(g->adj_list);
	free(g);
}

static void comp_dfs(Graph *g, int v, int count, int *component, int *visited)
{
	visited[v] = 1;
	component[v] = count;
	Vector neighbours = g->adj_list[v];
	Edge *e;
	for (unsigned int i = 0; i < size_vector(neighbours); ++i) {
		e = (Edge *)index_vector(neighbours, i);
		if (!visited[e->to])
			comp_dfs(g, e->to, count, component, visited);

	}	
}

int components(Graph *g, int component[])
{
	int *visited = malloc(sizeof(*visited) * g->n);
	if (!visited)
		return -1;
	for (int i = 0; i < g->n; ++i)
		visited[i] = 0;

	int count = 0;
	for (int v = 0; v < g->n; ++v) {
		if (!visited[v]) {
			comp_dfs(g, v, count, component, visited);
			++count;
		}
	}

	free(visited);
	return count;
}

static void nothing(void *p)
{
}

int *sssp_unweighted(Graph *g, int s)
{
	if (g->directed || s < 0)
		return NULL;
	int *visited = malloc(sizeof(*visited) * g->n);
	int *prev = malloc(sizeof(*visited) * g->n);
	if (!visited || !prev)
		return NULL;
	for (int i = 0; i < g->n; ++i) {
		visited[i] = 0;
		prev[i] = -1;
	}
	visited[s] = 1;

	Queue q = create_queue(nothing, NULL);
	if (!q) {
		/* memory error */
		free(visited);
		return NULL;
	}
	insert_queue(q, &s);

	int *v;
	Vector neighbours;
	Edge *e;

	while (!isempty_queue(q)) {
		v = (int *)pop_queue(q);
		neighbours = g->adj_list[*v];
		for (unsigned int i = 0; i < size_vector(neighbours); ++i) {
			e = (Edge *)index_vector(neighbours, i);
			if (!visited[e->to]) {
				insert_queue(q, &e->to);
				visited[e->to] = 1;
				prev[e->to] = *v;
			}

		}
	}
	
	destroy_queue(q);
	free(visited);
	return prev;
}

int *construct_path(Graph *g, int e, int *sssp, int *len)
{
	*len = 0;
	if (sssp[e] == -1)
		return NULL;
	/* shortest path can visit each vertex at most once */
	int *path = malloc(sizeof(*path) * g->n);
	if (!path) {
		/* memory error */
		*len = -1;
		return NULL;
	}
	/* reconstruct the path */
	while (e >= 0) {
		path[*len] = e;
		e = sssp[e];
		*len += 1;
	}
	/* resize the array */
	int *ptr = realloc(path, sizeof(*path) * *len);
	if (!ptr) {
		/* memory error */
		free(path);
		*len = -1;
		return NULL;
	}
	path = ptr;
	/* reverse the array */
	int tmp;
	for (int i = 0; i < *len/2; ++i) {
		tmp = path[i];
		path[i] = path[*len - i - 1];
		path[*len - i - 1] = tmp;
	}

	return path;
}

int *shortest_path_unweighted(Graph *g, int s, int e, int *len)
{
	int *sssp = sssp_unweighted(g, s);
	int *path = construct_path(g, e, sssp, len);
	free(sssp);
	return path;
}

/* returns true if the graph contains a cycle */
static int dfs_dacyclic(Graph *g, int v, int *visited, int *in_stack)
{
	visited[v] = 1;
	in_stack[v] = 1;
	Vector neighbours = g->adj_list[v];
	Edge *e;
	for (unsigned int i = 0; i < size_vector(neighbours); ++i) {
		e = (Edge *)index_vector(neighbours, i);
		if (!visited[e->to]) {
			if (dfs_dacyclic(g, e->to, visited)
				return 1;
		} else if (in_stack[e->to]) {
			return 1;
		}
	}
	in_stack[v] = 0;
	return 0;
}

static int is_dag(Graph *g, int *visited, int *in_stack)
{
	for (int i = 0; i < g->n; ++i) {
		if (!visited[i]) {
			if (dfs_dacyclic(g, i, visited))
				return 0;
		}
	}
	return 1;
}

static void dfs_acyclic(Graph *g, int v, int *visited, int *n, int *deg)
{
	visited[v] = 1;
	*n += 1;
	Vector neighbours = g->adj_list[v];
	Edge *e;
	for (unsigned int i = 0; i < size_vector(neighbours); ++i) {
		*deg += 1;
		e = (Edge *)index_vector(neighbours, i);
		if (!visited[e->to])
			dfs_acyclic(g, e->to, visited, n, deg);
	}
}

static int is_uwag(Graph *g, int *visited)
{
	int n, deg;
	for (int i = 0; i < g->n; ++i) {
		if (!visited[i]) {
			n = 0;
			deg = 0;
			dfs_acyclic(g, i, visited, &n, &deg);
			if (n - 1 != deg / 2)
				return 0;
		}
	}
	return 1;
}

int acyclic(Graph *g)
{
	int *in_stack;
	int *visited = malloc(sizeof(*visited) * g->n);
	if (!visited)
		return -1;
	if (g->directed) {
		in_stack = malloc(sizeof(*visited) * g->n);
		if (!in_stack) {
			free(visited);
			return -1;
		}
	}
	for (int i = 0; i < g->n; ++i) {
		visited[i] = 0;
		if (g->directed)
			in_stack[i] = 0;
	}

	int ret;
	if (g->directed) {
		ret = is_dag(g, visited, in_stack);
		free(in_stack);
	} else {
		ret = is_uwag(g, visited);
	}

	free(visited);
	return ret;
}

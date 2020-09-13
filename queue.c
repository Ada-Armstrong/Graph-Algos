#include "queue.h"

struct queue {
	DLL list;
};


Queue create_queue(void (*free_func)(void *), void *(*cpy_func)(void *))
{
	Queue q = malloc(sizeof(*q));
	q->list = create_dll(free_func, cpy_func);
	if (!q->list)
		return NULL;
	return q;
}

void destroy_queue(Queue q)
{
	if (!q)
		return;
	destroy_dll(q->list);
	free(q);
}

int insert_queue(Queue q, void *item)
{
	return insert_back_dll(q->list, item);
}

void *pop_queue(Queue q)
{
	return pop_front_dll(q->list);
}

int isempty_queue(Queue q)
{
	return size_dll(q->list) == 0;
}

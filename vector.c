#include "vector.h"

struct vector {
	void **data;
	unsigned int len;
	unsigned int maxlen;
	void (*free_func)(void *);
	void *(*cpy_func)(void *);
};

Vector create_vector(void (*free_func)(void *), void *(*cpy_func)(void *))
{
	Vector v = malloc(sizeof(*v));
	if (!v)
		return NULL;

	v->len = 0;
	v->maxlen = 2;
	v->free_func = free_func;
	v->cpy_func = cpy_func;

	v->data = malloc(sizeof(*(v->data)) * v->maxlen);
	if (!v->data) {
		free(v);
		return NULL;
	}

	return v;
}


void destroy_vector(Vector v)
{
	if (!v)
		return;

	for (unsigned int i = 0; i < v->len; ++i) {
		if (v->data[i])
			v->free_func(v->data[i]);
	}

	free(v->data);
	free(v);
}

Vector copy_vector(Vector v)
{
	assert(v);
	
	Vector cpy = create_vector(v->free_func, v->cpy_func);
	if (!cpy)
		return NULL;

	for (unsigned int i = 0; i < v->len; ++i) {
		void *cpy_item = v->cpy_func(v->data[i]);
		if (!cpy_item) {
			/* some sort of cpy error */
			destroy_vector(cpy);
			return NULL;
		}

		int ret = append_vector(cpy, cpy_item);
		if (ret) {
			/* append error */
			destroy_vector(cpy);
			return NULL;
		}
	}

	return cpy;
}

int append_vector(Vector v, void *item)
{
	assert(v);

	if (v->len >= v->maxlen) {
		v->maxlen *= 2;

	void **tmp = realloc(v->data, sizeof(*(v->data)) * v->maxlen);
	if (!tmp) {
			/* out of memory error */
			return 1;
		}

		v->data = tmp;
	}

	v->data[v->len] = item;
	v->len += 1;

	return 0;
}

void *index_vector(Vector v, unsigned int i)
{
	assert(v);

	if (i >= v->len)
		return NULL;

	return v->data[i];
}

int index_replace_vector(Vector v, unsigned int i, void *item)
{
	assert(v);

	if (i >= v->len)
		return 1;

	void *curr = v->data[i];
	if (curr)
		v->free_func(curr);

	v->data[i] = item;

	return 0;
}

unsigned int size_vector(Vector v)
{
	assert(v);
	return v->len;
}

int resize_vector(Vector v, unsigned int size)
{
	assert(v);

	unsigned int old_len = v->len;
	if (old_len > size) {
		/* free memory for items that will be out of bounds */
		for (unsigned int i = size; i < old_len; ++i) {
			v->free_func(v->data[i]);
		}
	}

	void **tmp = realloc(v->data, sizeof(*(v->data)) * size);
	if (!tmp)
		return 1;

	v->data = tmp;
	v->len = size;

	for (unsigned int i = old_len; i < size; ++i) {
		v->data[i] = NULL;
	}

	return 0;
}

int capacity_vector(Vector v, unsigned int cap)
{
	assert(v);

	unsigned int new_max = v->len + cap;

	void **tmp = realloc(v->data, sizeof(*(v->data)) * new_max);
	if (!tmp)
		return 1;

	v->maxlen = new_max;
	v->data = tmp;

	return 0;
}

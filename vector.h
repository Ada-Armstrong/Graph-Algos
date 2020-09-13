#ifndef CONTAINER_VECTOR_H
#define CONTAINER_VECTOR_H

/* An implementation of a resizing array.
 */

#include <stdlib.h>
#include <assert.h>

typedef struct vector *Vector;


/* create_vector: Allocates memory on the heap for a Vector and returns a pointer.
 * 	If memory allocation fails, returns NULL. Client must call destroy_vector
 * 	to free the memory
 * Runtime: O(1)
 */
Vector create_vector(void (*free_func)(void *), void *(*cpy_func)(void *));

/* destroy_vector: Frees the memory allocated by create_vector.
 * Runtime: O(1)
 */
void destroy_vector(Vector v);

/* copy_vector: Makes a deep copy of the Vector v and returns a pointer. If the
 * 	memory allocation fails, returns NULL. Client must call destroy_vector.
 * Runtime: O(n), where n is the size of the Vector v.
 */
Vector copy_vector(Vector v);

/* append_vector: Places item at the back of the Vector, item must be heap allocated.
 * 	Returns 0 if successful, 1 otherwise.
 * Runtime: O(1)
 */
int append_vector(Vector v, void *item);

/* index_vector: Returns the item at index i in the Vector, NULL if out of bounds.
 * Runtime: O(1)
 */
void *index_vector(Vector v, unsigned int i);

/* index_replace_vector: Frees the item at index i in the Vector and replaces it
 * 	with item, item must be heap allocated. Returns 0 if successful, 1 otherwise.
 * Runtime: O(1)
 */
int index_replace_vector(Vector v, unsigned int i, void *item);

/* size_vector: Returns the number of items in the Vector
 * Runtime: O(1)
 */
unsigned int size_vector(Vector v);

/* resize_vector: Changes the capacity of the Vector to be of length size. If
 * 	size is less than the current size, elements that are out of the new
 * 	bounds are freed. If size is greater, then NULL fills the new indices.
 * 	Returns 0 if successful, 1 otherwise.
 * Runtime: O(n), where n is the size of the Vector v.
 */
int resize_vector(Vector v, unsigned int size);

/* capacity_vector: Changes the padding of the Vector, cap is the amount of buffer
 * 	past the last element in the Vector. For example, if cap = 0 there will be
 * 	no extra buffer for the data. Returns 0 if successful, 1 otherwise.
 * Runtime: O(1)
 */
int capacity_vector(Vector v, unsigned int cap);

#endif

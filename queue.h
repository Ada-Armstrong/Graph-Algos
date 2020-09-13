#ifndef CONTAINER_QUEUE_H
#define CONTAINER_QUEUE_H

#include "dlinked_list.h"

typedef struct queue *Queue;


/* create_queue: Allocates memory for a queue. Client must call
 * 	destroy_queue to free the memory.
 * Runtime: O(1)
 */
Queue create_queue(void (*free_func)(void *), void *(*cpy_func)(void *));

/* destroy_queue: Frees the memory allocated by create_queue.
 * Runtime: O(n)
 */
void destroy_queue(Queue q);

/* insert_queue: Inserts item to the back of the queue.
 * Runtime: O(1)
 */
int insert_queue(Queue q, void *item);

/* pop_queue: Pops the front item from the queue.
 * Runtime: O(1)
 */
void *pop_queue(Queue q);

/* isempty_queue: Returns 1 if the queue is empty 0 otherwise.
 * Runtime: O(1)
 */
int isempty_queue(Queue q);

#endif

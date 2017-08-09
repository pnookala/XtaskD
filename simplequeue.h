#ifndef __MTC_QUEUE_H
#define __MTC_QUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

enum class task_type {
    SOURCE, SINK
};

struct task_desc {
    void* (*func)(void* indata); //left sibling of the tree
    task_type taskType;
    task_desc* rightSibling;
    task_desc* parentTask; //only one parent task, if parent task is null, it will have the end result.
    void* data; //input/output data, can be user-defined struct
    int isLastTask;
    int isHeadNode; // top node of the graph that will hold the final result.
};

struct queue {
    task_desc **tasks; /* the actual queue */
    int capacity; /* maximum number of task slots */
    int rear; /* tasks[rear % capacity] is the last item */
    int front; /* tasks[(front+1) % capacity] is the first */
    pthread_mutex_t lock; /* protects access to the queue */
    sem_t task_sem; /* counts available tasks in queue */
    sem_t spaces_sem; /* counts available task slots */
};

struct queue *create_queue(int size);
void dispose_queue(struct queue *q);
void enqueue(struct task_desc *task, struct queue *q);
struct task_desc *dequeue(struct queue *q);

#endif /* __MTC_QUEUE_H */

/*
 * NOTES:
 * [Code Optimization & Readability] Change **tasks to zero-length array
 * [Reasoning] Look at the Finite Producer-Consumer Solution of LBoS
 * [Issue] Make rear and front unsigned long maybe? possibly capacity too
 *         so nothing funky happens when converting during mod. If not
 *         then I need to explicity assign front & rear so they are bounded
 *         by the capacity of the queue as they used to be. (Look previous
 *         commits )
 */

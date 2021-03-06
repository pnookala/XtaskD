/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "simplequeue.h"
#include <stdlib.h>

struct queue *create_queue(int size)
{
	struct queue*q = (struct queue *) malloc(sizeof(struct queue));

	q->tasks =  (task_desc**)malloc(sizeof(void*) * size);
	q->capacity = size;
	q->front = 0;
	q->rear = 0;
	pthread_mutex_init(&q->lock, NULL);
	sem_init(&q->task_sem, 0, 0);
	sem_init(&q->spaces_sem, 0, size);

	return q;
}

void dispose_queue(struct queue *q)
{
	free(q->tasks);
	pthread_mutex_destroy(&q->lock);
	sem_destroy(&q->task_sem);
	sem_destroy(&q->spaces_sem);
	free(q);
}

void enqueue(struct task_desc *task, struct queue *q)
{

	sem_wait(&q->spaces_sem);
	pthread_mutex_lock(&q->lock);
	q->tasks[(++q->rear) % (q->capacity)] = (task_desc*)task;
	pthread_mutex_unlock(&q->lock);
	sem_post(&q->task_sem);

	return;
}

task_desc *dequeue(struct queue *q)
{
	task_desc* task;
	sem_wait(&q->task_sem);
	pthread_mutex_lock(&q->lock);
	/* WARNING: THIS MAY NOT WORK AS I WANT LATER */
        task = q->tasks[(++q->front) % q->capacity];
	pthread_mutex_unlock(&q->lock);
	sem_post(&q->spaces_sem);
	return task;
}



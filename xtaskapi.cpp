/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#define __GNU_SOURCE

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "xtaskapi.h"

struct mproc_state *mps;

struct task_desc *execute_task(struct task_desc *task) {

    sleep(0);
    return 0;
}

void *worker_handler(void *data) {
    struct mproc_state *mps = (struct mproc_state *) data;
    struct task_desc *task;

    while (!(*(mps->kill_master))) {
        task = dequeue(mps->incoming);
        __sync_add_and_fetch(mps->totaltasks,1);
        if (task) {
            if (task->isLastTask)
                *mps->kill_master = 1;

            //Execute the task which might spawn more child tasks and will be linked to this task as parent to propagate the results.
            task_desc *prev = task; //Save the previous for processing after a source task.
            task_desc *t = (task_desc*)task->func(task);

            //TODO - Add some logic to cleverly push tasks and maintain the task chain so dependent tasks are not fired before child tasks complete
            if (t) {
                //Push the task and siblings
                xtask_push(t);
                if (t->rightSibling)
                {
                    xtask_push(t->rightSibling);
                    if(t->rightSibling->parentTask)
                        xtask_push(t->rightSibling->parentTask);
                }
            }
        }
    }
    
    return NULL;
}

void xtask_setup(int queue_size, int workers, int* tasks) {
    int *kill_master = (int *) malloc(sizeof (int));
    *kill_master = 0;

    mps = (struct mproc_state *) malloc(sizeof (struct mproc_state));
    mps->queueSize = queue_size;
    mps->incoming = create_queue(queue_size);
    //mps->mystack = new simplestack();
    //mps->results = new Queue<struct task_desc > ();
    mps->kill_master = kill_master;
    mps->numWorkers = workers;
    mps->totaltasks = tasks;

    for (int t = 0; t < workers; t++) {
        mps->workers.push_back(std::thread(worker_handler, (void *) mps));
    }
}

void xtask_cleanup() {
    *mps->kill_master = 1;
    for (std::thread &worker : mps->workers)
        worker.join();
    
    free(mps->kill_master);
    free(mps);
}

void xtask_push(void *task) {
    enqueue((task_desc*) task, mps->incoming);
}

void xtask_put(void* task) {

}

struct task_desc xtask_poll() {
    struct task_desc task;
    //mps->results->dequeue(task);

    return task;
}
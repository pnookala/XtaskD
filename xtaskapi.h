/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xtaskapi.h
 * Author: pnookala
 *
 * Created on June 26, 2017, 9:06 AM
 */

#ifndef XTASKAPI_H
#define XTASKAPI_H

#include <future>
#include <vector>
#include "simplestack.cpp"

struct mproc_state {
    int queueSize;
    struct queue *incoming;
    //simplestack<X,Y,Z> *mystack;
    //Do we need a results queue?
    //struct queue *results;
    std::vector< std::thread > workers;
    int *kill_master;
    int numWorkers;
    cpu_set_t cpuset;
    int *totaltasks;
};

void xtask_setup(int queue_size, int workers, int* tasks);
void xtask_cleanup();
void xtask_push(void *task);
template<class X, class Y, class Z>
struct task_desc xtask_poll();

#endif /* XTASKAPI_H */


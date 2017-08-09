/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: pnookala
 *
 * Created on June 26, 2017, 8:53 AM
 */

#include <cstdlib>
#include <future>
#include <iostream>
#include <omp.h>
#include "xtaskapi.h"

#define SAMPLES 200000
#define WORKERS 2

using namespace std;

int fib(int n) {
    printf("Now serving n = %d\n", n);
    if (n <= 1)
        return n;
    future<int> f1 = async(launch::async, fib, n - 1);
    future<int> f2 = async(launch::async, fib, n - 2);

    int a1 = f1.get();
    int a2 = f2.get();
    printf("Now serving add with %d %d\n", a1, a2);
    return a1 + a2;
}

int fib_openmp(int n) {
    int i, j;
    printf("Now serving n = %d\n", n);
    if (n < 2)
        return n;
    else {
#pragma omp task shared(i)
        i = fib_openmp(n - 1);
#pragma omp task shared(j)
        j = fib_openmp(n - 2);
#pragma omp taskwait
        printf("Now serving add on %d and %d\n", i, j);
        return i + j;
    }
}

int fib_recursive_serial(int n) {
    int i, j;
    if (n < 2) return n;
    i = fib(n - 1);
    j = fib(n - 2);
    return (i + j);
}

struct data {
    int n; //input data
    std::promise<int> out; //output data
};

struct adddata {
    std::future<int> a, b;
    std::promise<int> about;
};

void* add_xtask(void *d) {
    task_desc* dat = (task_desc*) d;
    adddata *indata = (adddata*) dat->data;

    //printf("Now serving add \n");
    if (indata->a.wait_for(std::chrono::milliseconds(0)) != future_status::ready)
        return d;
    else if (indata->b.wait_for(std::chrono::milliseconds(0)) != future_status::ready)
        return d;
    else {
        int a = indata->a.get();
        int b = indata->b.get();

        int res = a + b;
        //printf("Now serving add %d + %d = %d\n", a, b, res);
        (indata->about).set_value(res);
    }
    //free(indata);
    return NULL;
}   

static void* fib_xtask(void *d) {
    task_desc* dat = (task_desc*) d;
    data *indata = (data*) (dat->data);

    int local_n = indata->n;

    //printf("Now serving %d\n", indata->n);
    if (indata->n <= 1) {
        (indata->out).set_value(indata->n);
        //free(indata);
        return NULL;
    }

    data *fibdata1 = (data*) malloc(sizeof (data));
    data *fibdata2 = (data*) malloc(sizeof (data));
    adddata *ad = (adddata*) malloc(sizeof (adddata));

    task_desc *thisTask = (task_desc*) malloc(sizeof (task_desc));
    task_desc *fib2 = (task_desc*) malloc(sizeof (task_desc));
    task_desc *add = (task_desc*) malloc(sizeof (task_desc));

    fibdata1->n = local_n - 1;
    fibdata2->n = local_n - 2;

    ad->about = std::promise<int>();
    ad->about = std::move(indata->out);

    fibdata1->out = std::promise<int>();
    fibdata2->out = std::promise<int>();
    ad->a = std::future<int>();
    ad->b = std::future<int>();

    ad->a = (fibdata1->out).get_future();
    ad->b = (fibdata2->out).get_future();


    *add = {add_xtask, task_type::SOURCE, NULL, NULL, ad, 0, 0};
    *fib2 = {fib_xtask, task_type::SINK, NULL, add, fibdata2, 0, 0};
    *thisTask = {fib_xtask, task_type::SINK, fib2, NULL, fibdata1, 0, 0};

    return thisTask;
}

int main(int argc, char** argv) {
    int n = 11;
    int numThreads = 2;
    int tasks;
    xtask_setup(1000, numThreads, &tasks);

    data d;
    d.n = n;

    task_desc maintask = {fib_xtask, task_type::SOURCE, NULL, NULL, &d, 0, 1};

    double dequeuethroughput;
    struct timespec tstart, tend;
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    
    xtask_push(&maintask);
    int result = d.out.get_future().get();
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double deq_elapsed = ((tend.tv_sec - tstart.tv_sec) * 1000000000.0) + ((tend.tv_nsec - tstart.tv_nsec));
    dequeuethroughput = ((tasks * 1000000000.0) / deq_elapsed);
    
    printf("Fib(%d) = %d\n", n, result);
    printf("%d %d %f %f\n", tasks, numThreads, deq_elapsed, dequeuethroughput);
    
      //printf("Fib(%d) = %d\n", n, fib(n));

    return 0;
}


///*
// * To change this license header, choose License Headers in Project Properties.
// * To change this template file, choose Tools | Templates
// * and open the template in the editor.
// */
//
///* 
// * File:   main.cpp
// * Author: pnookala
// *
// * Created on June 26, 2017, 8:53 AM
// */
//
//#include <cstdlib>
//#include <future>
//#include <iostream>
//#include <omp.h>
//#include "xtaskapi.h"
//
//#define SAMPLES 200000
//#define WORKERS 2
//
//using namespace std;
//
//int fib(int n) {
//    printf("Now serving n = %d\n", n);
//    if (n <= 1)
//        return n;
//    future<int> f1 = async(launch::async, fib, n - 1);
//    future<int> f2 = async(launch::async, fib, n - 2);
//   
//    int a1 = f1.get();
//    int a2 = f2.get();
//    printf("Now serving add with %d %d\n", a1,a2);
//    return a1 + a2;
//}
//
//int fib_openmp(int n) {
//    int i, j;
//    printf("Now serving n = %d\n", n);
//    if (n < 2)
//        return n;
//    else {
//#pragma omp task shared(i)
//        i = fib_openmp(n - 1);
//#pragma omp task shared(j)
//        j = fib_openmp(n - 2);
//#pragma omp taskwait
//        printf("Now serving add on %d and %d\n",i,j);
//        return i + j;
//    }
//}
//
//int fib_recursive_serial(int n) {
//    int i, j;
//    if (n < 2) return n;
//    i = fib(n - 1);
//    j = fib(n - 2);
//    return (i + j);
//}
//
//struct data
//{
//    int n; //input data
//    int* out; //output data
//};
//
//struct adddata
//{
//    int a, b;
//    int *out;
//};
//
//void* add_xtask(void *d) {
//    task_desc* dat = (task_desc*)d;
//    adddata *indata = (adddata*)dat->data;
//    
//    *indata->out = indata->a + indata->b;
//    printf("Now serving add %d + %d = %d\n", indata->a, indata->b, *indata->out);
//    return NULL;
//}
//
//void* fib_xtask(void *d) 
//{
//    task_desc* dat = (task_desc*)d;
//    data *indata = (data*)(dat->data);
//
//    printf("Now serving %d\n", indata->n);
//    if (indata->n <= 1) {
//        *indata->out = indata->n;
//        return NULL;
//    } 
//    
//    adddata *ad = (adddata*)malloc(sizeof(adddata));
//    ad->out = indata->out;
//
//    task_desc *add = (task_desc*)malloc(sizeof(task_desc));
//    *add = {add_xtask, task_type::SOURCE, NULL, NULL, ad, dat->isLastTask};
//    
//    data* fibdata1 = (data*)malloc(sizeof(data));
//    fibdata1->n = indata->n-1;
//    fibdata1->out = &(ad->a);
//    
//    data* fibdata2 = (data*)malloc(sizeof(data));
//    fibdata2->n = indata->n-2;
//    fibdata2->out = &(ad->b);
//    
//    task_desc *fib2 = (task_desc*)malloc(sizeof(task_desc));
//    *fib2 = {fib_xtask, task_type::SINK, NULL, add, fibdata2, 0};
//    
//    task_desc *thisTask = (task_desc*)malloc(sizeof(task_desc));
//    *thisTask = {fib_xtask, task_type::SINK, fib2, NULL, fibdata1, 0};
//   
//    return thisTask;
//}
//
///*
// * 
// */
//int main(int argc, char** argv) {
//    int n = 4;
//
//    xtask_setup(1000, 2);
////    #pragma omp parallel num_threads(1) private(tid)
////        {
////            #pragma omp single
////            {
////                //tid = omp_get_thread_num();
////                printf("Fib(%d) = %d\n", n, fib_openmp(n));
////    
////            }
////        } // all threads join master thread and terminates 
//
////   printf("Fib(%d) = %d\n", n, fib(n));
//
////    fibdata fd = {n};
////    outdata* od = (outdata*)malloc(sizeof(outdata));
////    od->out = (int*) malloc(sizeof (int));
////    
////    int* depCounter = (int*)malloc(sizeof(int));
////
////    //fibtask maintask = {{fib_xtask, task_status::READY, NULL, NULL, NULL}, 3, &result};
////    task_desc maintask = {fib_xtask, task_status::READY, NULL, NULL, 0, depCounter, (void*) (&fd), (void*) (od)};
////
////    xtask_push(&maintask);
////    xtask_cleanup();
////    
//////    printf("Fib(%d) = %d\n", n, *od->out);
////   
//    data *inout = (data*)malloc(sizeof(data));
//    inout->n = n;
//    inout->out = (int*)malloc(sizeof(int));
//    
//    task_desc maintask = {fib_xtask, task_type::SOURCE, NULL, NULL, inout, 1};
//
//    xtask_push(&maintask);
//    xtask_cleanup();
//    
//    printf("Fib(%d) = %d\n", n, *inout->out);
//
//    return 0;
//}
//

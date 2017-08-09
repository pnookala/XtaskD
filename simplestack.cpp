/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stack>
#include <vector>
#include <queue>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "simplequeue.h"

template<class X, class Y, class Z>
class simplestack
{
public:
    std::stack< task_desc >  _stack;
    pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    void push(task_desc* indata)
    {
        pthread_mutex_lock(&my_mutex);
        _stack.emplace(*indata);
        pthread_mutex_unlock(&my_mutex);
    }

    void pop(task_desc* popped_data)
    {
        pthread_mutex_lock(&my_mutex);
        *popped_data = _stack.top();
        _stack.pop();

        pthread_mutex_unlock(&my_mutex);
    }
    
    bool isEmpty()
    {
        return _stack.empty();
    }
};

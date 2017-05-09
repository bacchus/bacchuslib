#pragma once
/* compile string
 * gcc threads.c -D_REENTERANT -I/usr/include/nptl -o thread -L/usr/lib/nptl -lpthread
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

//_syscall0(pid_t,gettid);

//sem_t sem;// semaphore
//pthread_mutex_t mutex;// mutex

void exit_func(void* arg) {
    free(arg);
    printf("Freed the allocated memory.\n");
}

void* thread_func(void* arg) {
    //pid_t my_tid;
    //my_tid = gettid();

    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    //pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    //pthread_mutex_lock(&mutex);// mutex
    int loc_id = *(int*)arg;
//    sem_post(&sem);// semaphore
    //pthread_mutex_unlock(&mutex);// mutex
    void* mem;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    mem = malloc(1024);
    printf("Allocated some memory.\n");
    pthread_cleanup_push(exit_func, mem);
    for(int i=0; i < 4; i++) {
        sleep(1);
        printf("Thread %i is running\n", loc_id);
        if (i == 2) pthread_exit(0);
    }
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
    printf("%i YOU WILL NOT STOP ME!!!\n", loc_id);
    pthread_cleanup_pop(1);
}

void test_err(int res, const char* msg) {
    if (res != 0) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int test_threads() {
    int res;

    int id = 1;//int id1 = 1
//    sem_init(&sem, 0, 0);// semaphore
    //pthread_mutex_init(&mutex, NULL);// mutex
    //int size1 = 4;
    pthread_t thread1;

//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_t* pattr = nullptr;

    res = pthread_create(&thread1, pattr, thread_func, &id);//&size1
    //pthread_detach
    //test_err(res, "Creating the first thread");

//    sem_wait(&sem);// semaphore
    //pthread_mutex_lock(&mutex);// mutex
    id = 2;//int id2 = 2
    //pthread_mutex_unlock(&mutex);// mutex

    //size1 = 3;
    //int size2 = 3;
    pthread_t thread2;
    res = pthread_create(&thread2, NULL, thread_func, &id);//&size2
    //test_err(res, "Creating the second thread");

    pthread_cancel(thread1);

    res = pthread_join(thread1, NULL);
    //test_err(res, "Joining the first thread");

    res = pthread_join(thread2, NULL);
    //test_err(res, "Joining the second thread");

//    sem_destroy(&sem);// semaphore
    //pthread_mutex_destroy(&mutex);// mutex
    printf("Done\n");
    return EXIT_SUCCESS;
}

#ifndef WRITERTHREAD_H
#define WRITERTHREAD_H

#include "johma_bot.h"

typedef struct {
    char *message;
    char *file_path;
} WriteTask;

typedef struct {
    WriteTask *tasks;
    size_t capacity;
    size_t size;
    size_t head;
    size_t tail;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    bool running;
} WriteQueue;

int
init_write_queue(void);
int 
cleanup_write_queue(void);
int
async_write(const char *file_path, const char *message);
bool 
is_write_queue_empty(void);

#endif  // WRITERTHREAD_H
#include "johma_bot.h"

static WriteQueue queue;
static pthread_t writer_thread;

static void 
*writer_thread_func(void *arg) {
    while (true) {
         pthread_mutex_lock(&queue.mutex);

        while (queue.size == 0 && queue.running) {
            pthread_cond_wait(&queue.not_empty, &queue.mutex);
        }

        if (!queue.running && queue.size == 0) {
            pthread_mutex_unlock(&queue.mutex);
            break;
        }

        WriteTask task = queue.tasks[queue.head];
        queue.head = (queue.head + 1) % queue.capacity;
        queue.size--;

        pthread_cond_signal(&queue.not_full);
        pthread_mutex_unlock(&queue.mutex);

        FILE *file = fopen(task.file_path, "a");
        if (file) {
            fprintf(file, "%s\n", task.message);
            fclose(file);
        } else {
            log_error("ファイルのオープンに失敗しました: %s", strerror(errno));
        }

        free(task.message);
        free(task.file_path);

    }
    return NULL;
}

int
async_write(const char *file_path, const char *message) {
    pthread_mutex_lock(&queue.mutex);

    while (queue.size == queue.capacity && queue.running) {
        pthread_cond_wait(&queue.not_full, &queue.mutex);
    }

    if (!queue.running) {
        pthread_mutex_unlock(&queue.mutex);
        return -1;
    }

    WriteTask task = {
        .message = strdup(message),
        .file_path = strdup(file_path)
    };

    queue.tasks[queue.tail] = task;
    queue.tail = (queue.tail + 1) % queue.capacity;
    queue.size++;

    pthread_cond_signal(&queue.not_empty);
    pthread_mutex_unlock(&queue.mutex);
}

int
cleanup_write_queue(void) {
    pthread_mutex_lock(&queue.mutex);
    queue.running = false;
    pthread_cond_broadcast(&queue.not_empty);
    pthread_mutex_unlock(&queue.mutex);
    pthread_join(writer_thread, NULL);
    pthread_cond_destroy(&queue.not_empty);
    pthread_cond_destroy(&queue.not_full);
    pthread_mutex_destroy(&queue.mutex);
    free(queue.tasks);
    return 0;
}

bool 
is_write_queue_empty(void) {
    return queue.size == 0;
}

int
init_write_queue(void) {
    queue.capacity = 1024;
    queue.size = 0;
    queue.head = 0;
    queue.tail = 0;
    queue.running = true;
    queue.tasks = malloc(sizeof(WriteTask) * queue.capacity);
    if (!queue.tasks) {
        log_error("メモリの確保に失敗しました");
        return -1;
    }
    pthread_mutex_init(&queue.mutex, NULL);
    if (pthread_mutex_init(&queue.mutex, NULL) != 0) {
        free(queue.tasks);
        log_error("mutexの初期化に失敗しました");
        return -1;
    }
    pthread_cond_init(&queue.not_empty, NULL);
    if (pthread_cond_init(&queue.not_empty, NULL) != 0) {
        free(queue.tasks);
        pthread_mutex_destroy(&queue.mutex);
        log_error("not_emptyの初期化に失敗しました");
        return -1;
    }
    pthread_cond_init(&queue.not_full, NULL);
    if (pthread_cond_init(&queue.not_full, NULL) != 0) {
        free(queue.tasks);
        pthread_mutex_destroy(&queue.mutex);
        pthread_cond_destroy(&queue.not_empty);
        log_error("not_fullの初期化に失敗しました");
        return -1;
    }
    pthread_create(&writer_thread, NULL, writer_thread_func, NULL);
    return 0;
}
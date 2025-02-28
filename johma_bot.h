#ifndef JOHMA_BOT_H
#define JOHMA_BOT_H

#include <concord/discord.h>
#include <concord/log.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "cJSON.h"
#include "libs/filecontroller.h"
#include "commands/watch_dogs.h"
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include "include/json_type.h"
#include "include/vector.h"
#include "libs/writerthred.h"
#include <pthread.h>

typedef enum {
    GENERAL_NOT_EXISTS = -2,
    GENERAL_ERROR = -1,
    GENERAL_SUCCESS = 0,
    GENERAL_EXISTS = 1
} General_status;

typedef enum {
    ERROR = 1,
} Command_type;

extern u64snowflake g_app_id;

void on_ready(struct discord *client,const struct discord_ready *event);
void on_interaction(struct discord *client, const struct discord_interaction *event);


#endif  // JOHMA_BOT_H
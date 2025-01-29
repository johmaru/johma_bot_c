#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include "johma_bot.h"
#include "include/json_type.h"

// private functions
static char*
json_supporter(void);

static char*
write_json_supporter(json_object_j *json);


// public functions

int
watch_dogs_dir_check(void);

int
watch_dogs_file_check(const char* file_path);

int
create_json(const char* file_path);

int
write_json(const char* file_path, json_object_j *json);

json_object_j*
get_json_object(const char* file_path);

#endif
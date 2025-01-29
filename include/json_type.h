#ifndef JSON_TYPE_H
#define JSON_TYPE_H

typedef struct json_object_j {
    u64snowflake *channel_id;
    char *api_key;
    char *service;
}json_object_j;

#endif  // JSON_TYPE_H
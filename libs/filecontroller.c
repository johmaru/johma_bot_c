#include "johma_bot.h"

int 
watch_dogs_dir_check() {
    struct stat st = {0};
    if (stat("watch_dogs", &st) == 0) {
        log_info("ディレクトリが既に存在します");
        return GENERAL_EXISTS;
    }

    if (errno == ENOENT) {
        if (mkdir("watch_dogs", 0700) == -1) {
            log_error("ディレクトリの作成が出来ません: %s", strerror(errno));
            return GENERAL_ERROR;
        }
        log_info("watch_dogsディレクトリを作成しました");
         return GENERAL_SUCCESS;
    }

    log_error("予期せぬエラーが発生しました: %s", strerror(errno));
    return GENERAL_ERROR;
}

int
data_dir_check() {
    struct stat st = {0};
    if (stat("data", &st) == 0) {
        log_info("ディレクトリが既に存在します");
        return GENERAL_EXISTS;
    }

    if (errno == ENOENT) {
        if (mkdir("data", 0700) == -1) {
            log_error("ディレクトリの作成が出来ません: %s", strerror(errno));
            return GENERAL_ERROR;
        }
        log_info("dataディレクトリを作成しました");
        return GENERAL_SUCCESS;
    }

    log_error("予期せぬエラーが発生しました: %s", strerror(errno));
    return GENERAL_ERROR;
}

int
watch_dogs_file_check(const char* file_path) {
    FILE *file = fopen(file_path, "r");

    if (file) {
        log_info("ファイルが既に存在します");
        fclose(file);
        return GENERAL_EXISTS;
    }

    if (errno == ENOENT) {
        log_info("ファイルが存在しません");
        return GENERAL_NOT_EXISTS;
    }

    log_error("予期せぬエラーが発生しました: %s", strerror(errno));
    return GENERAL_ERROR;
}

int
data_file_check(const char* file_path) {
    FILE *file = fopen(file_path, "r");

    if (file) {
        log_info("ファイルが既に存在します");
        fclose(file);
        return GENERAL_EXISTS;
    }

    if (errno == ENOENT) {
        log_info("ファイルが存在しません");
        return GENERAL_NOT_EXISTS;
    }

    log_error("予期せぬエラーが発生しました: %s", strerror(errno));
    return GENERAL_ERROR;
}

int
create_json(const char* file_path) {
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        log_error("ファイルの作成に失敗しました: %s", strerror(errno));
        return GENERAL_ERROR;
    }

    char *string = json_supporter();
    if (string == GENERAL_ERROR) {
        log_error("JSONの作成に失敗しました");
        return GENERAL_ERROR;
    }

    fprintf(file, "%s", string);
    fclose(file);
    return GENERAL_SUCCESS;
}

int
write_json(const char* file_path,json_object_j *json) {
    log_info("write_jsonを開始します");
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        log_error("ファイルの作成に失敗しました: %s", strerror(errno));
        return GENERAL_ERROR;
    }
    char *string = write_json_supporter(json);
    if (string == GENERAL_ERROR) {
        log_error("JSONの作成に失敗しました");
        return GENERAL_ERROR;
    }
    fprintf(file, "%s", string);
    fclose(file);
    return GENERAL_SUCCESS;
}

int
write_append_txt(const char* file_path, const char* data) {
    FILE *file = fopen(file_path, "a");
    if (file == NULL) {
        log_error("ファイルの作成に失敗しました: %s", strerror(errno));
        return GENERAL_ERROR;
    }

    fprintf(file, "%s\n", data);
    fclose(file);
    return GENERAL_SUCCESS;
}

json_object_j*
get_json_object(const char* file_path){
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        log_error("ファイルの読み込みに失敗しました: %s", strerror(errno));
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = malloc(length + 1);
    if (buffer == NULL) {
        log_error("メモリの確保に失敗しました");
        fclose(file);
        return NULL;
    }

    if(fread(buffer, 1, length, file) != length) {
        log_error("ファイルの読み込みに失敗しました: %s", strerror(errno));
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);

    cJSON* json = cJSON_Parse(buffer);
    free(buffer);

    if (json == NULL) {
        log_error("JSONのパースに失敗しました");
        return NULL;
    }

    const cJSON* channel_id = cJSON_GetObjectItemCaseSensitive(json, "channel_id");
    const cJSON* api_key = cJSON_GetObjectItemCaseSensitive(json, "api_key");
    const cJSON* service = cJSON_GetObjectItemCaseSensitive(json, "service");
    const cJSON* type = cJSON_GetObjectItemCaseSensitive(json, "type");

    json_object_j* result = malloc(sizeof(json_object_j));
    result->channel_id = strdup(cJSON_GetStringValue(channel_id));
    result->api_key = strdup(cJSON_GetStringValue(api_key));
    result->service = strdup(cJSON_GetStringValue(service));
    result->type = strdup(cJSON_GetStringValue(type));

    cJSON_Delete(json);
    return result;
}

static char *
json_supporter(void) {
    log_info("json_supporterを開始します");

     cJSON *monitor = cJSON_CreateObject();

     if (monitor == NULL) {
         log_error("JSONの作成に失敗しました");
         return NULL;
     }

    if (cJSON_AddStringToObject(monitor, "channel_id", "") == NULL) {
            log_error("channel_idの追加に失敗しました");
            cJSON_Delete(monitor);
            return NULL;
    }
   
   if (cJSON_AddStringToObject(monitor, "api_key", "") == NULL) {
       log_error("api_keyの追加に失敗しました");
       cJSON_Delete(monitor);
       return NULL;
   }

    if (cJSON_AddStringToObject(monitor, "service", "") == NULL) {
         log_error("serviceの追加に失敗しました");
         cJSON_Delete(monitor);
         return NULL;
    }

    if (cJSON_AddStringToObject(monitor, "type", "") == NULL) {
        log_error("typeの追加に失敗しました");
        cJSON_Delete(monitor);
        return NULL;
    }

  char *string = cJSON_Print(monitor);
   if (string == NULL) {
       log_error("JSONの作成に失敗しました");
       cJSON_Delete(monitor);
        return NULL;
   }

    log_info("JSONを作成しました: %s", string);
    cJSON_Delete(monitor);
    return string;
}

static char *
write_json_supporter(json_object_j *json){
    log_info("write_json_supporterを開始します");
    
    if (!json || !json->api_key || !json->service || !json->channel_id) {
        log_error("Invalid JSON object");
        return NULL;
    }

    cJSON *monitor = cJSON_CreateObject();

    log_info("api_key: %s", json->api_key);
    log_info("service: %s", json->service);

    if (monitor == NULL) {
        log_error("JSONの作成に失敗しました");
        return NULL;
    }

    // 読み取り時は文字列から数値を読み取り u64snowflake に変換する
    char buffer[21];
    u64snowflake channel_id_value = *json->channel_id;
    snprintf(buffer, sizeof(buffer), "%" PRIu64,channel_id_value);

    log_info("channel_id: %s", buffer);

    if (cJSON_AddStringToObject(monitor, "channel_id",buffer) == NULL) {
        log_error("channel_idの追加に失敗しました");
        cJSON_Delete(monitor);
        return NULL;
    }

    if (cJSON_AddStringToObject(monitor, "api_key", json->api_key) == NULL) {
        log_error("api_keyの追加に失敗しました");
        cJSON_Delete(monitor);
        return NULL;
    }

    if (cJSON_AddStringToObject(monitor, "service", json->service) == NULL) {
        log_error("serviceの追加に失敗しました");
        cJSON_Delete(monitor);
        return NULL;
    }

    if (cJSON_AddStringToObject(monitor, "type", json->type) == NULL) {
        log_error("typeの追加に失敗しました");
        cJSON_Delete(monitor);
        return NULL;
    }

    char *string = cJSON_Print(monitor);
    if (string == NULL) {
        log_error("JSONの作成に失敗しました");
        cJSON_Delete(monitor);
        return NULL;
    }

    log_info("JSONを作成しました: %s", string);
    cJSON_Delete(monitor);
    return string;
}
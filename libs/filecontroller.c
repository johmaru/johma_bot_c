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

static char *json_supporter(void) {


     cJSON *monitor = cJSON_CreateObject();

     if (monitor == NULL) {
         log_error("JSONの作成に失敗しました");
         return NULL;
     }
   
   if (cJSON_AddStringToObject(monitor, "api_key", "") == NULL) {
       log_error("api_keyの追加に失敗しました");
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

static char *write_json_supporter(json_object_j *json){
    cJSON *monitor = cJSON_CreateObject();

    log_info("api_key: %s", json->api_key);
    log_info("service: %s", json->service);

    if (monitor == NULL) {
        log_error("JSONの作成に失敗しました");
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
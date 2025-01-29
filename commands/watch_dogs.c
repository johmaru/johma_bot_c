#include "watch_dogs.h"


struct 
discord_application_command_option_choice watch_dogs_choices[] = {
    {.name = "Google Drive", .value = "\"google_drive\""},
    {.name = "Notion", .value = "\"notion\""},
};

Vector* watch_dogs_vec = NULL;

struct 
discord_application_command_option watch_dogs_options[] = {
    {
        .type = DISCORD_APPLICATION_OPTION_BOOLEAN,
        .name = "画像", .description = "画像を送信します",
        .required = true,
    },
    {
        .type = DISCORD_APPLICATION_OPTION_BOOLEAN, .name = "チャット", .description = "チャットを送信します",
        .required = true
    },
    {
        .type = DISCORD_APPLICATION_OPTION_STRING,
        .name = "api_key", .description = "外部サービスのAPIキー",
        .required = true
    },
    {
        .type = DISCORD_APPLICATION_OPTION_STRING,
        .name = "service", .description = "外部サービス",
        .choices = 
        &(struct discord_application_command_option_choices) {
            .size = sizeof(watch_dogs_choices) / sizeof * watch_dogs_choices,
            .array = watch_dogs_choices
        },
        .required = true
    }
};

struct 
discord_create_guild_application_command  watch_dogs_parmas = {
        .name = "watch_dogs",
        .description = "discordのチャットを監視し、任意のコンテンツを任意の外部サービスに送信します。",
        .default_permission = true,
        .options = 
        &(struct discord_application_command_options){
            .size = sizeof(watch_dogs_options) / sizeof * watch_dogs_options,
            .array = watch_dogs_options
        }
    };

int
load_watch_dogs(void) {

    log_info("watch_dogsの読み込みを開始します");

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        log_error("カレントディレクトリの取得に失敗しました");
        return GENERAL_ERROR;
    }

    char full_path[4133];
    snprintf(full_path, sizeof(full_path),"%s/watch_dogs",cwd);

    DIR *dir = opendir(full_path);
    if (!dir) {
        log_error("ディレクトリのオープンに失敗しました: %s", strerror(errno));
        return GENERAL_ERROR;
    }

    struct dirent *entry;

    if (watch_dogs_vec != NULL) {
        vector_free(watch_dogs_vec);
    }

    watch_dogs_vec = vector_create();
    if (watch_dogs_vec == NULL) {
        log_error("vectorの作成に失敗しました");
        return GENERAL_ERROR;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        char *file_path = malloc(WATCH_DOGS_FILE_PATH_SIZE);
        if (!file_path) {
            log_error("メモリの確保に失敗しました");
            continue;
        }

        snprintf(file_path, WATCH_DOGS_FILE_PATH_SIZE,"%s/%s",full_path,entry->d_name);
        if (watch_dogs_file_check(file_path) == GENERAL_SUCCESS || watch_dogs_file_check(file_path) == GENERAL_EXISTS) {
            vector_push(watch_dogs_vec, file_path);
        } else {
            free(file_path);
        }
    }
    closedir(dir);
    return GENERAL_SUCCESS;
}

void
get_watch_dogs_debug(void){
    log_info("watch_dogs_vecのデバッグを開始します");
    if (watch_dogs_vec == NULL) {
        log_error("watch_dogs_vecがNULLです");
        return;
    }

    for (size_t i = 0; i < watch_dogs_vec->size; i++) {
       char *path = (char*)vector_get(watch_dogs_vec, i);
       log_info("watch_dogs_vec[%lu]: %s",i,path);
       if (path == NULL) {
           log_error("vectorの取得に失敗しました");
           continue;
       }
    }
    return;
}
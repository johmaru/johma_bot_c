#include "johma_bot.h"
#include "defer.h"

//commands headers
#include "commands/yaa.h"
#include "commands/watch_dogs.h"

u64snowflake 
g_app_id = 0;

json_object_j
json_obj = {
    .channel_id = NULL,
    .api_key = NULL,
    .service = NULL,
    .type = NULL
};

Vector* 
vector_create(void) {
    Vector* vec = malloc(sizeof(Vector));
    vec->data = malloc(sizeof(void*) * 8);
    vec->size = 0;
    vec->capacity = 8;
    return vec;
}

void
vector_push(Vector* vec, void* data) {
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->capacity * sizeof(void*));
    }
    vec->data[vec->size++] = data;
}

void*
vector_get(Vector* vec, size_t index) {
    if (index >= vec->size) return NULL;
    return vec->data[index];
}

void
vector_free(Vector* vec) {
    free(vec->data);
    free(vec);
}

void 
on_ready(struct discord *client,const struct discord_ready *event) {
    log_info("Logged in as %s!", event->user->username);

    g_app_id = event->application->id;

    int result = load_watch_dogs();
    if (result == GENERAL_SUCCESS) {
        get_watch_dogs_debug();
    } else {
        log_error("watch_dogsの読み込みに失敗しました");
    }
}

Vector* commands_vec = NULL;

void 
all_commands_load () {
    commands_vec = vector_create();
    //ここからコマンドを追加
    vector_push(commands_vec, &hello_parmas);
    vector_push(commands_vec, &watch_dogs_parmas);
}

void 
on_slash_command_create(struct discord *client,const struct discord_message *event){
    if (event->author->bot) return;

    if (!commands_vec) {
        log_error("commands_vec is not initialized");
        return;
    }

    for (size_t i = 0; i < commands_vec->size; i++) {
        struct discord_create_guild_application_command* cmd = vector_get(commands_vec, i);
        if (!cmd) {
            log_error("Failed to get command at index %zu", i);
            continue;
        }
        discord_create_guild_application_command(client, g_app_id,event->guild_id, cmd, NULL);
        log_info("Command %s created", cmd->name);
    }
}

void 
on_interaction(struct discord *client, const struct discord_interaction *event) {
    Deferral

    if (!event || !event->data || !event->data->name) {
        log_error("Invalid interaction");
        goto error_end;
    }

    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND) {
        log_error("Not an application command");
        goto error_end;
    }

    log_info("Received command: %s", event->data->name);

    if (strcmp(event->data->name, "yaa") == 0) {

         if (!event->data->options || !event->data->options->array) {
            log_error("No options provided");
            return;
        }

       const char *message = event->data->options->array[0].value;
       log_info("Received message: %s", message);

       size_t len = strlen(message) + strlen("やあやあ！") + 1;
       char *final_message = malloc(len);
       snprintf(final_message, len, "やあやあ！%s", message);
       Defer(free(final_message));

        struct discord_interaction_response response = {
            .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
            .data = &(struct discord_interaction_callback_data) {
                .content = final_message,
            },
        };
        discord_create_interaction_response(client, event->id,event->token,&response, NULL);
    } 
     if (strcmp(event->data->name, "watch_dogs") == 0)
    {
       bool Picture = false;
       bool Chat = false;
       char* api_key = NULL;
       char* service = NULL;
       char* type = NULL;
       u64snowflake channel_id = event->channel_id;

       struct discord_interaction_response defer_response = {
           .type = DISCORD_INTERACTION_DEFERRED_CHANNEL_MESSAGE_WITH_SOURCE,
           .data = NULL,
       };
         discord_create_interaction_response(client, event->id, event->token, &defer_response, NULL);
       

       for (int i = 0; i < event->data->options->size; i++) {
          char *name = event->data->options->array[i].name;
          char *value = event->data->options->array[i].value;
           if (strcmp(name, "画像") == 0) {
               if (strcmp(value, "true") == 0) {
                Picture = true;
                Chat = false;
               }
           } else if (strcmp(name, "チャット") == 0) {
             if (strcmp(value, "true") == 0) {
                Chat = true;
                Picture = false;
               }
           } else if (strcmp(name, "api_key") == 0) {
               api_key = value;
           } else if (strcmp(name, "service") == 0) {
               service = value;
           }
       }
        if (Picture) {
            type = "Picture";
         } else if (Chat) {
            type = "Chat";
         } 
         
        log_info("api key: %s", api_key);
        log_info("service: %s", service);
        log_info("type: %s", type);

        if (watch_dogs_dir_check() == GENERAL_ERROR) {
            log_error("watch_dogsディレクトリの作成に失敗しました");
            goto error_end;
        } 

        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            log_error("カレントディレクトリの取得に失敗しました");
            goto error_end;
        }


       char full_path[4133];
       snprintf(full_path, sizeof(full_path),"%s/watch_dogs/%lu.json",cwd, channel_id);

       int file_status = watch_dogs_file_check(full_path);

       log_info("file_status: %d", file_status);       

       if (file_status == GENERAL_NOT_EXISTS) {
            json_obj.channel_id = channel_id;
            json_obj.api_key = api_key;
            json_obj.service = service;
            json_obj.type = type;

           if (write_json(full_path, &json_obj) != GENERAL_SUCCESS) {
            log_error("JSONファイルの作成に失敗しました");
            goto error_end;
           }
        } else if (file_status == GENERAL_EXISTS)
        {
            log_info("ファイルが存在します。更新を開始します");
            log_info("type: %s", type);
            json_obj.channel_id = channel_id;
            json_obj.api_key = strdup(api_key);
            json_obj.service = strdup(service);
            json_obj.type = strdup(type);

        if (write_json(full_path, &json_obj) != GENERAL_SUCCESS) {
        free(json_obj.api_key);
        free(json_obj.service);    
        free(json_obj.type);
        log_error("JSONファイルの更新に失敗しました");
        goto error_end;

        }
        free(json_obj.api_key);
        free(json_obj.service);
        free(json_obj.type);
        log_info("JSONファイルを更新しました: %s", full_path);
        } else {
           log_error("予期せぬエラーが発生しました");
           goto error_end;
        }

        end:
        struct discord_edit_original_interaction_response response = {
            .content = "設定を更新しました",
        };
        discord_edit_original_interaction_response(client, event->application_id, event->token, &response, NULL);
        return;
        
        error_end: 
        { struct discord_edit_original_interaction_response error_response = 
        { 
            .content = "エラーが発生しました",  
        };
        discord_edit_original_interaction_response(client, event->application_id, event->token, &error_response, NULL);
        return; 
    }

    }
    
}

void
on_message_create(struct discord *client, const struct discord_message *msg) {
    log_info("on_message_createを開始します");
    for (size_t i = 0; i < watch_dogs_vec->size; i++) {
        char *file_path = vector_get(watch_dogs_vec, i);
        if (!file_path) continue;

        json_object_j* result = get_json_object(file_path);
        if (!result || !result->channel_id) continue;

        unsigned long channel_id = strtoul((const char*)result->channel_id, NULL, 10);

        if (channel_id == msg->channel_id) {
            log_info("チャンネルが一致しました");

            if(data_dir_check() == GENERAL_ERROR) {
                log_error("dataディレクトリの作成に失敗しました");
                return;
            }

            if (strcmp(result->type, "Chat") == 0) {
                  size_t path_size = 0;
            char* file_path = NULL;

            log_info("メッセージを書き込みます");

            path_size = pathconf(".", _PC_PATH_MAX);
            if (path_size == -1) {
                log_error("パスの最大長の取得に失敗しました");
                return;
            }

            file_path = malloc(path_size);
            if (file_path == NULL) {
                log_error("メモリの確保に失敗しました");
                return;
            }

            if(getcwd(file_path, path_size) == NULL) {
                log_error("カレントディレクトリの取得に失敗しました");
                free(file_path);
                return;
            }

            char *full_path = malloc(path_size + 128);
            if (full_path == NULL) {
                log_error("メモリの確保に失敗しました");
                free(file_path);
                return;
            }

            snprintf(full_path, path_size + 128, "%s/data/%lu.txt", file_path, msg->channel_id);
            log_info("作成されたファイルパス: %s", full_path);
            free(file_path);

           if ( init_write_queue() != 0) {
                log_error("write queueの初期化に失敗しました");
                free(full_path);
                return;
           }

           if (async_write(full_path, msg->content) != 0) {
                log_error("メッセージの書き込みに失敗しました");
                cleanup_write_queue();
                free(full_path);
                return;
           }

           int timeout_counter = 0;
           const int MAX_TIMEOUT = 10;

            while (!is_write_queue_empty()) {
                sleep(1);
                timeout_counter++;

                if (is_write_queue_empty()) {
                    log_info("write queueが空になりました");
                    break;
                }
            }
            
            cleanup_write_queue();
            free(full_path);
            }
          
        }

        free(result->api_key);
        free(result->service);
        free(result->channel_id);
        free(result);
    }
}

int 
main(int argc, char *argv[]) {
    
    const char *config_file;
    if (argc > 1)
        config_file = argv[1];
    else
        config_file = "config.json";
 
    ccord_global_init();

    all_commands_load();

    struct discord *client = discord_config_init(config_file);
    assert(NULL != client && "Could not initialize client");

    discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT | DISCORD_GATEWAY_GUILD_MESSAGES | DISCORD_GATEWAY_GUILDS);
    discord_set_on_message_create(client, &on_message_create);

    discord_set_on_command(client,"jm!" ,&on_slash_command_create);
    discord_set_on_ready(client, &on_ready);
    
    discord_set_on_interaction_create(client, &on_interaction);

    discord_run(client);

    if (commands_vec) {
        vector_free(commands_vec);
        commands_vec = NULL;
    }

    discord_cleanup(client);
    ccord_global_cleanup();
    return 0;
}
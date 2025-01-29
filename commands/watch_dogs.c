#include "watch_dogs.h"


struct 
discord_application_command_option_choice watch_dogs_choices[] = {
    {.name = "Google Drive", .value = "\"google_drive\""},
    {.name = "Notion", .value = "\"notion\""},
};

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
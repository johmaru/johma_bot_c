#include <concord/discord.h>

struct 
discord_application_command_option yaa_options[] = {
   {
    .type = DISCORD_APPLICATION_OPTION_STRING,
    .name = "name",
    .description = "名前を入力してください",
    .required = true
   }
};

struct 
discord_create_guild_application_command  hello_parmas = {
        .name = "yaa",
        .description = "greet with yaa",
        .type = DISCORD_APPLICATION_CHAT_INPUT,
        .options = &(struct discord_application_command_options){
            .size = sizeof(yaa_options) / sizeof * yaa_options,
            .array = yaa_options
        }
    };
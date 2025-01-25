#include "johma_bot.h"

void on_ready(struct discord *client, const struct discord_ready *event) {
    log_info("Logged in as %s!", event->user->username);

    g_app_id = event->application->id;
}

void on_slash_command_create(struct discord *client,const struct discord_message *event){
    if (event->author->bot) return;

    struct discord_create_guild_application_command  hello_parmas = {
        .name = "yaa",
        .description = "greet with yaa",
        .default_permission = true,
    };

    discord_create_guild_application_command(client, g_app_id,event->guild_id, &hello_parmas, NULL);
}

void on_interaction(struct discord *client, const struct discord_interaction *event) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND) return;

    if (strcmp(event->data->name, "yaa") == 0) {
        struct discord_interaction_response response = {
            .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
            .data = &(struct discord_interaction_callback_data) {
                .content = "やあやあ!",
            },
        };
        discord_create_interaction_response(client, event->id,event->token,&response, NULL);
    }
}

int main(int argc, char *argv[]) {
    
    const char *config_file;
    if (argc > 1)
        config_file = argv[1];
    else
        config_file = "config.json";
 
    ccord_global_init();

    struct discord *client = discord_config_init(config_file);
    assert(NULL != client && "Could not initialize client");

    discord_set_on_command(client,"jm!" ,&on_slash_command_create);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_interaction_create(client, &on_interaction);

    discord_run(client);

    discord_cleanup(client);
    ccord_global_cleanup();
}


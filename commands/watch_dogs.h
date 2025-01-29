#ifndef WATCH_DOGS_H
#define WATCH_DOGS_H

#include <concord/discord.h>
#include "../johma_bot.h"
#include "../include/vector.h"

#define WATCH_DOGS_FILE_PATH_SIZE 4389

extern Vector* watch_dogs_vec;


extern struct discord_application_command_option_choice watch_dogs_choices[];
extern struct discord_application_command_option watch_dogs_options[];
extern struct discord_create_guild_application_command watch_dogs_parmas;

int
load_watch_dogs(void);
void
get_watch_dogs_debug(void);

#endif
#include <concord/discord.h>
#include <concord/log.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#define GUILD_ID 1064571288645029918

u64snowflake g_app_id;

void on_ready(struct discord *client, const struct discord_ready *event);
void on_interaction(struct discord *client, const struct discord_interaction *event);

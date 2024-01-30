#include "discord_rpc_init.h"
#include <ctime>
#include "../../utils/xorstr/xorstr.hpp"

void Discord::initialize()
{
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));
    Discord_Initialize(X("1197474891969089566"), &Handle, 1, NULL);
}

void Discord::update()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    static auto elapsed = std::time(nullptr);

    discordPresence.largeImageText = X("Counter-Strike 2");
    discordPresence.state = X("Premium <3");
    discordPresence.largeImageKey = X("https://assets.faceit-cdn.net/hubs/avatar/e63afec5-b332-425f-943b-2f138c01c6d5_1679600694167.jpg");
    discordPresence.startTimestamp = elapsed;
    discordPresence.smallImageKey = X("https://insage.ru/favicon.png");
    discordPresence.smallImageText = X("[insage.ru] angeldancy");
    Discord_UpdatePresence(&discordPresence);
}
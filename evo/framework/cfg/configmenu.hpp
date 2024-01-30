#pragma once
#include <filesystem>
#include <string>
#include "../../hacks/features/triggerbot/triggerbot.h"
#include "../../hacks/features/aimbot/aimbot.hpp"
#include "../../hacks/features/menu/font/iconsfontwwesome5.h"
#include "cfg.hpp"
#include "../../utils/xorstr/xorstr.hpp"

namespace ConfigMenu 
{
    void render_config_menu(const char *Tab);
    void reset_to_default();
}
